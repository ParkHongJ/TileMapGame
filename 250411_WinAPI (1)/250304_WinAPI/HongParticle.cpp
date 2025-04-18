#include "pch.h"
#include "HongParticle.h"
#include "CommonFunction.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
HRESULT HongParticle::Init()
{
	float angleRad = RandomRange(0.0f, 2.0f * 3.141592f); // 0 ~ 360�� (����)
	float speed = RandomRange(450.0f, 550.0f);            // �ӵ��� ����

	velocity =
	{
		cosf(angleRad) * speed,  // 135�� (���� ��)
		-sinf(angleRad) * speed
	};
	acceleration = { 0, 0 };  // ���ӵ�
	totalForce = { 0.f,0.f };

	useGravity = true;
	bPhysics = true;

	blood = ImageManager::GetInstance()->FindImage("Effect");
    return S_OK;
}

void HongParticle::Release()
{
}

void HongParticle::Update(float TimeDelta)
{
	if (bPhysics)
	{
		if (useGravity)
		{
			AddForce({ gravity.x * mass, gravity.y * mass });
		}

		// force ����
		ClampVector(totalForce, 450.f);

		acceleration = totalForce / mass;
		velocity += acceleration * TimeDelta;

		FPOINT moveVec = { velocity.x * TimeDelta, velocity.y * TimeDelta };
		FPOINT nextPos = { Pos.x + moveVec.x, Pos.y + moveVec.y };

		Ray ray;
		ray.origin = Pos;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastAll(ray, moveLength, out))
		{
			//�浹������ �߷��� ������ ��� �ʱ�ȭ
			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			hitDistance = out.distance;

			FPOINT colliderPos = out.collider->GetWorldPos();
			FPOINT colliderScale = out.collider->GetScale();

			// 2. ���� ����
			FPOINT toHit = out.point - colliderPos;

			// ������ ������ ���� ����
			float xRatio = toHit.x / (colliderScale.x * 0.5f);
			float yRatio = toHit.y / (colliderScale.y * 0.5f);

			if (fabs(xRatio) > fabs(yRatio))
				hitNormal = { (xRatio < 0 ? -1.f : 1.f), 0.f };
			else
				hitNormal = { 0.f, (yRatio < 0 ? -1.f : 1.f) };

			velocity = Reflect(velocity, hitNormal.Normalized());
			
			// �ݻ� �ӵ��� �ʹ� ���ϸ�, ��¦ ����
			/*const float MIN_BOUNCE_SPEED = 150.f;
			if (velocity.Length() < MIN_BOUNCE_SPEED)
			{
				velocity = velocity.Normalized() * MIN_BOUNCE_SPEED;
			}*/
			
			velocity *= bounciness;


			const float STOP_THRESHOLD = 100.f;
			/*if (fabs(velocity.x) < STOP_THRESHOLD)
				velocity.x = 0.f;*/
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// ���� ��ġ
			Pos += ray.direction * hitDistance;

			ClampVector(velocity, 350.f);

			if (velocity.Length() < STOP_THRESHOLD && velocity.y > 0.f)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
				bPhysics = false;
			}
			Pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			Pos = nextPos;
		}
	}
}

void HongParticle::Render(ID2D1HwndRenderTarget* renderTarget)
{

	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();
	//DrawCenteredRect(renderTarget, cameraPos, 10.f, D2D1::ColorF::Magenta);

	float EffectSize = 30.f / ATLAS_TILE_SIZE;
	blood->Render(renderTarget, cameraPos.x, cameraPos.y, EffectSize, EffectSize, 0, 0, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);

	wstring velocityText = L"x : " + to_wstring(velocity.x);
	DrawD2DText(renderTarget, velocityText.c_str(), cameraPos.x, cameraPos.y + 15.f);
}
