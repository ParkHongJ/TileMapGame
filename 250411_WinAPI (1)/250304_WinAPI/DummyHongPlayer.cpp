#include "DummyHongPlayer.h"
#include "CommonFunction.h"
#include "CollisionManager.h"
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
		Ray ray;
		ray.origin = Pos;

		FPOINT dir = { Pos.x + 30.f, Pos.y };
		dir -= Pos;
		ray.direction = dir.Normalized();

		RaycastHit out;
		//CollisionManager::GetInstance()->RaycastAll(ray, 100.f, out, true, 3.f);

		//if (out.hit)
		{
			int a = 10;
		}

		isFalling = !isFalling;
		velocity.y = JUMP_SPEED;
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
}

void DummyHongPlayer::Render(ID2D1HwndRenderTarget* renderTarget)
{
	DrawCenteredRect(renderTarget, Pos, 35.f, D2D1::ColorF::Magenta);
}
