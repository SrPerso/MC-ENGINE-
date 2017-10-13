#include "Component.h"


Component::Component(GameObject * object, Component_Type type):object(object),type(type)
{

}

Component::~Component()
{
}

Component_Type Component::getType()
{
	return Component_Type(type);
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

void Component::OnUpdate(float dt)
{
}
