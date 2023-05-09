#include "UserData.h"

UserData::UserData() :
	m_playerName(""),
	m_numberOfPotions(0),
	m_playerLifePoints(25),
	m_areWeaponsEquiped(false),
	m_isPlayerSeenDeadEnd(false),
	m_isPlayerSeenVictory(false),
	m_isPlayerDead(false)
{
}

std::string UserData::GetPlayerName()
{
	return m_playerName;
}

void UserData::SetPlayerName(std::string playerName)
{
	m_playerName = playerName;
}

bool UserData::GetAreWeaponsEquiped()
{
	return m_areWeaponsEquiped;
}

void UserData::SetAreWeaponsEquiped(bool areWeaponEquiped)
{
	m_areWeaponsEquiped = areWeaponEquiped;
}

bool UserData::GetIsPlayerSeenDeadEnd()
{
	return m_isPlayerSeenDeadEnd;
}

void UserData::SetIsPlayerSeenDeadEnd(bool isPlayerSeenDeadEnd)
{
	m_isPlayerSeenDeadEnd = isPlayerSeenDeadEnd;
}

bool UserData::GetIsPlayerSeenVictory()
{
	return m_isPlayerSeenVictory;
}

void UserData::SetIsPlayerSeenVictory(bool isPlayerSeenVictory)
{
	m_isPlayerSeenVictory = isPlayerSeenVictory;
}

unsigned short int UserData::GetNumberOfPotions()
{
	return m_numberOfPotions;
}

void UserData::SetNumberOfPotions(unsigned short int numberOfPotions)
{
	m_numberOfPotions = numberOfPotions;
}

short int UserData::GetPlayerLifePoints()
{
	return m_playerLifePoints;
}

void UserData::SetPlayerLifePoints(short int playerLifePoints)
{
	m_playerLifePoints = playerLifePoints;
}

bool UserData::GetIsPlayerDead()
{
	return m_isPlayerDead;
}

void UserData::SetIsPlayerDead(bool isPlayerDead)
{
	m_isPlayerDead = isPlayerDead;
}

// TODO If the player attacked Kobold at the beginning, the help menu in combat mode should not be accessible
//bool UserData::GetAttackedKobold()
//{
//	return m_attackedKobold;
//}
//
//void UserData::SetAttackedKobold(bool attackedKobold)
//{
//	m_attackedKobold = attackedKobold;
//}