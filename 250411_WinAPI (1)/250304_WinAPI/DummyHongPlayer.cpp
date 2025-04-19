#include "pch.h"
#include "DummyHongPlayer.h"
#include "CommonFunction.h"
#include "CollisionManager.h"
#include "Tile.h"
#include "Collider.h"
#include "HongParticle.h"
#include "CameraManager.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "Image.h"
HRESULT DummyHongPlayer::Init()
{

	int a = 10;
	len1 = elbow.Length();
	len2 = (hand - elbow).Length();

	img = ImageManager::GetInstance()->FindImage("Explosion");

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

	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
	//{
	//	FPOINT cameraPos = CameraManager::GetInstance()->GetPos();

	//	targetPos = { (float)g_ptMouse.x, (float)g_ptMouse.y /*Pos.x + 350.f, Pos.y - 150.f*/};
	//	worldElbow = Pos + elbow;
	//	worldHand = Pos + hand;

	//	SolveIK2Bone(Pos, worldElbow, worldHand, targetPos, len1, len2);

	//	elbow = worldElbow - Pos;
	//	hand = worldHand - Pos;

	//}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		//isFalling = !isFalling;
		//velocity.y = JUMP_SPEED;
		//isKnockback = !isKnockback;
		
		//useGravity = !useGravity;
		//bPhysics = !bPhysics;

		//float angleRad = RandomRange(0.0f, 2.0f * 3.141592f); // 0 ~ 360�� (����)
		//float speed = RandomRange(100.0f, 300.0f);            // �ӵ��� ����

		//velocity = 
		//{
		//	cosf(angleRad) * speed,  // 135�� (���� ��)
		//	-sinf(angleRad) * speed
		//};
		//acceleration = { 0, 0 };  // ���ӵ�
		//totalForce = { 0.f,0.f };


			/*GameObject* obj = new HongParticle;
			ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_MONSTER, obj);
			obj->SetPos(Pos);*/


		bExplosion = !bExplosion;

		for (int i = 0; i < 15; i++)
		{
			{
				FPOINT randPos = { RandomRange(-100, 100.f), RandomRange(-100, 100.f) };
				Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos + randPos, 0.f, 30.f, 3.f, 1, 0);

				PhysicsOption* physicsOp = new PhysicsOption;
				SizeOption* sizeOp = new SizeOption(0.04f);
				TrailOption* trailOp = new TrailOption("Effect", 0.02f, 0.2f);

				//float angleRad = RandomRange(-3.141592 / 4.0f, 3.141592 / 4.0f);
				float angleRad = RandomRange(0.0f, 3.141592f * 2.0f); // 0 ~ 360��
				//float speed = RandomRange(350.f, 375.0f);            // �ӵ��� ����
				float speed = RandomRange(850.f, 1175.0f);            // �ӵ��� ����

				velocity =
				{
					sinf(angleRad) * speed,
					-cosf(angleRad) * speed  // 135�� (���� ��)
				};

				physicsOp->Init(velocity, 0.3f);
				//physicsOp->Init(velocity, 0.5f);

				particle->AddParticleOption(physicsOp);
				particle->AddParticleOption(sizeOp);
				particle->AddParticleOption(trailOp);
			}

			{

				float angleRad = RandomRange(-3.141592 / 4.0f, 3.141592 / 4.0f);
				FPOINT randPos = { RandomRange(-100, 100.f), RandomRange(-100, 100.f) };

				Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos + randPos, angleRad, 105.f, 1.f, 1, 2);

				AlphaOption* alphaOp = new AlphaOption(5.0f);
				particle->AddParticleOption(alphaOp);
			}

			//{
			//	Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos, 0.f, 45.f, 0.01f, 4, 1);
			//	PhysicsOption* physicsOp = new PhysicsOption;

			//	float angleRad = RandomRange(-3.141592 / 4.0f, 3.141592 / 4.0f);
			//	float speed = RandomRange(450.f, 555.0f);            // �ӵ��� ����

			//	velocity =
			//	{
			//		sinf(angleRad) * speed,
			//		-cosf(angleRad) * speed  // 135�� (���� ��)
			//	};

			//	physicsOp->Init(velocity, 0.5f);

			//	AlphaOption* alphaOp = new AlphaOption(0.5f);
			//	particle->AddParticleOption(physicsOp);
			//	particle->AddParticleOption(alphaOp);
			//}
		}
		/*vector<GameObject*> OutObject;
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
		}*/
	}

	//if (isFalling)
	//{
	//	velocity.y += GRAVITY * TimeDelta;
	//	if (velocity.y > MAX_FALL_SPEED)
	//		velocity.y = MAX_FALL_SPEED;

	//	FPOINT rayPos = Pos;
	//	rayPos.y += 35.f;
	//	float nextY = rayPos.y + velocity.y * TimeDelta;
	//	Ray ray = { rayPos, {0.f, 1.f} };
	//	RaycastHit out;
	//	bool hit = CollisionManager::GetInstance()->RaycastAll(ray, velocity.y * TimeDelta, out, true, 1.f);

	//	if (hit && nextY >= out.point.y) {
	//		// �浹 �������� ����
	//		Pos.y = out.point.y - 35.f;
	//		velocity.y = 0;
	//		isFalling = false;
	//	}
	//	else {
	//		// ���� ����
	//		Pos.y = nextY - 35.f;
	//	}

	//}

	//if (isKnockback)
	//{
	//	FPOINT nextPos = Pos + knockbackVelocity * TimeDelta;

	//	// 1. �˹� ���⿡ ���� �ִ��� ����ĳ��Ʈ
	//	RaycastHit hit;
	//	FPOINT rayDir = nextPos.Normalized();//knockbackVelocity.Normalized();
	//	Ray ray = { Pos, rayDir };

	//	bool hitWall = CollisionManager::GetInstance()->RaycastAll(ray,
	//		knockbackVelocity.Length() * TimeDelta,
	//		hit, true, 1.0f);

	//	if (hitWall)
	//	{
	//		// 2. �浹 ��ġ���� ���߰ų� ƨ��
	//		Pos = hit.point - rayDir * 2.0f; // �ణ �ڷ� �б� (��!)
	//		knockbackVelocity *= -0.3f;           // �ݴ�� ƨ��� ���ϰ�
	//		knockbackTime -= TimeDelta;

	//		// ���� ª�� �ݵ� �� ���߱�
	//		if (knockbackVelocity.Length() < 30.0f)
	//		{
	//			//knockbackVelocity = { 0.f, 0.f };

	//			knockbackVelocity = { 130.f, 10.f };
	//			isKnockback = false;
	//		}
	//	}
	//	else
	//	{
	//		// �� �� ������ ��� �и�
	//		Pos = nextPos;
	//		knockbackTime -= TimeDelta;

	//		if (knockbackTime <= 0.f)
	//		{
	//			isKnockback = false;
	//			//knockbackVelocity = { 0.f, 0.f };
	//			knockbackTime = 0.7f;
	//			knockbackVelocity = { 130.f, 10.f };
	//		}
	//	}
	//}

	if (bPhysics)
	{
		if (useGravity)
		{
			AddForce({ gravity.x * mass, gravity.y * mass });
		}

		// force ����
		ClampVector(totalForce, 450.f);

		acceleration = totalForce / mass;
		velocity += acceleration * TimeDelta;

		FPOINT moveVec = { velocity.x * TimeDelta, velocity.y * TimeDelta };
		FPOINT nextPos = { Pos.x + moveVec.x, Pos.y + moveVec.y };

		Ray ray;
		ray.origin = Pos;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastAll(ray, moveLength, out))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
		{
			hitDistance = out.distance;

			FPOINT colliderPos = out.collider->GetWorldPos();
			FPOINT colliderScale = out.collider->GetScale();

			// 2. ���� ����
			FPOINT toHit = out.point - colliderPos;

			// ������ ������ ���� ����
			float xRatio = toHit.x / (colliderScale.x * 0.5f);
			float yRatio = toHit.y / (colliderScale.y * 0.5f);

			/*if (fabs(xRatio) > fabs(yRatio))
			{
				hitNormal = { (xRatio < 0 ? -1.f : 1.f), 0.f };
			}
			else
			{
				hitNormal = { 0.f, (yRatio < 0 ? -1.f : 1.f) };
			}*/

			float threshold = 0.1f;

			if (fabs(fabs(xRatio) - fabs(yRatio)) < threshold)
			{
				// �𼭸��� ���� �� �ݻ� ���� ���߰ų�, �밢�� ƨ�� (�幰��)
				hitNormal = { xRatio, yRatio }; // �ݻ絵 �밢��
				hitNormal.Normalize();
			}
			else
			{
				// ���� ��� ����
				if (fabs(xRatio) > fabs(yRatio))
					hitNormal = { (xRatio < 0 ? -1.f : 1.f), 0.f };
				else
					hitNormal = { 0.f, (yRatio < 0 ? -1.f : 1.f) };
			}

						
			FPOINT perturbedNormal = RotateVector(hitNormal, RandomRange(-30.f, 30.f));
			velocity = Reflect(velocity, /*perturbedNormal.Normalized()*/hitNormal.Normalized());

			velocity *= bounciness;

			totalForce.x = 0.0f;
			totalForce.y = 0.0f;
			
			const float STOP_THRESHOLD = 100.f;
			if (fabs(velocity.x) < STOP_THRESHOLD) 
				velocity.x = 0.f;
			if (fabs(velocity.y) < STOP_THRESHOLD) 
				velocity.y = 0.f;

			// ���� ��ġ
			Pos += ray.direction * hitDistance;

			ClampVector(velocity, 350.f);

			if (velocity.Length() < 130.f && hitNormal.y <= -0.99f)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
				bPhysics = false;
			}

			// ��¦ �б� (��ħ ����)

			Pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			Pos = nextPos;
		}

		// force �ʱ�ȭ
		//totalForce = { 0.f, 0.f };
	}
}

