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

	virtual void Equip() override; // 장착
	virtual void Equip(void* info) override; // 장착했을 때 플레이어 정보 던져서 갱신
	virtual void UnEquip() override; // 해제
	virtual void UnEquip(void* info) override; // 해제했을 때도 갱신 처리
	virtual void Use() override;
	// Item을(를) 통해 상속됨
	void DropMove(float TimeDelta) override;

	virtual void Detect(GameObject* obj) override;

	virtual void FrameUpdate(float TimeDelta) override;

	void Explosion();

private:
	float gravity = 5.f;
	float explosionTime = 3.f;
	float animAcc = 5.f;
	float animationTime = 1.f;

	// 사용
};

