#pragma once
#include "Item.h"
class Whip : public Item
{
public:
	Whip();
	virtual ~Whip();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
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

	// �����ų� �����鼭 ������ DropMove�� ����������.
	virtual void DropMove(float TimeDelta) override;
	virtual void DropMoveX(float TimeDelta) override;
	virtual void DropMoveY(float TimeDelta) override;

	virtual void FrameUpdate(float TimeDelta) override;
private:
	int whipFrame = 0;
	class BoxCollider* col = nullptr; // ���� �̷��� ���� �ʾҾ�
	bool isHit = false;
};

