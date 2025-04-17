#include "TestAnimationObject.h"
#include "AnimationManager.h"
#include "PlayerStatus.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "Item.h"
#include "Image.h"

TestAnimationObject::TestAnimationObject()
{
	objectRenderId = RENDER_PLAYER;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	string Temp = typeid(TestAnimationObject).name();
	// GetComponent Ȱ���ؼ� ĳ���� �δ� �����.
	int i = 5;
}

HRESULT TestAnimationObject::Init()
{
	//AnimManager = new AnimationManager();

	//Image* Temp = ImageManager::GetInstance()->FindImage("TestJunYongAttack");

	//Animation* Anim = new Animation(Temp, false);
	//AnimManager->RegisterAnimation("����", Anim);
	//AnimManager->RegisterAnimationEvent("����", "Attack", 3.f, this, &TestAnimationObject::Attack, 123);

	//Temp = ImageManager::GetInstance()->FindImage("TestJunyongWalk");
	//	/*ImageManager::GetInstance()->AddImage(
	//	"TestJunyongWalk", L"Image/TestJunyongWalk.bmp", 904, 113,
	//	8, 1, true, RGB(255, 0, 255));*/

	//Anim = new Animation(Temp);
	//AnimManager->RegisterAnimation("�ȱ�", Anim, true);
	//Count = 0;

	image = ImageManager::GetInstance()->FindImage("TestJunYongAttack");

	status = new PlayerStatus();
	Pos = { 500, 100 };
	BoxCollider* col = new BoxCollider({ 0,0 }, { 150,150 }, this);
	return S_OK;
}

void TestAnimationObject::Update(float TimeDelta) 
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('Q'))
	{
		//AnimManager->ChangeAnimation("����");
		objectRenderId = RENDER_HOLD;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('W'))
	{
		//AnimManager->ChangeAnimation("����");
		objectRenderId = RENDER_BACKGROUND;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
	{
		objectRenderId = RENDER_PLAYER;
		//AnimManager->ChangeAnimation("����");
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown('R'))
	{
		objectRenderId = RENDER_PLAYERCLIMB;
		//AnimManager->ChangeAnimation("����");
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
		//pair<GameObject*, GameObject*> temp = CollisionManager::GetInstance()->GetInteractObjectPairInCircle(this, 30.f);
		//if ((temp.first && temp.second))
		//{
		//	GameObject* dest = temp.second;
		//	dest->SetObjectRenderId(RENDER_HOLD);
		//	dynamic_cast<Item*>(dest)->Equip(this);
		//}
	}

	testAbleHold = false;

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_SPACE))
	{
		testAbleHold = true;
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
		if (dynamic_cast<Item*>(obj) && testAbleHold && !testHold)
		{
			Item* temp = dynamic_cast<Item*>(obj);
			switch (temp->GetItemType())
			{
			case ItemType::TYPE_ONCE:
				temp->Equip(status->GetInfo());
				break;
			case ItemType::TYPE_LIMIT:
				temp->Equip(this);
				break;
			case ItemType::TYPE_ALWAYS:
				temp->Equip(this);
				break;
			}

			testHold = true;
		}
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
