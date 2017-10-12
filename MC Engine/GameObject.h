#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Component.h"
#include <string>
#include <vector>
#include "Globals.h"

class GameObject
{
public:

	GameObject(GameObject* parent);
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
	
	//TO ADD 
	// to move the objects and change the parent is necesary a function newparent(gameobject)


	//COMPONENTS.................................

	Component* CreateComponent(Component_Type type);
	Component* GetComponent(Component_Type type);
	uint ComponentVectorSize();
	void DeleteComponent(Component* comp);

	//ENABLE/DISABLE............................

	bool IsEnable()const;
	void SetEnable(bool isEnable);
	void Enable();
	void Disable();

	//ACTIONS BASIC.............................

	/* TODOOOOOOOO
	void Init();
	void PreUpdate();
	void Update(float dt);
	void cleanUp();
	*/
public:
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

private:
	std::string name;
	GameObject* parent = nullptr;
	bool isEnable = true;
};

#endif