#include "IMesh.h"
#include "Application.h"
#include "Globals.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"


#include "Glew\include\glew.h"


//filesystem

#include <fstream>
#include <iostream>
#include <new>

ImporterMesh::ImporterMesh()
{
}

ImporterMesh::~ImporterMesh()
{
}

DMesh* ImporterMesh::ImportMesh(aiMesh * buffer)
{

	aiMesh* newMesh = buffer;

	if (newMesh != nullptr)
	{
		DMesh* mesh = new DMesh();
	
		//VERTEX------------------------------------------------------------------------------

		mesh->nVertex = newMesh->mNumVertices;
		mesh->Vertex = new float[mesh->nVertex * 3];
		memcpy(mesh->Vertex, newMesh->mVertices, sizeof(float)* mesh->nVertex * 3);

		glGenBuffers(1, (GLuint*)&mesh->idVertex);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->Vertex, GL_STATIC_DRAW);

		LOGUI("{Importer}- Mesh Imported %i Vertices", mesh->nVertex);


		//FACES // INDICES------------------------------------------------------------------------------

		if (newMesh->HasFaces())
		{
			mesh->nFaces = newMesh->mNumFaces;

			mesh->nIndex = newMesh->mNumFaces * 3;
			mesh->Index = new float[mesh->nIndex]; //every face is a triangle.

			for (uint i = 0; i < newMesh->mNumFaces; ++i)
			{
				if (newMesh->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
					LOGUI("WARNING- geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&mesh->Index[i * 3], newMesh->mFaces[i].mIndices, sizeof(float) * 3);
				}
			}

			glGenBuffers(1, (GLuint*)&(mesh->idIndex));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->nIndex, mesh->Index, GL_STATIC_DRAW);


			LOGUI("[OK]- Mesh %i indices Loaded!", mesh->nIndex);
		}// has faces			
		else
		{
			LOGUI("[ERROR]- Mesh has not indices!");
		}

		// NORMALS------------------------------------------------------------------------------

		if (newMesh->HasNormals())
		{
			mesh->nNormals = mesh->nVertex * 3;
			mesh->normals = new float[mesh->nVertex * 3];
			memcpy(mesh->normals, newMesh->mNormals, (sizeof(float) * mesh->nVertex * 3));

			glGenBuffers(1, (GLuint*)&(mesh->idNormals)); // 
			glBindBuffer(GL_ARRAY_BUFFER, mesh->idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->normals, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);

			LOGUI("[OK]- Mesh %i normals Loaded!", mesh->nNormals);
		}// has normals
		else
		{
			LOGUI("[ERROR]- geometry has not normals!");
		}

		// Vertex Color ------------------------------------------------------------------------------

		if (newMesh->HasVertexColors(0))
		{
			mesh->colors = new float[mesh->nVertex * 3];
			memcpy(mesh->colors, newMesh->mColors, sizeof(float) * mesh->nVertex * 3);

			glGenBuffers(1, (GLuint*) &(mesh->idColors));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->idColors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->colors, GL_STATIC_DRAW);

			LOGUI("[OK]- Mesh vertex Colors Loaded!");
		}
		else
		{
			LOGUI("[ERROR]- Vertex has not colors!");
		}

		if (newMesh->HasTextureCoords(0))
		{
			mesh->texCoords = new float[mesh->nVertex * 3];
			memcpy(mesh->texCoords, newMesh->mTextureCoords[0], sizeof(float) * mesh->nVertex * 3);
			
			glGenBuffers(1, (GLuint*) &(mesh->idTexCoords));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->idTexCoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->nVertex * 3, mesh->texCoords, GL_STATIC_DRAW);
		}
		
		mesh->debugBox.SetNegativeInfinity();//
		mesh->debugBox.Enclose((float3*)mesh->Vertex, mesh->nVertex);

		App->camera->CenterCameraToObject(&mesh->debugBox);

		

		//delete mesh;

		//Load(&mesh, nullptr, 20);
		Save(&mesh, nullptr, 20);
		return mesh;
	}
	else
	{
		return nullptr;
		LOGUI("[ERROR]{Importer}- The mesh has not vertices");
	}
	return nullptr;
}

DTransformation* ImporterMesh::ImportTrans(aiNode* node)
{

	aiVector3D move;
	aiVector3D scale;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scale, rotation, move);
	
	float3 pos(move.x, move.y, move.z);
	float3 sca(scale.x, scale.y, scale.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	DTransformation* transl = new DTransformation(pos, sca, rot);

		return transl;
}

