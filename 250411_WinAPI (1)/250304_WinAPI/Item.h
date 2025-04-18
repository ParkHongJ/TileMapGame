#pragma once
#include "GameObject.h"
#include "PlayerStatus.h"

enum class ItemState
{
	STATE_UNEQUIP, STATE_EQUIP	
};

/// <summary>
/// 소모품, N번 사용, 먹으면 영구 사용.
/// </summary>
enum class ItemType
{
	TYPE_ONCE, TYPE_LIMIT, TYPE_ALWAYS
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
	/// 플레이어가 공격 때 계속 Attack()함수 호출이면 그냥
	/// </summary>
	virtual void Equip(); // 장착
	virtual void Equip(void* info); // 장착했을 때 플레이어 정보 던져서 갱신
	virtual void Equip(GameObject* owner);
	virtual void UnEquip(); // 해제
	virtual void UnEquip(void* info); // 해제했을 때도 갱신 처리
	virtual void Use(); // 사용
	virtual void Use(void* info);
	virtual void Detect(GameObject* obj) override;

	virtual void FrameUpdate(float TimeDelta);

	// 던지거나 죽으면서 뱉으면 DropMove로 움직여보자.
	virtual void DropMove(float TimeDelta);
	virtual void DropMoveX(float TimeDelta);
	virtual void DropMoveY(float TimeDelta);

	void ChangeState(ItemState state); // 장착 및 해제

	virtual void DeadEvent(); // 당장은 생각하지 말자.

	inline void SetPrice(unsigned int price) { this->price = price; };
	inline const unsigned int GetPrice() const { return price; };
                                                         
	inline void SetItemType(ItemType type) { itemType = type; };
	inline const ItemType GetItemType() const { return itemType; };

protected:
	unsigned int price;

	float RayDis;  // Ray 활용 타일과 튕기기 테스트
	float gravity; // GameObject에서 처리?

	Image* holdImage;
	Image* dropImage;
	FPOINT moveDir;
	FPOINT moveReverseDir;
	FPOINT movePower;
	FPOINT prePos;

	ItemType itemType;
	ItemState itemState;

	int startFrameIndexX;
	int startFrameIndexY;

	int endFrameIndexX;
	int endFrameIndexY;

	int curFrameIndexX;
	int curFrameIndexY;

	float elipsedTime;
	float frameSpeed;

	float equipColCoolTime;
	float equipColMaxCoolTime = 0.5f;

private:
	


};

