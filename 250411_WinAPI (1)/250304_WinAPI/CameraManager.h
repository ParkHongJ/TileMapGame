#pragma once
#include "Singleton.h"
#include "config.h"

typedef struct Viewport
{
	float left;
	float right;
	float top;
	float bottom;
} VIEWPORT_INFO;

class CameraManager : public Singleton<CameraManager>
{
private:
	FPOINT pos;
	FPOINT target;
	FPOINT offset;
	FPOINT mapSize;

	VIEWPORT_INFO ViewPort;

	float mapHeight;
	float mapWidth;

	bool isLookUp;
	bool isLookDown;
	float yOffset;
	float currYOffset;
	float yOffsetSpeed;

	bool isShake = false;
	float shakeTime = 0.f;
	float shakePower = 1.f;
	float shakeX = 1.f;
	float shakeY = 1.f;

	FPOINT curPos;
	FPOINT curOffset;
	bool isPlayerDead = false;
	FPOINT deadOffset;

public: 
	HRESULT Init();
	void Release();
	void Update(float TimeDelta);
	void ShakeUpdate( float TimeDelta);

	Viewport& GetInRect() { return ViewPort; }

	const FPOINT GetPos() { return pos; }
	void SetMapWidth(float mapWidth) { this->mapWidth = mapWidth; }
	void SetMapHeight(float mapHeight) { this->mapHeight = mapHeight; }
	void SetYOffset(float yOffset) { this->yOffset = yOffset; }
	void SetLookingState(bool isLookUp, bool isLookDown) { this->isLookUp = isLookUp; this->isLookDown = isLookDown; }

	void SetTargetPos(FPOINT input) { this->target = input; }
	void CameraShake(float time, float power);
	void SetDeadCam();
};

