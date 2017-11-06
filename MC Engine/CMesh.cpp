#include "CMesh.h"
#include "GameObject.h"
#include "CTransformation.h"
#include "MathGeoLib/Geometry/Triangle.h"
#include "MathGeoLib/Geometry/LineSegment.h"
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
		dType = D_MESH;
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
	{	//	ImGui::Text("Texture Coords: %i", idTexCoords);
		
		ImGui::TreePop();
	}



}

void CMesh::OnInspector()
{
		ImGui::Text("\t Number of Vertex: %i", nVertex);
		ImGui::Text("\t Vertex ID: %i", idVertex);

		ImGui::Text("\t Number of Index: %i", nIndex);
		ImGui::Text("\t Index ID: %i", idIndex);

		ImGui::Text("\t Number of Normals: %i", nNormals);
		ImGui::Text("\t Normals ID: %i", idNormals);

		ImGui::Text("\t Colors ID: %i", idColors);
		

		ImGui::Text("\t ID: %i", this->object->GetGOId());

		//	ImGui::Text("Texture Coords: %i", idTexCoords);
	
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

const void * CMesh::GetData() 
{
	DMesh* data = new DMesh;

	data->nVertex = nVertex;
	data->idVertex = idVertex;
	data->Vertex = Vertex;
	data->wire = wire;
	data->nIndex = nIndex;
	data->idIndex = idIndex;
	data->Index = Index;
	data->nFaces = nFaces;
	data->normals = normals;
	data->idNormals = idNormals;
	data->nNormals = nNormals;
	data->idColors = idColors;
	data->colors = colors;
	data->idTexCoords = idTexCoords;
	data->texCoords = texCoords;
	data->debugMode = debugMode;
	data->debugBox = debugBox;

	DMesh* ret= data;

	delete data;

	return (DMesh*)ret;
}

bool CMesh::IntersectTriangle(LineSegment & picking, float& distance, float3 &hitPoint)
{
	bool ret = false;
	CTransformation* thisTransformation = nullptr;
	float distance2 = distance;
	float prevDistance = distance;
	
	thisTransformation = (CTransformation*)object->GetComponent(COMP_TRANSFORMATION);

	LineSegment newSegment = picking;
	newSegment.Transform(thisTransformation->GetTransMatrix().Inverted());

	for (uint i = 0; i < nIndex;i+=3)
	{
	Triangle tri(float3(Vertex[Index[i] * 3], Vertex[Index[i] * 3 + 1], Vertex[Index[i] * 3 + 2]), float3(Vertex[Index[i + 1] * 3], Vertex[Index[i + 1] * 3 + 1], Vertex[Index[i + 1] * 3 + 2]), float3(Vertex[Index[i + 2] * 3], Vertex[Index[i + 2] * 3 + 1], Vertex[Index[i + 2] * 3 + 2]));
	
	
		/*tri.a.Set(nVertex[&Index[i++]], nVertex[&Index[i]], nVertex[&Index[i]]);
		tri.b.Set(nVertex[&Index[i++]], nVertex[&Index[i]], nVertex[&Index[i]]);
		tri.c.Set(nVertex[&Index[i++]], nVertex[&Index[i]], nVertex[&Index[i]]);*/
			
		
		if (newSegment.Intersects(tri, &distance2, &hitPoint))
		{
			if (distance2 < prevDistance)
			{
				prevDistance = distance2;
				distance = distance2;
				ret = true;
			}
			

		}
	}

	return ret;

}
