#pragma once
#include "GameObject.h"
enum class GateState
{
	NONE = 0,
	OPEN,
	BLOCK
};

//
class Gate : public GameObject
{
	GENERATE_BODY(Textures/GateOpen.png,396,396)
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void LateUpdate(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)


	void EnterGate();
private:
	GateState gateState;
	class Image* gateImage;
	float objectScale;
	
	int frameX;
	int frameY;

	class BoxCollider* collider;

	float nextStageDelay = 2.f;
	bool bNextStage = false;
};

