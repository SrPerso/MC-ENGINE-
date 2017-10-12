#include "GameObject.h"

GameObject::GameObject(GameObject * parent): parent(parent)
{
}

GameObject::~GameObject()
{
}

const char * GameObject::GetName() const
{
	return name.c_str();
}

bool GameObject::SetName(const char * newName)
{
	bool ret = true;

	name = newName;

	return ret;
}

GameObject * GameObject::CreateChild()
{
	GameObject* ret = nullptr;

	ret = new GameObject(this);
	childs.push_back(ret);

	return ret;
}

void GameObject::DeleteChild(GameObject * objectToDelete)
{
	if (objectToDelete != nullptr)
	{
		std::vector<GameObject*>::iterator it = std::find(childs.begin(), childs.end(), objectToDelete);
	
		if (it != childs.end()) {
			childs.erase(it);
		}
		
	}
}

void GameObject::DeleteChilds()
{
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

Component * GameObject::CreateComponent(Component_Type type)
{
	Component* ret = nullptr;
	/*
		COMP_UNKNOWN,
	COMP_MESH,
	COMP_MATERIAL,
	COMP_CAMERA,
	COMP_SOUND
	*/

	//TODO COMPONENTS  news 4 all components

	/*
	switch (type)
	{
	case COMP_MESH:
		break;

	default://COMP_UNKNOWN
		break:

	

	}

	*/
	return ret;
}

Component * GameObject::GetComponent(Component_Type type)
{
	Component* ret = nullptr;
	
	for(std::vector<Component*>::iterator it = components.begin(); it!=components.end(); it++)
	{
		if((*it)->getType()== type){
			ret = (*it);
		}
	}

	return ret;
}

uint GameObject::ComponentVectorSize()
{
	return uint(components.size());
}

void GameObject::DeleteComponent(Component * comp)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it) == comp) {
			components.erase(it);
		}
	}

}

bool GameObject::IsEnable() const
{
	return isEnable;
}

void GameObject::SetEnable(bool isEnable)
{
	this->isEnable = isEnable;
}

void GameObject::Enable()
{
	isEnable = true;
}

void GameObject::Disable()
{
	isEnable = false;
}
