#include "CMesh.h"

 
DataMesh::~DataMesh()
{

	//if (Vertex != nullptr) {
	//	delete Vertex;
	//	Vertex = nullptr;
	//}
	//if (Index != nullptr) {
	//	delete Index;
	//	Index = nullptr;
	//}
	//if (normals != nullptr) {
	//	delete normals;
	//	normals = nullptr;
	//}
}

CMesh::CMesh(GameObject * object) :Component(object, COMP_MESH)
{

}

CMesh::~CMesh()
{
}

void CMesh::OnUpdate(float dt)
{


}
