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

void CMesh::OnEditor()
{
	/*
	nVertex, idVertex, nIndex, idIndex, idNormals, nNormals, idColors, idTexCoords, ...
	*/
	if (ImGui::TreeNodeEx("Mesh Component"))
	{
		ImGui::Text("Number of Vertex: %i", nVertex);
		ImGui::Text("Vertex ID: %i", idVertex);

		ImGui::Text("Number of Index: %i", nIndex);
		ImGui::Text("Index ID: %i", idIndex);

		ImGui::Text("Number of Normals: %i", nNormals);
		ImGui::Text("Normals ID: %i", idNormals);

		ImGui::Text("Colors ID: %i", idColors);

		ImGui::Text("Texture Coords: %i", idTexCoords);

		ImGui::TreePop();
	}



}
