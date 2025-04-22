#pragma once
#include "config.h"

class Scene
{
public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget);		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta) {};
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

	Scene();
	virtual ~Scene();
};
