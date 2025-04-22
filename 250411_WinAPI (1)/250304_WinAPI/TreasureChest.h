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
	/// 장착하고 바로 먹어지면 Equip(info)로 내부 처리 + 바로 UnEquip 해보기.
	/// 플레이어가 들었을 때 계속 끼고 있는거면 Equip해서 상태 바꾸기.
	/// 플레이어가 멤버로 들고 있으면 Item->Use()로 사용하게? 고민
	/// </summary>
	virtual void Equip() override; // 장착
	virtual void Equip(void* info) override; // 장착했을 때 플레이어 정보 던져서 갱신
	virtual void Equip(GameObject* owner) override;
	virtual void UnEquip() override; // 해제
	virtual void UnEquip(void* info) override; // 해제했을 때도 갱신 처리
	virtual void Use() override; // 사용
	virtual void Use(void* info) override;
	virtual void Detect(GameObject* obj) override;
	virtual void DeadEvent();

	// 던지거나 죽으면서 뱉으면 DropMove로 움직여보자.
	virtual void DropMove(float TimeDelta) override;
	virtual void DropMoveX(float TimeDelta) override;
	virtual void DropMoveY(float TimeDelta) override;
};

