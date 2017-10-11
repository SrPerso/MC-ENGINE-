#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#define MAX_LIGHTS 8
#define CHECKERS_HEIGHT 128
#define CHECKERS_WIDTH 128
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

	void Draw(ObjectMesh meshToDraw);
	void DrawDebug(ObjectMesh* meshToDraw);

public: // View Modes
	void TextureView();
	void WireSet(bool wireon=false);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};