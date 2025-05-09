#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>
#include <glm/gtc/type_ptr.hpp>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	//Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	//hide the cursor and keep it in the window
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\Ball.obj");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\Rim.vert", "..\\res\\Rim.frag");
	what.init("..\\res\\what.vert", "..\\res\\what.frag");
	bump.init("..\\res\\bump.vert", "..\\res\\bump.frag");
	noBump.init("..\\res\\noBump.vert", "..\\res\\noBump.frag");
	texture1.load ("..\\res\\bricks.jpg"); //load texture
	texture2.load("..\\res\\rock.jpg"); //load texture
	bricksTexture.load("..\\res\\brickwall.jpg");
	bricksNormalMap.load("..\\res\\normal.jpg");
	floor.load("..\\res\\Floor\\floor.jpg");
	bumpMapping.loadNormals("..\\res\\n.jpg");
	floorNormal.loadNormals("..\\res\\Floor\\floorN.jpg");

	std::vector<std::string> faces = 
	{
		"..\\res\\Skybox\\sp2_rt.png",
		"..\\res\\Skybox\\sp2_lf.png",
		"..\\res\\Skybox\\sp2_up.png",
		"..\\res\\Skybox\\sp2_dn.png",
		"..\\res\\Skybox\\sp2_bk.png",
		"..\\res\\Skybox\\sp2_ft.png"
	};

	skybox.init(faces);

	GLint loc;

	//noBump.Bind();
	//
	//glUniform1i(glGetUniformLocation(noBump.getID(), "diffuse"), 0);
	//glUniform1i(glGetUniformLocation(noBump.getID(), "normalT"), 1);
	//tilingLoc = glGetUniformLocation(noBump.getID(), "tiling");

	//// camera position
	//loc = glGetUniformLocation(noBump.getID(), "viewPos");
	//glUniform3f(loc, myCamera.getPos().x, myCamera.getPos().y, myCamera.getPos().z);
	//
	//loc = glGetUniformLocation(noBump.getID(), "lightPos");
	//glUniform3f(loc, 0.0f, 1.0f, 0.0f);
	//loc = glGetUniformLocation(noBump.getID(), "lightColor");
	//glUniform3f(loc, 1.0f, 1.0f, 1.0f);
	//loc = glGetUniformLocation(noBump.getID(), "ambientColor");
	//glUniform3f(loc, 0.1f, 0.1f, 0.1f);

	bump.Bind();

	glUniform1i(glGetUniformLocation(bump.getID(), "diffuse"), 0);
	glUniform1i(glGetUniformLocation(bump.getID(), "normalT"), 1);
	tilingLoc = glGetUniformLocation(bump.getID(), "tiling");

	// camera position
	loc = glGetUniformLocation(bump.getID(), "viewPos");
	glUniform3f(loc, myCamera.getPos().x, myCamera.getPos().y, myCamera.getPos().z);

	// Ambient–Diffuse–Specular settings
	loc = glGetUniformLocation(bump.getID(), "lightPos");
	glUniform3f(loc, 0.0f, 10.0f, 0.0f);   // light position
	loc = glGetUniformLocation(bump.getID(), "lightColor");
	glUniform3f(loc, 1.0f, 1.0f, 1.0f);   // white light
	loc = glGetUniformLocation(bump.getID(), "ambientColor");
	glUniform3f(loc, 0.1f, 0.1f, 0.1f);   // low ambient

	GLint cLoc = glGetUniformLocation(bump.getID(), "lightConstant");
	GLint lLoc = glGetUniformLocation(bump.getID(), "lightLinear");
	GLint qLoc = glGetUniformLocation(bump.getID(), "lightQuadratic");
	glUniform1f(cLoc, 1.0f);
	glUniform1f(lLoc, 0.014f);
	glUniform1f(qLoc, 0.0007f);

	// Cache locations for uniforms you’ll update each frame
	tilingLoc = glGetUniformLocation(bump.getID(), "tiling");
	viewPosLoc = glGetUniformLocation(bump.getID(), "viewPos");
	modelLoc = glGetUniformLocation(bump.getID(), "model");
	viewLoc = glGetUniformLocation(bump.getID(), "view");
	projLoc = glGetUniformLocation(bump.getID(), "projection");


	myCamera.initCamera(glm::vec3(2, 0, -4), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	//Walls and floor
	Vertex quadVerts[6] = {
		// triangle 1
		Vertex({-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}),
		Vertex({-0.5f, 0.0f,  0.5f}, {0.0f, 1.0f}),
		Vertex({ 0.5f, 0.0f,  0.5f}, {1.0f, 1.0f}),
		// triangle 2
		Vertex({ 0.5f, 0.0f,  0.5f}, {1.0f, 1.0f}),
		Vertex({ 0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}),
		Vertex({-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}),
	};
	for (auto& v : quadVerts) v.normal = { 0,1,0 };
	
	for (size_t i = 0; i < 6; i += 3) {
		Vertex& v0 = quadVerts[i + 0];
		Vertex& v1 = quadVerts[i + 1];
		Vertex& v2 = quadVerts[i + 2];

		glm::vec3 edge1 = v1.pos - v0.pos;
		glm::vec3 edge2 = v2.pos - v0.pos;
		glm::vec2 dUV1 = v1.texCoord - v0.texCoord;
		glm::vec2 dUV2 = v2.texCoord - v0.texCoord;

		float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);
		glm::vec3 tangent = glm::normalize(f * (edge1 * dUV2.y - edge2 * dUV1.y));
		glm::vec3 bitan = glm::normalize(f * (edge2 * dUV1.x - edge1 * dUV2.x));

		v0.tangent = tangent;
		v1.tangent = tangent;
		v2.tangent = tangent;
		v0.bitangent = bitan;
		v1.bitangent = bitan;
		v2.bitangent = bitan;
	}

	meshQuad.loadVertexs(quadVerts, 6);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		// get current time in seconds
		float currentTime = SDL_GetTicks() / 1000.0f;
		float deltaTime = currentTime - _lastFrameTime;
		_lastFrameTime = currentTime;

		processInput(deltaTime);
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput(float deltaTime)
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}

	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	
	// Quit on ESC
	if (keys[SDL_SCANCODE_ESCAPE]) {
		_gameState = GameState::EXIT;
		return;        
	}
	
	const float speed = 5.0f; 
	
	const float mouseSensitivity = 0.1f;

	// get relative motion since last frame
	int xrel, yrel;
	SDL_GetRelativeMouseState(&xrel, &yrel);

	// turn the camera: xrel → yaw, yrel → pitch (invert Y so moving mouse up looks up)
	myCamera.rotate(xrel * mouseSensitivity,-yrel * mouseSensitivity);

	// forward/back
	if (keys[SDL_SCANCODE_W]) myCamera.moveForward(speed * deltaTime);
	if (keys[SDL_SCANCODE_S]) myCamera.moveForward(-speed * deltaTime);

	// left/right
	if (keys[SDL_SCANCODE_D]) myCamera.moveRight(speed * deltaTime);
	if (keys[SDL_SCANCODE_A]) myCamera.moveRight(-speed * deltaTime);

	// up/down
	if (keys[SDL_SCANCODE_SPACE])myCamera.moveUp(speed * deltaTime);
	if (keys[SDL_SCANCODE_LCTRL])myCamera.moveUp(-speed * deltaTime);

}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

