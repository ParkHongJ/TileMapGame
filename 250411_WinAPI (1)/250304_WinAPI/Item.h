#pragma once
#include "GameObject.h"

enum class ItemState
{
	STATE_UNEQUIP, STATE_EQUIP	
};
 // 채찍, 폭탄, (로프, 총...)

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
	/// 장착하고 바로 먹어지면 Equip(info)로 내부 처리 + 바로 UnEquip 해보기.
	/// 플레이어가 들었을 때 계속 끼고 있는거면 Equip해서 상태 바꾸기.
	/// 플레이어가 멤버로 들고 있으면 Item->Use()로 사용하게? 고민
	/// </summary>
	virtual void Equip(); // 장착
	virtual void Equip(void* info); // 장착했을 때 플레이어 정보 던져서 갱신
	virtual void UnEquip(); // 해제
	virtual void UnEquip(void* info); // 해제했을 때도 갱신 처리
	virtual void Use(); // 사용
	virtual void Use(void* info); // 사용
	virtual void DropMove(float TimeDelta);
	virtual void DropMoveX(float TimeDelta);
	virtual void DropMoveY(float TimeDelta);

	void ChangeState(ItemState state); // 장착 및 해제

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

	float RayDis;  // Ray 활용 타일과 튕기기 테스트
	float gravity; // GameObject에서 처리?

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

