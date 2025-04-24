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

		//if (dist < 4.f)  // 도달했으면 다음 노드
		//{
		//	pathIndex++;
		//	return;
		//}

		//bool needToFall = (to.y > Pos.y + 4.f);    // 목표가 아래쪽이다 → 낙하 상황
		//bool needToJump = (to.y < Pos.y - 4.f);    // 목표가 위쪽이다 → 점프 상황
		//bool isSameX = fabs(Pos.x - to.x) < 4.f;   // 목표 X까지 정렬됐는가?

		//// ============================
		//// [1] 낙하 처리
		//// ============================
		//if (needToFall)
		//{
		//	if (!isSameX)
		//	{
		//		// 아직 X 정렬 안 됨 → X 정렬만 수행
		//		velocity.x = (to.x > Pos.x ? 1.f : -1.f) * speed;
		//		velocity.y = 0.f; // Y는 움직이지 않음 (중력만 적용)
		//	}
		//	else
		//	{
		//		// 정렬 완료 → 자연 낙하 허용
		//		Pos.x = floorf(Pos.x);
		//		velocity.x = 0.f;
		//		// 중력은 아래 elsewhere 적용
		//	}
		//}

		//// ============================
		//// [2] 점프
		//// ============================
		//else if (needToJump && bGround)
		//{
		//	bJumping = true;
		//	velocity.y = jumpPower;
		//	velocity.x = (to.x > Pos.x ? 1.f : -1.f) * speed;
		//}

		//// ============================
		//// [3] 같은 높이 → 걷기
		//// ============================
		//else
		//{
		//	velocity.y = 0.f;
		//	velocity.x = (to.x > Pos.x ? 1.f : -1.f) * speed;
		//}
	}

	//FPOINT dir = targetPos - Pos;

	//float dist = dir.Length();
	//if (dist < 4.f)  // 도달했으면 다음 노드
	//{
	//	pathIndex++;
	//	return;
	//}

	//dir.Normalize();

	//// ★ 수직 방향 먼저 검사 (점프/낙하)
	//if (fabs(dir.y) > 0.5f)
	//{
	//	if (dir.y < 0.f && bGround)  // 위로 → 점프
	//	{
	//		totalForce = { 0.f,0.f };
	//		acceleration = { 0.f,0.f };
	//		velocity.y = jumpPower;
	//	}
	//	else if (dir.y > 0.f) // 아래 → 그냥 중력에 맡김
	//	{
	//		// 낙하 중이므로 X이동 억제 가능
	//	}
	//}

	//// ★ 좌우 이동
	//if (fabs(dir.x) > 0.2f)
	//{
	//	velocity.x = dir.x * speed;
	//}

	Pos += velocity * TimeDelta;

	// 중력 적용
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
	// 방향 제한 가능 (예: 좌우만)
	if (fabs(dir.y) > 0.5f) return false;

	velocity = dir * strength;

	float timeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

	FPOINT nextPos = Pos + velocity * timeDelta;

	int tileIndexX = static_cast<int>(nextPos.x / GAME_TILE_SIZE);
	int tileIndexY = static_cast<int>(nextPos.y / GAME_TILE_SIZE);

	//이동 가능
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

		// force 제한
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
			//충돌했으면 중력의 영향을 잠시 초기화
			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			hitDistance = min(outLeft.distance, outRight.distance);

			const float STOP_THRESHOLD = 150.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// 보정 위치
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
