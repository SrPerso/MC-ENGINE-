#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Glew\include\glew.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "CCamera.h"
#include "GameObject.h"
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
	LOGUI("-START- Loading Intro assets");
	bool ret = true;	
	GameObject* camera = new GameObject(App->goManager->GetRoot());
	
	DCamera* dcamera = new DCamera();
	App->goManager->GetRoot()->AddChild(camera);
	camera->CreateComponent(COMP_CAMERA, dcamera);
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	LOGUI("-CLEANUP- Cleaning scene Objects");


	while (!GeometryObjects.empty()) {

		delete GeometryObjects.front();
		GeometryObjects.pop_front();
	}
	
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

void ModuleSceneIntro::CreateSphere(float radius, vec3 pos, int numStacks, int numSides)
{
	PrimitiveSphere* sphere = new PrimitiveSphere;

	sphere->radius = radius;
	sphere->SetPos(pos.x, pos.y, pos.z);
	sphere->numSides = numSides;
	sphere->numStacks = numStacks;

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

void ModuleSceneIntro::CreateCube1(vec3 size, vec3 pos)
{
	Cube1* cube1 = new Cube1(size);
	cube1->size.x= size.x;
	cube1->size.y = size.y;
	cube1->size.z = size.z;
	cube1->SetPos(pos.x, pos.y, pos.z);

	GeometryObjects.push_back(cube1);
	App->physics->AddBody(*cube1);
	

}

void ModuleSceneIntro::CreateCube2(vec3 size, vec3 pos)
{
	Cube2* cube2 = new Cube2(size);
	cube2->size.x = size.x;
	cube2->size.y = size.y;
	cube2->size.z = size.z;
	cube2->SetPos(pos.x, pos.y, pos.z);

	
	GeometryObjects.push_back(cube2);
	App->physics->AddBody(*cube2);


}
void ModuleSceneIntro::CreateLine(vec3 Origin, vec3 destintation,Color color)
{
	PrimitiveLine* line = new PrimitiveLine(Origin, destintation);
	line->color = color;
	NormalsLines.push_back(line);
}


void ModuleSceneIntro::Draw()
{
	/*for (std::list<Primitive*>::iterator it = GeometryObjects.begin(); it != GeometryObjects.end(); ++it)
	{
		(*it)->Render();
	}*/

	for (std::list<PrimitiveLine*>::iterator it = NormalsLines.begin(); it != NormalsLines.end(); ++it)
	{
		(*it)->Render();
	}
}

void ModuleSceneIntro::CreateCylinder(const float x, const float y, const float z, const float radious, const float h)
{
	PrimitiveCylinder* toCreate = new PrimitiveCylinder(radious, h);
	toCreate->SetPos(x, y, z);
	toCreate->color = White;
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
	
	PrimitivePlane p(0, -1, 0, 200);
	p.color = White;
	p.axis = false;
	p.Render();

	return UPDATE_CONTINUE;

}

