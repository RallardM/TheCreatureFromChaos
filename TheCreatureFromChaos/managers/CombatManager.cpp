#include <random>
#include <string>

#include "CombatManager.h"
#include "DebugMessageSystem.h"
#include "PublicConstants.h"


CombatManager::CombatManager(UserData* userData, MenuManager* menuManager) :
	m_ennemyLifePoints(50),
	m_enemyHitPoints(10),
	m_weaponManager(nullptr),
	m_userData(userData),
	m_menuManager(menuManager),
	m_isEnemyDefeated(false),
	m_isFightStarted(false),
	m_isFightLogCleared(true),
	m_isPLayerTurn(false),
	m_currentfightLog("")
{
}

void CombatManager::SetCombatAction(E_UserInput userInput)
{
	switch (userInput)
	{	
	case PublicConstants::E_UserInput::EMPTY:
		ReprintCurrentFightLog();

		break;
	case PublicConstants::E_UserInput::LEFT: // Attack
		SetIsPlayerTurn(true);
		RefreshMenuAndLogFrame();
		PlayerAttack();
		SetIsPlayerTurn(false);
		break;

	case PublicConstants::E_UserInput::RIGHT: // Potion
		SetIsPlayerTurn(true);
		RefreshMenuAndLogFrame();
		TakePotion();
		SetIsPlayerTurn(false);
		break;

	case PublicConstants::E_UserInput::UP:  // Khail help

		RefreshMenuAndLogFrame();
		//EnnemyAttack(); // TODO: 
		break;

	case PublicConstants::E_UserInput::DOWN: // Flee
		SetIsPlayerTurn(true);
		RefreshMenuAndLogFrame();
		TryToFlee();
		SetIsPlayerTurn(false);
		break;
		// TODO:
	default:
		// TODO: Add error message
		break;

	}
	SetIsFightStarted(true);
	//GetWeaponManager()->SelectWeapon(E_UserInput::EMPTY);
}

void CombatManager::RefreshMenuAndLogFrame()
{
	//GetWeaponManager()->ClearWeaponLogLine();
	GetMenuManager()->ClearConsolePreviousLine();
	SetIsFightLogCleared(true);
	GetWeaponManager()->SetIsWeaponBeltCleared(true);
	GetMenuManager()->ClearConsoleNavigationMenu();
	GetMenuManager()->SelectMenuFromScene(E_UserInput::EMPTY);
	GetWeaponManager()->SelectWeapon(E_UserInput::EMPTY);
}

void CombatManager::PlayerAttack()
{
	S_Weapon currentWeapon = GetWeaponManager()->GetCurrentWeapon(GetWeaponManager()->GetCurrentWeaponIndex());

	// Random hit point between the current weapon min and max hit points or fail
	// 
	// Create a random number generator and distribution for hit points
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(currentWeapon.minHitPoints, currentWeapon.maxHitPoints);

	std::string playerHitLog = "";

	// Generate a random hit point value between the current weapon min and max hit points
	unsigned short int hitPoints = dist(gen);

	std::bernoulli_distribution zeroDist(0.2); // Probability of zero/fail
	if (zeroDist(gen)) 
	{
		hitPoints = 0;
	}

	if (hitPoints == 0)
	{
		//std::string spaces = "                           ";
		std::string spaces = "..........................."; // Debug TODO : Remove
		std::string text = "You missed!";
		std::string nextLine = "\n";
		playerHitLog = spaces + text + nextLine;
	}
	else
	{
		//std::string spaces = "             ";
		std::string spaces = "............."; // Debug TODO : Remove
		std::string text = "You hit the monster : ";
		playerHitLog = spaces + text;
	}

	InflictDamage(hitPoints);
	PrintCausaltyLog(playerHitLog, hitPoints);
	//SetIsPlayerTurn(false);
}

