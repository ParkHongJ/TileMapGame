#pragma once
#include "config.h"
#include "ObjectManager.h"

enum class INTERACTSTATE
{
	INTERACT_UNABLE, INTERACT_ABLE
};

// config Ȥ�� gameobject �� enumclass ���� ������Ʈ���� �̸� �ִ� �� ����� ex) ��ٸ�, ����, ����, ����, ��

enum class OBJECTNAME
{
	PLAYER, MONSTER, ITEM, LADDER, TILE, WHIP, GUN, ROPE, KEY, DOOR, BOMB, INCREASEBOMB, GOLDBAR, TURKEY, HEAVYBLOCK
};

class Collider;

class GameObject
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
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

	inline void SetFlip(bool bFlipX) { this->bFlipX = bFlipX; }

	const FPOINT& GetVelocity() { return velocity; }

	void SetCaveRender(bool bCaveRender) { isCave = bCaveRender; }
	bool IsCaveRender() { return isCave; }

	virtual bool OnPush(FPOINT dir, float strength) { return false; }

protected:
	bool isCave = false;

	FPOINT objectScale;
	// ���� �б�.
	RENDERORDER	objectRenderId;

	// ��ȣ�ۿ� �������� �Ǵ��ؼ� �б��Ϸ� �Ѱǵ� �ݸ��� ����ŷ�ϰԵǸ�..?
	INTERACTSTATE interactState;

	float collisionCoolTime;
	float collisionMaxCoolTime;

	FPOINT Pos;
	FPOINT velocity;

	//������Ʈ�� ���� �Ѵ� ����;��
	bool bActive;

	//�޸𸮿��� �����;��
	bool bDestroy = false;

	//������Ʈ�� ���� �������� ����;��
	bool bHidden = false;

	// �ݶ��̴� ���� ������Ʈ�ε� �浹 ������ �� ����
	bool bAbleCol = false;
protected:
	OBJECTNAME objectName;

	string playerKey;

	// �¿����. �̰Ը³�?
	bool bFlipX = false;
public:
	template<typename T>
	T* GetType() 
	{
		return dynamic_cast<T*>(this);
	}

	OBJECTNAME GetObjectName() { return objectName; }
	void SetObjectName(OBJECTNAME objectName) { this->objectName = objectName; }

	void SetPlayerKey(string input) { this->playerKey = input;  }

};

