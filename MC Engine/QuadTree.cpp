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

template<typename TYPE>
inline void QuadtreeNode::CollectIntersections(std::vector<GameObject*>& Listojects, const TYPE & primitive) const
{
	if (primitive.Intersects(Box))
	{
		for (std::list<GameObject*>::const_iterator it = this->ListObjects.begin(); it != this->ListObjects.end(); ++it)
		{
			CMesh* TryMesh = (CMesh*)(*it)->GetComponent(COMP_MESH);
			if (TryMesh != nullptr)
			{
				if (primitive.Intersects(TryMesh->debugBox))
				{
					Listojects.push_back(*it);
				}
			}
		}
		for (int i = 0; i < MAXIMUM; i++)
			if (childs[i] != nullptr)
			{
				childs[i]->CollectIntersections(Listojects, primitive);
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

void QuadtreeNode::OrganizeChilds()
{
	for (std::list<GameObject*>::iterator it = ListObjects.begin(); it != ListObjects.end();)
	{
		for (int i = 0; i < 4; i++)
		{
			CMesh* TryMesh = (CMesh*)(*it)->GetComponent(COMP_MESH);
			CTransformation* TryTransform = (CTransformation*)(*it)->GetComponent(COMP_TRANSFORMATION);
			AABB TryBox = TryMesh->dataMesh->debugBox;
			TryBox.TransformAsAABB(TryTransform->dataTransformation->globalTransformMatrix);
			if (TryMesh != nullptr)
			{
				if (childs[i]->Box.Intersects(TryBox))
				{
					childs[i]->Insert((*it));
				}
			}
		}
		it = ListObjects.erase(it);
	}
}

void QuadtreeNode::DrawDebug() const
{
	Color color = Blue;

	if (IsEmpty() == true)
	{
		Box.DrawDebug(color);
	}
	else
	{
		for (int i = 0; i < MAXIMUM; i++)
		{
			childs[i]->DrawDebug();
		}
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


//QuadTree

QuadTree::QuadTree()
{
}

QuadTree::QuadTree(const AABB& box)
{
	Root = new QuadtreeNode(box);
}

QuadTree::~QuadTree()
{
	Clear();
}

void QuadTree::Insert(GameObject* AddObj)
{
	CMesh* TryMesh = (CMesh*)AddObj->GetComponent(COMP_MESH);
	CTransformation* TryTransform = (CTransformation*)AddObj->GetComponent(COMP_TRANSFORMATION);

	if (TryMesh != nullptr)
	{
		AABB TryBox = TryMesh->dataMesh->debugBox;
		TryBox.TransformAsAABB(TryTransform->dataTransformation->globalTransformMatrix);

		if (Root != nullptr && Root->Box.Contains(TryBox))
		{
			Root->Insert(AddObj);
		}
		else if (Root != nullptr && !Root->Box.Contains(TryBox))
		{
			Root->Box.Enclose(TryBox);
			Root->Insert(AddObj);
		}
		else if (Root == nullptr)
		{
			Root = new QuadtreeNode(TryBox);
			Root->Insert(AddObj);
		}
	}
}

void QuadTree::Remove(GameObject* QuitObj)
{
	CMesh* tmp = (CMesh*)QuitObj->GetComponent(COMP_MESH);
	if (Root != nullptr && Root->Box.Contains(tmp->dataMesh->debugBox))
	{
		Root->Remove(QuitObj);
	}
}

void QuadTree::Clear()
{
	delete Root;
	Root = nullptr;
}

void QuadTree::DrawDebug() const
{
	Color Colors = Blue;
	if (Root != nullptr)
	{
		Root->DrawDebug();
	}
}