void CombatManager::EnemyCounterAttack()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::string ennemyHitLog = "";

	short int hitPoints = GetEnemyHitPoints();

	std::bernoulli_distribution zeroDist(0.5); // Probability of zero/fail
	if (zeroDist(gen))
	{
		hitPoints = 0;
	}

	if (hitPoints == 0)
	{
		//std::string spaces = "                    ";
		std::string spaces = "...................."; // Debug TODO : Remove
		std::string text = "The monster missed!";
		std::string nextLine = "\n";
		ennemyHitLog = spaces + text + nextLine;
	}
	else
	{
		//std::string spaces = "             ";
		std::string spaces = "............."; // Debug TODO : Remove
		std::string text = "The monster hit you : ";
		ennemyHitLog = spaces + text;
	}
	
	ReceiveDamage(hitPoints);
	if (GetUserData()->GetIsPlayerDead())
	{
		GetMenuManager()->GetScenesManager()->SetNextScene(E_MenuChoices::PLAYER_DIED);
		GetMenuManager()->SetIsMenuCleared(false);
		GetUserData()->SetAreWeaponsEquiped(false);
		return;
	}

	RefreshMenuAndLogFrame();
	PrintCausaltyLog(ennemyHitLog, hitPoints);
	SetIsPlayerTurn(true);
}

void CombatManager::TakePotion()
{
	std::string playerHealLog = "";

	if (GetUserData()->GetNumberOfPotions() == 0)
	{
		//std::string spaces = "       ";
		std::string spaces = "......."; // Debug TODO : Remove
		std::string text = "You don't have any potions left!";
		std::string nextLine = "\n";
		playerHealLog = spaces + text + nextLine;
	}
	else
	{
		GetUserData()->SetPlayerLifePoints(25);
		GetUserData()->SetNumberOfPotions(GetUserData()->GetNumberOfPotions() - 1);
		//std::string spaces = "                     ";
		std::string spaces = "....................."; // Debug TODO : Remove
		std::string text = "You took a potion!";
		std::string nextLine = "\n";
		playerHealLog = spaces + text + nextLine;
	}

	PrepareAndClearBeltLog();
	std::cout << playerHealLog;
	SetCurrentFightLog(playerHealLog);
	SetIsFightLogCleared(false);
}

void CombatManager::TryToFlee()
{
	srand(time(nullptr)); // seed the random number generator with the current time

	// generate a random number between 0 and 3 (inclusive)
	int choice = rand() % 4;

	switch (choice) 
	{
	case 0:
		// TODO: code to handle fleeing backwards
		GetMenuManager()->GetScenesManager()->SetPlayerCurrentScene(E_SceneSequence::ROOM_THREE_FRONT);
		GetMenuManager()->GetScenesManager()->SetNextScene(E_MenuChoices::NAVIGATION_BACK);
		// TODO: Add Narration Scene informing the player that he flee back in the tunnel but the monster is after him
		// add a count down until the monster catch up with the player
		break;
	case 1:
		// Handle fleeing foward
		PreparePlayerWonScene();
		// TODO: Add Narration Scene informing the player that he knock down and passed the monster
		break;
	case 2:
		// Handle dying while fleeing
		PreparePlayerDiedScene();
		// Add Narration Scene informing the player that he died while fleeing
		break;
	case 3:
		// TODO: code to handle failing to flee
		break;
	default:
    	// TODO:
		std::cerr << "Error: invalid choice generated.\n";
		break;
	}
}

void CombatManager::InflictDamage(short int hitPoints)
{
	short int resultingCausalty = GetEnnemyLifePoints() - hitPoints;
	if (resultingCausalty <= 0)
	{
		PreparePlayerWonScene();
		return;
	}

	SetEnnemyLifePoints(resultingCausalty);

}

void CombatManager::ReceiveDamage(short int hitPoints)
{
	short int resultingCausalty = GetUserData()->GetPlayerLifePoints() - hitPoints;
	if (resultingCausalty <= 0)
	{
		PreparePlayerDiedScene();
		return;
	}

	GetUserData()->SetPlayerLifePoints(resultingCausalty);
}

