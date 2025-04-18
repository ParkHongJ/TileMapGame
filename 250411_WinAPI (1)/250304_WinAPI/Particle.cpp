#include "pch.h"
#include "Particle.h"
#include "CommonFunction.h"
#include "Image.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "CameraManager.h"

void Particle::Init(string imageStr, FPOINT pos, float angle, float size, float lifeTime)
{
	this->angle = angle;
	isEnd = false;
}

void Particle::Update(float TimeDelta)
{
	// 수명이 다하면 종료
	lifeTime -= TimeDelta;
	if (lifeTime <= 0.f)
	{
		isEnd = true;
		return;
	}

	for (auto& opt : options)
		opt->Update(*this, TimeDelta);
}

void Particle::Render(ID2D1HwndRenderTarget* rt)
{
	bool customRender = false;

	for (auto& opt : options)
	{
		if (opt->HandlesRender())
			customRender = true;
	}

	for (auto& opt : options)
		opt->Render(*this, rt);

	// 옵션 중 아무도 렌더하지 않았다면 기본 렌더링 수행
	if (!customRender && image)
	{
		FPOINT cameraPos = pos + CameraManager::GetInstance()->GetPos();

		float effectScale = size / ATLAS_TILE_SIZE;

		image->Render(rt, cameraPos.x, cameraPos.y, effectScale, effectScale, atlas.x, atlas.y, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
	}
}

void Particle::Release()
{
	for (auto& iter : options)
	{
		if(iter != nullptr)
			delete iter;
	}
	options.clear();
}

void PhysicsOption::Init(string imageStr, FPOINT velocity, float bounciness, float lifeTime)
{

}

void PhysicsOption::Update(Particle& particle, float TimeDelta)
{
	if (bPhysics)
	{
		if (useGravity)
		{
			AddForce({ gravity.x * mass, gravity.y * mass });
		}

		// force 제한
		ClampVector(totalForce, 850.f);

		acceleration = totalForce / mass;
		particle.velocity += acceleration * TimeDelta;

		FPOINT moveVec = particle.velocity * TimeDelta;
		FPOINT nextPos = particle.pos + moveVec;

		Ray ray;
		ray.origin = particle.pos;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastAll(ray, moveLength, out))
		{
			//충돌했으면 중력의 영향을 잠시 초기화
			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			hitDistance = out.distance;

			FPOINT colliderPos = out.collider->GetWorldPos();
			FPOINT colliderScale = out.collider->GetScale();

			// 2. 방향 벡터
			FPOINT toHit = out.point - colliderPos;

			// 스케일 보정한 방향 추정
			float xRatio = toHit.x / (colliderScale.x * 0.5f);
			float yRatio = toHit.y / (colliderScale.y * 0.5f);

			if (fabs(xRatio) > fabs(yRatio))
				hitNormal = { (xRatio < 0 ? -1.f : 1.f), 0.f };
			else
				hitNormal = { 0.f, (yRatio < 0 ? -1.f : 1.f) };

			particle.velocity = Reflect(particle.velocity, hitNormal.Normalized());

			particle.velocity *= bounciness;

			const float STOP_THRESHOLD = 100.f;
			if (fabs(particle.velocity.y) < STOP_THRESHOLD)
				particle.velocity.y = 0.f;

			// 보정 위치
			particle.pos += ray.direction * hitDistance;

			ClampVector(particle.velocity, 450.f);

			if (particle.velocity.Length() < STOP_THRESHOLD && particle.velocity.y > 0.f)
			{
				particle.velocity = { 0.f, 0.f };
				useGravity = false;
				bPhysics = false;
			}
			particle.pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			particle.pos = nextPos;
		}
	}
}

void PhysicsOption::Render(Particle& particle, ID2D1HwndRenderTarget* rt)
{
}

void HomingLinearOption::Render(Particle& p, ID2D1HwndRenderTarget* rt)
{
	if (!p.image)
		return;

	ID2D1Bitmap* bmp = p.image->GetBitmap();
	D2D1_SIZE_F size = bmp->GetSize();

	// 회전 중심을 이미지 중심으로
	D2D1_POINT_2F centerPoint = { size.width / 2.f, size.height / 2.f };

	// 월드 위치 기준 변환
	D2D1::Matrix3x2F mat =
		D2D1::Matrix3x2F::Rotation(angle * 180.f / 3.14159f, centerPoint) *
		D2D1::Matrix3x2F::Translation(p.pos.x - centerPoint.x, p.pos.y - centerPoint.y);

	rt->SetTransform(mat);
	rt->DrawBitmap(bmp, D2D1::RectF(0, 0, size.width, size.height), 1.0f);
	rt->SetTransform(D2D1::Matrix3x2F::Identity()); // 원래대로
}