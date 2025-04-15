#pragma once
class PlayerStatus
{
public:
	PlayerStatus();
	virtual ~PlayerStatus();

public:
	inline const unsigned int GetPlayerHP() const { return playerHP; };
	inline void SetPlayerHP(unsigned int hp) 
	{
		hp < playerMaxHP ? playerHP = hp : playerHP = playerMaxHP;
	};
	
	inline void PlusPlayerHP(unsigned int hp) 
	{
		playerHP + hp < playerMaxHP ? playerHP += hp : playerHP = playerMaxHP;
	};

	inline const unsigned int GetPlayerMaxHP() const { return playerMaxHP; };
	inline void SetPlayerMaxHP(unsigned int hp) { playerMaxHP = hp; };

	inline const unsigned int GetBombCount() const { return bombCount; };
	inline void SetBombCount(unsigned int cnt) { bombCount = cnt; };

	//inline const unsigned int GetRopeCount() const { return ropeCount; };
	//inline void SetRopeCount(unsigned int cnt) { ropeCount = cnt; };

	inline const unsigned int GetGold() const { return gold; };
	inline void SetGold(unsigned int gold) { this->gold = gold; };

private:
	unsigned int playerHP;
	unsigned int playerMaxHP;

	unsigned int bombCount;
	//unsigned int ropeCount;
	unsigned int gold;

};

