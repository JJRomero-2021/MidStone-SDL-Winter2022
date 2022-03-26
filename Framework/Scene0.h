#ifndef SCENE0_H
#define SCENE0_H
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

class Scene0 : public Scene {
private:
	Body* spaceShip, * planet, * obstacle;
	GameObject* gameObject;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	GameManager* scene;
	SDL_Window* windowPtr;
	SDL_Renderer* renderer;


public:
	explicit Scene0(SDL_Window* sdlWindow, GameManager* scene);
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render();
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H
