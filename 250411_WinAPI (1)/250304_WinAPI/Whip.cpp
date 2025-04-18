#include "Whip.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"

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

	BoxCollider* col = new BoxCollider({ -30,0 }, { 50,50 }, this);
	whipFrame = 10; // 이미지 스케일 필요.

	return S_OK;
}

void Whip::Update(float TimeDelta)
{
	//objectRenderId = RENDER_HOLD;

	if (KeyManager::GetInstance()->IsOnceKeyDown('M'))
	{
		++whipFrame;
		if (15 < whipFrame)
		{
			whipFrame = 10;
		}
	}

	FrameUpdate(TimeDelta);
}

void Whip::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	holdImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, whipFrame, 12, 0.75f, 0.75f); // 임의값
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


}

void Whip::Detect(GameObject* obj)
{
	//objectRenderId = RENDER_HOLD;
	//interactState = INTERACTSTATE::INTERACT_UNABLE;
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
