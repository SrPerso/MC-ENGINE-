#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"
#pragma comment (lib, "Devil/libx86/DevIL.lib" ) 
#pragma comment (lib, "Devil/libx86/ILU.lib" )
#pragma comment (lib, "Devil/libx86/ILUT.lib" ) 


#include <iostream>
#include <fstream>


ModuleTexture::ModuleTexture(Application * app, bool start_enabled): Module(app, start_enabled)
{
	name = "Texture Loader";
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Innit(JSON_Object * data)
{
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

bool ModuleTexture::CleanUp(JSON_Object * data)
{
	return false;
}

uint ModuleTexture::LoadTexture(const char * imagepath)
{
	ILuint imageID;				// Create an image ID as a ULuint

	GLuint textureID;			// Create a texture ID as a GLuint

	ILboolean success = false;			// Create a flag to keep track of success/failure

	ILenum error;				// Create a flag to keep track of the IL error state

	ilGenImages(1, &imageID); 		// Generate the image ID

	ilBindImage(imageID); 			// Bind the image

	//success = ilLoadImage("Lenna.png"); 	// Load the image file

	std::ifstream inputFile(imagepath, std::ifstream::binary);

	inputFile.seekg(0, inputFile.end);

	int length = inputFile.tellg();

	inputFile.seekg(0, inputFile.beg);
	if (length > 0)
	{
		LOG("Loading texture %s", imagepath);
		char * buffer = new char[length];

		inputFile.read(buffer, length);
		inputFile.close();
		success = ilLoadL(IL_TYPE_UNKNOWN, buffer, length); 	// Load the image file


		delete[] buffer;
		//MEMORY LEAKS
	}
	else
	{
		LOG("Error loading texture %s", imagepath);
	}

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
			App->ui->AddLogToConsole("Image conversion failed - IL reports error: ");
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

