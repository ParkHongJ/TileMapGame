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
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
	virtual void CheckCulling();		// ������ ������ ���� ���� ����(������ ���)
	
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
	// ���� �б�.
	RENDERORDER	objectRenderId;

	// ��ȣ�ۿ� ��������
	INTERACTSTATE interactState;

	float collisionCoolTime;
	float collisionMaxCoolTime;

	FPOINT Pos;

	//������Ʈ�� ���� �Ѵ� ����;��
	bool bActive;

	//�޸𸮿��� �����;��
	bool bDestroy = false;

	//������Ʈ�� ���� �������� ����;��
	bool bHidden = false;

	// �ݶ��̴� ���� ������Ʈ�ε� �浹 ������ �� ����
	bool bAbleCol = false;

public:
	template<typename T>
	T* GetType() 
	{
		return dynamic_cast<T*>(this);
	}
};

