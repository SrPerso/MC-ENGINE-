#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Globals.h"
#include "DContainer.h"
#include <string>

class Importer
{
public:
	Importer()
	{}

	virtual ~Importer()
	{}
public:

	//virtual bool LoadResource(DContainer* resource) = 0;

	virtual bool Import(const char* file, const char*path, std::string& output_file);
	virtual bool Import(const void*buffer, uint size, std::string& output_file);

	virtual bool Save(const void*buffer, const char* saveFile, uint id);

};

#endif