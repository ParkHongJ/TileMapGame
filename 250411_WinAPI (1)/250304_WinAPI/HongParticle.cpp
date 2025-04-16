#include "HongParticle.h"
#include "CommonFunction.h"
#include "CollisionManager.h"
#include "Collider.h"
HRESULT HongParticle::Init()
{
	float angleRad = RandomRange(0.0f, 2.0f * 3.141592f); // 0 ~ 360�� (����)
	float speed = RandomRange(100.0f, 300.0f);            // �ӵ��� ����

	velocity =
	{
		cosf(angleRad) * speed,  // 135�� (���� ��)
		-sinf(angleRad) * speed
	};
	acceleration = { 0, 0 };  // ���ӵ�
	totalForce = { 0.f,0.f };

	useGravity = true;
	bPhysics = true;
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

		if (CollisionManager::GetInstance()->RaycastAll(ray, moveLength, out))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
		{
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

			FPOINT perturbedNormal = RotateVector(hitNormal, RandomRange(-50.f, 50.f));
			velocity = Reflect(velocity, perturbedNormal.Normalized()/*hitNormal.Normalized()*/);

			velocity *= bounciness;

			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			const float STOP_THRESHOLD = 100.f;
			if (fabs(velocity.x) < STOP_THRESHOLD)
				velocity.x = 0.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// ���� ��ġ
			Pos += ray.direction * hitDistance;

			ClampVector(velocity, 350.f);

			if (velocity.Length() < 130.f)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
				bPhysics = false;
			}

			// ��¦ �б� (��ħ ����)

			Pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			Pos = nextPos;
		}

		// force �ʱ�ȭ
		//totalForce = { 0.f, 0.f };
	}
}

void HongParticle::Render(ID2D1HwndRenderTarget* renderTarget)
{
	DrawCenteredRect(renderTarget, Pos, 10.f, D2D1::ColorF::Magenta);

}
