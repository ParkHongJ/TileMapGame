#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public: 
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(const FPOINT& playerPos, float TimeDelta);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	const FPOINT GetPos() { return pos; }	// ������ ������Ʈ���� ��ġ �ֱ� 
	void SetMapSize(FPOINT mapSize) { this -> mapSize = mapSize; }
	void SetMapWidth(float mapWidth) { this->mapWidth = mapWidth; }
	void SetMapHeight(float mapHeight) { this->mapHeight = mapHeight; }

private: 
	
	FPOINT pos;			// ī�޶��� ��ġ
	FPOINT target;		// �÷��̾��� ��ġ
	FPOINT offset;
	FPOINT mapSize;

	float mapHeight;
	float mapWidth;

	float moveSpeed;	// ī�޶� �ӵ� 
};

