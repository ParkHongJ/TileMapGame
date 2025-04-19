#include "pch.h"
#include "Image.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "GunBullet.h"

GunBullet::GunBullet() : image(nullptr)
{

}

GunBullet::~GunBullet()
{

}

HRESULT GunBullet::Init()
{
	image = ImageManager::GetInstance()->FindImage("items");

	BoxCollider* col = new BoxCollider({ 0,0 }, { 30,30 }, CollisionMaskType::ITEM, this);
	interactState = INTERACTSTATE::INTERACT_ABLE;

	//SetDrop();
	return S_OK;
}

void GunBullet::Update(float TimeDelta)
{
	Move(TimeDelta);
	//Pos.x += 30.f * TimeDelta;

}

void GunBullet::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
	image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 14);
}

void GunBullet::Release()
{
}

void GunBullet::Detect(GameObject* obj)
{
}

void GunBullet::Move(float TimeDelta)
{
	if (bPhysics)
	{
		if (useGravity)
		{
			AddForce({ gravity.x * mass, gravity.y * mass });
		}

		// force 제한
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

		if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE, true, 1.f))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
		{
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

			FPOINT perturbedNormal = RotateVector(hitNormal, RandomRange(-50.f, 50.f));
			velocity = Reflect(velocity, /*perturbedNormal.Normalized()*/hitNormal.Normalized());

			velocity *= bounciness;

			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			const float STOP_THRESHOLD = 100.f;
			if (fabs(velocity.x) < STOP_THRESHOLD)
				velocity.x = 0.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// 보정 위치
			Pos += ray.direction * hitDistance;

			ClampVector(velocity, 350.f);

			if (velocity.Length() < 130.f)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
				bPhysics = false;
			}

			// 살짝 밀기 (겹침 방지)

			Pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			Pos = nextPos;
		}
	}
}

void GunBullet::SetDrop()
{
	float angleRad = RandomRange(0.0f, 2.0f * 3.141592f); // 0 ~ 360도 (라디안)
	float speed = RandomRange(250.0f, 450.0f);            // 속도도 랜덤
	//float speed = 300.f;            // 속도도 랜덤

	velocity =
	{
		//cosf(angleRad) * speed,  // 135도 (왼쪽 위)
		//-sinf(angleRad) * speed

		cosf(DEG_TO_RAD(135.f)) * speed,  // 135도 (왼쪽 위)
		-sinf(DEG_TO_RAD(135.f)) * speed
	};
	acceleration = { 0, 0 };  // 가속도
	totalForce = { 0.f,0.f };

	useGravity = true;
	bPhysics = true;
}
