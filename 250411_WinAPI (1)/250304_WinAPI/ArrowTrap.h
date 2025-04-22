#pragma once
#include "GameObject.h"
class ArrowTrap : public GameObject
{
	GENERATE_BODY(Textures/floormisc.png, 128, 128)
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void LateUpdate(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)
	virtual void CheckCulling();		// 프레임 단위로 게임 로직 실행(데이터 계산)

private:
	void Fire();

	class Image* trapImage;
	class BoxCollider* collider;

	float objectScale;


	bool isFire = false;
};

