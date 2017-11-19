#include "Component.h"


#include "Application.h"
Component::Component(GameObject * object,int UID, Component_Type type):object(object), Ctype(type), UID(UID)
{

}

Component::Component(int UID, Component_Type type): Ctype(type), UID(UID)
{
}

Component::~Component()
{
}

Component_Type Component::getType()const
{
	return Component_Type(Ctype);
}

DType Component::GetDataType()const
{
	return DType(dType);
}

bool Component::IsEnable() const
{
	return isEnable;
}

void Component::SetEnable(bool isEnable)
{
	isEnable = isEnable;
}

void Component::Enable()
{
	isEnable = true;
}

void Component::Disable()
{
	isEnable = false;
}

void Component::OnEditor()
{
}

void Component::OnInspector()
{
}

void Component::OnUpdate(float dt)
{
}

void Component::SetObjectParent(GameObject *obj)
{
	object = obj;
}

const void * Component::GetData()
{
	return nullptr;
}

void Component::SetName(std::string name)
{
	this->name = name;

}