bool ImporterMesh::Save(const void* buffer, const char * saverFile, uint id)
{
	bool ret = true;
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	
	DMesh* mesh = (DMesh*)buffer;

	std::string path;
	
	//path to save----------

	path = "Library/Mesh";
	path.append("/");
	path.append("Mesh");
	path.append(std::to_string(id));
	path.append(".mcm");

	LOGUI("[SAVING]- %s", path.c_str());

	// Opening ----------

	FILE* file = fopen(path.c_str(), "r");

	if (file != nullptr)
	{
		fclose(file);
		LOGUI("[ERROR]- %s  is already created", path.c_str());

		return false;
	}

	// contains the number of every thing ----------

	uint colorSize = 0;
	if (mesh->colors != nullptr)  // number of colors ( vertex)
		colorSize = mesh->nVertex;

	uint textureCoodsSize = 0;
	if (mesh->texCoords != nullptr)// number of text coods
		textureCoodsSize = mesh->nVertex;
	
	uint ranges[5] = {
		mesh->nIndex,
		mesh->nVertex,
		colorSize/* colors */,
		mesh->nNormals  /* normals*/,
		textureCoodsSize  /* text coods*/
	};

	//size of all ----------

	uint size = 0;
	size += sizeof(ranges);

	size += mesh->nVertex * sizeof(float) * 3;	 // number of vertex

	size += mesh->nIndex * sizeof(uint);		// number of index  

	if (mesh->colors != nullptr)				// number of colors ( vertex)
		size += mesh->nVertex * sizeof(float) * 3;
	
	if (mesh->normals != nullptr)				// number of normals
		size += mesh->nNormals * sizeof(float) * 3;

	if (mesh->texCoords != nullptr)				// number of text coods
		size += mesh->nVertex * sizeof(float) * 3;

	
	//Now is going to copy all to the array ----------

	char* data = new char[size];
	char* cursor = data;	
	uint allocsize = 0;

	allocsize = sizeof(ranges);
	memcpy(cursor, ranges, allocsize);						//contains the number of every thing 
	
	cursor += allocsize;
	allocsize = sizeof(float) * mesh->nVertex * 3;			//Vertices
	memcpy(cursor, (DMesh*)mesh->nVertex, allocsize);
		
	cursor += allocsize;
	allocsize = sizeof(uint) *mesh->nIndex;					//Index
	memcpy(cursor, mesh->Index, allocsize);

	if (mesh->normals != nullptr)				
	{
		cursor += allocsize;
		allocsize = sizeof(float) * mesh->nNormals * 3;		//Nortmals
		memcpy(cursor, mesh->normals, allocsize);
	}

	if (mesh->texCoords != nullptr)
	{
		cursor += allocsize;
		allocsize = sizeof(float) *mesh->nVertex * 3;		//Texture coods
		memcpy(cursor, mesh->texCoords, allocsize);
	}

	if (mesh->colors != nullptr)
	{
		cursor += allocsize;
		allocsize = sizeof(float) * mesh->nVertex * 3;		//colors
		memcpy(cursor, mesh->colors, allocsize);
	}

	std::ofstream file_end(path.c_str(), std::ofstream::binary);

	if (file_end.good()) //write file
		file_end.write(data, size);
	else
		LOGUI("[ERROR]- writting error ", path.c_str() );

	file_end.close();

	RELEASE_DELET(data);
		
	LOGUI("[OK]- Saving %s ", );

	return ret;
}

bool ImporterMesh::Load(const void * buffer, const char * loadFile, uint id)
{

	bool ret = true;

	//DMesh* data = (DMesh*)buffer;

	//std::string path;

	//path = "Library/Mesh";
	//path.append("/");
	//path.append("Mesh");
	//path.append(std::to_string(id));
	//path.append(".McMesh");

	//std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

	//int size;
	//size = file.gcount();

	//char* cursor = path.c_str;

	//if (file.read(cursor, size))
	//{
	//	LOGUI("[ERROR]- Cant read %s.", path.c_str);
	//	return  false;
	//}
	//else
	//{
	//	LOGUI("[ERROR]- Cant read %s.", path.c_str);
	//	ret =  true;
	//}

	//uint ranges[5];
	//uint bytes = sizeof(ranges);


	//memcpy(ranges, cursor, bytes);
	//data->nIndex = ranges[0];
	//data->nVertex = ranges[1];

	//// Load indices
	//cursor += bytes;

	//bytes = sizeof(float) * data->nIndex;
	//data->Index = new float[data->nIndex];

	//glGenBuffers(1, (GLuint*)&data->idIndex);
	//glBindBuffer(GL_ARRAY_BUFFER, data->idIndex);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data->nIndex * 3, data->Index, GL_STATIC_DRAW);

	//bytes = sizeof(float) *data->nVertex * 3;
	//data->Vertex = new float[data->nVertex];

	//glGenBuffers(1, (GLuint*)&data->idVertex);
	//glBindBuffer(GL_ARRAY_BUFFER, data->idVertex);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data->nVertex * 3, data->Vertex, GL_STATIC_DRAW);

	//bytes = sizeof(uint) *data->nNormals * 3;
	//data->normals = new float[data->nNormals];

	//glGenBuffers(1, (GLuint*)&data->idNormals);
	//glBindBuffer(GL_ARRAY_BUFFER, data->idNormals);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data->nNormals * 3, data->normals, GL_STATIC_DRAW);

	//memcpy(data->Index, cursor, bytes);


	return ret;
}
