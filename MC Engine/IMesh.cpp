#include "IMesh.h"
#include "Application.h"
#include "Globals.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "Glew\include\glew.h"


ImporterMesh::ImporterMesh()
{
}

ImporterMesh::~ImporterMesh()
{
}

DMesh* ImporterMesh::ImportMesh(const aiMesh * newMesh)
{
	DMesh* ret= nullptr;
	
	if (newMesh != nullptr)
	{
		//VERTEX------------------------------------------------------------------------------

		ret->nVertex = newMesh->mNumVertices;
		ret->Vertex = new float[ret->nVertex * 3];
		memcpy(ret->Vertex, newMesh->mVertices, sizeof(float)* ret->nVertex * 3);

		glGenBuffers(1, (GLuint*)&ret->idVertex);
		glBindBuffer(GL_ARRAY_BUFFER, ret->idVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ret->nVertex * 3, ret->Vertex, GL_STATIC_DRAW);

		LOGUI("{Importer}- Mesh Imported %i Vertices", ret->nVertex);


		//FACES // INDICES------------------------------------------------------------------------------

		if (newMesh->HasFaces())
		{
			ret->nFaces = newMesh->mNumFaces;

			ret->nIndex = newMesh->mNumFaces * 3;
			ret->Index = new float[ret->nIndex]; //every face is a triangle.

			for (uint i = 0; i < newMesh->mNumFaces; ++i)
			{
				if (newMesh->mFaces[i].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
					LOGUI("WARNING- geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&ret->Index[i * 3], newMesh->mFaces[i].mIndices, sizeof(float) * 3);
				}
			}

			glGenBuffers(1, (GLuint*)&(ret->idIndex));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->idIndex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * ret->nIndex, ret->Index, GL_STATIC_DRAW);

			LOGUI("[OK]- Mesh %i indices Loaded!", ret->nIndex);
		}// has faces			
		else
		{
			LOGUI("[ERROR]- Mesh has not indices!");
		}
		
		// NORMALS------------------------------------------------------------------------------

		if (newMesh->HasNormals())
		{

			ret->nNormals = ret->nVertex * 3;
			ret->normals = new float[ret->nVertex * 3];
			memcpy(ret->normals, newMesh->mNormals, (sizeof(float) * ret->nVertex * 3));

			glGenBuffers(1, (GLuint*)&(ret->idNormals)); // 
			glBindBuffer(GL_ARRAY_BUFFER, ret->idNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ret->nVertex * 3, ret->normals, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			LOGUI("[OK]- Mesh %i normals Loaded!", ret->nNormals);
		}// has normals
		else
		{
			LOGUI("[ERROR]- geometry has not normals!");
		}

		// Vertex Color ------------------------------------------------------------------------------

		if (newMesh->HasVertexColors(0))
		{
			ret->colors = new float[ret->nVertex * 3];
			memcpy(ret->colors, newMesh->mColors, sizeof(float) * ret->nVertex * 3);

			glGenBuffers(1, (GLuint*) &(ret->idColors));
			glBindBuffer(GL_ARRAY_BUFFER, ret->idColors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ret->nVertex * 3, ret->colors, GL_STATIC_DRAW);
			LOGUI("[OK]- Mesh vertex Colors Loaded!");
		}
		else
		{
			LOGUI("[ERROR]- Vertex has not colors!");
		}
	}
	else
	{
		LOGUI("[ERROR]{Importer}- The mesh has not vertices");
	}


	return ret;
}
