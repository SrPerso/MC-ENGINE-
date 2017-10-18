

#include "GameObject.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CTransformation.h"

GameObject::GameObject(GameObject* parent): parent(parent)
{
	name = "GameObject_";	

	if (parent != nullptr)
	{
		this->GameOIbject_ID = parent->GameOIbject_ID + parent->childs.size() + 1;
	}
	else 
	{
		this->GameOIbject_ID = 0;
	}

	name.append(std::to_string(GameOIbject_ID));

}

GameObject::~GameObject()
{
	while (components.size() != 0)
	{

		delete components.back();
		components.pop_back();
	}
	components.clear();

	while (childs.size()!=0) 
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
	
	ret->SetEnable(this->IsEnable());		
	
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
	this->childs.clear();
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::newParent(GameObject * newparent)
{
	if (newparent != this->parent) 
	{
		if (newparent) 
		{

			std::vector<GameObject*>::iterator it = std::find(childs.begin(), childs.end(), newparent);

			if (it != this->parent->childs.end())
			{
				this->parent->childs.erase(it);
			}	
		}

		this->parent = newparent;

		if (newparent)
		{
			newparent->childs.push_back(this);
		}
	}
}

void GameObject::AddChild(GameObject * child)
{
	childs.push_back(child);
	child->parent = this;
}

GameObject * GameObject::GetFirstChild()
{
	return childs[0];
}

Component * GameObject::CreateComponent(Component_Type type)
{	
	/*
		COMP_UNKNOWN,COMP_MESH,COMP_TEXTURE,COMP_CAMERA,COMP_SOUND
	*/

	Component* ret = nullptr;

	switch (type)
	{
	case COMP_MESH:

		ret = new CMesh(this);
		this->components.push_back(ret);

		break;
	case COMP_TRANSFORMATION:

		ret = new CTransformation(this);
		this->components.push_back(ret);

		break;
	case COMP_TEXTURE:// future implementation materials
	
		ret = new CTexture(this);
		this->components.push_back(ret);
		
		break;
	default://COMP_UNKNOWN

		break;
	}

	
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

uint GameObject::NumComponentTypeSize(Component_Type type)
{
	uint ret = 0;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->getType() == type) 
			ret++;
		
	}

	return uint(ret);
}

void GameObject::DeleteComponent(Component * comp)
{
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it) == comp)
		{
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

void GameObject::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{

		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnEditor();
		}

		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->OnEditor();
		}

		ImGui::TreePop();
	}
}


