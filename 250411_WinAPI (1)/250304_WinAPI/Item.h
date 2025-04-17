#pragma once
#include "GameObject.h"

enum class ItemState
{
	STATE_UNEQUIP, STATE_EQUIP	
};
 // ä��, ��ź, (����, ��...)

class Image;

class Item : public GameObject
{
public:
	Item();
	virtual ~Item();

public:
	virtual HRESULT Init();
	virtual void Update(float TimeDelta);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);
	virtual void Release();
	/// <summary>
	/// �����ϰ� �ٷ� �Ծ����� Equip(info)�� ���� ó�� + �ٷ� UnEquip �غ���.
	/// �÷��̾ ����� �� ��� ���� �ִ°Ÿ� Equip�ؼ� ���� �ٲٱ�.
	/// �÷��̾ ����� ��� ������ Item->Use()�� ����ϰ�? ���
	/// </summary>
	virtual void Equip(); // ����
	virtual void Equip(void* info); // �������� �� �÷��̾� ���� ������ ����
	virtual void UnEquip(); // ����
	virtual void UnEquip(void* info); // �������� ���� ���� ó��
	virtual void Use(); // ���
	virtual void Use(void* info); // ���
	virtual void DropMove(float TimeDelta);
	virtual void DropMoveX(float TimeDelta);
	virtual void DropMoveY(float TimeDelta);

	void ChangeState(ItemState state); // ���� �� ����

	inline void SetLock(bool isLock) { this->isLock = isLock; };
	inline const bool GetLock () const { return isLock; };

	inline void SetHold(bool isHold) { this->isHold = isHold; };
	inline const bool GetHold() const { return isHold; };

	inline void SetPrice(unsigned int price) { this->price = price; };
	inline const unsigned int GetPrice() const { return price; };

protected:
	bool isLock;
	bool isHold; // ?
	unsigned int price;

	float RayDis;  // Ray Ȱ�� Ÿ�ϰ� ƨ��� �׽�Ʈ
	float gravity; // GameObject���� ó��?

	Image* holdImage;
	Image* dropImage;
	FPOINT moveDir;
	FPOINT moveReverseDir;
	FPOINT movePower;
	FPOINT prePos;

	ItemState state;
	GameObject* owner;

private:
	


};

