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
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

private:
	GateState gateState;
	class Image* gateImage;
	float objectScale;
	
	int frameX;
	int frameY;
};

