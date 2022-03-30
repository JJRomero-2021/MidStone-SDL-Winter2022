#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SDL.h>
#include "Scene.h"
#include <string>

class GameManager {
private:
	/// These are called "forward declarations" The idea is that a pointer is 
	/// really just an unsigned int, so to create a pointer to it you really only
	/// need to tell the compiler that there is a class called Window and I need
	/// a pointer to it, thus the "class Window*" means trust me, there is a class 
	/// called Window, I just need a pointer to it.

	/// If that was too much for your brain, just #include "Window.h" and declare
	/// Window *ptr and don't use the word "class"  This is a second semester C++
	/// topic anyway
	enum SCENE_NUMBER 
	{
		SCENE0 = 0,
		SCENE1,
		SCENE2,

	};
	class Window *windowPtr;
	class Timer *timer;
	bool isRunning;
	bool fullScreen;
	Scene *currentScene;
	Uint32 changeSceneEventType, changeSceneEventType2;
	void BuildScene(SCENE_NUMBER scene_);

public:
	GameManager();
	~GameManager();
	//bool OnCreate(std::string name_);
	void OnDestroy();
	bool Initialize(std::string name_, int width_, int height_);
	void Run();
	void GetEvents();
	Uint32 getChangeScene();
	Uint32 getChangeScene2();
};
#endif


