#ifndef __ModuleData_H__
#define __ModuleData_H__

#include <string>
#include <list>
#include "Globals.h"
#include "Module.h"
#include "parson\parson.h"

#include "Glew\include\glew.h"

#include "imgui\imgui.h"

class DataJSON {
public:
	DataJSON();
	~DataJSON();

	bool Init();
	void SaveAll() const;
	void LoadAll() const;

	//TODO Get & add vec4 & vec 2

	//get
	int GetInt(JSON_Object*  object, const char* name)const;
	float GetFloat(JSON_Object*  object, const char* name)const;
	double GetDouble(JSON_Object*  object, const char* name)const;
	bool GetBoolean(JSON_Object*  object, const char* name)const;
	const char*  GetString(JSON_Object*  object, const char* name)const;
	//ImVec2 GetVec2(JSON_Object * object,std::string name);
	//ImVec4 GetVec4(std::string name);

	//Style & config load and save

	//add
	void AddInt(JSON_Object*  object, const char* name, int number);
	void AddFloat(JSON_Object*  object, const char* name, float number);
	void AddDouble(JSON_Object*  object, const char* name, double number);
	void AddBool(JSON_Object*  object, const char* name, bool boolean);
	void AddString(JSON_Object*  object, const char* name, const char* string);


private: 

	std::list<std::string> files;
	JSON_Value* value_json = nullptr;
	JSON_Object* object_json = nullptr;

};


class DataFBX:public Module {
public:
	DataFBX(Application* app, bool start_enabled = true);
	~DataFBX();

	bool Init();
	bool Start();
	bool CleanUp();
public:
	bool LoadMesh(const char* path);



};


#endif