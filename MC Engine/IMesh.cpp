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


void ImporterMesh::SaveMesh(DMesh mesh)
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	
	uint ranges[2] = { mesh.nIndex, mesh.nVertex };

	uint size = sizeof(ranges) + sizeof(uint) * mesh.nIndex + sizeof(float) * mesh.nVertex * 3;
	
	char* data = new char[size]; // Allocate
	char* cursor = data;
	
	uint bytes = sizeof(ranges); // First store ranges
	
	memcpy(cursor, ranges, bytes);
	cursor += bytes; // Store indices

	bytes = sizeof(uint) * mesh.nIndex;
	
	memcpy(cursor, mesh.Index, bytes);

}

void ImporterMesh::LoadMesh(char* buffer ,DMesh*data)
{
	char* cursor = buffer;

	uint ranges[5];
	uint bytes = sizeof(ranges);

	memcpy(ranges, cursor, bytes);
	data->nIndex = ranges[0];
	data->nVertex = ranges[1];

	// Load indices
	cursor += bytes;

	bytes = sizeof(float) * data->nIndex;
	data->Index = new float[data->nIndex];

	memcpy(data->Index, cursor, bytes);
	
}
