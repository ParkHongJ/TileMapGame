#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public: 
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(const FPOINT& playerPos, float TimeDelta);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	const FPOINT GetPos() { return pos; }	// 각각의 오브젝트에게 위치 주기 
	void SetMapSize(FPOINT mapSize) { this -> mapSize = mapSize; }
	void SetMapWidth(float mapWidth) { this->mapWidth = mapWidth; }
	void SetMapHeight(float mapHeight) { this->mapHeight = mapHeight; }

private: 
	
	FPOINT pos;			// 카메라의 위치
	FPOINT target;		// 플레이어의 위치
	FPOINT offset;
	FPOINT mapSize;

	float mapHeight;
	float mapWidth;

	float moveSpeed;	// 카메라 속도 
};

