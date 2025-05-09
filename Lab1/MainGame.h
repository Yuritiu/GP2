#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Skybox.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput(float deltaTime);
	void gameLoop();
	void drawGame();
	void linkBumpMapping(Texture& diffuseMap, Texture& normalMap);
	void linkFogShader();
	void linkToon();
	void linkRimShader();
	void linkWhat();
	void drawSceneWithBump(const glm::mat4&);
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	//void playAudio(unsigned int Source, glm::vec3 pos);

	float _lastFrameTime = 0.0f;

	GLint tilingLoc = -1;
	GLint viewPosLoc;
	GLint modelLoc;
	GLint viewLoc;
	GLint projLoc;

	// Reflection FBO
	GLuint reflectFBO;
	GLuint reflectTex;
	int screenWidth, screenHeight;


	Display _gameDisplay;
	GameState _gameState;

	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Camera myCamera;
	Shader fogShader;
	Shader toonShader;
	Shader rimShader;
	Shader bump;
	Shader noBump;
	Shader what;
	Shader shadowShader;
	Shader reflectShader;
	
	glm::vec3 lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
	glm::mat4 shadowMat;
	glm::mat4 shadowMatFloor,
		shadowMatWallXpos,
		shadowMatWallXneg,
		shadowMatWallZpos,
		shadowMatWallZneg;

	// Walls and Floor Mesh
	Mesh meshQuad;

	Texture texture1;
	Texture texture2;
	Texture bricksTexture;
	Texture bricksNormalMap;
	Texture bumpMapping;
	Texture floor;
	Texture floorNormal;
	Texture monkey;
	Texture monkeyN;
	
	SkyBox skybox;
	
	//Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
	int numOfSurfaces;
};

