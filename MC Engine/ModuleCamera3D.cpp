#include "Globals.h"
#include "Application.h"

#include "ModuleCamera3D.h"
#include "parson\parson.h"

#include "Primitive.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 1.0f, 0.0f);
	GeometryCentre = nullptr;
	name = "module camera 3d";

//	dcamera = (DCamera*)App->datamanager->CreateNewDataContainer(D_CAMERA, App->randGen->Int());
	//dcamera = (DCamera*)App->datamanager->CreateNewDataContainer(D_CAMERA, App->randGen->Int());


	dcamera = new DCamera(App->randGen->Int());
	editorCam = new CCamera(App->randGen->Int(), COMP_CAMERA, dcamera);
	
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init()
{
	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{

	LOG("Setting up the camera");
	LOGUI("-START- Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("-CLEANUP- Cleaning camera");
	LOGUI("-CLEANUP- Cleaning camera");

	delete dcamera;
	delete editorCam;
	editorCam = nullptr;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0, 0, 0);
	vec3 norm(0, 0, 0);
	float speed = 100.0f * dt;


	if (App->input->GetMouseZ() == -1)
	{
		newPos += Z * speed *ZOOM_FACTOR;
	}

	if (App->input->GetMouseZ() == 1)
	{
		newPos -= Z * speed * ZOOM_FACTOR;
	}

	Position += newPos;
	Reference += newPos;

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT &&  App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		//	CenterCamera();
		//ookAt(centreB);
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) 
			{
				float DeltaX = (float)dx * Sensitivity;
				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}
		}


		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}


		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}
	else
	{
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				speed = 8.0f * dt;

			if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
				CenterCamera();

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;



			Position += newPos;
			Reference += newPos;

			// Mouse motion ----------------


			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
				{
					float DeltaX = (float)dx * Sensitivity;
					X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				}
			}


			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}


			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}

		if(App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
		{	
			
			LineSegment picking = editorCam->GetFrustum().UnProjectLineSegment(-(1 - App->input->GetNormalized_x() * 2), 1 - App->input->GetNormalized_y() * 2);
			thispick = picking;
			selected = App->scene_intro->SelectObject(picking);
			App->ui->show_Inspector_window = false;
			App->scene_intro->ObjectSelected(selected);
		}
		editorCam->SetPos(float3(Position.x, Position.y, Position.z), -float3(Z.x, Z.y, Z.z), float3(Y.x, Y.y, Y.z));
		CalculateViewMatrix();
	}
	// Recalculate matrix -------------


	PrimitiveLine_Ray a(thispick.a.x, thispick.a.y, thispick.a.z, thispick.b.x, thispick.b.y, thispick.b.z);
	a.color = Yellow;
	a.Render();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
const float* ModuleCamera3D::GetViewMatrix()
{
	if (mainCam != nullptr && mainCam->dataCamera->Active==true)
	{
		return mainCam->GetViewMatrix();
	}
	else
	{
		return editorCam->GetViewMatrix();
	}
	
}

void ModuleCamera3D::CalculateAspectRatio(float width, float height)
{
	editorCam->dataCamera->aspectRatio= width / height;
	editorCam->SetFov();
}


void ModuleCamera3D::CenterCameraToObject(AABB * box)
{
	if (box == nullptr)
	{
		Reference = vec3{0,1,0};
		return;
	}

		float3 centre = box->CenterPoint();
		float3 difference = box->maxPoint - box->minPoint;
		float wide = difference.Length();
		float FOVdistance = (wide*0.5f) / (tan(60.0f * 0.5f * DEGTORAD));
		
		Reference = vec3(centre.x, centre.y, centre.z);	

		centreB = vec3(centre.x, centre.y, centre.z);
		Z = normalize(Position / 2 - Reference);
		GeometryCentre = box;
		Position = Z * FOVdistance;
		Position.y = -Position.y;

		LookAt(Reference);
		
}

void ModuleCamera3D::CenterCamera()
{
	CenterCameraToObject(GeometryCentre);
}

void ModuleCamera3D::SetMainCam(CCamera * cam)
{
	mainCam = cam;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}