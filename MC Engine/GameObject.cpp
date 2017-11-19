

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
	SetGOUID(App->randGen->Int());

	if (parent != nullptr)
	{
		this->GameOIbject_ID = parent->GameOIbject_ID + parent->childs.size() + 1;
		parent->AddChild(this);
		//	SetParentUID(parent->GetGOUId());
	}
	else
	{
		SetParentUID(0);
		parent->AddChild(this);
	}

	name.append(std::to_string(GameOIbject_ID));
}

GameObject::GameObject(int  exem)
{
	name = "GameObject_";
}

GameObject::GameObject(GameObject* parent): parent(parent)
{
	name = "GameObject_";	
	
	SetGOUID(App->randGen->Int());
	
	if (parent != nullptr)
	{
		this->GameOIbject_ID = parent->GameOIbject_ID + parent->childs.size() + 1;
		parent->AddChild(this);
	//	SetParentUID(parent->GetGOUId());
	}
	else 
	{
		
		SetParentUID(0);
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


	if (newName)
	{
		name = newName;
	}
	else
	{
		name = "GameObject_";
		name.append(std::to_string(GameOIbject_ID));
	}

	return ret;
}

GameObject * GameObject::CreateChild()
{
	GameObject* ret = nullptr;

	ret = new GameObject(this);	
	
	ret->SetEnable(this->IsEnable());			

	return ret;
}

void GameObject::DeleteChild(GameObject * objectToDelete)
{
	if (objectToDelete != nullptr)
	{
		std::vector<GameObject*>::iterator it = std::find(childs.begin(), childs.end(), objectToDelete);
	
		if (it != childs.end()) 
		{
			childs.erase(it);
		}		
	}
}

void GameObject::DeleteChilds()
{
	while (!childs.empty())
	{
		delete childs.back();
		childs.back() = nullptr;

		childs.pop_back();
	}
	childs.clear();
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
			newparent->childs.push_back(this);

	}
}

void GameObject::AddChild(GameObject * child)
{
	childs.push_back(child);
	child->SetParentID(this->GameOIbject_ID);
	child->SetParentUID(this->GameOIbject_UID);
	child->parent = this;
	
}

GameObject * GameObject::GetFirstChild()const
{
	return childs[0];
}

GameObject * GameObject::FindGameObject(int UID)
{
	if (UID == GameOIbject_UID)
		return this;

	GameObject* ret = nullptr;

	for (int i = 0; i < childs.size() && ret == nullptr; i++)
		ret = childs[i]->FindGameObject(UID);

	return ret;
}

void GameObject::SetParentID(uint parentID)
{
	Parent_ID = parentID;
}

uint GameObject::GetParentId()const
{
	return uint(Parent_ID);
}

void GameObject::SetGOID(uint newID)
{
	GameOIbject_ID = newID;
}

int GameObject::GetGOId()const
{
	return int(GameOIbject_ID);
}

void GameObject::SetParentUID(int parentUID)
{
	Parent_UID = parentUID;
}

int GameObject::GetParentUID() const
{
	return Parent_UID;
}

void GameObject::SetGOUID(int UID)
{
	GameOIbject_UID = UID;
}

int GameObject::GetGOUId() const
{
	return GameOIbject_UID;
}

