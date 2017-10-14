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
	App->ui->AddLogToConsole("Creating 3D Renderer context");
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
		App->ui->AddLogToConsole("OpenGL context could not be created! ");
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0) {
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			App->ui->AddLogToConsole("Warning: Unable to set VSync!");
		}
		
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->ui->AddLogToConsole("Error initializing OpenGL!");
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
			App->ui->AddLogToConsole("Error initializing OpenGL!");
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
			App->ui->AddLogToConsole("Error initializing OpenGL!");
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
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	glLineWidth(2.0f);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
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
	App->ui->AddLogToConsole("Destroying 3D Renderer");


	SDL_GL_DeleteContext(context);

	return true;
}

GLuint ModuleRenderer3D::loadBMP_custom(const char * imagepath)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char * data;


	FILE * file = fopen(imagepath, "rb");
	if (!file)
	{ 
		printf("Image could not be opened\n");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) 
	{ 
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') 
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)    
		imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      
		dataPos = 54;


	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);


	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
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


	for (std::vector<Component*>::iterator it = GOToDraw->components.begin(); it != GOToDraw->components.end(); it++)
	{
		if ((*it)->getType() == COMP_MESH) {

			CMesh* componentMesh = (CMesh*)(*it);

			if (componentMesh->IsEnable() == true && componentMesh != nullptr)
			{
				if (App->ui->sb_Wire_Face == true)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//WIRE
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//WIRE


				if (componentMesh->nNormals != 0)
				{
					glEnableClientState(GL_NORMAL_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, componentMesh->idNormals);
					glNormalPointer(GL_FLOAT, 0, NULL);

				}

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
				glBindBuffer(GL_ARRAY_BUFFER, componentMesh->idVertex);
				glVertexPointer(3, GL_FLOAT, 0, NULL);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, componentMesh->idIndex);
				glDrawElements(GL_TRIANGLES, componentMesh->nIndex, GL_UNSIGNED_INT, NULL);

				//--------------------- MESH----------------------------------------------

				if (!App->ui->sb_Wire_Face == true)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//WIRE
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//WIRE

				//--------------------- TEXTURE----------------------------------------------
				CTexture* componentTexture = (CTexture*)GOToDraw->GetComponent(COMP_MESH);
			
				if (componentTexture->IsEnable() == true)
				{

					if (componentTexture != nullptr)
					{
						glBindTexture(GL_TEXTURE_2D, componentTexture->image);
					}
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, componentTexture->idTexCoords);
					glTexCoordPointer(3, GL_FLOAT, 0, NULL);



					glEnableClientState(GL_COLOR_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, componentTexture->idColors);
					glColorPointer(3, GL_FLOAT, 0, NULL);
				}
				//--------------------- TEXTURE----------------------------------------------

				// Viewwws

				EDglView();

			if (App->ui->debug_active == true) {
					DrawDebug(componentMesh);
				}


				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);

				glPopMatrix();
				glEnd();
				glBindTexture(GL_TEXTURE_2D, 0);
				glUseProgram(0);


			}
		}
	}
}


void ModuleRenderer3D::DrawDebug(CMesh* meshToDraw)
{
	if (meshToDraw->debugMode == true)
	{
		if (App->ui->debug_Tri_Normals == true && meshToDraw->nVertex > 2)
		{

		/*	glLineWidth(2.0f);
			glBegin(GL_LINES);

			for (int i = 0; i < meshToDraw->nVertex*3; ++i)
			{*/
		/*		float3 vertex(&meshToDraw->Vertex[i * 3]);
				float3 vertex2(&meshToDraw->Vertex[i * 6]);
				float3 vertex3(&meshToDraw->Vertex[i * 9]);

				Triangle face(float3(a), float3(b), float3(c));

*/
			//	float3 a = float3(meshToDraw->Vertex[i], meshToDraw->Vertex[i + 1], meshToDraw->Vertex[i + 2]);
			//	float3 b = float3(meshToDraw->Vertex[i + 3], meshToDraw->Vertex[i + 4], meshToDraw->Vertex[i + 5]);
			//	float3 c = float3(meshToDraw->Vertex[i + 6], meshToDraw->Vertex[i + 7], meshToDraw->Vertex[i + 8]);

			//	Triangle face(float3(a), float3(b), float3(c));

			//	float3 center = (a + b + c) / (float)3;
			//	float3 normal = Cross(b - a, c - a);
			//	normal = normal.Normalized();

			//	float3 pnormal(center + (normal*NORMAL_SIZE));

			//	glColor3f(Red.r, Red.g, Red.b);
			//	glVertex3f(center.x, center.y, center.z);

			//	glColor3f(White.r, White.g, White.b);
			//	glVertex3f(pnormal.x, pnormal.y, pnormal.z);

			//	glColor3f(White.r, White.g, White.b);
			//	i += 8;
			//}


			//glEnd();
		}

		if (App->ui->debug_Vertex_Normals == true && meshToDraw->nVertex > 0)
		{

			glLineWidth(2.0f);
			glBegin(GL_LINES);

			for (uint i = 0; i < meshToDraw->nVertex; ++i)
			{
				float3 vertex(&meshToDraw->Vertex[i * 3]);
				float3 normal(&meshToDraw->normals[i * 3]);
				float3 pnormal(vertex + (normal*NORMAL_SIZE));

				glColor4f(Blue.r, Blue.g, Blue.b, 255);
				glVertex3f(vertex.x, vertex.y, vertex.z);

				glColor3f(White.r, White.g, White.b);
				glVertex3f(pnormal.x, pnormal.y, pnormal.z);

		
				glColor4f(White.r, White.g, White.b, 0);
			}
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glDrawElements(GL_TRIANGLES, meshToDraw->nIndex, GL_UNSIGNED_INT, NULL);
			glEnd();
		}

		if (App->ui->debug_Box == true)
		{
			float3 vertex[8];
			meshToDraw->debugBox.GetCornerPoints(vertex);

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

			glEnd();
		}// box


		glPopMatrix();
	}
}


void ModuleRenderer3D::TextureView()
{
	if (App->ui->sb_Texture_2D)
		glEnable(GL_LINE);
	else if (!App->ui->sb_Texture_2D)
		glDisable(GL_LINE);

}