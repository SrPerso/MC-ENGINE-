#include "CCamera.h"



CCamera::CCamera(GameObject* object, Component_Type type, DCamera * data) :Component(object,COMP_CAMERA)
{
	name = "Camera";
	/*this->aspectRatio = aspectRatio;
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

	frustumCulling = true;*/
}

CCamera::~CCamera()
{
}

void CCamera::SetPos(float3 newPos)
{
	frustum.pos = newPos;
}

void CCamera::OnUpdate(float dt)
{
	DrawFrustum();
}

void CCamera::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		ImGui::SliderFloat("X", &frustum.pos.x, -50, 50);
		ImGui::SliderFloat("Y", &frustum.pos.y, -50, 50);
		ImGui::SliderFloat("Z", &frustum.pos.z, -50, 50);
		ImGui::TreePop();
	}
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