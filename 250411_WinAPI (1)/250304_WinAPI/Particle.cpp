#include "pch.h"
#include "Particle.h"
#include "CommonFunction.h"
#include "Image.h"

void Particle::Init(FPOINT pos, float moveSpeed, float angle, float size, float lifeTime)
{
	this->Pos = pos;
	this->moveSpeed = defaultMoveSpeed = moveSpeed;
	this->angle = angle;
	this->size = defaultSize = size;
	this->lifeTime = maxLifeTime = lifeTime;
	isEnd = false;
}

void Particle::Update(float TimeDelta)
{
	// ���� ���� ������Ʈ
	lifeTime = max(lifeTime - TimeDelta, 0.0f);

	// ���� ���� ���
	float lifeRatio = lifeTime / maxLifeTime;

	// ũ�� ���� (�ӵ��� ����)
	size = max(defaultSize * lifeRatio, 0.0f);

	// �̵� ���� (moveSpeed�� ������ ����)
	float radianAngle = DEG_TO_RAD(angle);
	float moveDist = defaultMoveSpeed * TimeDelta * lifeRatio; // ������ �ӵ� ���

	Pos.x += cos(radianAngle) * moveDist;
	Pos.y -= sin(radianAngle) * moveDist;

	// ������ ���ϸ� ����
	if (lifeTime <= 0.0f)
		isEnd = true;
}

void Particle::Render(ID2D1HwndRenderTarget* rt)
{
	image->Render(rt);
}

void Particle::Release()
{
}
