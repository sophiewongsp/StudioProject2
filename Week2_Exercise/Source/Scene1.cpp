#include "Scene1.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"

Mtx44 MVP;
bool flying = false; // trail appears when flying
bool fence = false; // fence will fall when true
bool hit = false; // wooden structure will break when angry bird hits it

Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
{
	glClearColor(0.5f, 0.5f, 1.f, 0.5f);
	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	camera.Init(Vector3(200, 0, 300), Vector3(0, 0, 0), Vector3(0, 1, 0));

	rotateAngle = 0.0f;
	rotateAngle1 = 0.0f;
	rotateAngle2 = 0.0f;
	shoot = 0.0f; // for flying animation
	band = 1.0f; // for sling shot
	move = 0.0f; // flying
	ball = 0.0f; // kick ball
	drop = 0.0f; // coconut drop
	direction = 0.0f;
	translateX = 0.0f;
	scaleAll = 0.0f;

	rotateAmt = 60;
	translateAmt = 10;
	scaleAmt = 2;

	LSPEED = 10.f;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] =
		glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID,
		"material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID,
		"material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID,
		"material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID,
		"material.kShininess");
	// light 0
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID,
		"lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID,
		"lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	// light 1
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID,
		"lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID,
		"lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	// light 2
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID,
		"lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID,
		"lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	// light 3
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID,
		"lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID,
		"lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	// prac 7 part 2
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID,
		"lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[0].exponent");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID,
		"lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[1].exponent");

	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID,
		"lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[2].exponent");

	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID,
		"lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID,
		"lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID,
		"lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID,
		"lights[3].exponent");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID,
		"textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,
		"textColor");

	//
	//in case you missed out practical 7
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID,
		"colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// directional light
	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(300, 700, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 0.5;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);
	// spot lights
	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(20, 20, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	light[2].type = Light::LIGHT_SPOT;
	light[2].position.Set(-5, 0, -290);
	light[2].color.Set(1, 1, 1);
	light[2].power = 1;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	light[3].type = Light::LIGHT_SPOT;
	light[3].position.Set(565, -25, -165);
	light[3].color.Set(1, 1, 1);
	light[3].power = 1;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = cos(Math::DegreeToRadian(45));
	light[3].cosInner = cos(Math::DegreeToRadian(30));
	light[3].exponent = 3.f;
	light[3].spotDirection.Set(0.f, 1.f, 0.f);
	
	// Use our shader
	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Enable blending (Prac 9 part 2)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	meshList[GEO_TRAIL] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 15, 15, 15);
	meshList[GEO_TRAIL]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_TRAIL]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TRAIL]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_TRAIL]->material.kShininess = 1.f;

	meshList[GEO_EYEBROW1] = MeshBuilder::GenerateQuad("Quad", Color(0.2, 0.15, 0.12), 5);
	meshList[GEO_EYEBROW1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYEBROW1]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_EYEBROW1]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYEBROW1]->material.kShininess = 1.f;

	meshList[GEO_EYEBROW2] = MeshBuilder::GenerateQuad("Quad", Color(0.2, 0.15, 0.12), 5);
	meshList[GEO_EYEBROW2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYEBROW2]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_EYEBROW2]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYEBROW2]->material.kShininess = 1.f;

	meshList[GEO_EYES] = MeshBuilder::GenerateCircle("Circle", Color(1, 1, 1), 15, 15);
	meshList[GEO_EYES]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYES]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_EYES]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_EYES]->material.kShininess = 1.f;

	meshList[GEO_PUPILS] = MeshBuilder::GenerateCircle("Circle", Color(0, 0, 0), 15, 15);
	meshList[GEO_PUPILS]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PUPILS]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PUPILS]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PUPILS]->material.kShininess = 1.f;

	meshList[GEO_TAILS] = MeshBuilder::GenerateCube("Cube", Color(0.2, 0.15, 0.12), 5, 5, 5);
	meshList[GEO_TAILS]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_TAILS]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TAILS]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_TAILS]->material.kShininess = 1.f;

	meshList[GEO_SPOTS] = MeshBuilder::GenerateCircle("Circle", Color(0.65, 0, 0), 15, 15);
	meshList[GEO_SPOTS]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPOTS]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SPOTS]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPOTS]->material.kShininess = 1.f;

	meshList[GEO_BODY] = MeshBuilder::GenerateSphere("Sphere", Color(1, 0, 0), 30, 10, 10);
	meshList[GEO_BODY]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BODY]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BODY]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BODY]->material.kShininess = 1.f;

	meshList[GEO_UNDERBELLY] = MeshBuilder::GenerateSphere("Sphere", Color(0.86, 0.77, 0.71), 30, 10, 10);
	meshList[GEO_UNDERBELLY]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_UNDERBELLY]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_UNDERBELLY]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_UNDERBELLY]->material.kShininess = 1.f;

	meshList[GEO_FEATHERS] = MeshBuilder::GenerateSphere("Sphere", Color(1, 0, 0), 30, 10, 10);
	meshList[GEO_FEATHERS]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FEATHERS]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FEATHERS]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FEATHERS]->material.kShininess = 1.f;

	meshList[GEO_BEAK1] = MeshBuilder::GenerateCone("Cone", Color(1, 1, 0), 100, 10, 10);
	meshList[GEO_BEAK1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BEAK1]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BEAK1]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BEAK1]->material.kShininess = 1.f;

	meshList[GEO_BEAK2] = MeshBuilder::GenerateCone("Cone", Color(1, 1, 0), 100, 10, 10);
	meshList[GEO_BEAK2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BEAK2]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BEAK2]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BEAK2]->material.kShininess = 1.f;

	meshList[GEO_SLING] = MeshBuilder::GenerateCylinder("Cylinder", Color(0.54, 0.45, 0.33), 100, 10, 10, 10);
	meshList[GEO_SLING]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SLING]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SLING]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SLING]->material.kShininess = 1.f;

	meshList[GEO_SLING1] = MeshBuilder::GenerateCylinder("Cylinder", Color(0.31, 0.22, 0.17), 100, 10, 10, 10);
	meshList[GEO_SLING1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SLING1]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SLING1]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SLING1]->material.kShininess = 1.f;

	meshList[GEO_BAND] = MeshBuilder::GenerateTorus("Torus", Color(0.31, 0.22, 0.17), 10, 10, 20, 2);
	meshList[GEO_BAND]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BAND]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BAND]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BAND]->material.kShininess = 1.f;

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 16, 32, 1);	
	meshList[GEO_LIGHTBALL1] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 16, 32, 1);
	meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 16, 32, 1);
	meshList[GEO_LIGHTBALL3] = MeshBuilder::GenerateSphere("sphere", Color(1, 1, 1), 16, 32, 1);
	
	// Skybox
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad2("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad2("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad2("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad2("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");
	meshList[GEO_BOTTOM]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BOTTOM]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOTTOM]->material.kShininess = 1.f;
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad2("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad2("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	// Models
	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJ("model1", "OBJ//fence.obj");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//fence.tga");
	meshList[GEO_MODEL1]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL1]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL1]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL1]->material.kShininess = 1.f;
	meshList[GEO_MODEL2] = MeshBuilder::GenerateOBJ("model1", "OBJ//soccer.obj");
	meshList[GEO_MODEL2]->textureID = LoadTGA("Image//soccer.tga");
	meshList[GEO_MODEL2]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL2]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL2]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL2]->material.kShininess = 1.f;
	meshList[GEO_MODEL3] = MeshBuilder::GenerateOBJ("model1", "OBJ//tree.obj");
	meshList[GEO_MODEL3]->textureID = LoadTGA("Image//tree.tga");
	meshList[GEO_MODEL3]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL3]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL3]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL3]->material.kShininess = 1.f;
	meshList[GEO_MODEL4] = MeshBuilder::GenerateOBJ("model1", "OBJ//plant.obj");
	meshList[GEO_MODEL4]->textureID = LoadTGA("Image//plant.tga");
	meshList[GEO_MODEL4]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL4]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL4]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL4]->material.kShininess = 1.f;
	meshList[GEO_MODEL5] = MeshBuilder::GenerateOBJ("model1", "OBJ//wood.obj");
	meshList[GEO_MODEL5]->textureID = LoadTGA("Image//wood.tga");
	meshList[GEO_MODEL5]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL5]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL5]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL5]->material.kShininess = 1.f;
	meshList[GEO_MODEL6] = MeshBuilder::GenerateOBJ("model1", "OBJ//rock.obj");
	meshList[GEO_MODEL6]->textureID = LoadTGA("Image//rock.tga");
	meshList[GEO_MODEL6]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL6]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL6]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL6]->material.kShininess = 1.f;
	meshList[GEO_MODEL7] = MeshBuilder::GenerateOBJ("model1", "OBJ//blue.obj");
	meshList[GEO_MODEL7]->textureID = LoadTGA("Image//blue.tga");
	meshList[GEO_MODEL7]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL7]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL7]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL7]->material.kShininess = 1.f;
	meshList[GEO_MODEL8] = MeshBuilder::GenerateOBJ("model1", "OBJ//sign.obj");
	meshList[GEO_MODEL8]->textureID = LoadTGA("Image//sign1.tga");
	meshList[GEO_MODEL8]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL8]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL8]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL8]->material.kShininess = 1.f;
	meshList[GEO_MODEL9] = MeshBuilder::GenerateOBJ("model1", "OBJ//mailbox.obj");
	meshList[GEO_MODEL9]->textureID = LoadTGA("Image//mailbox.tga");
	meshList[GEO_MODEL9]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL9]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL9]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL9]->material.kShininess = 1.f;
	meshList[GEO_MODEL10] = MeshBuilder::GenerateOBJ("model1", "OBJ//house.obj");
	meshList[GEO_MODEL10]->textureID = LoadTGA("Image//house.tga");
	meshList[GEO_MODEL10]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL10]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL10]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL10]->material.kShininess = 1.f;
	meshList[GEO_MODEL11] = MeshBuilder::GenerateOBJ("model1", "OBJ//coconut.obj");
	meshList[GEO_MODEL11]->textureID = LoadTGA("Image//coconut.tga");
	meshList[GEO_MODEL11]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL11]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL11]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL11]->material.kShininess = 1.f;
	meshList[GEO_MODEL12] = MeshBuilder::GenerateOBJ("model1", "OBJ//wood1.obj");
	meshList[GEO_MODEL12]->textureID = LoadTGA("Image//wood1.tga");
	meshList[GEO_MODEL12]->material.kAmbient.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL12]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MODEL12]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MODEL12]->material.kShininess = 1.f;
	//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga"); 
	meshList[GEO_TEXT1] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT1]->textureID = LoadTGA("Image//calibri.tga"); 
	meshList[GEO_TEXT2] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT2]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT3] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT3]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT4] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT4]->textureID = LoadTGA("Image//calibri.tga");

	// Make sure you pass uniform parameters after glUseProgram()
	// prac 7 part 2
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 4);
	//
}

