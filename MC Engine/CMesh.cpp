#include "CMesh.h"

#include "GameObject.h"
DataMesh::~DataMesh()
{
	if (Vertex != nullptr) 
	{
		delete Vertex;
		Vertex = nullptr;
	}
	if (Index != nullptr) 
	{
		delete Index;
		Index = nullptr;
	}
	if (normals != nullptr)
	{
		delete normals;
		normals = nullptr;
	}
}

CMesh::CMesh(GameObject * object) :Component(object, COMP_MESH)
{
	if (object != nullptr)
	{
		this->mesh_ID = object->NumComponentTypeSize(this->type) + 1;
	}
	else
	{
		this->mesh_ID = 0;
	}
	//const char * preName = "Component Mesh _";
	name= "- Component Mesh_";
	name.append(std::to_string(mesh_ID));
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
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("\t Number of Vertex: %i", nVertex);
		ImGui::Text("\t Vertex ID: %i", idVertex);

		ImGui::Text("\t Number of Index: %i", nIndex);
		ImGui::Text("\t Index ID: %i", idIndex);

		ImGui::Text("\t Number of Normals: %i", nNormals);
		ImGui::Text("\t Normals ID: %i", idNormals);

		ImGui::Text("\t Colors ID: %i", idColors);

	//	ImGui::Text("Texture Coords: %i", idTexCoords);

		ImGui::TreePop();
	}



}
