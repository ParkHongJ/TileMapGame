#pragma once
#include "GameObject.h"
#include "config.h"
class Rope;

class RopeController : public GameObject
{
public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Detect(GameObject* obj) override;

	void Shoot(FPOINT pos);
	void UpMove(float TimeDelta);
	void DownMove(float TimeDelta);
	void Shake(float TimeDelta);
	void CreateRope();


private:
	class Image* image;
	bool isMove = false;
	bool isLanch = false;
	int CurIndex;
	int RopeCnt = 10;
	int curFrameX;
	int curFrameY;
	float downTime;
	float downMaxTime;
	int createCnt = 0;
	vector<Rope*> Ropes;
};

