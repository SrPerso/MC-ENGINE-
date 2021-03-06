#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Glew\include\glew.h"
#pragma comment (lib, "Glew/libx86/glew32.lib") 



#include <gl/GL.h>
#include <gl/GLU.h>


#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128
#define NORMAL_SIZE 0.5f
class CMesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	GLuint loadBMP_custom(const char *imagepath);
	

	void EDglView();

	void OnResize(int width, int height);
	
	void DrawGO(GameObject* GOToDraw);

	
	void Draw(CMesh* meshToDraw);
	void DrawDebug(CMesh* meshToDraw);

public:
	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};