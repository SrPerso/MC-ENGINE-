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
	DMesh* ret;
	
	ret->nVertex = newMesh->mNumVertices;
	ret->Vertex = new float[ret->nVertex * 3];
	memcpy(ret->Vertex, newMesh->mVertices, sizeof(float)* ret->nVertex * 3);

	glGenBuffers(1, (GLuint*)&ret->idVertex);
	glBindBuffer(GL_ARRAY_BUFFER, ret->idVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ret->nVertex * 3, ret->Vertex, GL_STATIC_DRAW);

	LOGUI("{Importer}- Mesh Imported %i Vertices", ret->nVertex);

	return ret;
}
