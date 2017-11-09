#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"
#include <string>
#include "DTexture.h"
#include "ModuleDataFile.h"

class GameObject;


class CTexture : public Component, public DTexture
{
public:
	CTexture(GameObject* object,int UID, Component_Type type = COMP_TEXTURE, DTexture* data = nullptr);
	~CTexture();

	void OnUpdate(float dt) override;
	void OnEditor() override;
	void OnInspector() override;

	void OnSave(DataJSON&file)const override;
	void OnLoad(DataJSON&file) override;

	void SetData(DTexture*data);
	uint Texture_ID; 
};
#endif //_TEXTURE_H