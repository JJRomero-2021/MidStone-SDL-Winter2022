#ifndef SCENE0_H
#define SCENE0_H

#include <vector>
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
	Body* spaceShip, * obstacle1, * obstacle2, * obstacle3, * obstacle4, * obstacle5, * background, * planet, * sun, * mars, * jupiter;
	GameObject* gameObject;
	Matrix4 projectionMatrix, inverseProjection;
	Matrix4 viewMatrix;
	GameManager* scene;
	SDL_Window* windowPtr;
	SDL_Renderer* renderer;
	std::vector<Body*>bullets;
	int spaceshipDirec, totalTargetsDestroyed;
	bool spaceShipDestroyed;


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
