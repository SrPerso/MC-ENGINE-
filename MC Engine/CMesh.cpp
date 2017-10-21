﻿#include "CMesh.h"
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

void CMesh::Move(float3 destiny, float3 start)
{

	float3 diff = destiny - start;

	for (int i = 0; i <= nVertex* 3; i += 3)
	{
		Vertex[i] += diff.x;
	}
	for (int i = 1; i <= nVertex * 3; i += 3)
	{
		Vertex[i] += diff.y;
	}
	for (int i = 2; i <= nVertex * 3; i += 3)
	{
		Vertex[i] += diff.z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, idVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nVertex * 3, Vertex, GL_STATIC_DRAW);

	debugBox.SetNegativeInfinity();
	debugBox.Enclose((float3*)Vertex, nVertex);
}
