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
	// 남은 수명 업데이트
	lifeTime = max(lifeTime - TimeDelta, 0.0f);

	// 수명 비율 계산
	float lifeRatio = lifeTime / maxLifeTime;

	// 크기 조정 (속도는 고정)
	size = max(defaultSize * lifeRatio, 0.0f);

	// 이동 연산 (moveSpeed는 변하지 않음)
	float radianAngle = DEG_TO_RAD(angle);
	float moveDist = defaultMoveSpeed * TimeDelta * lifeRatio; // 고정된 속도 사용

	Pos.x += cos(radianAngle) * moveDist;
	Pos.y -= sin(radianAngle) * moveDist;

	// 수명이 다하면 종료
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
