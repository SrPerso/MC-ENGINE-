#include "IMesh.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

ImporterMesh::ImporterMesh()
{
}

ImporterMesh::~ImporterMesh()
{
}

DMesh* ImporterMesh::ImportMesh(const aiMesh * newMesh)
{
	DMesh* ret;
	
	ret->nVertex = newMesh->mVertices;


	return ret;
}
