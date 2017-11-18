#include "CMesh.h"
#include "GameObject.h"
#include "ModuleDataFile.h"
#include "CTransformation.h"
#include "MathGeoLib/Geometry/Triangle.h"
#include "MathGeoLib/Geometry/LineSegment.h"

CMesh::CMesh(GameObject * object, int UID, Component_Type type, DMesh* data) : Component(object, UID, COMP_MESH)
{
	dataMesh = new DMesh();

	SetData(data);

	if (object != nullptr)
	{
		this->mesh_ID = object->NumComponentTypeSize(this->Ctype) + 1;
	}
	else
	{
		this->mesh_ID = 0;  
	}
	name = "- Component Mesh_";
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
		ImGui::Text("Number of Vertex: %i", dataMesh->nVertex);
		ImGui::SameLine();
		ImGui::Text("Vertex ID: %i", dataMesh->idVertex);

		ImGui::Text("Number of Index: %i", dataMesh->nIndex);
		ImGui::SameLine();
		ImGui::Text("Index ID: %i", dataMesh->idIndex);

		ImGui::Text("\t Number of Normals: %i", dataMesh->nNormals);
		ImGui::SameLine();
		ImGui::Text("Normals ID: %i", dataMesh->idNormals);

		ImGui::Text("Colors ID: %i", dataMesh->idColors);

		ImGui::Text("ID: %i", this->object->GetGOId());
		ImGui::SameLine();
		ImGui::Text("Object UID: %i", this->object->GetGOUId());

		ImGui::Text("component UID: %i", UID);

}

void CMesh::OnCleanUp()
{
	
	delete dataMesh;
}

void CMesh::OnSave(DataJSON & file) const
{
	file.AddInt("Component UID", UID);
	//file.AddInt("Component Type", Ctype);
}

void CMesh::OnLoad(DataJSON & file)
{
	UID = file.GetFloat("Component UID");

	dataMesh = App->datamanager->importerMesh->Load(this,nullptr,this->object->GetGOUId());

}

void CMesh::Move(float3 destiny, float3 start)
{

	float3 diff = destiny - start;

	for (int i = 0; i <= dataMesh->nVertex* 3; i += 3)
		dataMesh->Vertex[i] += diff.x;

	for (int i = 1; i <= dataMesh->nVertex * 3; i += 3)
		dataMesh->Vertex[i] += diff.y;
	
	for (int i = 2; i <= dataMesh->nVertex * 3; i += 3)
		dataMesh->Vertex[i] += diff.z;
	
	glBindBuffer(GL_ARRAY_BUFFER, dataMesh->idVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dataMesh->nVertex * 3, dataMesh->Vertex, GL_STATIC_DRAW);

	dataMesh->debugBox.SetNegativeInfinity();
	dataMesh->debugBox.Enclose((float3*)dataMesh->Vertex, dataMesh->nVertex);
}

const void * CMesh::GetData() 
{
	return (DMesh*)dataMesh;
}

void CMesh::SetData(DMesh * data)
{
	if (data!= nullptr)
		dataMesh = data;
	else
		LOGUI("[ERROR]-Cant Set Data - Component Mesh");

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

	for (uint i = 0; i < dataMesh->nIndex; i += 3)
	{
		Triangle tri(float3(dataMesh->Vertex[dataMesh->Index[i] * 3], dataMesh->Vertex[dataMesh->Index[i] * 3 + 1], dataMesh->Vertex[dataMesh->Index[i] * 3 + 2]),
			float3(dataMesh->Vertex[dataMesh->Index[i + 1] * 3], dataMesh->Vertex[dataMesh->Index[i + 1] * 3 + 1], dataMesh->Vertex[dataMesh->Index[i + 1] * 3 + 2]),
			float3(dataMesh->Vertex[dataMesh->Index[i + 2] * 3], dataMesh->Vertex[dataMesh->Index[i + 2] * 3 + 1], dataMesh->Vertex[dataMesh->Index[i + 2] * 3 + 2]));
		
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
