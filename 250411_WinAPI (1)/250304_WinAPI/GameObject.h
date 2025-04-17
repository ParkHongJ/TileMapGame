#pragma once
#include "config.h"

class GameObject
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

	virtual void CheckCulling();		// ������ ������ ���� ���� ����(������ ���)
	
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

	//������Ʈ�� ���� �Ѵ� ����;��
	bool bActive;

	//�޸𸮿��� �����;��
	bool bDestroy = false;

	//������Ʈ�� ���� �������� ����;��
	bool bHidden = false;

};

