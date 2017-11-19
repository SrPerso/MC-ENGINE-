#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"
#include "imGUI\imgui.h"
#include <string>
#include "DContainer.h"

class GameObject;
class DataJSON;

enum Component_Type
{
	COMP_UNKNOWN,
	COMP_MESH,
	COMP_TEXTURE,
	COMP_CAMERA,
	COMP_TRANSFORMATION,
	COMP_SOUND
};

class Component
{
public:
	Component(GameObject* object,int UID, Component_Type type = COMP_UNKNOWN);
	Component(int UID, Component_Type type = COMP_UNKNOWN);
	virtual ~Component();

	Component_Type getType()const;
	DType GetDataType()const;

	virtual bool IsEnable()const;
	virtual void SetEnable(bool isEnable);
	virtual void Enable();
	virtual void Disable();

	virtual void OnUpdate(float dt);
	virtual void OnEditor();
	virtual void OnInspector();
	virtual void OnCleanUp() = 0;

	virtual void OnSave(DataJSON&file)const = 0;
	virtual void OnLoad(DataJSON&file) = 0;

	void SetObjectParent(GameObject*);

	virtual const void*GetData() ;

	void SetName(std::string name);
protected:
	std::string name;
	Component_Type Ctype;
	DType dType;
	GameObject* object = nullptr;
	bool isEnable = true;

	int UID;
};

#endif