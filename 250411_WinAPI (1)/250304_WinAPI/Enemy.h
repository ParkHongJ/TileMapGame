#pragma once
#include "GameObject.h"

class Missile;
class Tank;
class Image;
class MissileManager;
class Enemy : public GameObject	// is-a
{
private:
	FPOINT pos;
	float moveSpeed;		// 5.0f (초당 이동속도) 물리에서의 속도 : 거리와 시간
	float angle;
	bool isAlive;
	int size;
	Tank* target;
	std::unique_ptr<Tank> target2;
	Image* image;
	int animationFrame;
	int elapsedFrame;
	float elapsedTime;
	MissileManager* missileManager;
	Missile* missile;

public:
	void Init(float posX = 0.0f, float posY = 0.0f);		
	void Release();		
	void Update();		
	void Render(HDC hdc);

	void Move();

	inline void SetIsAlive(bool isAlive) { this->isAlive = isAlive; }
	inline bool GetIsAlive() { return isAlive;
	}
	inline void SetTarget(Tank* target) { this->target = target; }
	inline FPOINT GetPos() { return pos; }
	inline int GetSize() { return size; }

	Enemy();
	~Enemy();

};

