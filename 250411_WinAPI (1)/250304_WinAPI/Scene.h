#pragma once
#include "config.h"

class Scene
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(HDC hdc);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

	Scene();
	virtual ~Scene();
};
