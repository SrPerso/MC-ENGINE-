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
	DataJSON(JSON_Object*);
	~DataJSON();

	bool Init();
	void SaveAll() const;


	//TODO Get & add vec4 & vec 2

	//get
	int GetInt(const char* name, JSON_Object*  object = nullptr)const;
	float GetFloat(const char* name, int arrayI = -1, JSON_Object*  object = nullptr)const;
	double GetDouble(const char* name, JSON_Object*  object = nullptr)const;
	bool GetBoolean( const char* name, JSON_Object*  object = nullptr)const;
	const char*  GetString(const char* name, JSON_Object*  object = nullptr)const;
	DataJSON GetSection(const char*name)const;
	JSON_Value* GetValor(const char * field, int count, JSON_Object * object = nullptr) const;
	//Style & config load and save

	//add
	DataJSON AddSection(const char* name, JSON_Object*  object = nullptr);
	void AddInt( const char* name, int number, JSON_Object*  object = nullptr);
	void AddFloat(const char* name, float number, JSON_Object*  object = nullptr);
	void AddDouble( const char* name, double number, JSON_Object*  object = nullptr);
	void AddBool( const char* name, bool boolean, JSON_Object*  object = nullptr);
	void AddString(const char* name, const char* string, JSON_Object*  object = nullptr);

	void AddArrayF(const char* name, const float * arrays, uint size);

private: 

	std::list<std::string> files;
	JSON_Value* value_json = nullptr;
	JSON_Object* object_json = nullptr;

	JSON_Array* temp_array_json = nullptr;
};


#endif

