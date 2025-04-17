#pragma once
#include "config.h"

enum class INTERACTSTATE
{
	INTERACT_UNABLE, INTERACT_ABLE
};

class Collider;

class GameObject
{
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void LateUpdate(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)
	virtual void CheckCulling();		// 프레임 단위로 게임 로직 실행(데이터 계산)
	
	void UpdateAbleCollider(float TimeDelta);

public:
	GameObject();
	virtual ~GameObject();

public:
	virtual void Detect(GameObject* obj);

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

	inline void SetObjectInteractState(INTERACTSTATE state) { interactState = state; }
	inline const INTERACTSTATE GetObjectInteractState() const { return interactState; };

	inline void SetAbleCol(bool bAbleCol) { this->bAbleCol = bAbleCol; }
	inline const bool IsAbleCol() const { return this->bAbleCol; }

	void TakeCollision(float coolTime);
protected:
	// 렌더 분기.
	RENDERORDER	objectRenderId;

	// 상호작용 가능한지
	INTERACTSTATE interactState;

	float collisionCoolTime;
	float collisionMaxCoolTime;

	FPOINT Pos;

	//업데이트와 렌더 둘다 막고싶어요
	bool bActive;

	//메모리에서 지우고싶어요
	bool bDestroy = false;

	//업데이트는 돌되 렌더링만 막고싶어요
	bool bHidden = false;

	// 콜라이더 만들 오브젝트인데 충돌 금지일 때 끄기
	bool bAbleCol = false;

public:
	template<typename T>
	T* GetType() 
	{
		return dynamic_cast<T*>(this);
	}
};

