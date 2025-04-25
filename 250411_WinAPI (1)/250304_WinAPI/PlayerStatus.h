#pragma once
struct PlayerStatusInfo
{
	unsigned int playerHP;
	unsigned int playerMaxHP;
	unsigned int bombCount;
	unsigned int gold;
	unsigned int sumGold;
	bool IsCobweb;
	bool IsEnd;
	unsigned int ropeCount;
	float deadTime;
	float GameTime;
}typedef PLAYERSTATUSDESC;

class PlayerStatus
{
public:
	PlayerStatus();
	virtual ~PlayerStatus();

public:
	void Update(float TimeDelta);
public:
	inline const unsigned int GetPlayerHP() const { return info.playerHP; };
	inline void SetPlayerHP(unsigned int hp) 
	{
		hp < info.playerMaxHP ? info.playerHP = hp : info.playerHP = info.playerMaxHP;
	};
	
	inline void PlusPlayerHP(unsigned int hp) 
	{
		info.playerHP + hp < info.playerMaxHP ? info.playerHP += hp : info.playerHP = info.playerMaxHP;
	};


	inline void MinusPlayerHP()
	{
		1 <= info.playerHP ? --info.playerHP : info.playerHP = 0;
	};

	inline const unsigned int GetPlayerMaxHP() const { return info.playerMaxHP; };
	inline void SetPlayerMaxHP(unsigned int hp) { info.playerMaxHP = hp; };

	inline const unsigned int GetBombCount() const { return info.bombCount; };
	inline void SetBombCount(unsigned int cnt) { info.bombCount = cnt; };

	inline void MinusBombCount()
	{
		1 <= info.bombCount ? --info.bombCount  : info.bombCount = 0;
	};

	inline const unsigned int GetRopeCount() const { return info.ropeCount; };
	inline void SetRopeCount(unsigned int cnt) { info.ropeCount = cnt; };

	inline void MinusRopeCount()
	{
		1 <= info.ropeCount ? --info.ropeCount : info.ropeCount = 0;
	};

	inline const unsigned int GetGold() const { return info.gold; };
	inline const unsigned int GetSumGold() const { return info.sumGold; };
	void SetGold(unsigned int gold);
	void PlusGold(unsigned int gold);

	inline const bool IsCobweb() const { return info.IsCobweb; };


	inline PLAYERSTATUSDESC* GetInfo() { return &info; };

	inline const float  GetGameTime() const { return info.GameTime; };

	inline const bool IsEnd() const { return info.IsEnd; };


private:
	PLAYERSTATUSDESC info;

};

