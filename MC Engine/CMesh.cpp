#include "CMesh.h"
#include "GameObject.h"

CMesh::CMesh(GameObject * object, Component_Type type, DMesh* data) : Component(object, COMP_MESH)
{
	if (data)
	{
		nVertex = data->nVertex;
		idVertex = data->idVertex;
		Vertex = data->Vertex;
		wire = data->wire;
		nIndex = data->nIndex;
		idIndex = data->idIndex;
		Index = data->Index;
		nFaces = data->nFaces;
		normals = data->normals;
		idNormals = data->idNormals;
		nNormals = data->nNormals;
		idColors = data->idColors;
		colors = data->colors;
		idTexCoords = data->idTexCoords;
		texCoords = data->texCoords;
		debugMode = data->debugMode;
		debugBox = data->debugBox;
	}

		if (object != nullptr)
		{
			this->mesh_ID = object->NumComponentTypeSize(this->Ctype) + 1;
		}
		else
		{
			this->mesh_ID = 0;
		}
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