void DummyHongPlayer::LateUpdate(float TimeDelta)
{
	CameraManager::GetInstance()->SetTargetPos(Pos);

	if (bExplosion)
	{
		CurrentFrame += TimeDelta * 300.f;
		if (CurrentFrame >= 1.f)
		{
			CurrentIndex++;
			CurrentFrame = 0.f;
			if (CurrentIndex >= 11)
			{
				CurrentIndex = 0;
				bExplosion = false;
			}
		}
	}
	
}

void DummyHongPlayer::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();
	DrawCenteredRect(renderTarget, cameraPos, 35.f, D2D1::ColorF::Magenta);

	float radius = 100.f;
	renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(cameraPos.x, cameraPos.y), radius, radius), GBrush.Get());


	wstring wstr = L"TotalForce x : " + to_wstring(totalForce.x) + L" y : " + to_wstring(totalForce.y);
	DrawD2DText(renderTarget, wstr.c_str(), 500, 330);

	wstr = L"velocity x : " + to_wstring(velocity.x) + L" y : " + to_wstring(velocity.y);
	DrawD2DText(renderTarget, wstr.c_str(), 500, 230);

	//float radius = 20.f;
	//DrawCenteredRect(renderTarget, cameraPos, 35.f, D2D1::ColorF::Magenta);

	//FPOINT elbowPos = Pos + elbow + CameraManager::GetInstance()->GetPos();
	//DrawCenteredRect(renderTarget, elbowPos, 15.f, D2D1::ColorF::Blue);

	//FPOINT handPos = Pos + hand + CameraManager::GetInstance()->GetPos();
	//DrawCenteredRect(renderTarget, handPos, 15.f, D2D1::ColorF::Red);
	
	
	//renderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(handPos.x, handPos.y), radius, radius), GBrush.Get());

	if (img && bExplosion)
	{
		int x = CurrentIndex % 4;
		int y = CurrentIndex / 4;
		img->FrameRender(renderTarget, cameraPos.x, cameraPos.y, x, y);
	}
}

void DummyHongPlayer::SolveIK2Bone(FPOINT root, FPOINT& elbow, FPOINT& hand, const FPOINT& target, float len1, float len2)
{
	float totalLen = len1 + len2;
	float distToTarget = (target - root).Length();

	if (distToTarget > totalLen)
	{
		// ���� �Ұ� �� �ִ��� Ÿ�� �������� �� ����
		FPOINT dir = (target - root).Normalized();

		elbow = root + dir * len1;
		hand = elbow + dir * len2;
		return;
	}

	const int iteration = 10;
	for (int i = 0; i < iteration; ++i)
	{
		// Forward pass
		hand = target;
		FPOINT dir = (elbow - hand).Normalized();
		elbow = hand + dir * len2;

		dir = (root - elbow).Normalized();
		elbow = root + dir * len1;

		// Backward pass
		dir = (elbow - root).Normalized();
		elbow = root + dir * len1;

		dir = (hand - elbow).Normalized();
		hand = elbow + dir * len2;
	}
}
