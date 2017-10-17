#include "ITexture.h"

#include "Application.h"
#include "Globals.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "Glew\include\glew.h"


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
	DTexture* ret = nullptr;

	if (newMaterial != nullptr)
	{

		uint numTextures = newMaterial->GetTextureCount(aiTextureType_DIFFUSE);
		aiString path;

		aiReturn retu;
		retu = newMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		if (retu == aiReturn_FAILURE)
			LOGUI("[ERROR]- fail getting texture");

		std::string fullPath = FilePath;
		fullPath.append(path.C_Str());

		ret->image = App->texture->LoadTexture(fullPath.c_str());
		ret->Textname = fullPath;

		LOGUI("[OK]- Imported Texture");
	}
	glBindTexture(GL_TEXTURE_2D, ret->image);

	return ret;
}
