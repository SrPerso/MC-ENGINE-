#ifndef _MODULE_TEXTURE
#define _MODULE_TEXTURE


#include "Application.h"
#include "Globals.h"





class ModuleTexture : public Module {

public:
	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture();
	bool Innit(JSON_Object * data);
	bool CleanUp(JSON_Object * data);

	uint LoadTexture(const char* imagepath);
	bool correct=true;

};



#endif // !_MODULE_TEXTURE

