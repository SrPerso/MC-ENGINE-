#include "ITexture.h"

#include "Application.h"
#include "Globals.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "Glew\include\glew.h"
#include <fstream>

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"
#pragma comment (lib, "Devil/libx86/DevIL.lib" ) 
#pragma comment (lib, "Devil/libx86/ILU.lib" )
#pragma comment (lib, "Devil/libx86/ILUT.lib" ) 


ImporterTexture::ImporterTexture()
{
}

ImporterTexture::~ImporterTexture()
{
}

DTexture * ImporterTexture::ImportTexture(aiMaterial* newMaterial,const char*  FilePath)
{
	LOGUI("-------------------------------------------");
	DTexture* ret = new DTexture();

	if (newMaterial != nullptr)
	{

		uint numTextures = newMaterial->GetTextureCount(aiTextureType_DIFFUSE);
		aiString path;

		aiReturn retu;
		retu = newMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		if (retu == aiReturn_FAILURE)
			LOGUI("[ERROR]- fail getting texture");

		std::string fullPath = path.C_Str();

		int i = fullPath.find_last_of("\\");

		if (i == -1)
		{
			std::string newpath = "Assets/";
			newpath.append(path.C_Str());

			ret->image = App->texture->LoadTexture(newpath.c_str());
			ret->textNamePath = newpath;
			ret->textureName = path.C_Str();
		}
		else 
		{

			int length = strlen(path.C_Str());
			std::string namePath = path.C_Str();
			ret->textureName = path.C_Str();
			int i = namePath.find_last_of("\\") + 1;

			if (length > 0 && i > 0)
			{
				char* testM = new char[length - i];
				namePath.copy(testM, length - i, i);

				delete[] testM;
				testM = nullptr;
			}

			ret->image = App->texture->LoadTexture(fullPath.c_str());
			ret->textNamePath = fullPath;
		}

		LOGUI("[OK]- Imported Texture");
		glBindTexture(GL_TEXTURE_2D, ret->image);
	
		Save(ret, path.C_Str(),0);
		Load(nullptr, path.C_Str(), 0);
	}
	//glBindTexture(GL_TEXTURE_2D, ret->image);



	return ret;
}


bool ImporterTexture::Save(const void * buffer, const char * saverFile, uint id)
{
	bool ret = true;

	DTexture * text = (DTexture*)buffer;

	//path to save ----------

	std::string path;

	path = "Library/Material";
	path.append("/");
	path.append(saverFile);
	path.append(".dds");

	LOGUI("[SAVING]{Texture}- %s", path.c_str());



	// Opening ----------

	FILE* file = fopen(path.c_str(), "r");

	if (file != nullptr)
	{
		fclose(file);
		LOGUI("[ERROR]- %s  is already created", path.c_str());

		return false;
	}

	//save path 

	uint size = path.size();
	uint i = path.find_last_of("/") + 1; //
								
	char* data = new char[size - i - 3]; //

	size = size - i;
	path.copy(data, size, i);

	data[size - 10] = '\0';// size - extension size


	std::ofstream file_end(path.c_str(), std::ifstream::out | std::ofstream::binary);

	if (file_end.good()) //write file
		file_end.write(data, size);
	else
		LOGUI("[ERROR]- writting error ", path.c_str());

	file_end.close();

	//RELEASE_ARRAY(data);
	LOGUI("[SAVED]{Texture}- %s ", path.c_str());

	return ret;
}


DTexture * ImporterTexture::Load(const void * buffer, const char * loadFile, uint id)
{
	LOGUI("-----------------");
	DTexture* data = new DTexture();

	std::string path; //path to load

	if (loadFile == nullptr)
	{
		return nullptr;
	}
	else
	{
		path = "Library/Material";
		path.append("/");
		path.append(loadFile);
		path.append(".dds");

		LOGUI("[LOADING]{Texture}- %s", path.c_str());
	}


	std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

	char * dataFile;

	if (file)
	{
		file.seekg(0, file.end);

		int size = file.tellg(); //size of the file

		file.seekg(0, file.beg);

		dataFile = new char[size];


		if (file.read(dataFile, size))
		{

			LOGUI("[READING]- %s", path.c_str());

			data->image = App->texture->LoadTexture(path.c_str());
		}
		else
		{
			LOGUI("{Load}[ERROR]- only %ll can be read on: %s", file.gcount(), path.c_str());

			RELEASE_ARRAY(dataFile);
			return nullptr;
		}

		file.close();

	}
	else
		LOGUI("[ERROR]- loading %s", path);

		return data;
}
