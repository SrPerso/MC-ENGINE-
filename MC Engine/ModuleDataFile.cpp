#include "ModuleDataFile.h"
#include <list>
#include "Application.h"
#include "Module.h"

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
	

	for (std::list<std::string>::iterator  file = files.begin(); file != files.end(); ++file)
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

	//	for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)



	}//for

	return ret;
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
