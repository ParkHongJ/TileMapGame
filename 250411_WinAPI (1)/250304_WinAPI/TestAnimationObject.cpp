#include "TestAnimationObject.h"
#include "AnimationManager.h"
#include "PlayerStatus.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Item.h"
#include "Image.h"
#include "Bomb.h"

TestAnimationObject::TestAnimationObject()
{
	objectRenderId = RENDER_PLAYER;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	// GetComponent 활용해서 캐스팅 부담 덜어보기.

	itemOffsetPos = { 30.f,10.f };
	int i = 5;
}

HRESULT TestAnimationObject::Init()
{
	//AnimManager = new AnimationManager();

	//Image* Temp = ImageManager::GetInstance()->FindImage("TestJunYongAttack");

	//Animation* Anim = new Animation(Temp, false);
	//AnimManager->RegisterAnimation("공격", Anim);
	//AnimManager->RegisterAnimationEvent("공격", "Attack", 3.f, this, &TestAnimationObject::Attack, 123);

	//Temp = ImageManager::GetInstance()->FindImage("TestJunyongWalk");
	//	/*ImageManager::GetInstance()->AddImage(
	//	"TestJunyongWalk", L"Image/TestJunyongWalk.bmp", 904, 113,
	//	8, 1, true, RGB(255, 0, 255));*/

	//Anim = new Animation(Temp);
	//AnimManager->RegisterAnimation("걷기", Anim, true);
	//Count = 0;

	image = ImageManager::GetInstance()->FindImage("Tae_Player");

	status = new PlayerStatus();
	Pos = { 500, 100 };
	BoxCollider* col = new BoxCollider({ 0,0 }, { 150,150 }, this);
	return S_OK;
}

void TestAnimationObject::Update(float TimeDelta) 
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('Q'))
	{
		//AnimManager->ChangeAnimation("공격");
		objectRenderId = RENDER_HOLD;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		//AnimManager->ChangeAnimation("공격");
		objectRenderId = RENDER_BACKGROUND;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
	{
		objectRenderId = RENDER_PLAYER;
		//AnimManager->ChangeAnimation("공격");
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('R'))
	{
		objectRenderId = RENDER_PLAYERCLIMB;
		//AnimManager->ChangeAnimation("공격");
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
	{
		Pos.x -= 300.f * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
	{
		Pos.x += 300.f * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
	{
		Pos.y -= 300.f * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
	{
		Pos.y += 300.f * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		if (HoldItem)
		{

			HoldItem->UnEquip(&Pos);
			HoldItem = nullptr;
		}
		//pair<GameObject*, GameObject*> temp = CollisionManager::GetInstance()->GetInteractObjectPairInCircle(this, 30.f);
		//if ((temp.first && temp.second))
		//{
		//	GameObject* dest = temp.second;
		//	dest->SetObjectRenderId(RENDER_HOLD);
		//	dynamic_cast<Item*>(dest)->Equip(this);
		//}
	}


	if (KeyManager::GetInstance()->IsOnceKeyDown('K') && 0 < status->GetBombCount())
	{
		status->MinusBombCount();
		Bomb* temp = new Bomb();
		ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
		FPOINT offset = { 10,0 };
		temp->SetPos(Pos + offset);
	}

	testAbleHold = false;

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_SPACE))
	{
		testAbleHold = true;
	}

	if (HoldItem)
	{
		HoldItem->SetPos(Pos + itemOffsetPos);
	}
}

void TestAnimationObject::Render(ID2D1HwndRenderTarget* renderTarget)
{
	image->FrameRender(renderTarget, Pos.x, Pos.y, 0, 0);

//	AnimManager->Render(renderTarget);
}

void TestAnimationObject::Release()
{
	if (AnimManager)
	{
		AnimManager->Release();
		delete AnimManager;
		AnimManager = nullptr;
	}

	if (status)
	{
		delete status;
		status = nullptr;
	}
}

void TestAnimationObject::Detect(GameObject* obj)
{
	if (obj)
	{
		//TakeCollision(1.f);

		if (auto item = obj->GetType<Item>())
		{
			if (testAbleHold)
			{
  				switch (item->GetItemType())
				{
				case ItemType::TYPE_LIMIT:
					HoldItem = item;
					break;
				case ItemType::TYPE_ALWAYS:
					HoldItem = item;
					break;
				}

				testHold = true;
			}

			else if (ItemType::TYPE_ONCE == item->GetItemType())
			{
				item->Equip(status->GetInfo());
			}
		}

		//if ("Item" == obj->GetName())
		//{
		//	if (testAbleHold && !testHold)
		//	{
		//		switch (item->GetItemType())
		//		{
		//		case ItemType::TYPE_ONCE:
		//			item->Equip(status->GetInfo());
		//			break;
		//		case ItemType::TYPE_LIMIT:
		//			HoldItem = item;
		//			break;
		//		case ItemType::TYPE_ALWAYS:
		//			HoldItem = item;
		//			break;
		//		}
		//	}

		//	testHold = true;
		//}
	}

}

void TestAnimationObject::Attack(int _Test)
{
	int i = _Test;
	int j = 5;
}

void TestAnimationObject::Guard()
{
	++Count;
}

void TestAnimationObject::Hit()
{
	int i = 5;
}
