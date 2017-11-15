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
	GameObject(int exem);
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
	GameObject* GetFirstChild() const;

	GameObject* FindGameObject(int UID);
	
	//IDs System
	void SetParentID(uint parentID);
	uint GetParentId()const;
	void SetGOID(uint parentID);
	int GetGOId()const;

	//uid

	void SetParentUID(int parentUID);
	int GetParentUID()const;
	void SetGOUID(int UID);
	int GetGOUId()const;
	
	//TO ADD 
	// to move the objects and change the parent is necesary a function newparent(gameobject)
	
	//COMPONENTS.................................

	Component* CreateComponent(Component_Type type,int UID=-1, const void*buffer = nullptr);	

	Component* GetComponent(Component_Type type);
	uint ComponentVectorSize()const;
	uint NumComponentTypeSize(Component_Type type);
	void DeleteComponent(Component* comp);
	void AddComponent(Component* comp);
	//void Rotate(vec3 rotation);
	//ENABLE/DISABLE............................

	void UpdateTranformChilds();
	void SetLocalTransform();
	bool IsEnable()const;
	void SetEnable(bool isEnable);
	void Enable();
	void Disable();

	void TestGizmo();
	//STATIC............................

	bool IsStatic()const;
	void SetStatic();
	void SetNoStatic();

	//ACTIONS BASIC.............................

	//void PreUpdate();
	void Update(float dt);
	void CleanUp();

	void OnEditor();
	void OnInspector();
	void OnSelection();

	void OnSerialize(DataJSON & file)const;
	void OnDeserialize(DataJSON & file);
	//DATA ------------------------------------

	void SaveData();
	//std::vector<const void*>* SaveData();
	//void LoadData();


	//ray
	void TriIntersection(LineSegment & line, float & distance, float3 & hitPoint);
public:
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	bool selecting = false;
	bool isStatic = true;

private:
	std::string name;
	GameObject* parent = nullptr;
	bool isEnable = true;
	
	
private: 
	uint Parent_ID = 0;
	uint GameOIbject_ID = 0;
	uint Parent_UID = 0;
	uint GameOIbject_UID = 0;
};
#endif