void CombatManager::PrintCausaltyLog(std::string logText, short int hitPoints)
{
	bool isPlayerInDialogueMode = (GetMenuManager()->GetNarrationManager()->GetUserInputManager()->GetCurrentInputType() == UserInputManager::E_CurrentInputType::DIALOGUES);
	bool isPlayerInNavigationMode = (GetMenuManager()->GetNarrationManager()->GetUserInputManager()->GetCurrentInputType() == UserInputManager::E_CurrentInputType::NAVIGATION);
	std::string finalOutput = "";

	if (isPlayerInDialogueMode || isPlayerInNavigationMode)
	{
		return;
	}

	PrepareAndClearBeltLog();

	if (hitPoints == 0)
	{
		finalOutput= logText;
		//std::cout << logText;
	}
	else if (hitPoints > 0 && hitPoints < 10)      // To fit the number of max characters 
	{											   // to remove in the log
		//std::cout << logText << " -" << hitPoints; // add a space before the minus sign
		//std::cout << logText << ".-" << hitPoints << std::endl; // Debug TODO : Remove
		//finalOutput = logText + " -" + std::to_string(hitPoints) + "\n";
		finalOutput = logText + ".-" + std::to_string(hitPoints) + "\n"; // Debug TODO : Remove
	}
	else
	{
		//std::cout << logText << "-" << hitPoints << std::endl;
		finalOutput = logText + "-" + std::to_string(hitPoints) + "\n";
	}
	std::cout << finalOutput;
	SetCurrentFightLog(finalOutput);
	SetIsFightLogCleared(false);
}

void CombatManager::ReprintCurrentFightLog()
{
	std::string currentFightLog = GetCurrentFightLog();
}

void CombatManager::PrepareAndClearBeltLog()
{
	MoveCursorAfterBeltLog();

	if (!GetIsFightLogCleared())
	{
		for (size_t i = 0; i < 38; i++)
		{
			std::cout << "\b";
		}
	}
}

void CombatManager::PreparePlayerWonScene()
{
	SetEnnemyLifePoints(0);
	SetIsEnemyDefeated(true);
	SetIsFightStarted(false);
	SetIsEnemyDefeated(true);
	GetMenuManager()->GetScenesManager()->SetNextScene(E_MenuChoices::PLAYER_WON);
}

void CombatManager::PreparePlayerDiedScene()
{
	GetUserData()->SetPlayerLifePoints(0);
	GetUserData()->SetIsPlayerDead(true);
}

void CombatManager::MoveCursorAfterBeltLog()
{
	std::cout << "\033[A";
	std::cout << "\033[43C";
}

WeaponManager* CombatManager::GetWeaponManager()
{
	return m_weaponManager;
}

void CombatManager::SetWeaponManager(WeaponManager* weaponManager)
{
	m_weaponManager = weaponManager;
}

short int CombatManager::GetEnnemyLifePoints()
{
	return m_ennemyLifePoints;
}

void CombatManager::SetEnnemyLifePoints(short int ennemyLifePoints)
{
	m_ennemyLifePoints = ennemyLifePoints;
}

UserData* CombatManager::GetUserData()
{
	return m_userData;
}

void CombatManager::SetUserData(UserData* userData)
{
	m_userData = userData;
}

unsigned short int CombatManager::GetEnemyHitPoints()
{
	return m_enemyHitPoints;
}

bool CombatManager::GetIsFightLogCleared()
{
	return m_isFightLogCleared;
}

void CombatManager::SetIsFightLogCleared(bool fightLogState)
{
	m_isFightLogCleared = fightLogState;
}

bool CombatManager::GetIsEnemyDefeated()
{
	return m_isEnemyDefeated;
}

void CombatManager::SetIsEnemyDefeated(bool isEnemyDefeated)
{
	m_isEnemyDefeated = isEnemyDefeated;
}

bool CombatManager::GetIsFightStarted()
{
	return m_isFightStarted;
}

void CombatManager::SetIsFightStarted(bool isFightStarted)
{
	m_isFightStarted = isFightStarted;
}

bool CombatManager::GetIsPlayerTurn()
{
	return m_isPLayerTurn;
}

void CombatManager::SetIsPlayerTurn(bool isPlayerTurn)
{
	m_isPLayerTurn = isPlayerTurn;
}

std::string CombatManager::GetCurrentFightLog()
{
	return m_currentfightLog;
}

void CombatManager::SetCurrentFightLog(std::string currentFightLog)
{
	m_currentfightLog = currentFightLog;
}

void CombatManager::ClearAllConsoleText()
{
	system("cls");
}

MenuManager* CombatManager::GetMenuManager()
{
	return m_menuManager;
}
