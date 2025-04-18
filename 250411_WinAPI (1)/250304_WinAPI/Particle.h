#pragma once
#include "GameObject.h"

class Particle : public GameObject
{
public:
	void Init(FPOINT pos, float moveSpeed, float angle, float size, float lifeTime);
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* rt) override;
	virtual void Release() override;

	bool IsEnd() const { return isEnd; }

private:
	float moveSpeed;
	float defaultMoveSpeed;
	float angle;
	float size;
	float defaultSize;
	float lifeTime;
	float maxLifeTime;
	bool isEnd;
	float elapsedTime;

	class Image* image;
};

