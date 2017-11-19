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
	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE & primitive) const;
	void OrganizeChilds();
	void DrawDebug() const;
	bool IsEmpty() const;

public:
	AABB Box;
	std::list<GameObject*> ListObjects;
	QuadtreeNode* parent;
	QuadtreeNode* childs[4];
	
};

class Quadtree
{

public:
	Quadtree();
	Quadtree(const AABB& box);
	~Quadtree();
	void Insert(GameObject* AddObj);
	void Remove(GameObject* QuitObj);
	void Clear();

	void DrawDebug() const;

public:
	QuadtreeNode* Root = nullptr;
	

};

