#include "pch.h"
#include "Whip.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
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

	col = new BoxCollider({ 0,0 }, { 200,200 }, CollisionMaskType::PLAYERATTACK, this);
	whipFrame = 10; // 이미지 스케일 필요.

	return S_OK;
}

void Whip::Update(float TimeDelta)
{
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

void Whip::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	holdImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, whipFrame, 12, 0.75f, 0.75f, isFlip); // 임의값
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
	FPOINT offset = { -50.f, 10.f }; // 충돌체 
	FPOINT posOffset = { -50.f, -10.f }; // 채찍 이미지
	// 태관님 GPT를 굴려주세요..

	switch (whipFrame)
	{
	case 0:
		offset = { -50.f, 10.f };
		//posOffset = {}
		//posOffset = { 100.f, 30.f };
		break;
	case 1:
		offset = { -50, 10.f };
		//posOffset = { -50.f, 0.f };
		//posOffset = { 100.f, 30.f };

		break;
	case 2:
		offset = { -50, 10.f };
		posOffset = { 20.f, -10.f };
		//posOffset = { -50.f, 0.f };
		//posOffset = { 100.f, 30.f };

		break;
	case 3:
		offset = { -50, 10.f };
		//posOffset = { -50.f, 0.f };
		//posOffset = { 100.f, 30.f };

		break;
	case 4:
		offset = { -50, 10.f };
		//posOffset = { -50.f, 0.f };
		//posOffset = { 100.f, 30.f };

		break;
	case 5:
		//posOffset = { -50.f, 0.f };
		//posOffset = { 100.f, 30.f };
		break;
	case 6:
		//offset = { -100000, -30000 };
		break;
	default:
		break;
	}

	if (isFlip)
	{
		offset.x *= -1;
		posOffset.x *= -1;
	}

	col->SetOffset(offset);
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
