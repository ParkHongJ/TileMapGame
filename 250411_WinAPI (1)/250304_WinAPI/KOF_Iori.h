#pragma once
#include "GameObject.h"

class Image;
class KOF_Iori : public GameObject
{
private:
	FPOINT pos;
	float moveSpeed;
	Image* image;
	int elapsedFrame;
	int currAnimaionFrame;	// 0 ~ 8

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Move();

	KOF_Iori();
	~KOF_Iori();
};

