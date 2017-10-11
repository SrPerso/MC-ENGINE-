#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "Glew\include\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "SDL\include\SDL_opengl.h"
#include "parson\parson.h"
#include "ModuleSceneIntro.h"
#include "ModuleUI.h"


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

	/*GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;  checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	
	uint ImageName = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &ImageName);
	glBindTexture(GL_TEXTURE_2D, ImageName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);*/


	//GLuint image = loadBMP_custom("lena.bmp");

	GLuint image = App->texture->LoadTexture("Lenna.png");

	glBegin(GL_TRIANGLES);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, image);

	//Direct Mode
	//front
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 1, 0);//(1,1)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 0);//(0,1)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, 0, 0);//(0,0)
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 1, 0);//(1,1)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, 0, 0);//(0,0)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 0);//(0,1)
						//right

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1, 1, 0);//(0,0)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1, 0, 0);//(0,1)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 1);//(1,0)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 1);//(1,0)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1, 0, 0);//(0,1)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, 0, 1);//(1,1)

						//left
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 1, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 1, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 1, 0);
	//up
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 1, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, 1, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 1, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 1, 1);
	//down
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1, 0, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1, 0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(0, 0, 1);
	//back
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0, 1, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1, 1, 1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1, 0, 1);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1, 1, 1);
	

	//Indices Mode:


	
	

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glLineWidth(1.0f);


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	App->ui->Draw();

	//if (App->ui->sb_Texture_2D)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else if (!App->ui->sb_Texture_2D)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

void ModuleRenderer3D::Draw(ObjectMesh meshToDraw)
{
	
	if (meshToDraw.wire == true)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glPushMatrix();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, meshToDraw.idVertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshToDraw.idIndex);
		glDrawElements(GL_TRIANGLES, meshToDraw.nIndex, GL_UNSIGNED_INT, NULL);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
		
		glPopMatrix();
		glEnd();
		glUseProgram(0);

}

void ModuleRenderer3D::TextureView()
{
	if (App->ui->sb_Texture_2D)
		glEnable(GL_LINE);
	else if (!App->ui->sb_Texture_2D)
		glDisable(GL_LINE);

}

void ModuleRenderer3D::WireSet(bool wireon)
{

	for (std::list<Primitive*>::iterator it = App->scene_intro->GeometryObjects.begin(); it != App->scene_intro->GeometryObjects.end(); ++it)
	{
		(*it)->wire = wireon;
	}
	for (std::list<ObjectMesh*>::iterator it = App->scene_intro->MeshObjects.begin(); it != App->scene_intro->MeshObjects.end(); ++it)
	{
		(*it)->wire = wireon;
	}


}
