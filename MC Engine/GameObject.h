#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeolib\Math\float3.h"
#include "MathGeolib\Geometry\AABB.h"
#include <string>
#include <vector>

class GameObject
{
public:
	GameObject();
	GameObject(GameObject* parent );
	virtual ~GameObject();


	// IDENTIFICATION ..........................

	const char* GetName()const; 
	bool SetName(const char* newName);
	//TO ADD: 
	//		$maybe it could be nice to set a new name 
	//		$Get & set tags? Like other engines like U.
	//		$Get & set layers ""

	//TREE.......................................

	GameObject* CreateChild();

	void DeleteChild(GameObject* objectToDelete);
	void DeleteChilds();

	GameObject* GetParent()const;
	
	void newParent(GameObject*newparent);
	void AddChild(GameObject* child);
	GameObject* GetFirstChild();

	//TO ADD 
	// to move the objects and change the parent is necesary a function newparent(gameobject)
	
	//COMPONENTS.................................

	Component* CreateComponent(Component_Type type, const void*buffet = nullptr);
	Component* GetComponent(Component_Type type);
	uint ComponentVectorSize();
	uint NumComponentTypeSize(Component_Type type);
	void DeleteComponent(Component* comp);
	void Move(float3 destiny, float3 position);
	//void Rotate(vec3 rotation);
	//ENABLE/DISABLE............................

	void UpdateTranformChilds();
	void SetLocalTransform();
	bool IsEnable()const;
	void SetEnable(bool isEnable);
	void Enable();
	void Disable();

	//STATIC............................

	bool IsStatic()const;
	void SetStatic();
	void SetNoStatic();

	//ACTIONS BASIC.............................

	//void PreUpdate();
	void Update(float dt);
	void cleanUp();

	void OnEditor();
	void OnInspector();

public:
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	AABB recalculatedBox;

private:
	std::string name;
	GameObject* parent = nullptr;
	bool isEnable = true;

	bool isStatic;

	uint GameOIbject_ID = 0;
};
#endif