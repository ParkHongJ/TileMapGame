#include "DummyHongPlayer.h"
#include "CommonFunction.h"
#include "CollisionManager.h"
#include "Tile.h"
HRESULT DummyHongPlayer::Init()
{
	return S_OK;
}

void DummyHongPlayer::Release()
{
}

void DummyHongPlayer::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
	{
		Pos.y -= moveSpeed * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
	{
		Pos.y += moveSpeed * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
	{
		Pos.x -= moveSpeed * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
	{
		Pos.x += moveSpeed * TimeDelta;
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		//isFalling = !isFalling;
		//velocity.y = JUMP_SPEED;
		//isKnockback = !isKnockback;
		
		vector<GameObject*> OutObject;
		if (CollisionManager::GetInstance()->GetObjectsInCircle(Pos, 100.f, &OutObject))
		{
			int a = 10;
			for (int i = 0; i < OutObject.size(); ++i)
			{
				if (Tile* tile = dynamic_cast<Tile*>(OutObject[i]))
				{
					tile->Destruction();
				}
			}
		}
	}

	if (isFalling)
	{
		velocity.y += GRAVITY * TimeDelta;
		if (velocity.y > MAX_FALL_SPEED)
			velocity.y = MAX_FALL_SPEED;

		FPOINT rayPos = Pos;
		rayPos.y += 35.f;
		float nextY = rayPos.y + velocity.y * TimeDelta;
		Ray ray = { rayPos, {0.f, 1.f} };
		RaycastHit out;
		bool hit = CollisionManager::GetInstance()->RaycastAll(ray, velocity.y * TimeDelta, out, true, 1.f);

		if (hit && nextY >= out.point.y) {
			// 충돌 지점에서 멈춤
			Pos.y = out.point.y - 35.f;
			velocity.y = 0;
			isFalling = false;
		}
		else {
			// 아직 공중
			Pos.y = nextY - 35.f;
		}

	}

	if (isKnockback)
	{
		FPOINT nextPos = Pos + knockbackVelocity * TimeDelta;

		// 1. 넉백 방향에 벽이 있는지 레이캐스트
		RaycastHit hit;
		FPOINT rayDir = nextPos.Normalized();//knockbackVelocity.Normalized();
		Ray ray = { Pos, rayDir };

		bool hitWall = CollisionManager::GetInstance()->RaycastAll(ray,
			knockbackVelocity.Length() * TimeDelta,
			hit, true, 1.0f);

		if (hitWall)
		{
			// 2. 충돌 위치에서 멈추거나 튕김
			Pos = hit.point - rayDir * 2.0f; // 약간 뒤로 밀기 (퉁!)
			knockbackVelocity *= -0.3f;           // 반대로 튕기되 약하게
			knockbackTime -= TimeDelta;

			// 아주 짧은 반동 후 멈추기
			if (knockbackVelocity.Length() < 30.0f)
			{
				//knockbackVelocity = { 0.f, 0.f };

				knockbackVelocity = { 130.f, 10.f };
				isKnockback = false;
			}
		}
		else
		{
			// 벽 안 닿으면 계속 밀림
			Pos = nextPos;
			knockbackTime -= TimeDelta;

			if (knockbackTime <= 0.f)
			{
				isKnockback = false;
				//knockbackVelocity = { 0.f, 0.f };
				knockbackTime = 0.7f;
				knockbackVelocity = { 130.f, 10.f };
			}
		}
	}
}

void DummyHongPlayer::Render(ID2D1HwndRenderTarget* renderTarget)
{
	DrawCenteredRect(renderTarget, Pos, 35.f, D2D1::ColorF::Magenta);

	float radius = 100.f;
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(Pos.x, Pos.y), radius, radius), GBrush.Get());
}
