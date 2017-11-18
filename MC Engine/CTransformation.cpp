#include "CTransformation.h"
#include "ModuleDataFile.h"
#include "ImGuizmo\ImGuizmo.h"
#include "imGUI\imgui.h"
#include "CMesh.h"


CTransformation::CTransformation(GameObject * object,int UID, Component_Type type, DTransformation* data) :Component(object, UID, type)
{	
	if (data)
	{
			position = data->position;
			scale = data->scale;
			destiny = data->destiny;
			eulerAngles = data->eulerAngles;
			angle = data->angle;
			rotation = data->rotation;
			globalTransformMatrix = data->globalTransformMatrix;
			localTransformMatrix = data->localTransformMatrix;
	}
	if (object != nullptr)
	{
		this->Transformation_ID = object->NumComponentTypeSize(this->Ctype) + 1;
	}
	else
	{
		this->Transformation_ID = 0;
	}
	
	name = "- Component Transformation_";
	name.append(std::to_string(Transformation_ID));
	dType = D_TRANSFORMATION;
}

CTransformation::~CTransformation()
{
}

void CTransformation::OnUpdate(float dt)
{
	if (UpdateTrans && object != nullptr)
	{		
		position = destiny;
		TransUpdate();
		//UpdateTrans = false;		
	}

	
}

void CTransformation::OnGuizmo() {

	ImGuizmo::Enable(true);
	

	float* projMatrix = App->camera->editorCam->frustum.ViewProjMatrix().Transposed().ptr();
	float* matrix = globalTransformMatrix.Transposed().ptr();

	ImGuiIO& io = ImGui::GetIO();


	
	int w, h;
	App->window->GetWidth(w);
	App->window->GetHeight(h);
	ImGuizmo::SetRect(0, 0, (float)w, float(h));

	

	
	if (App->input->GetW() == true)
	{
		ImGuizmo::Manipulate(float4x4::identity.ptr(), projMatrix, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, matrix);
	}
	else if (App->input->GetE() == true)
	{
		ImGuizmo::Manipulate(float4x4::identity.ptr(), projMatrix, ImGuizmo::ROTATE, ImGuizmo::LOCAL, matrix);
	}
	else if (App->input->GetR() == true)
	{
		ImGuizmo::Manipulate(float4x4::identity.ptr(), projMatrix, ImGuizmo::SCALE, ImGuizmo::LOCAL, matrix);
	}
	
	

	

	if (ImGuizmo::IsUsing())
	{
		ImGuizmo::DecomposeMatrixToComponents(matrix,(float*)position.ptr(),(float*)eulerAngles.ptr(),(float*)scale.ptr());
		globalTransformMatrix.Transpose();
		ImGuizmo::RecomposeMatrixFromComponents((float*)position.ptr(), (float*)eulerAngles.ptr(), (float*)scale.ptr(),globalTransformMatrix.ptr());
		globalTransformMatrix.Transpose();
	}
}
void CTransformation::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		
		ImGui::TreePop();
	}
	
	
}

void CTransformation::OnInspector() {

	if (object->isStatic == true) 
	{
		ImGui::Text("Position:");

		ImGui::Text("X = %.2f", destiny.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", destiny.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", destiny.z);

		ImGui::Text("Rotation:");

		ImGui::Text("X = %.2f", eulerAngles.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", eulerAngles.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", eulerAngles.z);

		ImGui::Text("Scale:");

		ImGui::Text("X = %.2f", scale.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", scale.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", scale.z);

		ImGui::Checkbox("STATIC", &object->isStatic);
	}
	else {
		ImGui::Text("Position:");

		ImGui::Text("X = %.2f", destiny.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", destiny.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", destiny.z);

		//ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &destiny.x, -100, 100))
		{
			UpdateTrans = true;
		}

		if (ImGui::SliderFloat("Y", &destiny.y, -100, 100))
		{
			UpdateTrans = true;
		}

		if (ImGui::SliderFloat("Z", &destiny.z, -100, 100))
		{
			UpdateTrans = true;
		}

		ImGui::Text("Rotation:");

		ImGui::Text("X = %.2f", eulerAngles.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", eulerAngles.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", eulerAngles.z);

		//ImGui::Text("Rotation:");
		if (ImGui::SliderFloat("RX", &eulerAngles.x, 0, 360))
		{
			UpdateTrans = true;
		}

		if (ImGui::SliderFloat("RY", &eulerAngles.y, 0, 360))
		{
			UpdateTrans = true;
		}

		if (ImGui::SliderFloat("RZ", &eulerAngles.z, 0, 360))
		{
			UpdateTrans = true;
		}

		ImGui::Text("Scale:");

		ImGui::Text("X = %.2f", scale.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", scale.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", scale.z);

		if (ImGui::SliderFloat("SX", &scale.x, 1, 100))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("SY", &scale.y, 1, 100))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("SZ", &scale.z, 1, 100))
		{
			UpdateTrans = true;
		}

		if (ImGui::Checkbox("STATIC", &object->isStatic)) 
		{
			object->ChangeStatic();
		}

	}
	
}


