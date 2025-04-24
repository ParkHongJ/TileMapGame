  #pragma once
#include "GameObject.h"
#include "PlayerStatus.h"

enum class ItemState
{
	STATE_UNEQUIP, STATE_EQUIP	
};

/// <summary>
/// �Ҹ�ǰ, N�� ���, ������ ���� ���.
/// </summary>
enum class ItemType
{
	TYPE_WEAK, TYPE_ONCE, TYPE_LIMIT, TYPE_ALWAYS
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
	virtual void SetImage(string input);
	virtual void Update(float TimeDelta);
	virtual void Render(ID2D1RenderTarget* renderTarget);
	virtual void Release();
	/// <summary>
	/// �����ϰ� �ٷ� �Ծ����� Equip(info)�� ���� ó�� + �ٷ� UnEquip �غ���.
	/// �÷��̾ ����� �� ��� ���� �ִ°Ÿ� Equip�ؼ� ���� �ٲٱ�.
	/// �÷��̾ ����� ��� ������ Item->Use()�� ����ϰ�? ���
	/// �÷��̾ ���� �� ��� Attack()�Լ� ȣ���̸� �׳�
	/// </summary>
	virtual void Equip(); // ����
	virtual void Equip(void* info); // �������� �� �÷��̾� ���� ������ ����
	virtual void Equip(GameObject* owner);
	virtual void UnEquip(); // ����
	virtual void UnEquip(void* info); // �������� ���� ���� ó��
	virtual void Use(); // ���
	virtual void Use(void* info);
	virtual void Detect(GameObject* obj) override;

	virtual void FrameUpdate(float TimeDelta);

	// �����ų� �����鼭 ������ DropMove�� ����������.
	virtual void DropMove(float TimeDelta);
	virtual void DropMoveX(float TimeDelta);
	virtual void DropMoveY(float TimeDelta);

	void ChangeState(ItemState state); // ���� �� ����

	virtual void DeadEvent(); // ������ �������� ����.

	inline void SetPrice(unsigned int price) { this->price = price; };
	inline const unsigned int GetPrice() const { return price; };
                                                         
	inline void SetItemType(ItemType type) { itemType = type; };
	inline const ItemType GetItemType() const { return itemType; };

	float RandomRange(float min, float max)
	{
		float r = (float)rand() / RAND_MAX; // 0.0 ~ 1.0
		return min + (max - min) * r;
	}

	void ClampVector(FPOINT& vec, float maxLength)
	{
		float len = sqrtf(vec.x * vec.x + vec.y * vec.y);
		if (len > maxLength)
		{
			float scale = maxLength / len;
			vec.x *= scale;
			vec.y *= scale;
		}
	}

	FPOINT RotateVector(const FPOINT& vec, float degrees)
	{
		float rad = degrees * 3.1415926f / 180.0f;
		float cosA = cosf(rad);
		float sinA = sinf(rad);

		return {
			vec.x * cosA - vec.y * sinA,
			vec.x * sinA + vec.y * cosA
		};
	}

	void AddForce(const FPOINT& force)
	{
		totalForce.x += force.x;
		totalForce.y += force.y;
	}

	float Dot(const FPOINT& a, const FPOINT& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	FPOINT Reflect(const FPOINT& velocity, const FPOINT& normal)
	{
		float dot = Dot(velocity, normal);
		return {
			velocity.x - 2.0f * normal.x * dot,
			velocity.y - 2.0f * normal.y * dot
		};
	}

	void SetDrop(float speed, float angle, float mass = 1.0f, FPOINT gravity = { 0, 98.0f });
	void SetHoldItemPos(FPOINT pos, bool isFlip);

	bool IsPlayerDropItem(GameObject* player);
protected:
	unsigned int price;

	float RayDis;  // Ray Ȱ�� Ÿ�ϰ� ƨ��� �׽�Ʈ

	float DropTime;

	Image* holdImage;
	Image* dropImage;
	FPOINT moveDir;
	FPOINT moveReverseDir;
	FPOINT movePower;
	FPOINT prePos;

	FPOINT holdOffset;
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
	bool isDrop = false;
	bool isFlip = false;

protected:

	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // ���ӵ�

	float mass = 1.0f;
	FPOINT gravity = { 0, 98.0f };  // �߷�
	bool useGravity = false;
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.4f;


};

