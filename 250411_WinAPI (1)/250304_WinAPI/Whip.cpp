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
	whipFrame = 10; // �̹��� ������ �ʿ�.

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

	holdImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, whipFrame, 12, 0.75f, 0.75f, isFlip); // ���ǰ�
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
	// �������� �÷��̾�� �޾ƿͼ� ���߰� 
	// �׿� �´� �ݸ��� ���� or ������ �� ������ �ݸ���?
	// �ٵ� �̰� �÷��̾� �������̶� 1��1���� �ƴϰ� �����ؾߵǳ�
	whipFrame = (*(int*)info);
	FPOINT offset = { -50.f, 10.f }; // �浹ü 
	FPOINT posOffset = { -50.f, -10.f }; // ä�� �̹���
	// �°��� GPT�� �����ּ���..

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
