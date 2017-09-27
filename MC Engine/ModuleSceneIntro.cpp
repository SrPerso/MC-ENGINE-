#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "Math.h"


#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");

	bool ret = true;

	

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	
	return true;
}

void ModuleSceneIntro::CreateCylinder(const float x, const float y, const float z, const float radious, const float h)
{
	PrimitiveCylinder* toCreate = new PrimitiveCylinder(radious, h);
	toCreate->SetPos(x, y, z);
	toCreate->color = White;
	//test->push_front(toCreate);
}

// PreUpdate
update_status ModuleSceneIntro::PreUpdate(float dt)
{


	return(UPDATE_CONTINUE);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{	


	float sx = 1 * 0.5f;
	float sy = 1 * 0.5f;
	float sz = 1 * 0.5f;



	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);//c

	glVertex3f(0, 0, 1);//c
	glVertex3f(0, 1, 1);//d
	glVertex3f(0, 1, 0);//a

	glVertex3f(1, 1, 1);//e
	glVertex3f(0, 1, 1);//d
	glVertex3f(0, 0, 1);//c

	glVertex3f(0, 0, 1);//c
	glVertex3f(1, 0, 1);//f
	glVertex3f(1, 1, 1);//e

	glVertex3f(1, 0, 1);//f
	glVertex3f(1, 1, 1);//e
	glVertex3f(1, 1, 0);//g

	glVertex3f(1, 0, 1);//f
	glVertex3f(1, 0, 0);//h
	glVertex3f(1, 1, 0);//g

	glVertex3f(1, 1, 0);//g
	glVertex3f(1, 0, 0);//h
	glVertex3f(0, 0, 0);//bc

	glVertex3f(0, 0, 0);//bc
	glVertex3f(1, 1, 0);//g
	glVertex3f(0, 1, 0);//a
 
	glVertex3f(1, 1, 1);//e
	glVertex3f(1, 1, 0);//g
	glVertex3f(0, 1, 0);//a

	glVertex3f(0, 1, 0);//a
	glVertex3f(0, 1, 1);//d
	glVertex3f(1, 1, 1);//e

	glVertex3f(1, 0, 1);//f
	glVertex3f(1, 0, 0);//h
	glVertex3f(0, 0, 0);//b

	glVertex3f(0, 0, 0);//b
	glVertex3f(0, 0, 1);//c
	glVertex3f(1, 0, 1);//f
	glEnd();

	glLineWidth(1.0f);


	//PrimitiveSphere sp(2);
	//sp.SetPos(0, 3, 0);
	//sp.color = Red;
	//sp.Render();

	PrimitivePlane p(0, -1, 0, 200);
	p.color = White;
	p.axis = false;
	p.Render();

	//Cube Poste( 1,1,1);
	//Poste.SetRotation(90, vec3(0, 0, 1));
	//Poste.SetPos(0, 0,0);
	//Poste.color = White;
	//Poste.Render();

	return UPDATE_CONTINUE;

}

