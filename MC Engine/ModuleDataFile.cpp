#include "ModuleDataFile.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "GameObject.h"
#include "ModuleGameObjectManager.h"
#include "Component.h"
#include "CMesh.h"



#include "Glew\include\glew.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"
#pragma comment (lib, "Devil/libx86/DevIL.lib" ) 
#pragma comment (lib, "Devil/libx86/ILU.lib" ) 
#pragma comment (lib, "Devil/libx86/ILUT.lib" ) 

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
	
	GameObject* gameObject = new GameObject(App->goManager->GetRoot());
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{

			//create mesh
			
			Component* mesh = new Component(gameObject, COMP_MESH);
			mesh->Enable();
			
			aiMesh* newMesh = scene->mMeshes[i];

			mesh->componentMesh->data->nVertex = newMesh->mNumVertices;
			mesh->componentMesh->data->Vertex = new float[mesh->componentMesh->data->nVertex * 3];
			memcpy(mesh->componentMesh->data->Vertex, newMesh->mVertices, sizeof(float)* mesh->componentMesh->data->nVertex * 3);

			glGenBuffers(1, (GLuint*)&mesh->componentMesh->data->idVertex);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->componentMesh->data->idVertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->componentMesh->data->nVertex * 3, mesh->componentMesh->data->Vertex, GL_STATIC_DRAW);

			App->ui->AddLogToConsole("Load Mesh");
			LOG("loaded mesh %i vertex", mesh->componentMesh->data->nVertex);

			if (newMesh->HasFaces()) 
			{
				mesh->componentMesh->data->nFaces = newMesh->mNumFaces;

				mesh->componentMesh->data->nIndex = newMesh->mNumFaces * 3;
				mesh->componentMesh->data->Index = new float[mesh->componentMesh->data->nIndex]; //every face is a triangle.

				for (uint i = 0; i < newMesh->mNumFaces; ++i)
				{
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh->componentMesh->data->Index[i * 3], newMesh->mFaces[i].mIndices, sizeof(float)* 3);
					}
				}

				glGenBuffers(1, (GLuint*)&(mesh->componentMesh->data->idIndex));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->componentMesh->data->idIndex);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->componentMesh->data->nIndex, mesh->componentMesh->data->Index, GL_STATIC_DRAW);

			}// has faces			

			if (newMesh->HasNormals())
			{
				mesh->componentMesh->data->normals = new float[mesh->componentMesh->data->nVertex * 3];
				memcpy(mesh->componentMesh->data->normals, newMesh->mNormals, (sizeof(float) * mesh->componentMesh->data->nVertex * 3));

				glGenBuffers(1, (GLuint*)&(mesh->componentMesh->data->idNormals)); // 
				glBindBuffer(GL_ARRAY_BUFFER, mesh->componentMesh->data->idNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->componentMesh->data->nVertex * 3, mesh->componentMesh->data->normals, GL_STATIC_DRAW);

			}// has normals
			Component *material = new Component(gameObject, COMP_MATERIAL);
			material->Enable();
			if (newMesh->HasTextureCoords(0))
			{
				material->componentMaterial->data->texCoords = new float[mesh->componentMesh->data->nVertex * 3];
				memcpy(material->componentMaterial->data->texCoords, newMesh->mTextureCoords[0], sizeof(float) * mesh->componentMesh->data->nVertex * 3);

				glGenBuffers(1, (GLuint*) &(material->componentMaterial->data->idTexCoords));
				glBindBuffer(GL_ARRAY_BUFFER, material->componentMaterial->data->idTexCoords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->componentMesh->data->nVertex * 3, material->componentMaterial->data->texCoords, GL_STATIC_DRAW);
			}
			if (newMesh->HasVertexColors(0))
			{
				material->componentMaterial->data->colors = new float[mesh->componentMesh->data->nVertex * 3];
				memcpy(material->componentMaterial->data->colors, newMesh->mColors, sizeof(float) * mesh->componentMesh->data->nVertex * 3);

				glGenBuffers(1, (GLuint*) &(material->componentMaterial->data->idColors));
				glBindBuffer(GL_ARRAY_BUFFER, material->componentMaterial->data->idColors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->componentMesh->data->nVertex * 3, material->componentMaterial->data->colors, GL_STATIC_DRAW);
			}

			//TEXTURE COORDS
			

/*
			mesh->debugBox.SetNegativeInfinity();//
			mesh->debugBox.Enclose((float3*)mesh->componentMesh->data->Vertex, mesh->componentMesh->data->nVertex);
			App->scene_intro->CreateMesh(mesh->componentMesh);//??

			App->scene_intro->sceneDebugInfo.faces += mesh->componentMesh->data->nFaces;
			App->scene_intro->sceneDebugInfo.tris += mesh->componentMesh->data->nVertex/3;
			App->scene_intro->sceneDebugInfo.vertex += mesh->componentMesh->data->nVertex;*/

		

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




