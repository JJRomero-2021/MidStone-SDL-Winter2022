#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"
#include <SDL.h>
#include "Vector.h"
#include "Matrix.h"
#include "Body.h"
#include "GameManager.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class GameObject;
class PhysicsObject;
class Mesh;
class Shader;
class Texture;

class Scene3 : public Scene {
private:
	Body* title, * play, * credits, * controlls, * quit;
	GameObject* gameObject;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	GameManager* scene;
	SDL_Window* windowPtr;
	SDL_Renderer* renderer;


public:
	explicit Scene3(SDL_Window* sdlWindow, GameManager* scene);
	virtual ~Scene3();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render();
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H
