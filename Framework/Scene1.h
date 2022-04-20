#ifndef SCENE1_H
#define SCENE1_H

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

class Scene1 : public Scene {
private:
	Body* spaceShip, * enemy1, * enemy2, * enemy3, * enemy4, * enemy5, * background,
		* enemyAttack1, * enemyAttack2, * enemyAttack3, * enemyAttack4, * enemyAttack5
		, * sun, * jupiter, * neptune, * uranus;
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
	explicit Scene1(SDL_Window* sdlWindow, GameManager* scene);
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render();
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	
	
};


#endif // SCENE1_H