//void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
//{
//	
//	ALint state; 
//	alGetSourcei(Source, AL_SOURCE_STATE, &state);
//	/*
//	Possible values of state
//	AL_INITIAL
//	AL_STOPPED
//	AL_PLAYING
//	AL_PAUSED
//	*/
//	if (AL_PLAYING != state)
//	{
//		audioDevice.playSound(Source, pos);
//	}
//}

void MainGame::linkRimShader()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	rimShader.setMat4("modelMatrix", transform.GetModel());
	//rimShader.setMat4("viewMatrix", myCamera.getView());
	rimShader.setFloat("rimPower", 3.0f);
	rimShader.setVec3("rimColor", glm::vec3(0.8f, 0.0f, 0.0f));
	rimShader.setVec3("camPos", myCamera.getPos());
}

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon()
{
	toonShader.setMat4("modelMatrix", transform.GetModel());
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkWhat()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	what.setVec3("cameraPosition", myCamera.getPos());
	what.setFloat("atmosphereRadius", 1.3);
	what.setFloat("planetRadius", 5.5);
	what.setVec3("planetCenter", *transform.GetPos());
	what.setVec3("atmosphereColor", glm::vec4(0.5,0.5,0.5,0.1));
}
void MainGame::linkBumpMapping(Texture& diffuseMap, Texture& normalMap)
{
	// get the uniform locations
	GLuint diffuseLoc = glGetUniformLocation(bump.getID(), "diffuse");
	GLuint normalLoc = glGetUniformLocation(bump.getID(), "normalT");

	// bind the diffuse map on unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap.getID());
	glUniform1i(diffuseLoc, 0);

	// bind the normal map on unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap.getID());
	glUniform1i(normalLoc, 1);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour

	glDepthMask(GL_FALSE);
	skybox.draw(myCamera);
	glDepthMask(GL_TRUE);


	// Bind the shader 
	bump.Bind();

	GLint locD = glGetUniformLocation(bump.getID(), "diffuse");
	glUniform1i(locD, 0); // sampler2D diffuse  → texture unit 0
	GLint locN = glGetUniformLocation(bump.getID(), "normalT");
	glUniform1i(locN, 1); // sampler2D normalT  → texture unit 1

	//floor
	transform.SetPos(glm::vec3(0.0f, -2.5f, 0.0f));
	transform.SetRot(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.SetScale(glm::vec3(50.0f, 1.0f, 50.0f)); 
	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModel()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getProjection()));

	glUniform2f(tilingLoc, 50.0f, 50.0f);
	
	viewPosLoc = glGetUniformLocation(bump.getID(), "viewPos");
	
	glm::vec3 camP = myCamera.getPos();
	glUniform3f(viewPosLoc, camP.x, camP.y, camP.z);

	glActiveTexture(GL_TEXTURE0);
	floor.Bind(0);   // diffuse map
	glActiveTexture(GL_TEXTURE1);
	floorNormal.Bind(1);  // normal map

	meshQuad.drawVertexes();


	//wall1
	transform.SetPos(glm::vec3(25.0f, -1.25f, 0.0f));
	transform.SetRot(glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(90.0f)));
	transform.SetScale(glm::vec3(50.0f, 1.0f, 2.5f));
	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModel()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getProjection()));

	glUniform2f(tilingLoc, 50.0f, 5.0f);
	
	viewPosLoc = glGetUniformLocation(bump.getID(), "viewPos");

	glm::vec3 cp = myCamera.getPos();
	glUniform3f(viewPosLoc, cp.x, cp.y, cp.z);

	glActiveTexture(GL_TEXTURE0); 
	bricksTexture.Bind(0);
	glActiveTexture(GL_TEXTURE1); 
	bricksNormalMap.Bind(1);

	meshQuad.drawVertexes();
	
	//wall2
	transform.SetPos(glm::vec3(-25.0f, -1.25f, 0.0f));
	transform.SetRot(glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(-90.0f)));
	transform.SetScale(glm::vec3(50.0f, 1.0f, 2.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModel()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getProjection()));

	glUniform2f(tilingLoc, 50.0f, 5.0f);

	viewPosLoc = glGetUniformLocation(bump.getID(), "viewPos");

	glm::vec3 cp1 = myCamera.getPos();
	glUniform3f(viewPosLoc, cp1.x, cp1.y, cp1.z);

	glActiveTexture(GL_TEXTURE0);
	bricksTexture.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	bricksNormalMap.Bind(1);

	meshQuad.drawVertexes();

	//wall3
	transform.SetPos(glm::vec3(0.0f, -1.25f, 25.0f));
	transform.SetRot(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
	transform.SetScale(glm::vec3(50.0f, 1.0f, 2.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModel()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getProjection()));

	glUniform2f(tilingLoc, 50.0f, 5.0f);
	
	viewPosLoc = glGetUniformLocation(bump.getID(), "viewPos");

	glm::vec3 cp2 = myCamera.getPos();
	glUniform3f(viewPosLoc, cp2.x, cp2.y, cp2.z);

	glActiveTexture(GL_TEXTURE0);
	bricksTexture.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	bricksNormalMap.Bind(1);

	meshQuad.drawVertexes();

	//wall4
	transform.SetPos(glm::vec3(0.0f, -1.25f, -25.0f));
	transform.SetRot(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
	transform.SetScale(glm::vec3(50.0f, 1.0f, 2.5f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform.GetModel()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getView()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(myCamera.getProjection()));

	glUniform2f(tilingLoc, 50.0f, 5.0f);
	
	viewPosLoc = glGetUniformLocation(bump.getID(), "viewPos");

	glm::vec3 cp3 = myCamera.getPos();
	glUniform3f(viewPosLoc, cp3.x, cp3.y, cp3.z);

	glActiveTexture(GL_TEXTURE0);
	bricksTexture.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	bricksNormalMap.Bind(1);

	meshQuad.drawVertexes();

	counter = counter + 0.03f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 