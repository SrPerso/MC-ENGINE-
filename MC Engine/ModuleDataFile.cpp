#include "ModuleDataFile.h"
#include "Application.h"
#include "Module.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "GameObject.h"
#include "ModuleGameObjectManager.h"
#include "Component.h"
#include "CTransformation.h"
#include "CMesh.h"
#include "CTexture.h"
#include "ModuleUI.h"

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

	LOG("-START- Init FBXLoader");

	App->ui->AddLogToConsole("-START- Init FBXLoader");

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

	
	App->goManager->GetRoot()->DeleteChilds();

	GameObject* gameObject = App->goManager->GetRoot()->CreateChild();


	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			//create mesh

			CMesh* mesh = (CMesh*)gameObject->CreateComponent(COMP_MESH);
			mesh->Enable();

			aiMesh* newMesh = scene->mMeshes[i];

			mesh->nVertex = newMesh->mNumVertices;
			mesh->Vertex = new float[mesh->nVertex * 3];
			memcpy(mesh->Vertex, newMesh->mVertices, sizeof(float)* mesh->nVertex * 3);

	

			glGenBuffers(1, (GLuint*)&mesh->idVertex);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->Vertex, GL_STATIC_DRAW);

			
			LOG("loaded mesh %i vertex", mesh->nVertex);

			if (newMesh->HasFaces())
			{
				mesh->nFaces = newMesh->mNumFaces;

				mesh->nIndex = newMesh->mNumFaces * 3;
				mesh->Index = new float[mesh->nIndex]; //every face is a triangle.

				for (uint i = 0; i < newMesh->mNumFaces; ++i)
				{
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
						App->ui->AddLogToConsole("WARNING- geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh->Index[i * 3], newMesh->mFaces[i].mIndices, sizeof(float) * 3);
					}
				}

				glGenBuffers(1, (GLuint*)&(mesh->idIndex));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndex);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->nIndex, mesh->Index, GL_STATIC_DRAW);

			}// has faces			


			if (newMesh->HasNormals())
			{
				mesh->normals = new float[mesh->nVertex * 3];
				memcpy(mesh->normals, newMesh->mNormals, (sizeof(float) * mesh->nVertex * 3));

				glGenBuffers(1, (GLuint*)&(mesh->idNormals)); // 
				glBindBuffer(GL_ARRAY_BUFFER, mesh->idNormals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->normals, GL_STATIC_DRAW);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);

			}// has normals
			
			if (newMesh->HasVertexColors(0))
			{
				mesh->colors = new float[mesh->nVertex * 3];
				memcpy(mesh->colors, newMesh->mColors, sizeof(float) * mesh->nVertex * 3);

				glGenBuffers(1, (GLuint*) &(mesh->idColors));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->idColors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->colors, GL_STATIC_DRAW);
			}

			if (newMesh->HasTextureCoords(0))
			{
				CTexture* material = (CTexture*)gameObject->CreateComponent(COMP_TEXTURE);
				material->Enable();


				aiMaterial* newMaterial = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

				if (newMaterial != nullptr) {

					uint numTextures = newMaterial->GetTextureCount(aiTextureType_DIFFUSE);
					aiString path;

					aiReturn retu;
					retu = newMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					if (retu == aiReturn_FAILURE)
						App->ui->AddLogToConsole("[ERROR]- fail getting texture");

					std::string fullPath = "Assets/";
					fullPath.append(path.C_Str());
					material->image = App->texture->LoadTexture(fullPath.c_str());
					material->Textname = path.C_Str();
				}

				mesh->texCoords = new float[mesh->nVertex * 3];
				memcpy(mesh->texCoords, newMesh->mTextureCoords[0], sizeof(float) * mesh->nVertex * 3);

				glBindTexture(GL_TEXTURE_2D, material->image);
				glGenBuffers(1, (GLuint*) &(mesh->idTexCoords));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->idTexCoords);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->texCoords, GL_STATIC_DRAW);
			}

			
			
			
			 //TRANSFORMATION-------------- 
			aiNode * node = scene->mRootNode;
			CTransformation* transformation = (CTransformation*)gameObject->CreateComponent(COMP_TRANSFORMATION);

			aiVector3D position;
			aiVector3D scale;
			aiQuaternion rotation;

			node->mTransformation.Decompose(scale, rotation, position);

			transformation->position = float3(position.x, position.y, position.z);
			transformation->scale = float3(scale.x, scale.y, scale.z);
			transformation->rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
			//TRANSFORMATION-------------- 

			
			//TEXTURE COORDS



			mesh->debugBox.SetNegativeInfinity();//
			mesh->debugBox.Enclose((float3*)mesh->Vertex, mesh->nVertex);

			App->camera->CenterCameraToObject(&mesh->debugBox);


			App->scene_intro->sceneDebugInfo.faces += mesh->nFaces;
			App->scene_intro->sceneDebugInfo.tris += mesh->nVertex;
			App->scene_intro->sceneDebugInfo.vertex += mesh->nVertex;



		}//for	
		//scene.

		aiReleaseImport(scene);
		return true;
		App->ui->AddLogToConsole("[OK]- Loaded Mesh");
		LOG("Mesh %s loaded Ok", path);
	}//if scene


	else 
	{
		ret = false;
		LOG("[ERROR]- Error loading scene %s", path);
	}

	return ret;
}




