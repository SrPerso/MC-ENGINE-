#include "GameObject.h"
#include "Application.h"

GameObject::GameObject(GameObject * parent): parent(parent)
{
	name = "GameObject";
	App->ui->AddLogToConsole("Constructor Game Object");
}

GameObject::~GameObject()
{
	while (components.begin != components.end())
	{

		delete components.back();
		components.pop_back();
	}
	components.clear();

	while (childs.begin != childs.end()) 
	{

		delete childs.back();
		childs.pop_back();
	}
	childs.clear();

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

void GameObject::newParent(GameObject * newparent)
{
	if (newparent != this->parent) {
		if (newparent) {

			std::vector<GameObject*>::iterator it = std::find(childs.begin(), childs.end(), newparent);

			if (it != this->parent->childs.end())
			{
				this->parent->childs.erase(it);
			}	
		}

		this->parent = newparent;

		if (newparent) {
			newparent->childs.push_back(this);
		}
	}
}

void GameObject::AddChild(GameObject * child)
{
	childs.push_back(child);
	child->parent = this;
	//child->newParent(this);
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
		if((*it)->getType()== type)
		{
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

void GameObject::Update(float dt)
{

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		if ((*it)->IsEnable() == true)
		{
			(*it)->Update(dt);
		}
	}

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if((*it)->IsEnable()==true)
		{
			(*it)->OnUpdate(dt);
		}
	}

	App->renderer3D->DrawGO(this);
}

void GameObject::cleanUp()
{

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->cleanUp();
	}
}


