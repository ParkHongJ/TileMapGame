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

	virtual void Equip(); // ����
	virtual void Equip(void* info); // �������� �� �÷��̾� ���� ������ ����
	virtual void UnEquip(); // ����
	virtual void UnEquip(void* info); // �������� ���� ���� ó��
	virtual void Use(); // ���

	void ChangeState(ItemState state); // ����

	inline void SetLock(bool isLock) { this->isLock = isLock; };
	inline const bool GetLock () const { return isLock; };

	inline void SetHold(bool isHold) { this->isHold = isHold; };
	inline const bool GetHold() const { return isHold; };

	inline void SetPrice(unsigned int price) { this->price = price; };
	inline const unsigned int GetPrice() const { return price; };

protected:
	bool isLock;
	bool isHold;
	unsigned int price;

	Image* image;
};

