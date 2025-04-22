#include "pch.h"
#include "Particle.h"
#include "CommonFunction.h"
#include "Image.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "CameraManager.h"
#include "ImageManager.h"
#include "ParticleManager.h"

void Particle::Init(string imageStr, FPOINT pos, float angle, float size, float lifeTime, int atlasX, int atlasY)
{
	this->pos = pos;
	this->angle = angle;
	
	float randOffset = RandomRange(-0.3f, 0.3f); // ±0.3초
	this->lifeTime = lifeTime + randOffset;

	this->size = size / ATLAS_TILE_SIZE;
	
	atlas = { atlasX, atlasY };
	alpha = 1.f;
	isEnd = false;

	image = ImageManager::GetInstance()->FindImage(imageStr);

	if (image == nullptr)
	{
		MessageBox(g_hWnd, L"Image Load Error", L"경고", MB_OK);
	}
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

void Particle::Render(ID2D1RenderTarget* rt)
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

		image->Render(rt, cameraPos.x, cameraPos.y, size, size, atlas.x, atlas.y, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE, alpha, angle);
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

	image = nullptr;
}

void Particle::AddParticleOption(IParticleOption* particleOp)
{
	if (particleOp != nullptr)
	{
		options.push_back(particleOp);
	}
}

void PhysicsOption::Init(FPOINT _velocity, float bounciness)
{
	bPhysics = true;
	useGravity = true;
	velocity = _velocity;
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
		velocity += acceleration * TimeDelta;

		FPOINT moveVec = velocity * TimeDelta;
		FPOINT nextPos = particle.pos + moveVec;

		Ray ray;
		ray.origin = particle.pos;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE))
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

			velocity = Reflect(velocity, hitNormal.Normalized());

			velocity *= bounciness;

			const float STOP_THRESHOLD = 100.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// 보정 위치
			particle.pos += ray.direction * hitDistance;

			ClampVector(velocity, 450.f);

			if (velocity.Length() < STOP_THRESHOLD && velocity.y > 0.f)
			{
				velocity = { 0.f, 0.f };
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

void PhysicsOption::Render(Particle& particle, ID2D1RenderTarget* rt)
{
}

void HomingLinearOption::Render(Particle& p, ID2D1RenderTarget* rt)
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

void AlphaOption::Update(Particle& p, float dt)
{
	p.alpha -= lessAlpha * dt;
}

void AlphaOption::Render(Particle& p, ID2D1RenderTarget* rt)
{
}

void SizeOption::Update(Particle& p, float dt)
{
	p.size -= lessSize * dt;

	if (p.size <= 0.f)
	{
		p.size = 0.f;
	}
}

void SizeOption::Render(Particle& p, ID2D1RenderTarget* rt)
{
}

TrailOption::TrailOption(string trailStr, float interval, float lifeTime)
	: trailParticleStr(trailStr), spawnInterval(interval), trailLifeTime(lifeTime)
{
	timeAccumulator = 0.f;
}

void TrailOption::Update(Particle& p, float dt)
{
	timeAccumulator += dt;
	if (timeAccumulator >= spawnInterval)
	{
		timeAccumulator = 0.f;

		// 잔상 파티클 생성

		if (p.lifeTime <= 0.2f)
		{
			return;
		}

		if (p.size <= 0.15f)
		{
			return;
		}
		Particle* trail = ParticleManager::GetInstance()->GetParticle(trailParticleStr, p.pos, p.angle, p.size + 30.f, trailLifeTime, p.atlas.x, p.atlas.y);

		// 흔적용 옵션 붙이기 (예: 점점 사라짐, 작아짐)
		trail->AddParticleOption(new SizeOption(0.5f));
	}
}

void TrailOption::Render(Particle& p, ID2D1RenderTarget* rt)
{
}
