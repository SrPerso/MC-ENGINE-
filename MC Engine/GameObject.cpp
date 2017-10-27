

#include "GameObject.h"

#include "Application.h"
#include "ModuleRenderer3D.h"

#include "CMesh.h"
#include "CTexture.h"
#include "CTransformation.h"
#include "CCamera.h"

GameObject::GameObject()
{
	parent = App->goManager->GetRoot();

	name = "GameObject_";


	if (parent != nullptr)
	{
		this->GameOIbject_ID = parent->GameOIbject_ID + parent->childs.size() + 1;
		parent->AddChild(this);
	}
	else
	{
		this->GameOIbject_ID = 0;
	}

	name.append(std::to_string(GameOIbject_ID));
}

GameObject::GameObject(GameObject* parent): parent(parent)
{
	name = "GameObject_";	


	if (parent != nullptr)
	{
		this->GameOIbject_ID = parent->GameOIbject_ID + parent->childs.size() + 1;
		parent->AddChild(this);
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
	
	//childs.push_back(ret);

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
	child->SetParentID(this->GameOIbject_ID);
	child->parent = this;
	
}

GameObject * GameObject::GetFirstChild()
{
	return childs[0];
}

void GameObject::SetParentID(uint parentID)
{
	Parent_ID = parentID;
}

uint GameObject::GetParentId()
{
	return uint(Parent_ID);
}

void GameObject::SetGOID(uint newID)
{
	GameOIbject_ID = newID;
}

int GameObject::GetGOId()
{
	return int(GameOIbject_ID);
}

Component * GameObject::CreateComponent(Component_Type type, const void*buffer)
{	
	/*
		COMP_UNKNOWN,COMP_MESH,COMP_TEXTURE,COMP_CAMERA,COMP_SOUND
	*/

	Component* ret = nullptr;

	switch (type)
	{
	case COMP_MESH:

		ret = new CMesh(this, COMP_MESH,(DMesh*)buffer);
		this->components.push_back(ret);

		break;
	case COMP_TRANSFORMATION:

		ret = new CTransformation(this,COMP_TRANSFORMATION, (DTransformation*)buffer);
		this->components.push_back(ret);

		break;
	case COMP_TEXTURE:
	
		ret = new CTexture(this,COMP_TEXTURE, (DTexture*)buffer);
		this->components.push_back(ret);
		
		break;
	case COMP_CAMERA:
		//DCamera* camera = new DCamera();
		ret = new CCamera(this, COMP_CAMERA, (DCamera*)buffer);
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

void GameObject::UpdateTranformChilds()
{
	for (int i = 0; i < childs.size(); i++)
	{
		CTransformation* tmp = (CTransformation*)childs[i]->GetComponent(COMP_TRANSFORMATION);
		if (tmp != nullptr)
		{
			tmp->UpdateTransFromParent(this);
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

bool GameObject::IsStatic() const
{
	return isStatic;
}

void GameObject::SetStatic()
{
	isStatic = true;
}

void GameObject::SetNoStatic()
{
	isStatic = false;
}

void GameObject::Update(float dt)
{

	CCamera* camera = nullptr;
	for (int i = 0; i < App->goManager->GetRoot()->childs.size(); i++) 
	{
		for (int p = 0; p < App->goManager->GetRoot()->childs[i]->components.size(); p++) 
		{
			if(App->goManager->GetRoot()->childs[i]->components[p]->getType()==COMP_CAMERA)
			{
				if (App->goManager->GetRoot()->childs[i]->components[p] != nullptr) 
				{
					camera = (CCamera*)App->goManager->GetRoot()->childs[i]->components[p];
				}
			}
		}
	}	
	
	CMesh* debuger = (CMesh*)this->GetComponent(COMP_MESH);
	if (debuger != nullptr && camera!=nullptr)
	{		
		if (camera->needToCull) {
			recalculatedBox = debuger->debugBox;
			if (camera->Contains(recalculatedBox))
			{
				App->renderer3D->DrawGO(this);
			}
		}
		else {
			App->renderer3D->DrawGO(this);
		}
	}

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
	{
		if ((*it)->IsEnable() == true)
		{
			(*it)->Update(dt);
		}
	}

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		if ((*it)->IsEnable() == true)
		{
			(*it)->OnUpdate(dt);
		}
	}
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
			if (App->ui->show_Inspector_window = true) {
				App->ui->show_Inspector_window = false;
			}
			
			App->ui->ShowInspectorWindow(components[i]);
		}

		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->OnEditor();
		}

		ImGui::TreePop();
	}
}

void GameObject::OnInspector()
{	
	/*
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnInspector();
		}

		/*for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->OnInspector();
		}*/	
}

void GameObject::SaveData()
{
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->SaveData();
	}

	if (components.size()>0)
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->GetDataType() == D_MESH) {
				App->datamanager->SaveData(components[i]->GetData(), components[i]->GetDataType(), this->GameOIbject_ID);
			}			
		}
	}
}


/*
std::vector<const void*>* GameObject::SaveData() //make  (?)
{

	std::vector<const void*>* dataToSave = nullptr;

	dataToSave->push_back(this->GetName());

	for (int i = 0; i < childs.size(); i++)
	{
		dataToSave->push_back(childs[i]->SaveData());
	}

	if(components.size()>0)
	{
		for (int i = 0; i < components.size(); i++)
		{	
			dataToSave->push_back(components[i]->GetData());		
		}
		return dataToSave;	
	}	
	
	return dataToSave;
}
*/


void GameObject::Move(float3 destiny, float3 position)
{
	for (int i = 0; i < childs.size(); i++)
	{
		if (childs.size() <= 0)
			continue;

		for (int o = 0; o < childs[i]->components.size(); o++)
		{

			if (childs[i]->components[o]->getType() == COMP_MESH)
			{
				dynamic_cast<CMesh*>(childs[i]->components[o])->Move(destiny, position);
			}
		}

	}
}

/*void GameObject::Rotate(vec3 rotation)
{
	/*for (int i = 0; i < childs.size(); i++)
	{
		if (childs.size() > 0) {
			for (int o = 0; o < childs[i]->components.size(); o++) {

				if (childs[i]->components[o]->getType() == COMP_TRANSFORMATION)
				{
					dynamic_cast<CTransformation*>(childs[i]->components[o])->Rotate(rotation);
				}
			}
		}
	}
	
}*/

