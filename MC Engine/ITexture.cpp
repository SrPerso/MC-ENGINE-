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
	DTexture* ret = (DTexture*)App->datamanager->CreateNewDataContainer(D_TEXTURE, App->randGen->Int());


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

			int length = strlen(path.C_Str()); // "/0"
			std::string namePath = path.C_Str();
			ret->textureName = path.C_Str();
			int i = namePath.find_last_of("\\") + 1;

			if (length > 0 && i > 0)
			{
				char* temp = new char[length - i];
				namePath.copy(temp, length - i, i);
				fullPath = temp;
				fullPath[length - i] = '\0';
				delete[] temp;
				temp = nullptr;
			}

	
			ret->textureName = fullPath;
			ret->textNamePath = fullPath;	

			std::string newpath = "Assets/";
			newpath.append(fullPath);

			ret->image = App->texture->LoadTexture(newpath.c_str());
		}

		Save(ret, ret->textureName.c_str(), 0);
		LOGUI("[OK]- Imported Texture");
		glBindTexture(GL_TEXTURE_2D, ret->image);
	
	}
	
	return ret;
}


bool ImporterTexture::Save(const void * buffer, const char * saverFile, uint id)
{
	bool ret = true;
	LOGUI("--------------");
	DTexture * text = (DTexture*)buffer;

	text->textureName = saverFile;
	text->textNamePath = saverFile;

	std::string path;
	std::string dirPath;

	path = "Library/Material";
	path.append("/");
	path.append(saverFile);

	path.append(".dds");


	dirPath = "Assets/";
	dirPath.append(saverFile);

	LOGUI("[SAVING]{Texture}- %s", path.c_str());

	if (buffer)
	{
		ILuint ImageName;
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);


		bool ok = ilLoadImage(dirPath.c_str()); //can load the main image

		if (ok)
		{
			ilEnable(IL_FILE_OVERWRITE);

			ILuint   size;
			ILubyte *data;

			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

			size = ilSaveL(IL_DDS, NULL, 0);

			if (size > 0)
			{
				data = new ILubyte[size];

				if (ilSaveL(IL_DDS, data, size) > 0)
				{

					// save the file
					std::ofstream file(path.c_str(), std::ofstream::out | std::ofstream::binary);
					file.write((char*)data, size);
					file.close();

					RELEASE_ARRAY(data);
					LOGUI("[OK]- saved texture");

					ret = true;

				}

				RELEASE_ARRAY(data);
			}
			ilDeleteImages(1, &ImageName);
		}
		else
		{
			LOGUI("[ERROR]- ilLoadL error Texture save");

		}
	}
	return ret;
}


DTexture * ImporterTexture::Load(const void * buffer, const char * loadFile, uint id)
{
	LOGUI("-----------------");
	DTexture* data = (DTexture*)App->datamanager->CreateNewDataContainer(D_TEXTURE, App->randGen->Int());

	std::string path; //path to load

	std::string temp = loadFile;
	int length = strlen(loadFile);

	data->textureName = loadFile;

	int i = temp.find_last_of("\\") + 1;

	if (length > 0 && i > 0)
	{
		char* testM = new char[length - i];
		temp.copy(testM, length - i, i);

		delete[] testM;
		testM = nullptr;
	}
	data->textureName = temp;


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
			data->textNamePath = loadFile;
			data->textureName = loadFile;
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
