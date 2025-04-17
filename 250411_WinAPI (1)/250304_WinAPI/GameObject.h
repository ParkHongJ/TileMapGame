#pragma once
#include "config.h"

class GameObject
{
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void LateUpdate(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)

	virtual void CheckCulling();		// 프레임 단위로 게임 로직 실행(데이터 계산)
	
public:
	GameObject();
	virtual ~GameObject();

public:
	inline const FPOINT& GetPos() { return Pos; }
	inline void SetPos(const FPOINT& newPos) { Pos = newPos; }

	inline void SetActive(bool bActive) { this->bActive = bActive; }
	inline bool IsActive() { return bActive; }

	inline void SetHidden(bool bHidden) { this->bHidden = bHidden; }
	inline const bool IsHidden() const { return this->bHidden; }

	inline const bool IsDestroyed() const { return bDestroy; }
	inline void SetDestroy() { bDestroy = true; }

	inline void SetObjectRenderId(RENDERORDER id) { objectRenderId = id; }
	inline const RENDERORDER GetObjectRenderId() const { return objectRenderId; };

protected:
	RENDERORDER	objectRenderId;
	FPOINT Pos;

	//업데이트와 렌더 둘다 막고싶어요
	bool bActive;

	//메모리에서 지우고싶어요
	bool bDestroy = false;

	//업데이트는 돌되 렌더링만 막고싶어요
	bool bHidden = false;

};

