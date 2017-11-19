#include "CCamera.h"
#include "Application.h"
#include "ModuleDataFile.h"
#include "ModuleDataManager.h"
#include "DCamera.h"


CCamera::CCamera(int UID, Component_Type type, DCamera * data) :Component(UID,COMP_CAMERA)
{	
	

	if (data)
	{
		dataCamera = data;
		this->dataCamera->aspectRatio = data->aspectRatio;
		this->dataCamera->aspectRatio = (float)16 / 9;
		dataCamera->Active = false;
	}

	dataCamera->FOV = 15;
	dataCamera->frustum.verticalFov = DEGTORAD * dataCamera->FOV;
	dataCamera->frustum.horizontalFov = 2.f * atanf((tanf(dataCamera->frustum.verticalFov * 0.5f)) * (dataCamera->aspectRatio));

	dataCamera->frustum.ProjectionMatrix();
	dType = D_CAMERA;
	//frustumCulling = true;

	name = "Camera";


}

CCamera::CCamera(GameObject * object, int UID, Component_Type type, DCamera * data) :Component(object, UID, COMP_CAMERA)
{

	dataCamera = (DCamera*)App->datamanager->CreateNewDataContainer(D_CAMERA, App->randGen->Int());


	if (data)
	{
		dataCamera->frustum.type = data->frustum.type;
		dataCamera->frustum.pos = data->frustum.pos;
		dataCamera->frustum.front = data->frustum.front;
		dataCamera->frustum.up = data->frustum.up;
		dataCamera->frustum.nearPlaneDistance = data->frustum.nearPlaneDistance;
		dataCamera->frustum.farPlaneDistance = data->frustum.farPlaneDistance;
		dataCamera->Active = false;
	}

	this->dataCamera->aspectRatio = (float)16 / 9;
	
	dataCamera->FOV = 15;
	dataCamera->frustum.verticalFov = DEGTORAD * dataCamera->FOV;
	dataCamera->frustum.horizontalFov = 2.f * atanf((tanf(dataCamera->frustum.verticalFov * 0.5f)) * (dataCamera->aspectRatio));

	dataCamera->frustum.ProjectionMatrix();
	dType = D_CAMERA;
	//frustumCulling = true;

	name = "Camera";

}

CCamera::~CCamera()
{
}


void CCamera::SetPos(float3 newpos, float3 front, float3 up)
{
	dataCamera->frustum.pos = newpos;
	dataCamera->frustum.front = front;
	dataCamera->frustum.up = up;
}

void CCamera::OnUpdate(float dt)
{
	DrawFrustum();
}

void CCamera::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
	
		ImGui::TreePop();

	}
}

void CCamera::OnInspector()
{
	ImGui::Checkbox("CULLING", &dataCamera->needToCull);
	ImGui::Checkbox("ACTIVE", &dataCamera->Active);
	
}

void CCamera::OnCleanUp()
{
}

void CCamera::OnSave(DataJSON & file) const
{
	file.AddInt("Component UID",UID);
	//file.AddInt("Component Type", Ctype);

	file.AddFloat("FOV", dataCamera->FOV);
	file.AddFloat("Aspect Radio", dataCamera->aspectRatio);

	file.AddFloat("Frustum Far", dataCamera->frustum.farPlaneDistance);
	file.AddFloat("Frustum Near", dataCamera->frustum.nearPlaneDistance);
	file.AddFloat("HFOV", dataCamera->frustum.horizontalFov);
	file.AddFloat("VFOV", dataCamera->frustum.verticalFov);
	file.AddArrayF("FrustumPos", dataCamera->frustum.pos.ptr(), 3);
	file.AddArrayF("FrustumUp", dataCamera->frustum.up.ptr(), 3);
	file.AddArrayF("FrustumFront", dataCamera->frustum.front.ptr(), 3);

	file.AddBool("Component Active", dataCamera->Active);
	file.AddBool("needToCull", dataCamera->needToCull);
}

void CCamera::OnLoad(DataJSON & file)
{
	UID = file.GetFloat("Component UID");

	dataCamera->FOV = file.GetFloat("FOV");
	dataCamera->aspectRatio = file.GetFloat("Aspect Radio");

	dataCamera->frustum.farPlaneDistance = file.GetFloat("Frustum Far");
	dataCamera->frustum.nearPlaneDistance = file.GetFloat("Frustum Near");
	dataCamera->frustum.horizontalFov = file.GetFloat("HFOV");
	dataCamera->frustum.verticalFov = file.GetFloat("VFOV");

	dataCamera->frustum.pos.x = file.GetFloat("FrustumPos", 0);
	dataCamera->frustum.pos.y = file.GetFloat("FrustumPos", 1);
	dataCamera->frustum.pos.z = file.GetFloat("FrustumPos", 2);
	dataCamera->frustum.up.x = file.GetFloat("FrustumUp", 0);
	dataCamera->frustum.up.y = file.GetFloat("FrustumUp", 1);
	dataCamera->frustum.up.z = file.GetFloat("FrustumUp", 2);
	dataCamera->frustum.front.x = file.GetFloat("FrustumFront", 0);
	dataCamera->frustum.front.y = file.GetFloat("FrustumFront", 1);
	dataCamera->frustum.front.z = file.GetFloat("FrustumFront", 2);

	dataCamera->Active = file.GetBoolean("Component Active");
	dataCamera->needToCull = file.GetBoolean("needToCull");

}

void CCamera::DrawFrustum()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float3 vertices[8];
	dataCamera->frustum.GetCornerPoints(vertices);

	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);
	

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();

}

bool CCamera::Contains(const AABB & aabb) const
{
	return dataCamera->frustum.Contains(aabb);
}

Frustum CCamera::GetFrustum() const
{
	return dataCamera->frustum;
}

void CCamera::SetFov()
{
	dataCamera->frustum.verticalFov = DEGTORAD * dataCamera->FOV;
	dataCamera->frustum.horizontalFov = 2.f * atanf((tanf(dataCamera->frustum.verticalFov * 0.5f)) * (dataCamera->aspectRatio));

}

const float * CCamera::GetViewMatrix() const
{
	return dataCamera->frustum.ViewProjMatrix().Transposed().ptr();
}
