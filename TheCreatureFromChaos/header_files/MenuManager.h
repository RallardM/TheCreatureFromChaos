#pragma once

#include <fstream>
#include "ScenesManager.h"
class ScenesManager;
#include "ScenesControllers.h"
#include "UserData.h"
#include "ConsoleHandler.h"

using namespace ScenesControllers;

class MenuManager {
public:
	const short int EMPTY_MENU_LINE = -1;
	const std::string EMPTY_MENU_TEXT = "";

	MenuManager(ConsoleHandler* consoleHandler, ScenesManager* sceneManager, NarrationManager* narrationManager, UserData* userData);

	void PrintMenuFromScene(E_UserInput userInput);
	void SelectMenuChoice(E_UserInput userInput, E_MenuChoices LeftMenuChoice, E_MenuChoices rightMenuChoice);
	void PrintSelectedMenu(E_MenuChoices currentMenuChoice);
	void PrintEnterNameMenu();

	std::string GetLastLineInConsole();

	E_MenuChoices GetSelectedMenuLine();
	void SetSelectedMenuLine(E_MenuChoices selectedMenuLine);
	std::string GetMenuAtLine(std::string& filePath, E_MenuChoices atLine);

	void ClearConsolePreviousLine();

	bool GetIsMenuCleared();
	void SetIsMenuCleared(bool isMenuCleared);

	ConsoleHandler* GetConsoleHandler();
	NarrationManager* GetNarrationManager();
	ScenesManager* GetScenesManager();
	UserData* GetUserData();

	//UserInputManager* GetUserInputManager();
	//void SetUserInputManager(UserInputManager* inputManager);

private:

	E_MenuChoices m_selectedMenuLine;

	std::ifstream m_menuFilePath;
	bool m_isMenuCleared;

	ConsoleHandler* m_consoleHandler;
	ScenesManager* m_scenesManager;
	NarrationManager* m_narrationManager;
	UserData* m_userData;
	//UserInputManager* m_inputManager;

};
