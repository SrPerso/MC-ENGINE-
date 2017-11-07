#include "CCamera.h"
#include "ModuleDataFile.h"

CCamera::CCamera(Component_Type type, DCamera * data) :Component(UID, COMP_CAMERA)
{

	name = "Camera";
	this->aspectRatio = aspectRatio;
	this->aspectRatio = (float)16 / 9;
	frustum.type = data->frustum.type;
	frustum.pos = data->frustum.pos;
	frustum.front = data->frustum.front;
	frustum.up = data->frustum.up;
	frustum.nearPlaneDistance = data->frustum.nearPlaneDistance;
	frustum.farPlaneDistance = data->frustum.farPlaneDistance;
	FOV = 15;
	frustum.verticalFov = DEGTORAD * FOV;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspectRatio));

	frustum.ProjectionMatrix();
	dType = D_CAMERA;
	//frustumCulling = true;

}



CCamera::CCamera(int UID, Component_Type type, DCamera * data) :Component(UID,COMP_CAMERA)
{
	
		name = "Camera";
		this->aspectRatio = aspectRatio;
		this->aspectRatio = (float)16 / 9;
		frustum.type = data->frustum.type;
		frustum.pos = data->frustum.pos;
		frustum.front = data->frustum.front;
		frustum.up = data->frustum.up;
		frustum.nearPlaneDistance = data->frustum.nearPlaneDistance;
		frustum.farPlaneDistance = data->frustum.farPlaneDistance;
		FOV = 15;
		frustum.verticalFov = DEGTORAD * FOV;
		frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspectRatio));

		frustum.ProjectionMatrix();
		dType = D_CAMERA;
		//frustumCulling = true;

}

CCamera::CCamera(GameObject * object, int UID, Component_Type type, DCamera * data) :Component(object, UID, COMP_CAMERA)
{

	name = "Camera";
	this->aspectRatio = aspectRatio;
	this->aspectRatio = (float)16 / 9;
	frustum.type = data->frustum.type;
	frustum.pos = data->frustum.pos;
	frustum.front = data->frustum.front;
	frustum.up = data->frustum.up;
	frustum.nearPlaneDistance = data->frustum.nearPlaneDistance;
	frustum.farPlaneDistance = data->frustum.farPlaneDistance;
	FOV = 15;
	frustum.verticalFov = DEGTORAD * FOV;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspectRatio));

	frustum.ProjectionMatrix();
	dType = D_CAMERA;
	//frustumCulling = true;
}

CCamera::~CCamera()
{
}


void CCamera::SetPos(float3 newpos, float3 front, float3 up)
{
	frustum.pos = newpos;
	frustum.front = front;
	frustum.up = up;
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
	
		ImGui::Text("Position:");
		ImGui::SliderFloat("X", &frustum.pos.x, -100, 100);
		ImGui::SliderFloat("Y", &frustum.pos.y, -100, 100);
		ImGui::SliderFloat("Z", &frustum.pos.z, -100, 100);



		ImGui::Checkbox("CULLING", &needToCull);
	
}

void CCamera::OnSave(DataJSON & file) const
{
	file.AddInt("Component UID", UID);
	file.AddInt("Component Type", Ctype);

	file.AddFloat("FOV", FOV);
	file.AddFloat("Aspect Radio", aspectRatio);

	file.AddFloat("Frustum Far", frustum.farPlaneDistance);
	file.AddFloat("Frustum Near", frustum.nearPlaneDistance);
	file.AddFloat("HFOV", frustum.horizontalFov);
	file.AddFloat("VFOV", frustum.verticalFov);
	file.AddArrayF("Frustum Position", frustum.pos.ptr(), 3);
	file.AddArrayF("Frustum Up", frustum.up.ptr(), 3);

	file.AddBool("Component Active", Active);
	file.AddBool("needToCull", needToCull);
}

void CCamera::OnLoad(DataJSON & file)
{
	UID = file.GetFloat("Component UID");

	FOV = file.GetFloat("FOV");
	aspectRatio = file.GetFloat("Aspect Radio");

	frustum.farPlaneDistance = file.GetFloat("Frustum Far");
	frustum.nearPlaneDistance = file.GetFloat("Frustum Near");
	frustum.horizontalFov = file.GetFloat("HFOV");
	frustum.verticalFov = file.GetFloat("VFOV");

	frustum.pos.x = file.GetFloat("Frustum Pos", 0);
	frustum.pos.y = file.GetFloat("Frustum Pos", 1);
	frustum.pos.z = file.GetFloat("Frustum Pos", 2);
	frustum.up.x = file.GetFloat("Frustum Up", 0);
	frustum.up.y = file.GetFloat("Frustum Up", 1);
	frustum.up.z = file.GetFloat("Frustum Up", 2);
	frustum.front.x = file.GetFloat("Frustum Front", 0);
	frustum.front.y = file.GetFloat("Frustum Front", 1);
	frustum.front.z = file.GetFloat("Frustum Front", 2);

	Active = file.GetBoolean("Component Active");
	needToCull = file.GetBoolean("needToCull");

}

void CCamera::DrawFrustum()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float3 vertices[8];
	frustum.GetCornerPoints(vertices);

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
	return frustum.Contains(aabb);
}

Frustum CCamera::GetFrustum() const
{
	return frustum;
}

void CCamera::SetFov()
{
	frustum.verticalFov = DEGTORAD * FOV;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspectRatio));

}

const float * CCamera::GetViewMatrix() const
{
	return frustum.ViewProjMatrix().Transposed().ptr();
}
