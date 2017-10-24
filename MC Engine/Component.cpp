#include "Component.h"


#include "Application.h"
Component::Component(GameObject * object, Component_Type type):object(object), Ctype(type)
{

}

Component::~Component()
{
}

Component_Type Component::getType()
{
	return Component_Type(Ctype);
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

const void * Component::GetData()
{
	return nullptr;
}
