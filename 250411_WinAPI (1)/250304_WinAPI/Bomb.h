#pragma once
#include "Item.h"
class Bomb : public Item
{
public:
	Bomb();
	virtual ~Bomb();

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

	virtual void FrameUpdate(float TimeDelta) override;

	void Explosion();

private:
	float gravity = 5.f;
	float explosionTime = 3.f;
	float animAcc = 5.f;
	float animationTime = 1.f;

	// ���
};