void Scene1::Update(double dt)
{
	FPS = 1 / dt; // for fps

	if (Application::IsKeyPressed(0x31)) // Enable cull
	{
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x32)) // Disable cull
	{
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // default fill mode
	}
	if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe mode
	}

	if (Application::IsKeyPressed('Z')) // fly straight
	{
		flying = true;
		static const float ROTATE_SPEED = 100.f;
		if (shoot < 290)
		{
			shoot += (float)(ROTATE_SPEED * dt);
		}

		direction = 1;
		if (band * direction > -1) // slingshot is shot
		{
			band += (float)(-1.f * direction * dt);
			move += (float)(35.f * direction * dt);
		}
		else
		{

		}
		if (shoot > 280) // if bird flys to this point, wooden structure breaks
		{
			hit = true;
		}
	}
	if (Application::IsKeyPressed('X')) // reset
	{
		flying = false;
		hit = false;
		fence = false;
		drop = false;
		shoot = 0;
		band = 1;
		move = 0;
		ball = 0;
	}
	if (Application::IsKeyPressed('C')) // change expression of red
	{	
		static const float ROTATE_SPEED = 10.f;
		static int rotateDir = 1;
		if (rotateAngle * rotateDir > 6)
			rotateDir = -rotateDir;
		rotateAngle += (float)(rotateDir * ROTATE_SPEED * dt);

	}
	if (Application::IsKeyPressed('V')) // open red's mouth
	{
		static const float ROTATE_SPEED = 10.f;
		static int rotateDir = 1;
		if (rotateAngle1 * rotateDir > 3)
			rotateDir = -rotateDir;
		rotateAngle1 += (float)(rotateDir * ROTATE_SPEED * dt);
	}
	if (Application::IsKeyPressed('B')) // kick ball
	{
		static const float ROTATE_SPEED = 100.f;

		if (ball < 150)
		{
			ball += (float)(ROTATE_SPEED * dt);
		}
		if (ball > 150) // fence falls when ball hits
		{
			fence = true;
		}
	}

	if (Application::IsKeyPressed('N')) // Tree
	{
		static const float ROTATE_SPEED = 150.f;
		static int rotateDir = 1;
		if (rotateAngle2 * rotateDir > 2)
			rotateDir = -rotateDir;
		rotateAngle2 += (float)(rotateDir * 10 * dt);

		if (drop < 275) //stop dropping when coconut hits the ground
		{
			drop += (float)(ROTATE_SPEED * dt); // coconut drops when tree is shaking
		}
	}

	if (Application::IsKeyPressed('F')) //Key_Spot
	{
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('G')) //Key_Directional
	{
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}

	if (Application::IsKeyPressed('H')) //Key_Spot
	{
		light[1].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);

		light[2].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);

		light[3].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	}
	else if (Application::IsKeyPressed('J')) // Key_Point
	{
		light[1].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);

		light[2].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);

		light[3].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	}
	
	camera.Update(dt);
}

