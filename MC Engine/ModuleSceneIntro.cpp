#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

#include "Glew\include\glew.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "CCamera.h"
#include "GameObject.h"
#include "Math.h"
#include "CMesh.h"
#include "CTransformation.h"
#include "MathGeoLib\Geometry\LineSegment.h"


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


	camera->CreateComponent(COMP_CAMERA,-1, dcamera);
	CCamera* mainSceneCam = (CCamera*)camera->GetComponent(COMP_CAMERA);
	App->camera->mainCam = mainSceneCam;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	MinDistance = MINDISTANCE;

	delete dcamera;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	LOGUI("-CLEANUP- Cleaning scene Objects");


	while (!GeometryObjects.empty()) 
	{
		delete GeometryObjects.front();
		GeometryObjects.pop_front();
	}
	
	
	while (!NormalsLines.empty())
	{
		delete NormalsLines.front();
		NormalsLines.pop_front();
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

}

void ModuleSceneIntro::CreateCylinder(float radius,float height, vec3 pos)
{
	PrimitiveCylinder* cylinder = new PrimitiveCylinder;

	cylinder->radius = radius;
	cylinder->height = height;
	cylinder->SetPos(pos.x,pos.y,pos.z);

	cylinder->wire = true;
	GeometryObjects.push_back(cylinder);


}

void ModuleSceneIntro::CreateCube1(vec3 size, vec3 pos)
{
	Cube1* cube1 = new Cube1(size);
	cube1->size.x= size.x;
	cube1->size.y = size.y;
	cube1->size.z = size.z;
	cube1->SetPos(pos.x, pos.y, pos.z);

	GeometryObjects.push_back(cube1);

}

void ModuleSceneIntro::CreateCube2(vec3 size, vec3 pos)
{
	Cube2* cube2 = new Cube2(size);
	cube2->size.x = size.x;
	cube2->size.y = size.y;
	cube2->size.z = size.z;
	cube2->SetPos(pos.x, pos.y, pos.z);

	
	GeometryObjects.push_back(cube2);



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
	std::vector<GameObject*> DistanceList;

	IntersectAABB(picking, DistanceList);

	if (DistanceList.size() > 0)
	{
		float Lastdistance = MINDISTANCE;
		for (uint i = 0; i < DistanceList.size(); i++)
		{
			float distance = 15000;
			float3 hitpoint;
			DistanceList[i]->TriIntersection(picking, distance, hitpoint);
			if (distance < Lastdistance)
			{
				Lastdistance = distance;
				closest = DistanceList[i];
			}
		}
	}

	return closest;
	
}

void  ModuleSceneIntro::IntersectAABB(LineSegment &picking, std::vector<GameObject*>& DistanceList)
{

	GameObject* Closest = nullptr;
	

	
	

	for (uint i = 0; i < App->goManager->GetRoot()->childs.size(); i++)
	{
		if (App->goManager->GetRoot()->childs[i]->childs.size() > 0)
		{
			for (uint p = 0; p < App->goManager->GetRoot()->childs[i]->childs.size(); p++)
			{
				LineSegment newSegment(picking);
				CMesh* IntersectMesh = (CMesh*)App->goManager->GetRoot()->childs[i]->childs[p]->GetComponent(COMP_MESH);
				CTransformation* IntersectTransform = (CTransformation*)App->goManager->GetRoot()->childs[i]->childs[p]->GetComponent(COMP_TRANSFORMATION);
				if (IntersectTransform != nullptr) {

					newSegment.Transform(IntersectTransform->GetTransMatrix().Inverted());
					
				}

				if (IntersectMesh != nullptr)
				{
					if (newSegment.Intersects(IntersectMesh->debugBox) == true)
					{
						DistanceList.push_back(App->goManager->GetRoot()->childs[i]->childs[p]);


					}
				}
			}
		}
		else {
			LineSegment newSegment(picking);
			CMesh* IntersectMesh = (CMesh*)App->goManager->GetRoot()->childs[i]->GetComponent(COMP_MESH);
			CTransformation* IntersectTransform = (CTransformation*)App->goManager->GetRoot()->childs[i]->GetComponent(COMP_TRANSFORMATION);
			if (IntersectTransform != nullptr) {

				newSegment.Transform(IntersectTransform->GetTransMatrix().Inverted());
			}
			if (IntersectMesh != nullptr)
			{
				if (newSegment.Intersects(IntersectMesh->debugBox) == true)
				{
					DistanceList.push_back(App->goManager->GetRoot()->childs[i]);
					LOGUI("hit");

				}
			}
		}
	}
}



void ModuleSceneIntro::ObjectSelected(GameObject * selected)
{
	
	
		if (selected != nullptr) {			
		
				if (sceneSelected != selected)
				{
					if (this->sceneSelected != nullptr)
					{
						this->sceneSelected->selecting = false;
					}
					selected->selecting = true;
					this->sceneSelected = selected;
				}
				else
				{

					this->sceneSelected->selecting = false;
					this->sceneSelected = nullptr;
				}
			

		}
	
	
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

	if (sceneSelected != nullptr)
	{
		if (sceneSelected->selecting == true) {
			sceneSelected->OnSelection();
		}
	}

	return UPDATE_CONTINUE;

}

