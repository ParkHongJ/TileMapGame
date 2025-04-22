#pragma once
#include "Item.h"

class ChangeCobwebBomb : public Item
{
	GENERATE_BODY(Textures/items.png, 128, 128)

public:
	ChangeCobwebBomb();
	virtual ~ChangeCobwebBomb();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;

	virtual void Equip() override; // ����
	virtual void Equip(void* info) override; // �������� �� �÷��̾� ���� ������ ����
	virtual void UnEquip() override; // ����
	virtual void UnEquip(void* info) override; // �������� ���� ���� ó��
	virtual void Use() override;
	// Item��(��) ���� ��ӵ�
	void DropMove(float TimeDelta) override;

	virtual void Detect(GameObject* obj) override;
};

