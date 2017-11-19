#include "Globals.h"
#include "Application.h"

#include "ModuleRenderer3D.h"
#include "ModuleSceneIntro.h"
#include "ModuleUI.h"

#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include "parson\parson.h"
//
#include "CMesh.h"
#include "CTexture.h"
#include "Component.h"
#include "GameObject.h"
//
#include <gl/GL.h>
#include <gl/GLU.h>

#include "MathGeolib\Geometry\Triangle.h"
#include "MathGeolib\Math\float4x4.h"

#include "CTransformation.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Glew/libx86/glew32.lib") 
#pragma comment (lib, "glu32.lib") 
#pragma comment (lib, "opengl32.lib") 


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Render";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	LOGUI("-START- Creating 3D Renderer context");
	bool ret = true;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		LOGUI("[ERROR]- OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			LOGUI("[ERROR]- Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}
		
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			LOGUI("[ERROR]- Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			LOGUI("[ERROR]- Error initializing OpenGL!  %s\n", gluErrorString(error)); 
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			LOGUI("[ERROR]- Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		lights[0].Active(true);

		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		EDglView();

	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(App->camera->GetViewMatrix());
	glMatrixMode(GL_MODELVIEW);
	/*
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());*/

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	glLineWidth(2.0f);

	glEnd();
	glLineWidth(1.0f);


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->ui->Draw();

	App->scene_intro->Draw();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	LOGUI("-CLEANUP- Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

GLuint ModuleRenderer3D::loadBMP_custom(const char * imagepath)
{
	unsigned char header[54]; 
	unsigned int dataPos;     
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char * data;


	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{ 
		printf("[ERROR]- Image could not be opened\n");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) 
	{ 
		printf("[ERROR]- Not a correct BMP file \n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') 
	{
		printf("[ERROR]- Not a correct BMP file \n");
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)    
		imageSize = width*height * 3; 
	if (dataPos == 0)      
		dataPos = 54;


	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);


	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	return GLuint(data);
	
}



void ModuleRenderer3D::EDglView()
{

	if (App->ui->sb_Depth_Test)
		glEnable(GL_DEPTH_TEST);
	else if (!App->ui->sb_Depth_Test)
		glDisable(GL_DEPTH_TEST);

	if (!App->ui->sb_Cull_Face)
		glEnable(GL_CULL_FACE);
	else if (App->ui->sb_Cull_Face)
		glDisable(GL_CULL_FACE);
	
	if (!App->ui->sb_Lighting)
		glEnable(GL_LIGHTING);
	else if (App->ui->sb_Lighting)
		glDisable(GL_LIGHTING);

	if (App->ui->sb_Color_Material)
		glEnable(GL_COLOR_MATERIAL);
	else if (!App->ui->sb_Color_Material)
		glDisable(GL_COLOR_MATERIAL);

	if (App->ui->sb_Texture_2D)
		glEnable(GL_TEXTURE_2D);
	else if (!App->ui->sb_Texture_2D)
		glDisable(GL_TEXTURE_2D);

}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawGO(GameObject* GOToDraw)
{	
	//--------------------- MESH----------------------------------------------

	glEnable(GL_TEXTURE_2D);
	
	CTransformation* transform = (CTransformation*)GOToDraw->GetComponent(COMP_TRANSFORMATION);

	glPushMatrix();
	if (transform != nullptr)
	{
		glMultMatrixf(transform->GetTransMatrix().Transposed().ptr());
	}
	for (std::vector<Component*>::iterator it = GOToDraw->components.begin(); it != GOToDraw->components.end(); it++)
	{
		
		if ((*it)->getType() == COMP_MESH)
		{
			CMesh* componentMesh = dynamic_cast<CMesh*>(*it);			
			
			if (App->ui->debug_active == true) 			
				DrawDebug(componentMesh);
			
			if (componentMesh->IsEnable() == true && componentMesh != nullptr)
			{
				if (App->ui->sb_Wire_Face == true)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//WIRE
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//WIRE


				if (componentMesh->dataMesh->nNormals != 0)
				{
					glEnable(GL_LIGHTING);
					glEnableClientState(GL_NORMAL_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, componentMesh->dataMesh->idNormals);
					glNormalPointer(GL_FLOAT, 0, NULL);

				}

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
				glBindBuffer(GL_ARRAY_BUFFER, componentMesh->dataMesh->idVertex);
				glVertexPointer(3, GL_FLOAT, 0, NULL);

				//--------------------- MESH----------------------------------------------

				//--------------------- TEXTURE----------------------------------------------
		
				if (componentMesh->dataMesh->idColors>0 && App->ui->sb_Color_Material)
				{

					glEnableClientState(GL_COLOR_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, componentMesh->dataMesh->idColors);
					glColorPointer(3, GL_FLOAT, 0, NULL);
				}

				if (componentMesh->dataMesh->idTexCoords>0 && App->ui->sb_Texture_2D)
				{
					CTexture* componentTexture = (CTexture*)GOToDraw->GetComponent(COMP_TEXTURE);
					if (componentTexture != nullptr)
					{
						glBindTexture(GL_TEXTURE_2D, componentTexture->dataTexture->image);
					}
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, componentMesh->dataMesh->idTexCoords);
					glTexCoordPointer(3, GL_FLOAT, 0, NULL);

				}		

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, componentMesh->dataMesh->idIndex);
				glDrawElements(GL_TRIANGLES, componentMesh->dataMesh->nIndex, GL_UNSIGNED_INT, NULL);
		
				//--------------------- TEXTURE----------------------------------------------

				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);

				
				glEnd();
				glUseProgram(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
			LOGUI("[ERROR]- Mesh component does not exist");
			}

		}
	}
	glPopMatrix();
}


void ModuleRenderer3D::DrawDebug(CMesh* meshToDraw)
{

		if (App->ui->debug_Vertex_Normals == true && meshToDraw->dataMesh->nVertex > 0)
		{

			glLineWidth(2.0f);
			glBegin(GL_LINES);

			for (uint i = 0; i < meshToDraw->dataMesh->nVertex; ++i)
			{
				float3 vertex(&meshToDraw->dataMesh->Vertex[i * 3]);
				float3 normal(&meshToDraw->dataMesh->normals[i * 3]);
				float3 pnormal(vertex + (normal*NORMAL_SIZE));

				glColor4f(Blue.r, Blue.g, Blue.b, 255);
				glVertex3f(vertex.x, vertex.y, vertex.z);
				//if(sb_Color_Material)
				glColor3f(White.r, White.g, White.b);
				glVertex3f(pnormal.x, pnormal.y, pnormal.z);
						
				glColor4f(White.r, White.g, White.b, 0);
			}
			glVertexPointer(1, GL_FLOAT, 0, NULL);
			glDrawElements(GL_LINES, meshToDraw->dataMesh->nIndex, GL_UNSIGNED_INT, NULL);
			glEnd();
		}

		else if (meshToDraw->dataMesh->nVertex < 0) {
		LOGUI("[ERROR]- The number of verteh is down 0");
		}

		if (App->ui->debug_Box == true)
		{
			float3 vertex[8];
			meshToDraw->dataMesh->debugBox.GetCornerPoints(vertex);

			glPushMatrix();

			glMultMatrixf((float*)float4x4::identity.Transposed().ptr());

			glColor3f(Green.r, Green.g, Green.b);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glBegin(GL_QUADS);

			//1
			glVertex3fv((float*)&vertex[1]);
			glVertex3fv((float*)&vertex[5]);
			glVertex3fv((float*)&vertex[7]);
			glVertex3fv((float*)&vertex[3]);
			//2
			glVertex3fv((float*)&vertex[4]);
			glVertex3fv((float*)&vertex[0]);
			glVertex3fv((float*)&vertex[2]);
			glVertex3fv((float*)&vertex[6]);
			//3
			glVertex3fv((float*)&vertex[5]);
			glVertex3fv((float*)&vertex[4]);
			glVertex3fv((float*)&vertex[6]);
			glVertex3fv((float*)&vertex[7]);
			//4
			glVertex3fv((float*)&vertex[0]);
			glVertex3fv((float*)&vertex[1]);
			glVertex3fv((float*)&vertex[3]);
			glVertex3fv((float*)&vertex[2]);
			//5
			glVertex3fv((float*)&vertex[3]);
			glVertex3fv((float*)&vertex[7]);
			glVertex3fv((float*)&vertex[6]);
			glVertex3fv((float*)&vertex[2]);
			//6
			glVertex3fv((float*)&vertex[0]);
			glVertex3fv((float*)&vertex[4]);
			glVertex3fv((float*)&vertex[5]);
			glVertex3fv((float*)&vertex[1]);
				
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f(White.r, White.g, White.b);
			glEnd();
		}// box

		glPopMatrix();
	
}

