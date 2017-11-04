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
#include "CMesh.h"
#include "CTransformation.h"


#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define MINDISTANCE 1000;

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

	camera->CreateComponent(COMP_CAMERA, dcamera);
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	MinDistance = MINDISTANCE;

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

GameObject* ModuleSceneIntro::SelectObject(LineSegment  picking)
{
	
	GameObject* closest = nullptr;
	
	closest=IntersectAABB(picking);

	
	if (closest != nullptr) 
	{
		closest=IntersectTriangle(picking, closest);
		
	
	}
	else 
	{
		
		for (std::list<GameObject*>::iterator it = DistanceList.begin(); it != DistanceList.end(); ++it)
		{

		}
	}

	return closest;
	
}

GameObject*  ModuleSceneIntro::IntersectAABB(LineSegment picking)
{
	GameObject* Closest = nullptr;
	CMesh* IntersectMesh = nullptr;
	CTransformation* transform= nullptr;
	for (uint i = 0; i < App->goManager->GetRoot()->childs.size(); i++)
	{		
		IntersectMesh = (CMesh*)App->goManager->GetRoot()->childs[i]->GetComponent(COMP_MESH);
		if (IntersectMesh != nullptr) 
		{
			if (picking.Intersects(IntersectMesh->debugBox) == true)
			{
				DistanceList.push_back(App->goManager->GetRoot()->childs[i]);
				
			}
		}
	}

	
	if (DistanceList.size() > 0)
	{
		for (std::list<GameObject*>::iterator it = DistanceList.begin(); it != DistanceList.end(); ++it) 
		{			
			transform = ((CTransformation*)(*it)->GetComponent(COMP_TRANSFORMATION));
			float thisDist = (transform->position - App->camera->camera->frustum.pos).Length();
			if (thisDist < MinDistance) 
			{			
				Closest = (*it);
				MinDistance = thisDist;
			}
		}
	}
	

	return Closest;

}

GameObject* ModuleSceneIntro::IntersectTriangle(LineSegment picking, GameObject * closest)
{
	GameObject* newObj=nullptr;
	CMesh* thisMesh = nullptr;
	CTransformation* thisTransformation = nullptr;
	float distance = 0;

	thisMesh = (CMesh*)closest->GetComponent(COMP_MESH);
	thisTransformation = (CTransformation*)closest ->GetComponent(COMP_TRANSFORMATION);

	LineSegment newSegment = picking;
	newSegment.Transform(thisTransformation->GetTransMatrix().Inverted());

	Triangle tri;

	for (uint i = 0; i < thisMesh->nVertex;)
	{

		tri.a.Set(thisMesh->nVertex[&thisMesh->Index[i++]], thisMesh->nVertex[&thisMesh->Index[i]], thisMesh->nVertex[&thisMesh->Index[i]]);
		tri.b.Set(thisMesh->nVertex[&thisMesh->Index[i++]], thisMesh->nVertex[&thisMesh->Index[i]], thisMesh->nVertex[&thisMesh->Index[i]]);
		tri.c.Set(thisMesh->nVertex[&thisMesh->Index[i++]], thisMesh->nVertex[&thisMesh->Index[i]], thisMesh->nVertex[&thisMesh->Index[i]]);

		float* LocalDistance = 0;
		float3 LocalHitPoint;
		LocalHitPoint.x = 0; LocalHitPoint.y = 0; LocalHitPoint.z = 0;
		if (newSegment.Intersects(tri, LocalDistance, &LocalHitPoint)) 
		{
			newObj = closest;
			return newObj;
		}
	}

	return newObj;

}

void ModuleSceneIntro::ObjectSelected(GameObject * selected)
{
}


void ModuleSceneIntro::Draw()
{
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

