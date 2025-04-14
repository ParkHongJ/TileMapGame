#pragma once
#include "config.h"

class GameObject
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

public:
	GameObject();
	virtual ~GameObject();

public:
	inline const FPOINT& GetPos() { return Pos; }
	inline void SetPos(const FPOINT& newPos) { Pos = newPos; }

	inline void SetActive(bool bActive) { this->bActive = bActive; }
	inline bool IsActive() { return bActive; }

	inline void SetHidden(bool bHidden) { this->bHidden = bHidden; }
	inline bool IsHidden() { return this->bHidden; }

	inline bool IsDestroyed() const { return bDestroy; }
	inline void Destroy() { bDestroy = true; }

	inline void SetObjectId(int id) { objectId = id; }
protected:
	unsigned int objectId;
	FPOINT Pos;

	//������Ʈ�� ���� �Ѵ� ����;��
	bool bActive;

	//�޸𸮿��� �����;��
	bool bDestroy = false;

	//������Ʈ�� ���� �������� ����;��
	bool bHidden = false;

};

