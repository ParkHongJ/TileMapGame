#include "Image.h"
#include "Item.h"
#include "CollisionManager.h"


Item::Item() : isLock(false), isHold(false), 
price(0), holdImage(nullptr), 
dropImage(nullptr), moveDir({ 0,1 }), 
moveReverseDir({ 1,1 }),prePos({ 0,0 }), RayDis(35.f), 
state(ItemState::STATE_UNEQUIP)
{

}

Item::~Item()
{

}

HRESULT Item::Init()
{

	return S_OK;
}

void Item::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		movePower = { 500.f, 300.f };
	}

	if (false == isHold)
	{
		DropMove(TimeDelta);
	}

	prePos = Pos;
}

void Item::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//image->FrameRender(renderTarget, Pos.x, Pos.y, 0, 5); // ¿”¿«∞™
}

void Item::Release()
{

}

void Item::Equip()
{
	ChangeState(ItemState::STATE_EQUIP);

}

void Item::Equip(void* info)
{
	ChangeState(ItemState::STATE_EQUIP);
}

void Item::UnEquip()
{
	ChangeState(ItemState::STATE_UNEQUIP);
}

void Item::UnEquip(void* info)
{
	ChangeState(ItemState::STATE_UNEQUIP);
}

void Item::Use()
{
	if (ItemState::STATE_EQUIP == state)
	{
		return;
	}


}

void Item::DropMove(float TimeDelta)
{
	DropMoveX(TimeDelta);
	DropMoveY(TimeDelta);

}

void Item::DropMoveX(float TimeDelta)
{
	Ray ray;
	ray.origin = Pos;

	moveDir = {  Pos.x - prePos.x, Pos.y };
	moveDir.x *= moveReverseDir.x;

	ray.direction = moveDir.Normalized();
	
	FPOINT rayPos = Pos;
	rayPos.x += RayDis * moveReverseDir.x;
	float nextX = rayPos.x + moveReverseDir.x * movePower.x * TimeDelta;
	ray = { rayPos, {moveDir.x, 0.f} };
	RaycastHit out;
	bool hit = CollisionManager::GetInstance()->RaycastAll(ray, moveReverseDir.x * movePower.x * TimeDelta, out, true, 1.f);

	if (hit && nextX >= out.point.x)
	{
		// √Êµπ ¡ˆ¡°ø°º≠ ∏ÿ√„

		//Pos.x = out.point.x - 35.f;
		moveReverseDir.x *= -1;
		movePower.x /= 3.f;
		//movePower.x = 0.f;
	}

	else 
	{
		Pos.x = nextX - (RayDis * moveReverseDir.x);
		//Pos.x = nextX - 35.f;
		//Pos.y = nextY - 35.f;
	}
}

void Item::DropMoveY(float TimeDelta)
{
	Ray ray;
	ray.origin = Pos;

	moveDir = { Pos.x, Pos.y - prePos.y };
	moveDir.y *= moveReverseDir.y;

	ray.direction = moveDir.Normalized();

	FPOINT rayPos = Pos;
	rayPos.y += RayDis * moveReverseDir.y;
	float nextY = rayPos.y + moveReverseDir.y * movePower.y * TimeDelta;
	ray = { rayPos, {0.f, moveDir.y} };
	RaycastHit out;
	bool hit = CollisionManager::GetInstance()->RaycastAll(ray, moveReverseDir.y * movePower.y * TimeDelta, out, true, 1.f);

	if (hit && nextY >= out.point.y)
	{
		// √Êµπ ¡ˆ¡°ø°º≠ ∏ÿ√„

		//Pos.x = out.point.x - 35.f;
		moveReverseDir.y *= -1;
		movePower.y /= 3.f;
	}

	else
	{
		Pos.y = nextY - (RayDis * moveReverseDir.y);
		//Pos.x = nextX - 35.f;
		//Pos.y = nextY - 35.f;
	}
}



void Item::ChangeState(ItemState state)
{
	switch (state)
	{
	case ItemState::STATE_UNEQUIP:
		isHold = false;
		break;
	case ItemState::STATE_EQUIP:
		isLock = false;
		isHold = true;
		break;
	}
}
