#pragma once
#include "Singleton.h"
#include "config.h"

class CameraManager : public Singleton<CameraManager>
{
private:
	FPOINT pos;
	FPOINT target;
	FPOINT offset;
	FPOINT mapSize;

	float mapHeight;
	float mapWidth;

public: 
	HRESULT Init();
	void Release();
	void Update(const FPOINT& playerPos, float TimeDelta);

	const FPOINT GetPos() { return pos; }
	void SetMapWidth(float mapWidth) { this->mapWidth = mapWidth; }
	void SetMapHeight(float mapHeight) { this->mapHeight = mapHeight; }

};