void Scene1::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE,
			&modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	// Prac 9
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Scene1::RenderTrail()
{
	if (flying == true)
	{
		// trail 1
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -40);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, -1);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_TRAIL], false);
		modelStack.PopMatrix();
		//
		// trail 2
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -50);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, -1);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_TRAIL], false);
		modelStack.PopMatrix();
		//
		// trail 3
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -60);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, -1);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_TRAIL], false);
		modelStack.PopMatrix();
		//
		// trail 4
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -70);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, -1);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_TRAIL], false);
		modelStack.PopMatrix();
		//
		// trail 5
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -80);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, -1);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_TRAIL], false);
		modelStack.PopMatrix();
		//
	}
}

void Scene1::RenderBrow1()
{
	// eyebrow1
	modelStack.PushMatrix();
	modelStack.Translate(-5.25, 8, 18.75);
	modelStack.Rotate(10, -1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_EYEBROW1], true);
	modelStack.PopMatrix();
	//
}
void Scene1::RenderBrow2()
{
	// eyebrow2
	modelStack.PushMatrix();
	modelStack.Translate(5.25, 8, 18.75);
	modelStack.Rotate(10, -1, 0, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_EYEBROW2], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderEyes()
{
	// eye1
	modelStack.PushMatrix();
	modelStack.Translate(-4.25, 4, 18.7);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(10, -1, 0, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderMesh(meshList[GEO_EYES], true);
	modelStack.PopMatrix();
	//
	// eye2
	modelStack.PushMatrix();
	modelStack.Translate(4.25, 4, 18.7);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(10, -1, 0, 0);
	modelStack.Scale(0.3, 0.3, 0.3);
	RenderMesh(meshList[GEO_EYES], true);
	modelStack.PopMatrix();
	//
	// puppil1
	modelStack.PushMatrix();
	modelStack.Translate(-4, 4, 18.8);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(10, -1, 0, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_PUPILS], true);
	modelStack.PopMatrix();
	//
	// pupil2
	modelStack.PushMatrix();
	modelStack.Translate(4, 4, 18.8);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(10, -1, 0, 0);
	modelStack.Scale(0.1, 0.1, 0.1);
	RenderMesh(meshList[GEO_PUPILS], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderTail()
{
	// tail center
	modelStack.PushMatrix();
	modelStack.Translate(0, -4, -20);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(5, 15, 5);
	RenderMesh(meshList[GEO_TAILS], true);
	modelStack.PopMatrix();
	//
	// tail bottom
	modelStack.PushMatrix();
	modelStack.Translate(0, -10, -20);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Scale(5, 15, 5);
	RenderMesh(meshList[GEO_TAILS], true);
	modelStack.PopMatrix();
	//
	// tail top
	modelStack.PushMatrix();
	modelStack.Translate(0, 2, -20);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(45, 0, 0, -1);
	modelStack.Scale(5, 15, 5);
	RenderMesh(meshList[GEO_TAILS], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderSpots()
{
	// spots1
	modelStack.PushMatrix();
	modelStack.Translate(8, -3.5, 18);
	modelStack.Rotate(100, 1, 0, 0);
	modelStack.Rotate(25, 0, 0, -1);
	modelStack.Scale(0.1, 0.2, 0.1);
	RenderMesh(meshList[GEO_SPOTS], true);
	modelStack.PopMatrix();
	//
	// spots2
	modelStack.PushMatrix();
	modelStack.Translate(11, -3.5, 16);
	modelStack.Rotate(100, 1, 0, 0);
	modelStack.Rotate(40, 0, 0, -1);
	modelStack.Scale(0.1, 0.15, 0.1);
	RenderMesh(meshList[GEO_SPOTS], true);
	modelStack.PopMatrix();
	//
	// spots3
	modelStack.PushMatrix();
	modelStack.Translate(14, -3.5, 14);
	modelStack.Rotate(100, 1, 0, 0);
	modelStack.Rotate(45, 0, 0, -1);
	modelStack.Scale(0.1, 0.125, 0.1);
	RenderMesh(meshList[GEO_SPOTS], true);
	modelStack.PopMatrix();
	//
	// spots4
	modelStack.PushMatrix();
	modelStack.Translate(-8, -3.5, 18);
	modelStack.Rotate(100, 1, 0, 0);
	modelStack.Rotate(25, 0, 0, 1);
	modelStack.Scale(0.1, 0.2, 0.1);
	RenderMesh(meshList[GEO_SPOTS], true);
	modelStack.PopMatrix();
	//
	// spots5
	modelStack.PushMatrix();
	modelStack.Translate(-11, -3.5, 16);
	modelStack.Rotate(100, 1, 0, 0);
	modelStack.Rotate(40, 0, 0, 1);
	modelStack.Scale(0.1, 0.15, 0.1);
	RenderMesh(meshList[GEO_SPOTS], true);
	modelStack.PopMatrix();
	//
	// spots6
	modelStack.PushMatrix();
	modelStack.Translate(-14, -3.5, 14);
	modelStack.Rotate(100, 1, 0, 0);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Scale(0.1, 0.125, 0.1);
	RenderMesh(meshList[GEO_SPOTS], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderBody()
{
	// body
	modelStack.PushMatrix();
	modelStack.Scale(2.05, 2, 2);
	RenderMesh(meshList[GEO_BODY], true);
	modelStack.PopMatrix();
	//
	// underbelly
	modelStack.PushMatrix();
	modelStack.Translate(0, -9, 3.25);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(1.5, 1, 1.5);
	RenderMesh(meshList[GEO_UNDERBELLY], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderFeather()
{
	// feather1
	modelStack.PushMatrix();
	modelStack.Translate(4, 21, 7);
	modelStack.PushMatrix();
	modelStack.Translate(0, -10, 0); // back
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Translate(0, 10, 0); // to origin
	modelStack.Scale(0.2, 0.5, 0.2);
	RenderMesh(meshList[GEO_FEATHERS], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	//
	// feather2
	modelStack.PushMatrix();
	modelStack.Translate(4, 21, 7);
	modelStack.PushMatrix();
	modelStack.Translate(0, -10, 0); // back
	modelStack.Rotate(30, 0, 0, 1);
	modelStack.Translate(0, 10, 0); // to origin
	modelStack.Scale(0.2, 0.5, 0.2);
	RenderMesh(meshList[GEO_FEATHERS], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	//
}

void Scene1::RenderUBeak()
{
	// upper beak
	modelStack.PushMatrix();
	modelStack.Translate(0, -5, 15);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1.65, 1.5, 1.5);
	RenderMesh(meshList[GEO_BEAK1], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderLBeak()
{
	// lower beak
	modelStack.PushMatrix();
	modelStack.Translate(0, -5.4, 15);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1.6, 1.3, 1.3);
	RenderMesh(meshList[GEO_BEAK2], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderSlingshot()
{
	// Sling handle
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 40);
	modelStack.Scale(0.5, 5, 0.5);
	RenderMesh(meshList[GEO_SLING], true);
	modelStack.PopMatrix();
	//
	// Sling Left
	modelStack.PushMatrix();
	modelStack.Translate(-16.5, -19, 40);
	modelStack.Rotate(45, 0, 0, 1);
	modelStack.Scale(0.5, 5, 0.5);
	RenderMesh(meshList[GEO_SLING], true);
	modelStack.PopMatrix();
	//
	// Sling Right
	modelStack.PushMatrix();
	modelStack.Translate(16.5, -19, 40);
	modelStack.Rotate(45, 0, 0, -1);
	modelStack.Scale(0.5, 5, 0.5);
	RenderMesh(meshList[GEO_SLING], true);
	modelStack.PopMatrix();
	//
	// Sling1 Left
	modelStack.PushMatrix();
	modelStack.Translate(-27, -8, 40);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_SLING1], true);
	modelStack.PopMatrix();
	//
	// Sling1 Right
	modelStack.PushMatrix();
	modelStack.Translate(27, -8, 40);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_SLING1], true);
	modelStack.PopMatrix();
	//

}

void Scene1::RenderBand()
{
	// Band
	modelStack.PushMatrix();
	modelStack.Translate(0, -2, 40);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(1.25, 4, 0.5);
	RenderMesh(meshList[GEO_BAND], true);
	modelStack.PopMatrix();
	//
}

void  Scene1::RenderBluebird()
{
	// bird
	modelStack.PushMatrix();
	modelStack.Translate(-5, -75, -290);
	modelStack.Rotate(90, 0, -1, 0);
	//modelStack.Rotate(20, -1, 0, 0);
	modelStack.Scale(4, 4, 4);
	RenderMesh(meshList[GEO_MODEL7], true);
	modelStack.PopMatrix();
	//
}

void  Scene1::RenderBall()
{
	// ball
	modelStack.PushMatrix();
	modelStack.Translate(50, -55, -300);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_MODEL2], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderTree()
{
	// tree
	modelStack.PushMatrix();
	modelStack.Translate(-100, -80, -300);
	modelStack.Rotate(90, -1, 0, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	RenderMesh(meshList[GEO_MODEL3], true);
	modelStack.PopMatrix();
	//
}

void Scene1::RenderCoconut()
{
	// coconut
	modelStack.PushMatrix();
	modelStack.Translate(-80, 200, -300);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_MODEL11], false);
	modelStack.PopMatrix();
	//
}

static const float SKYBOXSIZE = 1000.f;
void Scene1::RenderSkybox() //render skybox
{
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 430,-200);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 430,-800);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(300, 430, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(-300, 430, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(300, 0, -930);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(300, 0, -70);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

void Scene1::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() *
			characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Scene1::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST);

	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() *
			characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Scene1::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f); //FOV, Aspect Ratio, Near plane, Far plane
	
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y,
		camera.up.z);
	modelStack.LoadIdentity();

	//Insert new code (Prac 7 part 2)
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
			&lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
			&lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *
			light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1,
			&spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1,
			&lightPosition_cameraspace.x);
	}

	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
			&lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
			&lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *
			light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1,
			&spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1,
			&lightPosition_cameraspace.x);
	}

	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1,
			&lightDirection_cameraspace.x);
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1,
			&lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *
			light[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1,
			&spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1,
			&lightPosition_cameraspace.x);
	}

	if (light[3].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1,
			&lightDirection_cameraspace.x);
	}
	else if (light[3].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1,
			&lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() *
			light[3].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1,
			&spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1,
			&lightPosition_cameraspace.x);
	}

	Mtx44 mvp = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	view.SetToLookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);

	//!fly & facial expression animation
	// Brow1 fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	// eyebrow1 animation
	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle, 0, 0, -2);
	RenderBrow1();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	//
	// Brow2 fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	// eyebrow2 animation
	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle, 0, 0, 2);
	RenderBrow2();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	//
	// Eyes fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	RenderEyes();
	modelStack.PopMatrix();
	//
	// Tail fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	RenderTail();
	modelStack.PopMatrix();
	//
	// Spots fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	RenderSpots();
	modelStack.PopMatrix();
	//
	// Body fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	RenderBody();
	modelStack.PopMatrix();
	//
	// Feathers fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	RenderFeather();
	modelStack.PopMatrix();
	//
	// UBeak fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	RenderUBeak();
	modelStack.PopMatrix();
	//
	// LBeak fly
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, shoot);
	// LBeak move
	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle1, 1, 0, 0);
	RenderLBeak();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	//
	// Band
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, move);
	modelStack.Scale(1.25, 4, band);
	RenderBand();
	modelStack.PopMatrix();
	//
	// trail
	if (flying == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, shoot);
		RenderTrail();
		modelStack.PopMatrix();
	}
	//!

	// light
	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL1], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[2].position.x, light[2].position.y, light[2].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[3].position.x, light[3].position.y, light[3].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL3], false);
	modelStack.PopMatrix();
	//

	// Render Sling
	RenderSlingshot();
	model.SetToIdentity();
	//

	// fence
	if (fence == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(400, -70, -350);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_MODEL1], true);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(225, -250, -350);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Rotate(90, 0, 0, -1);
		modelStack.Scale(40, 40, 40);
		RenderMesh(meshList[GEO_MODEL1], true);
		modelStack.PopMatrix();
	}
	//

	// kick ball
	modelStack.PushMatrix();
	modelStack.Translate(ball, 0, 0);
	RenderBall();
	modelStack.PopMatrix();
	//
	
	// blue bird
	RenderBluebird();
	//

	// tree
	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle2, 0, 0, -1);
	RenderTree();
	modelStack.PopMatrix();
	//
	// coconut
	modelStack.PushMatrix();
	modelStack.Translate(0, -drop, 0);
	RenderCoconut();
	modelStack.PopMatrix();
	//

	// plant
	modelStack.PushMatrix();
	modelStack.Translate(610, 200, -290);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_MODEL4], true);
	modelStack.PopMatrix();
	//

	// wood
	if (hit == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -70, 300);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_MODEL5], true);
		modelStack.PopMatrix();
	}
	else if (hit == true)
	{
		// wood1
		modelStack.PushMatrix();
		modelStack.Translate(0, -70, 300);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(20, 20, 20);
		RenderMesh(meshList[GEO_MODEL12], true);
		modelStack.PopMatrix();
		//
	}
	//

	// rock
	modelStack.PushMatrix();
	modelStack.Translate(680, -70, 420);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Scale(0.4, 0.4, 0.4);
	RenderMesh(meshList[GEO_MODEL6], true);
	modelStack.PopMatrix();
	//

	// sign
	modelStack.PushMatrix();
	modelStack.Translate(565, -75, -175);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_MODEL8], true);
	modelStack.PopMatrix();
	//

	// mailbox
	modelStack.PushMatrix();
	modelStack.Translate(725, -70, -180);
	modelStack.Rotate(90, 0, -1, 0);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[GEO_MODEL9], true);
	modelStack.PopMatrix();
	//

	// house
	modelStack.PushMatrix();
	modelStack.Translate(625, -80, -300);
	modelStack.Scale(8, 8, 8);
	RenderMesh(meshList[GEO_MODEL10], true);
	modelStack.PopMatrix();
	//

	// Render Axes
	RenderMesh(meshList[GEO_AXES], false);
	//
	RenderSkybox();

	// Must be last!
	// instructions on screen
	modelStack.PushMatrix();
	modelStack.Translate(0, 30, -300);
	modelStack.Scale(7, 7, 7);
	RenderText(meshList[GEO_TEXT], "Press 'B' to kick the ball!", Color(0, 1, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100, -80, -300);
	modelStack.Scale(7, 7, 7);
	RenderText(meshList[GEO_TEXT1], "Press 'N' to shake the tree!", Color(0, 1, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-50, 50,0);
	modelStack.Scale(7, 7, 7);
	RenderText(meshList[GEO_TEXT2], "Press 'Z' to shoot the bird!", Color(0, 1, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, -50, 50);
	modelStack.Scale(7, 7, 7);
	RenderText(meshList[GEO_TEXT3], "Press 'F' & 'G' to turn Directional light on and off!", Color(0, 1, 0));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 0, -165);
	modelStack.Scale(5, 5, 5);
	RenderText(meshList[GEO_TEXT4], "Press 'H' & 'J' to switch between Spot and Point light!", Color(0, 1, 0));
	modelStack.PopMatrix();

	RenderTextOnScreen(meshList[GEO_TEXT], "Welcome to Bird Island!", Color(0, 1, 1), 3, 1, 1); // text on screen
	RenderTextOnScreen(meshList[GEO_TEXT], "FPS: ", Color(1, 1, 0), 3, 1, 2);
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(FPS), Color(1, 1, 0), 3, 3, 2);
}

void Scene1::Exit()
{
	// Cleanup VBO here
	glDeleteProgram(m_programID);
}