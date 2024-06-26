#pragma once

#include <fstream>

class ScenesManager;

#include "ScenesManager.h"
#include "PublicConstants.h"

using namespace PublicConstants;

class NarrationManager{
public:
	const unsigned short int ASCII_IMAGE_HEIGHT = 33;
	const unsigned short int NAVIGATION_MENU_HEIGHT = 7;
	const unsigned short int STORY_TEXT_HEIGHT = 5;
	const unsigned short int NARRATION_MENU_HEIGHT = 1;

	NarrationManager(ScenesManager* sceneManager);
	void PrintLinesFromScene();
	std::string GetPictureTextScene(E_SceneSequence scene, const unsigned short int height);
	std::string& GetPictureFilePath001();
	std::string& GetPictureFilePath002();
	std::string& GetTextFilePath();
	std::string& GetMenuFilePath();
	std::string GetTextBetweenLines(E_SceneSequence scene, std::string& filePathStr, unsigned int firstLine, const unsigned short int height);

	ScenesManager* GetScenesManager();

	UserInputManager* GetUserInputManager();
	void SetUserInputManager(UserInputManager* userInputManager);

private:
	unsigned short int GetSceneTextLines(E_SceneSequence fromLine);
	unsigned short int GetSceneImageLines(E_SceneSequence fromLine);

	ScenesManager* m_sceneManager;
	UserInputManager* m_userInputManager;

	std::string m_textFilePath;
	std::string m_pictureFilePath001;
	std::string m_pictureFilePath002;
	std::string m_menuFilePath;
};