Component * GameObject::CreateComponent(Component_Type type, int UID, const void*buffer)
{	
	/*
		COMP_UNKNOWN,COMP_MESH,COMP_TEXTURE,COMP_CAMERA,COMP_SOUND
	*/

	//UID
	int newUID;

	if (UID == -1) //is new  
		newUID = App->randGen->Int();
	else
		newUID = UID; // on deseralize
	
	// creator
	Component* ret = nullptr;

	switch (type)
	{
	case COMP_MESH:

		ret = new CMesh(this, newUID, COMP_MESH,(DMesh*)buffer);
		this->components.push_back(ret);

		break;
	case COMP_TRANSFORMATION:

		ret = new CTransformation(this, newUID, COMP_TRANSFORMATION, (DTransformation*)buffer);
		this->components.push_back(ret);

		break;
	case COMP_TEXTURE:
	
		ret = new CTexture(this, newUID, COMP_TEXTURE, (DTexture*)buffer);
		this->components.push_back(ret);
		
		break;
	case COMP_CAMERA:
		//DCamera* camera = new DCamera();
		ret = new CCamera(this, newUID, COMP_CAMERA, (DCamera*)buffer);
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

uint GameObject::ComponentVectorSize()const
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

void GameObject::AddComponent(Component * comp)
{
	if (comp != nullptr)
	{
		components.push_back(comp);
		comp->SetObjectParent(this);
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

void GameObject::SetLocalTransform()
{
	CTransformation* myTrans = (CTransformation*)GetComponent(COMP_TRANSFORMATION);
	if (myTrans != nullptr)
	{
		myTrans->SetLocalTrans(GetParent());
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


void GameObject::ChangeStatic()
{
	if (isStatic)
	{
		App->scene_intro->AddQuadTree(this);
	}
	else
	{
		App->scene_intro->recalculate = true;
		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->isStatic = false;
			childs[i]->ChangeStatic();
		}
	}
}


void GameObject::ChangeQuad() 
{

	App->scene_intro->SetNewQuad();
	
}
void GameObject::InsertQuadTree()
{
	if (isStatic)
	{
		App->scene_intro->AddQuadTree(this);
	}
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->InsertQuadTree();
	}
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
		if ((*it)->IsEnable() == true)
		{
			(*it)->OnUpdate(dt);
		}
	}

	CCamera* camera = nullptr;
	for (int i = 0; i < App->goManager->GetRoot()->childs.size(); i++)
	{
		for (int p = 0; p < App->goManager->GetRoot()->childs[i]->components.size(); p++)	
		{
			if (App->goManager->GetRoot()->childs[i]->components[p]->getType() == COMP_CAMERA)
			{
				if (App->goManager->GetRoot()->childs[i]->components[p] != nullptr)
					camera = (CCamera*)App->goManager->GetRoot()->childs[i]->components[p];
			}
		}
	}

	CMesh* debuger = (CMesh*)this->GetComponent(COMP_MESH);
	CTransformation* transform = (CTransformation*)GetComponent(COMP_TRANSFORMATION);

	if (debuger != nullptr && transform !=nullptr)
	{
		//recalculatedBox = debuger->debugBox;

		if (camera != nullptr)
		{
			if (camera->needToCull) 
			{
				AABB recalculatedBox = debuger->debugBox;

				recalculatedBox.TransformAsAABB(transform->GetTransMatrix());				

				if (camera->Contains(recalculatedBox))
					App->renderer3D->DrawGO(this);
			}

			else 
				App->renderer3D->DrawGO(this);
		
		}
		else
			App->renderer3D->DrawGO(this);
		
	}
}

void GameObject::cleanUp()
{

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		(*it)->cleanUp();

}

void GameObject::OnEditor()
{
	if (strcmp(this->GetName(), "SceneRoot") == 0)
	{
		for (int i = 0; i < childs.size(); i++)
			childs[i]->OnEditor();
		return;
	}

	CCamera* camTry = (CCamera*)GetComponent(COMP_CAMERA);
	
	ImGuiTreeNodeFlags flagVec = 0;

	if (selecting)
	{
		flagVec |= ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx(this, flagVec, name.c_str()))
	{
	
		App->ui->show_Inspector_window = false;
		CMesh* meshTry = (CMesh*)GetComponent(COMP_MESH);
		CCamera* camTry = (CCamera*)GetComponent(COMP_CAMERA);
		if (meshTry != nullptr)
		{
			if (selecting == false) {

					App->ui->show_Inspector_window = false;
					App->scene_intro->ObjectSelected(this);		
			
			}


		}
		else if (camTry != nullptr)
		{
			App->ui->ShowInspectorWindow(this->components[0]);
		}

		for (int i = 0; i < childs.size(); i++)
		{

			childs[i]->OnEditor();
		}

		ImGui::TreePop();
	}
}

void GameObject::OnSelection()
{
	CTransformation* transform = (CTransformation*)GetComponent(COMP_TRANSFORMATION);

	App->ui->show_Inspector_window = true;
	
	for (int i = 0; i < components.size(); i++)
		App->ui->ShowInspectorWindow(components[i]);		
	
	if (isStatic == false) 
	{
		transform->OnGuizmo();
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

void GameObject::OnSerialize(DataJSON & file) const
{
	if (this != App->goManager->GetRoot())
	{
		DataJSON dataOnFile;

		dataOnFile.AddString("Name", name.c_str());

		// --  save UID 
		dataOnFile.AddInt("UID", GetGOUId());
		
		dataOnFile.AddInt("Parent UID", GetParentUID());
				
		// --	components info
		dataOnFile.AddArray("Components");	

		for (int i = 0; i < components.size(); i++)
		{
			DataJSON componentData;
			componentData.AddInt("Type", components[i]->getType());
			components[i]->OnSave(componentData);
			dataOnFile.AddArray(componentData);
		}

		// -- adds the array formed by all components
		file.AddArray(dataOnFile);
	}

	// call all childs to serialize
	for (int i = 0; i < childs.size(); i++) //rec
	{
		childs[i]->OnSerialize(file);
	}
}

void GameObject::OnDeserialize(DataJSON & file)
{
	// basic data
	SetName(file.GetString("Name"));
	SetGOUID(file.GetInt("UID"));
	SetParentUID(file.GetInt("Parent UID"));

	// assign parent

	if (GetParentUID() != 0)
	{
		GameObject* parent = App->goManager->GetRoot()->FindGameObject(GetParentUID());

		if (parent != nullptr)
		{
			parent->AddChild(this);
			parent->SetGOUID(file.GetInt("Parent UID"));
		}
	}
	else
		App->goManager->GetRoot()->AddChild(this);
	

	// Create components

	int nComponents = file.GetArrayLenght("Components"); 

	for (uint i = 0; i < nComponents; ++i)
	{
		DataJSON componentConfig(file.GetArray("Components", i));
	
		int cType = componentConfig.GetInt("Type");
		int componentUID = componentConfig.GetInt("Component UID");

		switch (cType)
		{
			case 1: //CMesh
			{
				CMesh*  cMesh = new CMesh(this, componentUID);
				cMesh->OnLoad(componentConfig);
				this->AddComponent(cMesh);

				break;
			}
			case 2://CTexture
			{
				CTexture*  cTexture = new CTexture(this, componentUID);
				cTexture->OnLoad(componentConfig);
				this->AddComponent(cTexture);
				break;
			}

			case 3: //CCamera
			{
				CCamera*  cCamera = new CCamera(this, componentUID);
				cCamera->OnLoad(componentConfig);
				this->AddComponent(cCamera);

				break;
			}
			case 4: //CTransformation
			{
				CTransformation*  cTransformation = new CTransformation(this, componentUID);
				cTransformation->OnLoad(componentConfig);
				this->AddComponent(cTransformation);
				break;
			}

			default: // unknown
			{
				LOGUI(" [ERROR]{Deserialize}- Unknown component type");
				break;
			}
		}//switch

	}//for
}

void GameObject::SaveData()
{
	//iterate childs
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->SaveData();
	}

	//components it

	if (components.size()>0)
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->GetDataType() == D_MESH) 
			{
				App->datamanager->SaveData(components[i]->GetData(), components[i]->GetDataType(), this->GameOIbject_ID);
			}			
		}
	}
}


void  GameObject::IntersectAABB(LineSegment &picking, std::vector<GameObject*>& DistanceList)
{

	GameObject* Closest = nullptr;

				LineSegment newSegment(picking);
				CMesh* IntersectMesh = (CMesh*)GetComponent(COMP_MESH);
				CTransformation* IntersectTransform = (CTransformation*)GetComponent(COMP_TRANSFORMATION);
				if (IntersectTransform != nullptr) {

					newSegment.Transform(IntersectTransform->GetTransMatrix().Inverted());
				}

				if (IntersectMesh != nullptr)
				{
					if (newSegment.Intersects(IntersectMesh->debugBox) == true)
					{
						DistanceList.push_back(this);


					}
				}
				for (int i = 0; i < childs.size(); i++)
				{
					childs[i]->IntersectAABB(picking, DistanceList);
				}
}
void GameObject::TriIntersection(LineSegment & line, float & distance, float3 & hitPoint)
{
	CMesh* mesh = (CMesh*)GetComponent(COMP_MESH);

	if (mesh != nullptr)
	{
		mesh->IntersectTriangle(line, distance, hitPoint);
	}
}



