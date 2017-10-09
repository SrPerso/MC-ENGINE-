#include "ModuleDataFile.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "Glew\include\glew.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")


DataJSON::DataJSON()
{
	value_json=json_value_init_object();
	object_json = json_value_get_object(value_json);
	
	files.push_back("config.json");
	files.push_back("uisave.json");

}

DataJSON::~DataJSON()
{
	json_value_free(value_json);
}

bool DataJSON::Init()
{
	bool ret = true;
	

	/*for (std::list<std::string>::const_reverse_iterator  file = files.rbegin(); file != files.crend(); ++file)
	{
		value_json = json_parse_file(file->c_str());

		if (value_json == nullptr) 
		{
			value_json = json_value_init_object();
			json_serialize_to_file(value_json,file->c_str());
		}
		else
		{
			object_json = json_value_get_object(value_json);
		}
	
		for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
		{
			if (json_object_get_object(object_json, (*item)->name.c_str()) == nullptr)
			{
				json_object_set_value(object_json, (*item)->name.c_str(), json_value_init_object());
			}			
		}
	

	}//for	*/

	return ret;
}

void DataJSON::SaveAll() const
{
	
	/*for (std::list<std::string>::const_reverse_iterator file = files.rbegin(); file != files.crend(); ++file) {


		JSON_Object* object = nullptr;

		for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
		{
			object = json_object_get_object(object_json, (*item)->name.c_str());
			(*item)->SaveJson(object);		
		}

		json_serialize_to_string_pretty(value_json);		
		json_serialize_to_file(value_json, file->c_str());
	}*/
}

int DataJSON::GetInt(JSON_Object * object, const char * name) const
{
	int ret = 0;

	JSON_Value* value = json_object_get_value(object, name);

	if (value && json_value_get_type(value) == JSONNumber) 
		ret = json_value_get_number(value);

	return (int)ret;
}

float DataJSON::GetFloat(JSON_Object * object, const char * name) const
{
	float ret = 0.0f;

	JSON_Value* value = json_object_get_value(object, name);

	if (value && json_value_get_type(value) == JSONNumber)
		ret = json_value_get_number(value);

	return (float)ret;
}

double DataJSON::GetDouble(JSON_Object * object, const char * name) const
{
	double ret = 0;

	JSON_Value* value = json_object_get_value(object, name);

	if (value && json_value_get_type(value) == JSONNumber)
		ret = json_value_get_number(value);

	return (double)ret;
}

bool DataJSON::GetBoolean(JSON_Object * object, const char * name) const
{
	bool ret = true;

	JSON_Value* value = json_object_get_value(object, name);

	if (value && json_value_get_type(value) == JSONBoolean)
		ret = json_value_get_boolean(value);

	return (bool)ret;
}

const char * DataJSON::GetString(JSON_Object * object, const char * name) const
{
	const char * ret = nullptr;

	JSON_Value* value = json_object_get_value(object, name);

	if (value && json_value_get_type(value) == JSONString)
		ret = json_value_get_string(value);

	return ret;
}

void DataJSON::AddInt(JSON_Object * object, const char * name, int number)
{
	json_object_set_number(object, name, (int)number);
}

void DataJSON::AddFloat(JSON_Object * object, const char * name, float number)
{
	json_object_set_number(object, name, (float)number);
}

void DataJSON::AddDouble(JSON_Object * object, const char * name, double number)
{
	json_object_set_number(object, name, (double)number);
}

void DataJSON::AddBool(JSON_Object * object, const char * name, bool boolean)
{
	json_object_set_boolean(object, name, (bool)boolean);
}

void DataJSON::AddString(JSON_Object * object, const char * name, const char * string)
{
	json_object_set_string(object, name, (const char *)string);
}

//TO DO
//ImVec2 DataJSON::GetVec2(JSON_Object * object, std::string name)
//{
//	ImVec2 ret;
//
//
//	JSON_Value* value = json_object_get_value(object, name.c_str());
//	if (value && json_value_get_type(value) == JSONNumber)
//		retX = json_value_get_number(value);
//
//	return ImVec2();
//}
//--------------------------------------------------------------------------------------------------------------------------------
DataFBX::DataFBX(Application* app, bool start_enabled): Module(app, start_enabled)
{
	name = "Loader FBX";
}

