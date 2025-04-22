#pragma once
#include "Item.h"
class TreasureChest : public Item
{
	GENERATE_BODY(Textures/items.png, 128, 128)

public:
	TreasureChest();
	virtual ~TreasureChest();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	/// <summary>
	/// �����ϰ� �ٷ� �Ծ����� Equip(info)�� ���� ó�� + �ٷ� UnEquip �غ���.
	/// �÷��̾ ����� �� ��� ���� �ִ°Ÿ� Equip�ؼ� ���� �ٲٱ�.
	/// �÷��̾ ����� ��� ������ Item->Use()�� ����ϰ�? ���
	/// </summary>
	virtual void Equip() override; // ����
	virtual void Equip(void* info) override; // �������� �� �÷��̾� ���� ������ ����
	virtual void Equip(GameObject* owner) override;
	virtual void UnEquip() override; // ����
	virtual void UnEquip(void* info) override; // �������� ���� ���� ó��
	virtual void Use() override; // ���
	virtual void Use(void* info) override;
	virtual void Detect(GameObject* obj) override;
	virtual void DeadEvent();

	// �����ų� �����鼭 ������ DropMove�� ����������.
	virtual void DropMove(float TimeDelta) override;
	virtual void DropMoveX(float TimeDelta) override;
	virtual void DropMoveY(float TimeDelta) override;
};

