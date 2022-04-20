#include "GameManager.h"
#include "Window.h"
#include "Timer.h"
#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include <iostream>

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
	fullScreen = false;
}


/// In this OnCreate() method, fuction, subroutine, whatever the word, 
bool GameManager::Initialize(std::string name_, int width_, int height_) 
{
	windowPtr = new Window();

	if (!windowPtr->OnCreate(name_, width_, height_)) 
	{
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) 
	{
		return false;
	}
	BuildScene(SCENE0);

	//create some user defined event
	changeSceneEventType = SDL_RegisterEvents(1);
	if (changeSceneEventType == ((Uint32)-1))
	{
		currentScene->OnDestroy();
		return false;
	}

	changeSceneEventType2 = SDL_RegisterEvents(1);
	if (changeSceneEventType2 == ((Uint32)-1))
	{
		currentScene->OnDestroy();
		return false;
	}
	return true;
}

/// Here's the whole game
void GameManager::Run() {
	timer->Start();
	while (isRunning) {
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		GetEvents();
		/// Keeep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

void GameManager::GetEvents() 
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		const Uint8* state = SDL_GetKeyboardState(nullptr); /// Do not free this! Read the docs
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == changeSceneEventType)
		{
			switch (sdlEvent.user.code) {
			
			case 1:

				BuildScene(SCENE0);
				break;

			case 2:

				BuildScene(SCENE1);
				break;

			case 3:

				BuildScene(SCENE2);
				break;

			case 4:
				BuildScene(SCENE3);
				break;

			case 5:
				isRunning = false;
				break;

			case 6:
				BuildScene(SCENE4);
				break;

			case 7:
				BuildScene(SCENE5);
				break;

			}
			

		}
		else if (sdlEvent.type == changeSceneEventType2)
		{
			// switch scene
			currentScene->OnDestroy();
			delete currentScene;
			currentScene = new Scene2(windowPtr->GetSDL_Window(), this);
			if (!currentScene->OnCreate())
			{
				isRunning = false;
			}
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
			case SDL_SCANCODE_Q:
				isRunning = false;
				return;

			case SDL_SCANCODE_F1:
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT])
				{
					//BuildScene(SCENE0_ADV);
				}
				else
				{
					BuildScene(SCENE0);
				}
				break;

			case SDL_SCANCODE_F2:
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT])
				{
					//BuildScene(SCENE1_ADV);
				}
				else
				{
					BuildScene(SCENE1);
				}
				break;

			case SDL_SCANCODE_F3:
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {
					//BuildScene(SCENE2_ADV);
				}
				else {
					BuildScene(SCENE2);
				}
				break;

			case SDL_SCANCODE_F4:
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {
					//BuildScene(SCENE3_ADV);
				}
				else {
					BuildScene(SCENE3);
				}
				break;

			case SDL_SCANCODE_F6:
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {
					//BuildScene(SCENE4_ADV);
				}
				else {
					BuildScene(SCENE4);
				}
				break;


			case SDL_SCANCODE_RETURN:
				if (fullScreen == false) {
					fullScreen = true;
					SDL_SetWindowFullscreen(windowPtr->GetSDL_Window(), SDL_WINDOW_FULLSCREEN);
				}
				break;

			default:
				currentScene->HandleEvents(sdlEvent);
				break;
			}
		}
		if (currentScene == nullptr) 
		{
			isRunning = false;
			return;
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

Uint32 GameManager::getChangeScene()
{
	return changeSceneEventType;
}

Uint32 GameManager::getChangeScene2()
{
	return changeSceneEventType2;
}
void GameManager::BuildScene(SCENE_NUMBER scene) {
	bool status;

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {
	case SCENE0:
		currentScene = new Scene0(windowPtr->GetSDL_Window(), this);
		status = currentScene->OnCreate();
		break;

	case SCENE1:
		currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
		status = currentScene->OnCreate();
		break;

	case SCENE2:
		currentScene = new Scene2(windowPtr->GetSDL_Window(), this);
		status = currentScene->OnCreate();
		break;

	case SCENE3:
		currentScene = new Scene3(windowPtr->GetSDL_Window(), this);
		status = currentScene->OnCreate();
		break;
	
	case SCENE4:
		currentScene = new Scene4(windowPtr->GetSDL_Window(), this);
		status = currentScene->OnCreate();
		break;

	case SCENE5:
		currentScene = new Scene5(windowPtr->GetSDL_Window(), this);
		status = currentScene->OnCreate();
		break;

	default:
		currentScene = nullptr;
		break;

	}
	if (! status) {
		isRunning = false;
	}
}
GameManager::~GameManager() 
{
	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (windowPtr) 
	{
		delete windowPtr;
		windowPtr = nullptr;
	}
}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}