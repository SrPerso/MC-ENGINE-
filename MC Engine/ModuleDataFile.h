#ifndef __ModuleData_H__
#define __ModuleData_H__

#include <string>
#include <vector>
#include "Globals.h"
#include "parson\parson.h"

class DataJSON {
public:
	DataJSON();
	~DataJSON();
	//todo Get & set of int, float, double, vec4 & string
	//Style & config load and save


private: 


	std::vector<std::string> files;
	JSON_Value* value_json = nullptr;
	JSON_Object* object_json = nullptr;
};
#endif