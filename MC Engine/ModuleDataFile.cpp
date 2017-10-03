#include "ModuleDataFile.h"

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
