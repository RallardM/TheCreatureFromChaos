#include <conio.h>
#include <iostream>
#include "UserInputManager.h"
#include "DebugMessageSystem.h"
#include "CombatManager.h"

UserInputManager::UserInputManager(ConsoleHandler* m_consoleHandler, ScenesManager* sceneManager, MenuManager* menuManager) :
    m_consoleHandler(),
    m_weaponManager(),
    m_sceneManager(sceneManager),
    m_menuManager(menuManager),
    m_hasInput(false)
{
    SetCurrentInputType(E_CurrentInputType::DIALOGUES);
}

E_UserInput UserInputManager::GetInput()
{
    int state = 0;
    char key = 0;
    while (true) 
    {
        key = _getch(); // Read the key
      
        switch (key)
        {
            case    -32:    // Special keys
                state = 1;
				break;

            case    97:    //'a':           // Attack - move left
            case    65:    //'A':
  				return E_UserInput::LEFT;
                break;

            case    75:    // Left arrow key
            case '\033[D': // Left arrow key
                return E_UserInput::LEFT;

            case   119:    //'w':           // Move forward - Khai Attack
            case    87:    //'W':
            case    72:    // Up arrow key
            case '\033[A': // Up arrow key
            //case -32750:   // Up arrow key
                if (GetCurrentInputType() == E_CurrentInputType::COMBAT || GetCurrentInputType() == E_CurrentInputType::NAVIGATION)
                {
                    return E_UserInput::UP;
                }
                else
                {
                    return E_UserInput::EMPTY;
                }

            case  100:     //'d':           // Move right - Potion
            case   68:     //'D':
                return E_UserInput::RIGHT;
                break;

            case   112:    //'p':           
            //case    80:    //'P': Taken by down arrow key
                if (GetCurrentInputType() == E_CurrentInputType::COMBAT)
                {
                    return E_UserInput::RIGHT; // Potion
                }
                else if (GetCurrentInputType() == E_CurrentInputType::NAVIGATION)
                {
                        return E_UserInput::DOWN;

                }
                else
                {
                    return E_UserInput::EMPTY;
                }
                break;

            case    77:    // Right arrow key 
            case '\033[C': // Right arrow key
                if (GetCurrentInputType() == E_CurrentInputType::COMBAT) 
                {
                    if (key != '\033[B') // Down arrow key = 80
                    {
                        return E_UserInput::RIGHT; // Potion
                    }
                    else
                    {
                        return E_UserInput::EMPTY;
                    }
                }
                else if (GetCurrentInputType() == E_CurrentInputType::NAVIGATION || GetCurrentInputType() == E_CurrentInputType::DIALOGUES)
                {
                    return E_UserInput::RIGHT;
                }

                break;

            case   102:    //'f':           // Flee
            case    70:    //'F': 
                if (GetCurrentInputType() == E_CurrentInputType::COMBAT)
                {
                    return E_UserInput::DOWN;
                }
                else
                {
                    return E_UserInput::EMPTY;
                }
                break;
            
            case   115:     //'s':
            case    83:     //'S':
                return E_UserInput::DOWN;
                break;

            case    80:     // Down arrow key Taken by P potion
            case '\033[B':  // Down arrow key
            case -32749:     // Down arrow key
                if (state == 1)
                {
                    return E_UserInput::DOWN;
                    state = 0;
				}
                else
                {
                    return E_UserInput::RIGHT; // Potion
                }

                break;
                
            case 'e':
            case 'E':
            case 13: // Enter key
                return E_UserInput::ENTER;
                break;

            case '1':
                return E_UserInput::ONE;
				break;

            case '2':
				return E_UserInput::TWO;
                break;

            case 27: // Escape key
                return E_UserInput::ESC;
                break;

            default: // Default case - returns an empty input
                state = 0;
                return E_UserInput::EMPTY;
                break;
        }
     }
}

void UserInputManager::SetAction(E_UserInput userInput)
{
    bool left = (userInput == E_UserInput::LEFT);
    bool right = (userInput == E_UserInput::RIGHT);
    bool up = (userInput == E_UserInput::UP);
    bool down = (userInput == E_UserInput::DOWN);


    if (left || right || up || down)
    {
        DEBUG_MSG("UserInputManager.cpp : SetAction() : User is using LEFT RIGHT.");
        GetMenuManager()->SelectMenuFromScene(userInput);
        SetHasInput(false);
        return;
	}
    if (userInput == E_UserInput::ENTER)
    {
        DEBUG_MSG("UserInputManager.cpp : SetAction() : User pressed ENTER.");
        EnterSelection();
        SetHasInput(false);
        return;
    }
    if (userInput == E_UserInput::ONE || userInput == E_UserInput::TWO)
    {
        GetWeaponManager()->SelectWeapon(userInput);
        SetHasInput(false);
        return;
    }
    if (userInput == E_UserInput::ESC)
    {
        DEBUG_MSG("UserInputManager.cpp : SetAction() : User pressed ESC.");
        return;
	}
}

void UserInputManager::EnterSelection()
{
    E_SceneSequence currentScene = GetScenesManager()->GetPlayerCurrentScene();
    unsigned short int currentNumberOfChoices = SCENE_NUMBER_OF_MENU_CHOICES[int(currentScene)];
    bool isPlayerTurn = GetScenesManager()->GetCombatManager()->GetIsPlayerTurn();
    
    if (currentNumberOfChoices == ONE_CHOICE_MENU)
    {
        GetMenuManager()->SelectMenuFromScene(E_UserInput::ENTER);
        GetScenesManager()->SetNextScene(GetMenuManager()->GetSelectedMenuLine());
        return;
	}
    else if (currentNumberOfChoices == TWO_CHOICES_MENU)
    {
        GetScenesManager()->SetNextScene(GetMenuManager()->GetSelectedMenuLine());
        return;
    }
    else if (currentNumberOfChoices == COMBAT_MENU && !isPlayerTurn)
    {
        GetScenesManager()->GetCombatManager()->EnemyCounterAttack();
        return;
    }
}

UserInputManager::E_CurrentInputType UserInputManager::GetCurrentInputType()
{
    return m_currentInputType;
}

void UserInputManager::SetCurrentInputType(E_CurrentInputType currentInputType)
{
    m_currentInputType = currentInputType;
}

bool UserInputManager::HasInput()
{
    return m_hasInput;
}

void UserInputManager::SetHasInput(bool hasInput)
{
    m_hasInput = hasInput;
}

ConsoleHandler* UserInputManager::GetConsoleHandler()
{
    return m_consoleHandler;
}

MenuManager* UserInputManager::GetMenuManager()
{
    return m_menuManager;
}

ScenesManager* UserInputManager::GetScenesManager()
{
    return m_sceneManager;
}

WeaponManager* UserInputManager::GetWeaponManager()
{
    return m_weaponManager;
}

void UserInputManager::SetWeaponManager(WeaponManager* weaponManager)
{
    m_weaponManager = weaponManager;
}

//CombatManager* UserInputManager::GetCombatManager()
//{
//    return m_combatManager;
//}