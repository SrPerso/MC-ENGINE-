#pragma once

#include "GameObject.h"
#include <list>

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box);
	virtual ~QuadtreeNode();
	void Insert(GameObject* AddObj);
	void Remove(GameObject* QuitObj);
	void AddChilds();	
	bool IsEmpty() const;

public:
	AABB Box;
	std::list<GameObject*> ListObjects;
	QuadtreeNode* parent;
	QuadtreeNode* childs[4];
};

class QuadTree
{

	QuadTree();
	QuadTree(const AABB& box);
	void Insert(GameObject* toInsert);
	void Remove(GameObject* toRemove);
	void Clear();

	void DrawDebug() const;

public:
	QuadtreeNode* Root = nullptr;


};

