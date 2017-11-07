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

}

DataJSON::DataJSON(std::string name)
{
	value_json = json_parse_file(name.c_str());
	object_json = json_value_get_object(value_json);
}

DataJSON::DataJSON(JSON_Object*fileobject)
{
	object_json = fileobject;
}


DataJSON::~DataJSON()
{
	if(value_json)
		json_value_free(value_json);
}

bool DataJSON::Init()
{
	bool ret = true;
	

	for (std::list<std::string>::const_reverse_iterator  file = files.rbegin(); file != files.crend(); ++file)
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
	

	}//for	

	return ret;
}

void DataJSON::SaveAll() const
{
	
	for (std::list<std::string>::const_reverse_iterator file = files.rbegin(); file != files.crend(); ++file) {

		JSON_Object* object = nullptr;

		for (std::list<Module*>::const_reverse_iterator item = App->GetModuleList()->rbegin(); item != App->GetModuleList()->crend(); ++item)
		{
			object = json_object_get_object(object_json, (*item)->name.c_str());
			(*item)->SaveJson(object);		
		}

		json_serialize_to_string_pretty(value_json);		
		json_serialize_to_file(value_json, file->c_str());
	}
}

int DataJSON::GetInt( const char * name, JSON_Object * object) const
{
	int ret = 0;

	if (object)
	{
		JSON_Value* value = json_object_get_value(object, name);

		if (value && json_value_get_type(value) == JSONNumber)
			ret = json_value_get_number(value);
	}
	else
	{
		JSON_Value* value = json_object_get_value(object_json, name);

		if (value && json_value_get_type(value) == JSONNumber)
			ret = json_value_get_number(value);
	}

	return (int)ret;
}

float DataJSON::GetFloat( const char * name, int arrayI, JSON_Object * object) const
{
	float ret = 0.0f;

	

	if (object)
	{
		JSON_Value* value = GetValor(name, arrayI, object);

		if (value && json_value_get_type(value) == JSONNumber)
			ret = json_value_get_number(value);
	}
	else
	{
		JSON_Value* value = GetValor(name, arrayI);

		if (value && json_value_get_type(value) == JSONNumber)
			ret = json_value_get_number(value);
	}
	return (float)ret;
}

double DataJSON::GetDouble(const char * name, JSON_Object * object) const
{
	double ret = 0;

	if (object)
	{
		JSON_Value* value = json_object_get_value(object, name);

		if (value && json_value_get_type(value) == JSONNumber)
			ret = json_value_get_number(value);
	}
	else
	{
		JSON_Value* value = json_object_get_value(object_json, name);

		if (value && json_value_get_type(value) == JSONNumber)
			ret = json_value_get_number(value);
	}

	return (double)ret;
}

bool DataJSON::GetBoolean(const char * name, JSON_Object * object) const
{
	bool ret = true;

	if (object)
	{
		JSON_Value* value = json_object_get_value(object, name);

		if (value && json_value_get_type(value) == JSONBoolean)
			ret = json_value_get_boolean(value);
	}
	else
	{
		JSON_Value* value = json_object_get_value(object_json, name);

		if (value && json_value_get_type(value) == JSONBoolean)
			ret = json_value_get_boolean(value);
	}
	return (bool)ret;
}

const char * DataJSON::GetString(const char * name, JSON_Object * object) const
{
	const char * ret = nullptr;

	if (object)
	{
		JSON_Value* value = json_object_get_value(object, name);

		if (value && json_value_get_type(value) == JSONString)
			ret = json_value_get_string(value);
	}
	else
	{
		JSON_Value* value = json_object_get_value(object_json, name);

		if (value && json_value_get_type(value) == JSONString)
			ret = json_value_get_string(value);
	}

	return ret;
}

DataJSON DataJSON::GetSection(const char * name)const
{
	return DataJSON(json_object_dotget_object(object_json, name));
}

JSON_Value * DataJSON::GetValor(const char * field, int count, JSON_Object * object) const
{
	if (object) 
	{
		if (count == -1)
			return json_object_get_value(object, field);

		JSON_Array* arrays = json_object_get_array(object, field);
		if (arrays != nullptr)
		{
			return json_array_get_value(arrays, count);
		}
	}
	else
	{
		if (count == -1)
			return json_object_get_value(object_json, field);

		JSON_Array* arrays = json_object_get_array(object_json, field);
		if (arrays != nullptr)
		{
			return json_array_get_value(arrays, count);
		}
	}

	return nullptr;
}

DataJSON DataJSON::AddSection(const char * name, JSON_Object * object)
{
	if(object)
		json_object_set_value(object, name, json_value_init_object());
	
	else
		json_object_set_value(object_json, name, json_value_init_object());

	return GetSection(name);
}

void DataJSON::AddInt(const char * name, int number, JSON_Object * object)
{
	if(object!=nullptr)
		json_object_set_number(object, name, number);
	else 
		json_object_set_number(object_json, name, number);
}

void DataJSON::AddFloat(const char * name, float number, JSON_Object * object)
{
	if (object)
		json_object_set_number(object, name, (float)number);
	else
		json_object_set_number(object_json, name, (float)number);

}

void DataJSON::AddDouble(const char * name, double number, JSON_Object * object)
{
	if (object)
		json_object_set_number(object, name, (double)number);
	else
		json_object_set_number(object_json, name, (double)number);
}

void DataJSON::AddBool( const char * name, bool boolean, JSON_Object * object)
{
	if (object)
		json_object_set_boolean(object, name, (bool)boolean);
	else
		json_object_set_boolean(object_json, name, (bool)boolean);
}

void DataJSON::AddString( const char * name, const char * string, JSON_Object * object)
{
	if (object)
		json_object_set_string(object, name, (const char *)string);
	else
		json_object_set_string(object_json, name, (const char *)string);
}

void DataJSON::AddArrayF(const char * name, const float * arrays, uint size)
{
	if (arrays != nullptr && size > 0)
	{
		JSON_Value* temp = json_value_init_array();
		temp_array_json = json_value_get_array(temp);
		json_object_set_value(object_json, name, temp);

		for (int i = 0; i < size; ++i)
		{
			if (json_array_append_number(temp_array_json, arrays[i]) != JSONSuccess)
			{
				LOGUI("[ERROR]- Cant add Float array to scene file number %i", i);
				break;
			}
		}
	}
}

void DataJSON::AddArray(const DataJSON& object)
{
	if (temp_array_json != nullptr)
	{
		if (json_array_append_value(temp_array_json, json_value_deep_copy(object.value_json)) == JSONSuccess)
		{
			LOGUI("[ERROR]- Cant add array to scene file");
		}
	}

}

void DataJSON::AddArray(const char * nstring)
{
	JSON_Value* tmpArray = json_value_init_array();

	temp_array_json = json_value_get_array(tmpArray);

	if (json_object_set_value(object_json, nstring, tmpArray) == JSONSuccess)
	{
		LOGUI("[ERROR]- Cant add array (string) to scene file");
	}


}

size_t DataJSON::buffSizeSaver(char ** buffer, const char * comment)
{
	size_t serSize = json_serialization_size(value_json);

	*buffer = new char[serSize];
	json_serialize_to_buffer(value_json, *buffer, serSize);

	return size_t(serSize);
}
