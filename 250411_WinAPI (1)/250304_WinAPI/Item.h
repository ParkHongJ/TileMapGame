#pragma once
#include "GameObject.h"

enum class ItemState
{
	STATE_UNEQUIP, STATE_EQUIP	
};

/// <summary>
/// �Ҹ�ǰ, N�� ���, ������ ���� ���.
/// </summary>
enum class ItemType
{
	TYPE_ONCE, TYPE_LIMIT, TYPE_ALWAYS
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
	virtual void Equip(GameObject* owner);
	virtual void UnEquip(); // ����
	virtual void UnEquip(void* info); // �������� ���� ���� ó��
	virtual void Use(); // ���
	virtual void Use(void* info);
	virtual void Detect(GameObject* obj) override;

	// �����ų� �����鼭 ������ DropMove�� ����������.
	virtual void DropMove(float TimeDelta);
	virtual void DropMoveX(float TimeDelta);
	virtual void DropMoveY(float TimeDelta);

	void ChangeState(ItemState state); // ���� �� ����

	/*inline void SetLock(bool isLock) { this->isLock = isLock; };
	inline const bool GetLock () const { return isLock; };

	inline void SetHold(bool isHold) { this->isHold = isHold; };
	inline const bool GetHold() const { return isHold; };*/

	inline void SetPrice(unsigned int price) { this->price = price; };
	inline const unsigned int GetPrice() const { return price; };

	inline void SetOwner(GameObject* owner) { this->owner = owner; };
	inline const GameObject* GetOwner() const { return owner; };
	                                                             
	inline void SetItemType(ItemType type) { itemType = type; };
	inline const ItemType GetItemType() const { return itemType; };

protected:
	unsigned int price;

	float RayDis;  // Ray Ȱ�� Ÿ�ϰ� ƨ��� �׽�Ʈ
	float gravity; // GameObject���� ó��?

	Image* holdImage;
	Image* dropImage;
	FPOINT moveDir;
	FPOINT moveReverseDir;
	FPOINT movePower;
	FPOINT prePos;

	ItemType itemType;
	ItemState itemState;
	GameObject* owner;

private:
	


};

