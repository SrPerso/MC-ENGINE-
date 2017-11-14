#include "QuadTree.h"
#include "CMesh.h"
#include "CTransformation.h"

#define BOX_SIZE 1	
#define MAXIMUM 4	

QuadtreeNode::QuadtreeNode(const AABB & box)
{
	parent = nullptr;
	for (int i = 0; i < MAXIMUM; i++)
	{
		childs[i] = nullptr;
	}
}

QuadtreeNode::~QuadtreeNode()
{		
	for (int i = 0; i < MAXIMUM; i++)
	{
		delete childs[i];
		childs[i] = nullptr;
	}
}

void QuadtreeNode::Insert(GameObject * AddObj)
{
	if (ListObjects.size() < MAXIMUM || Box.Size().x <= BOX_SIZE)
	{
		ListObjects.push_back(AddObj);
	}
	else
	{
		if (IsEmpty() == true) 
		{
			AddChilds();
		}
		OrganizeChilds();

	}
}

void QuadtreeNode::Remove(GameObject * QuitObj)
{
	for (std::list<GameObject*>::iterator it = ListObjects.begin(); it != ListObjects.end(); it++)
	{
		if ((*it) == QuitObj)
		{
			ListObjects.erase(it);
			return;
		}
	}
	if (IsEmpty() == false)
	{
		for (int i = 0; i < MAXIMUM; i++)
		{
			childs[i]->Remove(QuitObj);
		}
	}
}



void QuadtreeNode::AddChilds()
{
	AABB childBox;

	float3 MinBox = this->Box.minPoint;
	float3 MaxBox = this->Box.maxPoint;
	float3 BoxSize = this->Box.Size();

	
	childBox.minPoint = float3(MinBox.x, MinBox.y, MinBox.z);
	childBox.maxPoint = float3(MaxBox.x - BoxSize.x * 0.5f, MaxBox.y, MaxBox.z - BoxSize.z * 0.5f);
	childs[0] = new QuadtreeNode(childBox);//0


	childBox.minPoint = float3(MinBox.x + BoxSize.x * 0.5f, MinBox.y, MinBox.z);
	childBox.maxPoint = float3(MaxBox.x, MaxBox.y, MaxBox.z - BoxSize.z * 0.5f);
	childs[1] = new QuadtreeNode(childBox);//1


	childBox.minPoint = float3(MinBox.x, MinBox.y, MinBox.z + BoxSize.z * 0.5f);
	childBox.maxPoint = float3(MaxBox.x - BoxSize.x * 0.5f, MaxBox.y, MaxBox.z);
	childs[2] = new QuadtreeNode(childBox);//2


	childBox.minPoint = float3(MaxBox.x - BoxSize.x * 0.5f, MaxBox.y, MaxBox.z - BoxSize.z * 0.5f);
	childBox.maxPoint = float3(MaxBox.x, MaxBox.y, MaxBox.z);
	childs[3] = new QuadtreeNode(childBox);//3

	for (int i = 0; i < MAXIMUM; i++)
	{
		childs[i]->parent = this;
	}
}


bool QuadtreeNode::IsEmpty() const
{
	if (childs[0] == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}