void CTransformation::OnSave(DataJSON & file) const
{
	file.AddInt("Component UID", UID);

	//file.AddInt("Component Type", Ctype);

	file.AddArrayF("Position", &position.x, 3);
	file.AddArrayF("Scale", &scale.x, 3);
	file.AddArrayF("Rotation", &rotation.x, 4);
}

void CTransformation::OnLoad(DataJSON & file)
{
	UID = file.GetFloat("Component UID");

	UpdateTrans = false;

	position.x = file.GetFloat("Position", 0);
	position.y = file.GetFloat("Position", 1);
	position.z = file.GetFloat("Position", 2);

	scale.x = file.GetFloat("Scale", 0);
	scale.y = file.GetFloat("Scale", 1);
	scale.z = file.GetFloat("Scale", 2);

	rotation.x = file.GetFloat("Rotation", 0);
	rotation.y = file.GetFloat("Rotation", 1);
	rotation.z = file.GetFloat("Rotation", 2);
	rotation.w = file.GetFloat("Rotation", 3);

	destiny = position;
	eulerAngles = rotation.ToEulerXYZ();
	angle = rotation.Angle();
	rotation = rotation;
	globalTransformMatrix = float4x4::FromQuat(rotation);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);

	localTransformMatrix = globalTransformMatrix;
}



void CTransformation::TransUpdate()
{
	CMesh* mesh = (CMesh*)object->GetComponent(COMP_MESH);
	

	

	eulerAngles.x *= DEGTORAD;
	eulerAngles.y *= DEGTORAD;
	eulerAngles.z *= DEGTORAD;
	rotation = Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	eulerAngles.x *= RADTODEG;
	eulerAngles.y *= RADTODEG;
	eulerAngles.z *= RADTODEG;
	globalTransformMatrix = float4x4::FromQuat(rotation);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);


	//mesh->debugBox.TransformAsAABB(GetTransMatrix());
	
	SetLocalTrans(object->GetParent());
	object->UpdateTranformChilds();

	UpdateTrans = false;
}

void CTransformation::UpdateTransFromParent(GameObject * parent)
{
	CTransformation* parentTrans = (CTransformation*)parent->GetComponent(COMP_TRANSFORMATION);
	if (parentTrans != nullptr)
	{
		globalTransformMatrix = parentTrans->localTransformMatrix * globalTransformMatrix;
		float4x4 temp;
		globalTransformMatrix.Decompose(destiny, temp, scale);
		eulerAngles = temp.ToEulerXYZ();
		TransUpdate();
	}
}

void CTransformation::SetLocalTrans(GameObject * parent)
{
	if (parent != nullptr)
	{
		CTransformation* parentTrans = (CTransformation*)parent->GetComponent(COMP_TRANSFORMATION);
		if (parentTrans != nullptr)
		{
			float3 localPos = position - parentTrans->position;
			Quat localRot = rotation * parentTrans->rotation.Conjugated();
			float3 localScale = scale.Mul(parentTrans->scale.Recip());

			localTransformMatrix = float4x4::FromQuat(localRot);
			globalTransformMatrix = float4x4::Scale(localScale, float3(0, 0, 0)) * globalTransformMatrix;
			globalTransformMatrix.float4x4::SetTranslatePart(localPos.x, localPos.y, localPos.z);
		}
		else
		{
			localTransformMatrix = globalTransformMatrix;
		}
	}
	else
	{
		localTransformMatrix = globalTransformMatrix;
	}
}

void CTransformation::SetPos(float3 pos)
{
	position = pos;
}

float4x4 CTransformation::GetTransMatrix()
{
	return globalTransformMatrix;
}


void CTransformation::EditTransform(const CCamera camera)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (ImGui::IsKeyPressed(90))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(69))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(82)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	
	ImGui::InputFloat3("Tr", matrixTranslation, 3);
	ImGui::InputFloat3("Rt", matrixRotation, 3);
	ImGui::InputFloat3("Sc", matrixScale, 3);
	

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	static bool useSnap(false);
	if (ImGui::IsKeyPressed(83))
		useSnap = !useSnap;
	ImGui::Checkbox("", &useSnap);
	ImGui::SameLine();
	
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	
}