DataFBX::~DataFBX()
{

}


bool DataFBX::Init()
{
	bool ret= true;

	LOG("Init FBXLoader");

	App->ui->AddLogToConsole("Init FBXLoader");

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool DataFBX::Start()
{
	bool ret = true;
	
	return ret;
}

bool DataFBX::CleanUp()
{
	bool ret = true;
	aiDetachAllLogStreams();
	return ret;
}

bool DataFBX::LoadMesh(const char* path)
{
	bool ret = true;
 
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{

		for (uint i = 0; i < scene->mNumMeshes; i++)
		{

			//create mesh
			aiMesh* newMesh = scene->mMeshes[i];
			ObjectMesh* mesh = new ObjectMesh;

			mesh->nVertex = newMesh->mNumVertices;
			mesh->Vertex = new float[mesh->nVertex * 3];
			memcpy(mesh->Vertex, newMesh->mVertices, sizeof(float)* mesh->nVertex * 3);

			glGenBuffers(1, (GLuint*)&mesh->idVertex);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->Vertex, GL_STATIC_DRAW);

			App->ui->AddLogToConsole("Load Mesh");
		//	LOG("loaded mesh %s vertex", mesh->nVertex);

			if (newMesh->HasFaces()) 
			{
				mesh->nIndex = newMesh->mNumFaces * 3;
				mesh->Index = new uint[mesh->nIndex]; //every face is a triangle.

				for (uint i = 0; i < newMesh->mNumFaces; ++i)
				{
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh->Index[i * 3], newMesh->mFaces[i].mIndices, sizeof(uint)* 3);
					}
				}

				glGenBuffers(1, (GLuint*)&mesh->idIndex);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndex);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->nIndex, mesh->Index, GL_STATIC_DRAW);

			}// has faces			


		
			App->scene_intro->CreateMesh(mesh);
		}//for	
		//scene.

		aiReleaseImport(scene); 
		return true;
		LOG("Mesh %s loaded Ok", path);
	}//if scene

	else 
	{
		ret = false;
		LOG("Error loading scene %s", path);
	}

	return ret;
}

DataTexture::DataTexture(Application * app, bool start_enabled)
{
	name = "Data Texture";
}

DataTexture::~DataTexture()
{
}

bool DataTexture::Innit(JSON_Object * data)
{
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

bool DataTexture::CleanUp(JSON_Object * data)
{
	return false;
}

GLuint DataTexture::LoadTexture(const char * imagepath)
{
	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	success = ilLoadImage(imagepath); 	// Load the image file

											// If we managed to load the image, then we can start to do things with it...
	if (success)
	{
		// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert the image into a suitable format to work with
		// NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			App->ui->AddLogToConsole("Image conversion failed - IL reports error: " );
			exit(-1);
		}

		// Generate a new texture
		glGenTextures(1, &textureID);

		// Bind the texture to a name
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture clamping method
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// Set texture interpolation method to use linear interpolation (no MIPMAPS)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Specify the texture specification
		glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
			0,				// Pyramid level (for mip-mapping) - 0 is the top level
			ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
			ilGetInteger(IL_IMAGE_WIDTH),	// Image width
			ilGetInteger(IL_IMAGE_HEIGHT),	// Image height
			0,				// Border width in pixels (can either be 1 or 0)
			ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
			GL_UNSIGNED_BYTE,		// Image data type
			ilGetData());			// The actual image data itself
	}
	else // If we failed to open the image file in the first place...
	{
		error = ilGetError();
		App->ui->AddLogToConsole("Image load failed - IL reports error: ");
		exit(-1);
	}

	ilDeleteImages(1, &imageID); // Because we have already copied image data into texture data we can release memory used by image.

	App->ui->AddLogToConsole("Texture creation successful.");

	return textureID;
}
