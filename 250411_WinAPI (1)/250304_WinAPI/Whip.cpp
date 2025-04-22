#include "pch.h"
#include "Whip.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Monster.h"

Whip::Whip()
{
}

Whip::~Whip()
{
}

HRESULT Whip::Init()
{
	//dropImage = ImageManager::GetInstance()->FindImage("Tae_Player");
	holdImage = ImageManager::GetInstance()->FindImage("Tae_Player");
	Pos = { 300, 100 };

	//col = new BoxCollider({ 0,0 }, { 200,200 }, CollisionMaskType::PLAYERATTACK, this);
	whipFrame = 10; // 이미지 스케일 필요.

	return S_OK;
}

void Whip::Update(float TimeDelta)
{
	bActive = false;
	//objectRenderId = RENDER_HOLD;

	//if (KeyManager::GetInstance()->IsOnceKeyDown('M'))
	//{
	//	++whipFrame;
	//	if (15 < whipFrame)
	//	{
	//		whipFrame = 10;
	//	}
	//}

	//FrameUpdate(TimeDelta);
}

void Whip::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	holdImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, whipFrame, 12, objectScale.x * 0.75f, objectScale.y * 0.75f, isFlip); // 임의값
}

void Whip::Release()
{

}

void Whip::Equip()
{
}

void Whip::Equip(void* info)
{
}

void Whip::Equip(GameObject* owner)
{
}

void Whip::UnEquip()
{
}

void Whip::UnEquip(void* info)
{
}

void Whip::Use()
{

}

void Whip::Use(void* info)
{
	// 프레임을 플레이어에서 받아와서 맞추고 
	// 그에 맞는 콜리전 갱신 or 프레임 한 번에만 콜리전?
	// 근데 이거 플레이어 프레임이랑 1대1대응 아니고 보정해야되네
	whipFrame = (*(int*)info);
	FPOINT posOffset = { 0.f, 0.f }; // 채찍 이미지
	bActive = true;
	switch (whipFrame)
	{
	case 0:
		posOffset = {-50.f, -20.f};
		isHit = false;
		break;
	case 1:
		posOffset = { -30.f, -20.f };
		break;
	case 2:
		posOffset = { 0.f, -10.f };
		break;
	case 3:

		posOffset = { 20.f, 10.f };

		if (!isHit)
		{
			Ray ray;
			ray.origin = Pos + FPOINT{ 0.f, 10.f };
			ray.direction = { 1.f,0.f };
			if (isFlip)
			{
				ray.direction = { -1.f,0.f };
			}

			float moveLength = 100.f;
			RaycastHit out;

			if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::MONSTER, this, true, 1.f))
			{
				isHit = true;
			}
		}

		break;
	case 4:
		posOffset = { 40.f, 20.f };
		if (!isHit)
		{
			Ray ray;
			ray.origin = Pos + FPOINT{0.f, 10.f};
			ray.direction = { 1.f,0.f };
			if (isFlip)
			{
				ray.direction = { -1.f,0.f };
			}

			float moveLength = 100.f;
			RaycastHit out;

			if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::MONSTER, this, true, 1.f))
			{
				isHit = true;
			}
		}
		//posOffset = { -50.f, 0.f };
		//posOffset = { 100.f, 30.f };
		break;
	case 5: // 여까지 안오넹
		//posOffset = { 150.f, -20.f };

		//posOffset = { 100.f, 30.f };
		break;
	case 6:
		break;

	case 7:
		break;
	case 8:
		break;
	default:
		break;
	}

	if (isFlip)
	{
		//offset.x *= -1;
		posOffset.x *= -1;
	}

	//col->SetOffset(offset);
	Pos += posOffset;
	whipFrame += 10;
	curFrameIndexX = whipFrame;
}

void Whip::Detect(GameObject* obj)
{
	//objectRenderId = RENDER_HOLD;
	//interactState = INTERACTSTATE::INTERACT_UNABLE;
	if (nullptr != dynamic_cast<Monster*>(obj))
	{
		obj->SetDestroy();
	}

}

void Whip::DropMove(float TimeDelta)
{
}

void Whip::DropMoveX(float TimeDelta)
{
}

void Whip::DropMoveY(float TimeDelta)
{
}

void Whip::FrameUpdate(float TimeDelta)
{

}
