#include "Importer.h"

bool Importer::Import(const char * file, const char * path, std::string & output_file)
{
	bool ret = true;


	return ret;
}

bool Importer::Import(const void * buffer, uint size, std::string & output_file)
{
	bool ret = true;


	return ret;
}

bool Importer::Save(const void * buffer, const char * saveFile, uint id)
{

	LOGUI("[ERROR]- saving importes -> default saver");
	return true;
}

bool Importer::Load(const void * buffer, const char * saveFile, uint id)
{
	LOGUI("[ERROR]- saving importes -> default Loader");
	return true;
}
