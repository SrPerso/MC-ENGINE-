#include "CCamera.h"





CCamera::CCamera(Component_Type type, DCamera * data) :Component(COMP_CAMERA)
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

CCamera::CCamera(GameObject * object, Component_Type type, DCamera * data) :Component(object,COMP_CAMERA)
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

const float * CCamera::GetViewMatrix() const
{
	return frustum.ViewProjMatrix().Transposed().ptr();
}
