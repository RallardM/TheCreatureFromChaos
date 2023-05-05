#pragma once

class ScenesManager;

#include "ScenesManager.h"
#include "ConsoleHandler.h"
#include "ScenesControllers.h"
using namespace ScenesControllers;

class UserInputManager {
public:
	UserInputManager(ConsoleHandler* m_consoleHandler, ScenesManager* sceneManager, MenuManager* menuManager);
	
	void ActivateSelection(E_UserInput userInput);
	void EnterSelection();

	bool HasInput();
	void SetHasInput(bool hasInput);
	E_UserInput GetInput();
	void SetAction(E_UserInput userInput);

	ConsoleHandler* GetConsoleHandler();
	MenuManager* GetMenuManager();
	ScenesManager* GetScenesManager();

private:
	bool m_hasInput;

	ConsoleHandler* m_consoleHandler;
	MenuManager* m_menuManager;
	ScenesManager* m_sceneManager;
};
