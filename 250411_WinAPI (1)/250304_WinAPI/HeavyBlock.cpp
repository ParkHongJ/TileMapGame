#include "pch.h"
#include "HeavyBlock.h"
#include "CommonFunction.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "Image.h"
#include "Character.h"
#include "GameManager.h"
#include "ImageManager.h"
#include "Monster.h"
HRESULT HeavyBlock::Init()
{
	objectScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;

	collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, CollisionMaskType::TILE, this);
	heavyBlockImage = ImageManager::GetInstance()->FindImage("Trap");
	Pos.x = 1050.f;
	Pos.y = 200.f;

	bPhysics = true;
	useGravity = true;

	objectRenderId = RENDERORDER::RENDER_TILE;
	
	objectName = OBJECTNAME::HEAVYBLOCK;

	return S_OK;
}

void HeavyBlock::Release()
{
}

void HeavyBlock::Update(float TimeDelta)
{
	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ADD))
	//{
	//	paths.clear();
	//	if (bPhysics != true)
	//	{
	//		bPhysics = true;
	//	}
	//	//bPhysics = !bPhysics;
	//	totalForce = { 0.f,0.f };
	//	acceleration = { 0.f,0.f };
	//	start = GameManager::GetInstance()->FindClosestJumpNode(Pos);
	//	Character* player = ObjectManager::GetInstance()->GetPlayer();

	//	end = GameManager::GetInstance()->FindClosestJumpNode(player->GetPos());
	//	paths = GameManager::GetInstance()->FindPath(start, end);
	//}

	/*if (paths.empty()) return;
	if (pathIndex >= paths.size()) 
	{ 
		paths.clear();
		pathIndex = 0;
		return; 
	}*/

	{
		//FPOINT targetPos = paths[pathIndex]->worldPos;

		//JumpNode* target = paths[pathIndex];

		//FPOINT to = target->worldPos;
		//FPOINT dir = to - Pos;
		//float dist = dir.Length();

		//if (dist < 4.f)  // ���������� ���� ���
		//{
		//	pathIndex++;
		//	return;
		//}

		//bool needToFall = (to.y > Pos.y + 4.f);    // ��ǥ�� �Ʒ����̴� �� ���� ��Ȳ
		//bool needToJump = (to.y < Pos.y - 4.f);    // ��ǥ�� �����̴� �� ���� ��Ȳ
		//bool isSameX = fabs(Pos.x - to.x) < 4.f;   // ��ǥ X���� ���ĵƴ°�?

		//// ============================
		//// [1] ���� ó��
		//// ============================
		//if (needToFall)
		//{
		//	if (!isSameX)
		//	{
		//		// ���� X ���� �� �� �� X ���ĸ� ����
		//		velocity.x = (to.x > Pos.x ? 1.f : -1.f) * speed;
		//		velocity.y = 0.f; // Y�� �������� ���� (�߷¸� ����)
		//	}
		//	else
		//	{
		//		// ���� �Ϸ� �� �ڿ� ���� ���
		//		Pos.x = floorf(Pos.x);
		//		velocity.x = 0.f;
		//		// �߷��� �Ʒ� elsewhere ����
		//	}
		//}

		//// ============================
		//// [2] ����
		//// ============================
		//else if (needToJump && bGround)
		//{
		//	bJumping = true;
		//	velocity.y = jumpPower;
		//	velocity.x = (to.x > Pos.x ? 1.f : -1.f) * speed;
		//}

		//// ============================
		//// [3] ���� ���� �� �ȱ�
		//// ============================
		//else
		//{
		//	velocity.y = 0.f;
		//	velocity.x = (to.x > Pos.x ? 1.f : -1.f) * speed;
		//}
	}

	//FPOINT dir = targetPos - Pos;

	//float dist = dir.Length();
	//if (dist < 4.f)  // ���������� ���� ���
	//{
	//	pathIndex++;
	//	return;
	//}

	//dir.Normalize();

	//// �� ���� ���� ���� �˻� (����/����)
	//if (fabs(dir.y) > 0.5f)
	//{
	//	if (dir.y < 0.f && bGround)  // ���� �� ����
	//	{
	//		totalForce = { 0.f,0.f };
	//		acceleration = { 0.f,0.f };
	//		velocity.y = jumpPower;
	//	}
	//	else if (dir.y > 0.f) // �Ʒ� �� �׳� �߷¿� �ñ�
	//	{
	//		// ���� ���̹Ƿ� X�̵� ���� ����
	//	}
	//}

	//// �� �¿� �̵�
	//if (fabs(dir.x) > 0.2f)
	//{
	//	velocity.x = dir.x * speed;
	//}

	Pos += velocity * TimeDelta;

	// �߷� ����
	UpdatePhysics(TimeDelta);

	if (bJumping)
	{

	}

}

