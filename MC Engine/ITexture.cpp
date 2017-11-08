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
			ret->Textname = newpath;

		}
		else 
		{

			int length = strlen(path.C_Str());
			std::string namePath = path.C_Str();

			int i = namePath.find_last_of("\\") + 1;

			if (length > 0 && i > 0)
			{
				char* testM = new char[length - i];
				namePath.copy(testM, length - i, i);

				delete[] testM;
				testM = nullptr;
			}

			ret->image = App->texture->LoadTexture(fullPath.c_str());
			ret->Textname = fullPath;
		}

		LOGUI("[OK]- Imported Texture");
		glBindTexture(GL_TEXTURE_2D, ret->image);
	
		Save(ret, path.C_Str(),0);

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

	DTexture* data = new DTexture();

	std::string path; //path to load

	if (loadFile == nullptr)
	{
		path = "Library/Material";
		path.append("/");	
		path.append(std::to_string(id));
		path.append(".dds");
	}
	else
	{
		path = loadFile;
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

	if (dataFile == nullptr)
	{
		return nullptr;
	}

	ILuint idTextTemp;
	glGenTextures(1, &idTextTemp);
	glBindTexture(GL_TEXTURE_2D, idTextTemp);

	data->idTexCoords = idTextTemp;

	ILenum error = IL_FALSE;

	ILboolean success = ilLoadImage(path.c_str());

	if (success)
	{

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		success = ilConvertImage(ilGetInteger(IL_IMAGE_FORMAT), IL_UNSIGNED_BYTE);

		// Quit out if we failed the conversion
		if (!success)
		{
			error = ilGetError();
			LOG("[ERROR] on path:%s error: %s", path.c_str(), iluErrorString(error));
			LOGUI("[ERROR] on path:%s error: %s", path.c_str(), iluErrorString(error));
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0,	
			ilGetInteger(IL_IMAGE_FORMAT),	
			ilGetInteger(IL_IMAGE_WIDTH),	
			ilGetInteger(IL_IMAGE_HEIGHT),	0,								
			ilGetInteger(IL_IMAGE_FORMAT),	
			GL_UNSIGNED_BYTE,				
			ilGetData());					

		glGenerateMipmap(GL_TEXTURE_2D);
		LOG(" [LOAD]{Texture}- path %s", path.c_str());
		LOGUI(" [LOAD]{Texture}- path %s", path.c_str());
	}
	else
	{
		error = ilGetError();
		LOG("[ERROR] on path:%s error: %s", path.c_str(), iluErrorString(error));
		LOGUI("[ERROR] on path:%s error: %s", path.c_str(), iluErrorString(error));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

		return data;
}
