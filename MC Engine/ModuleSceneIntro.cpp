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

void ModuleSceneIntro::CreateCube(vec3 size, vec3 pos)
{
	Cube* cube = new Cube;
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);

	cube->wire = true;
	
	GeometryObjects.push_back(cube);
	App->physics->AddBody(*cube);

}

void ModuleSceneIntro::CreateSphere(float radius, vec3 pos)
{
	PrimitiveSphere* sphere = new PrimitiveSphere;

	sphere->radius = radius;
	sphere->SetPos(pos.x, pos.y, pos.z);

	sphere->wire = true;
	
	GeometryObjects.push_back(sphere);
	App->physics->AddBody(*sphere);

}

void ModuleSceneIntro::CreateCylinder(float radius,float height, vec3 pos)
{
	PrimitiveCylinder* cylinder = new PrimitiveCylinder;

	cylinder->radius = radius;
	cylinder->height = height;
	cylinder->SetPos(pos.x,pos.y,pos.z);

	cylinder->wire = true;
	GeometryObjects.push_back(cylinder);
	App->physics->AddBody(*cylinder);
	

}


void ModuleSceneIntro::Draw()
{
	for (std::list<Primitive*>::iterator it = GeometryObjects.begin(); it != GeometryObjects.end(); ++it)
	{
		(*it)->Render();
	}

	
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
	//Poste.Render();*/

	return UPDATE_CONTINUE;

}