void HeavyBlock::LateUpdate(float TimeDelta)
{
}

void HeavyBlock::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();
	heavyBlockImage->Render(renderTarget, cameraPos.x, cameraPos.y, objectScale, objectScale, 0, 0, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);

	/*for (int i = 0; i < GameManager::jumpNodes.size(); ++i)
	{
		FPOINT nodePos = GameManager::jumpNodes[i]->worldPos;

		cameraPos = nodePos + CameraManager::GetInstance()->GetPos();
		DrawCenteredRect(renderTarget, cameraPos, 15.f, D2D1::ColorF::Blue);
	}

	for (JumpNode* node : GameManager::jumpNodes)
	{
		for (JumpNode* neighbor : node->neighbors)
		{
			FPOINT nodeCameraPos = node->worldPos + CameraManager::GetInstance()->GetPos();
			FPOINT neighborCameraPos = neighbor->worldPos + CameraManager::GetInstance()->GetPos();

			DrawLine(renderTarget, nodeCameraPos, neighborCameraPos, D2D1::ColorF::Blue);
		}
	}

	{

		for (int i = 0; i < paths.size(); i++)
		{
			FPOINT nodePos = paths[i]->worldPos;

			cameraPos = nodePos + CameraManager::GetInstance()->GetPos();
			DrawCenteredRect(renderTarget, cameraPos, 15.f, D2D1::ColorF::Green, 2.f);
		}

		if (start)
		{
			FPOINT nodePos = start->worldPos;

			cameraPos = nodePos + CameraManager::GetInstance()->GetPos();
			DrawCenteredRect(renderTarget, cameraPos, 15.f, D2D1::ColorF::Red);
		}
		if (end)
		{
			FPOINT nodePos = end->worldPos;

			cameraPos = nodePos + CameraManager::GetInstance()->GetPos();
			DrawCenteredRect(renderTarget, cameraPos, 15.f, D2D1::ColorF::Yellow);
		}

	}*/
}

void HeavyBlock::Detect(GameObject* obj)
{
	if (velocity.y >= 50.f)
	{
		
		int a = 10;

		if (auto monster = dynamic_cast<Monster*>(obj))
		{
			monster->DeadStarEffect();
			
		}
		obj->SetDestroy();
	}
}

bool HeavyBlock::OnPush(FPOINT dir, float strength)
{
	if (bGround == false)
	{
		return false;
	}
	// ���� ���� ���� (��: �¿츸)
	if (fabs(dir.y) > 0.5f) return false;

	velocity = dir * strength;

	float timeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

	FPOINT nextPos = Pos + velocity * timeDelta;

	int tileIndexX = static_cast<int>(nextPos.x / GAME_TILE_SIZE);
	int tileIndexY = static_cast<int>(nextPos.y / GAME_TILE_SIZE);

	//�̵� ����
	if (GameManager::GetInstance()->HasTile(tileIndexX, tileIndexY) == false)
	{
		Pos = nextPos;
		return true;
	}
	return false;
}

void HeavyBlock::UpdatePhysics(float TimeDelta)
{
	if (bPhysics)
	{
		if (useGravity)
		{
			AddForce({ gravity.x * mass, gravity.y * mass });
		}

		// force ����
		ClampVector(totalForce, 850.f);

		acceleration = totalForce / mass;
		velocity += acceleration * TimeDelta;

		FPOINT moveVec = velocity * TimeDelta;
		FPOINT nextPos = Pos + moveVec;

		Ray rayLeft;
		Ray rayRight;

		rayLeft.origin = Pos;
		rayLeft.origin.x -= GAME_TILE_SIZE * 0.4f;
		rayLeft.origin.y += GAME_TILE_SIZE * 0.5f;
		rayLeft.direction = moveVec.Normalized();

		rayRight.origin = Pos;
		rayRight.origin.x += GAME_TILE_SIZE * 0.4f;
		rayRight.origin.y += GAME_TILE_SIZE * 0.5f;
		rayRight.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;

		RaycastHit outLeft;
		RaycastHit outRight;

		bGround = CollisionManager::GetInstance()->RaycastType(rayLeft, moveLength, outLeft, CollisionMaskType::TILE, this) ||
			CollisionManager::GetInstance()->RaycastType(rayRight, moveLength, outRight, CollisionMaskType::TILE, this);

		if (bGround)
		{
			bJumping = false;
			//�浹������ �߷��� ������ ��� �ʱ�ȭ
			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			hitDistance = min(outLeft.distance, outRight.distance);

			const float STOP_THRESHOLD = 150.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// ���� ��ġ
			Pos += rayRight.direction * hitDistance;

			ClampVector(velocity, 150.f);

			if (velocity.Length() <= STOP_THRESHOLD)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
			}
		}
		else
		{
			useGravity = true;
			Pos = nextPos;
		}
	}
}
