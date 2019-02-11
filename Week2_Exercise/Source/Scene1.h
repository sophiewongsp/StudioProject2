#ifndef SCENE_1_H
#define SCENE_1_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "MatrixStack.h"
#include "Light.h"

class Scene1 : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_NUMLIGHTS, //in case you missed out practical 7
		U_LIGHTENABLED,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		
		GEO_TRI,
		GEO_CUBE,
		GEO_HEMI,
		GEO_QUAD,
		GEO_TRAIL,
		GEO_EYEBROW1,
		GEO_EYEBROW2,
		GEO_EYES,
		GEO_PUPILS,
		GEO_TAILS,
		GEO_SPOTS,
		GEO_BODY,
		GEO_UNDERBELLY,
		GEO_FEATHERS,
		GEO_BEAK1,
		GEO_BEAK2,
		GEO_SLING,
		GEO_SLING1,
		GEO_BAND,
		GEO_FLOOR,
		GEO_AXES,
		GEO_LIGHTBALL,
		GEO_LIGHTBALL1,
		GEO_LIGHTBALL2,
		GEO_LIGHTBALL3,
		//add these enum in GEOMETRY_TYPE before NUM_GEOMETRY
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_MODEL3,
		GEO_MODEL4,
		GEO_MODEL5,
		GEO_MODEL6,
		GEO_MODEL7,
		GEO_MODEL8,
		GEO_MODEL9,
		GEO_MODEL10,
		GEO_MODEL11,
		GEO_MODEL12,
		GEO_TEXT,
		GEO_TEXT1,
		GEO_TEXT2,
		GEO_TEXT3,
		GEO_TEXT4,
		NUM_GEOMETRY,
	};
public:
	Scene1();
	~Scene1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	MS modelStack, viewStack, projectionStack;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY]; // New to Scene 3

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float rotateAngle1;
	float rotateAngle2;
	float shoot;
	float band;
	float move;
	float ball;
	float drop;
	float direction;
	float translateX;
	float scaleAll;

	float rotateAmt;
	float translateAmt;
	float scaleAmt;

	int FPS;

	float LSPEED;

	Camera2 camera;

	void RenderTrail();
	void RenderBrow1();
	void RenderBrow2();
	void RenderEyes();
	void RenderTail();
	void RenderSpots();
	void RenderBody();
	void RenderFeather();
	void RenderUBeak();
	void RenderLBeak();
	void RenderSlingshot();
	void RenderBand();
	void RenderSkybox(); // prac 9 part 2
	void RenderBall();
	void RenderBluebird();
	void RenderTree();
	void RenderCoconut();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

	Light light[4];
	void RenderMesh(Mesh *mesh, bool enableLight);
};

#endif