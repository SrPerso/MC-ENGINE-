#include "CTransformation.h"
#include "ModuleDataFile.h"
#include "ImGuizmo\ImGuizmo.h"
#include "imGUI\imgui.h"
#include "CMesh.h"


CTransformation::CTransformation(GameObject * object,int UID, Component_Type type, DTransformation* data) :Component(object, UID, type)
{	
	dataTransformation = (DTransformation*)App->datamanager->CreateNewDataContainer(D_TRANSFORMATION, App->randGen->Int());
	
	if (data)
	{
		dataTransformation = data;
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
	if (dataTransformation->GetUpdateTrans() && object != nullptr)
	{		
		dataTransformation->position = dataTransformation->destiny;
		TransUpdate();
		object->ChangeQuad();
	}

	
}

void CTransformation::OnGuizmo() {

	ImGuizmo::Enable(true);
	

	float* projMatrix = App->camera->editorCam->dataCamera->frustum.ViewProjMatrix().Transposed().ptr();
	float* matrix = dataTransformation->globalTransformMatrix.Transposed().ptr();

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
		ImGuizmo::DecomposeMatrixToComponents(matrix,(float*)dataTransformation->position.ptr(),(float*)dataTransformation->eulerAngles.ptr(),(float*)dataTransformation->scale.ptr());
		dataTransformation->globalTransformMatrix.Transpose();
		ImGuizmo::RecomposeMatrixFromComponents((float*)dataTransformation->position.ptr(), (float*)dataTransformation->eulerAngles.ptr(), (float*)dataTransformation->scale.ptr(), dataTransformation->globalTransformMatrix.ptr());
		dataTransformation->globalTransformMatrix.Transpose();
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

		ImGui::Text("X = %.2f", dataTransformation->destiny.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", dataTransformation->destiny.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", dataTransformation->destiny.z);

		ImGui::Text("Rotation:");

		ImGui::Text("X = %.2f", dataTransformation->eulerAngles.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", dataTransformation->eulerAngles.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", dataTransformation->eulerAngles.z);

		ImGui::Text("Scale:");

		ImGui::Text("X = %.2f", dataTransformation->scale.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", dataTransformation->scale.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", dataTransformation->scale.z);

		ImGui::Checkbox("STATIC", &object->isStatic);
	}
	else {
		ImGui::Text("Position:");

		ImGui::Text("X = %.2f", dataTransformation->destiny.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", dataTransformation->destiny.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", dataTransformation->destiny.z);

		//ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &dataTransformation->destiny.x, -100, 100))
		{

			dataTransformation->SetUpdateTrans(true);

		}

		if (ImGui::SliderFloat("Y", &dataTransformation->destiny.y, -100, 100))
		{

			dataTransformation->SetUpdateTrans(true);

		}

		if (ImGui::SliderFloat("Z", &dataTransformation->destiny.z, -100, 100))
		{

			dataTransformation->SetUpdateTrans(true);

		}

		ImGui::Text("Rotation:");

		ImGui::Text("X = %.2f", dataTransformation->eulerAngles.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", dataTransformation->eulerAngles.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", dataTransformation->eulerAngles.z);

		//ImGui::Text("Rotation:");
		if (ImGui::SliderFloat("RX", &dataTransformation->eulerAngles.x, 0, 360))
		{

			dataTransformation->SetUpdateTrans(true);

		}

		if (ImGui::SliderFloat("RY", &dataTransformation->eulerAngles.y, 0, 360))
		{

			dataTransformation->SetUpdateTrans(true);

		}

		if (ImGui::SliderFloat("RZ", &dataTransformation->eulerAngles.z, 0, 360))
		{

			dataTransformation->SetUpdateTrans(true);

		}

		ImGui::Text("Scale:");

		ImGui::Text("X = %.2f", dataTransformation->scale.x);
		ImGui::SameLine();
		ImGui::Text("  Y = %.2f", dataTransformation->scale.y);
		ImGui::SameLine();
		ImGui::Text("  Z = %.2f", dataTransformation->scale.z);

		if (ImGui::SliderFloat("SX", &	dataTransformation->scale.x, 1, 100))
		{

			dataTransformation->SetUpdateTrans(true);

		}
		if (ImGui::SliderFloat("SY", &dataTransformation->scale.y, 1, 100))
		{

			dataTransformation->SetUpdateTrans(true);

		}
		if (ImGui::SliderFloat("SZ", &dataTransformation->scale.z, 1, 100))
		{
    dataTransformation->SetUpdateTrans(true);
			
		}

		if (ImGui::Checkbox("STATIC", &object->isStatic)) 
		{
			object->ChangeQuad();
		}

	}
	
}

void CTransformation::OnCleanUp()
{
	delete dataTransformation;
}


void CTransformation::OnSave(DataJSON & file) const
{
	file.AddInt("Component UID", UID);

	//file.AddInt("Component Type", Ctype);

	file.AddArrayF("Position", &dataTransformation->position.x, 3);
	file.AddArrayF("Scale", &dataTransformation->scale.x, 3);
	file.AddArrayF("Rotation", &dataTransformation->rotation.x, 4);
}

void CTransformation::OnLoad(DataJSON & file)
{
	UID = file.GetFloat("Component UID");

	dataTransformation->SetUpdateTrans(false);

	dataTransformation->position.x = file.GetFloat("Position", 0);
	dataTransformation->position.y = file.GetFloat("Position", 1);
	dataTransformation->position.z = file.GetFloat("Position", 2);

	dataTransformation->scale.x = file.GetFloat("Scale", 0);
	dataTransformation->scale.y = file.GetFloat("Scale", 1);
	dataTransformation->scale.z = file.GetFloat("Scale", 2);

	dataTransformation->rotation.x = file.GetFloat("Rotation", 0);
	dataTransformation->rotation.y = file.GetFloat("Rotation", 1);
	dataTransformation->rotation.z = file.GetFloat("Rotation", 2);
	dataTransformation->rotation.w = file.GetFloat("Rotation", 3);

	dataTransformation->destiny = dataTransformation->position;
	dataTransformation->eulerAngles = dataTransformation->rotation.ToEulerXYZ();
	dataTransformation->angle = dataTransformation->rotation.Angle();
	dataTransformation->rotation = dataTransformation->rotation;
	dataTransformation->globalTransformMatrix = float4x4::FromQuat(dataTransformation->rotation);
	dataTransformation->globalTransformMatrix = float4x4::Scale(dataTransformation->scale, float3(0, 0, 0)) * dataTransformation->globalTransformMatrix;
	dataTransformation->globalTransformMatrix.float4x4::SetTranslatePart(dataTransformation->position.x, dataTransformation->position.y, dataTransformation->position.z);

	dataTransformation->localTransformMatrix = dataTransformation->globalTransformMatrix;
}



void CTransformation::TransUpdate()
{
	CMesh* mesh = (CMesh*)object->GetComponent(COMP_MESH);
	

	dataTransformation->eulerAngles.x *= DEGTORAD;
	dataTransformation->eulerAngles.y *= DEGTORAD;
	dataTransformation->eulerAngles.z *= DEGTORAD;
	dataTransformation->rotation = Quat::FromEulerXYZ(dataTransformation->eulerAngles.x, dataTransformation->eulerAngles.y, dataTransformation->eulerAngles.z);
	dataTransformation->eulerAngles.x *= RADTODEG;
	dataTransformation->eulerAngles.y *= RADTODEG;
	dataTransformation->eulerAngles.z *= RADTODEG;

	dataTransformation->globalTransformMatrix = float4x4::FromQuat(dataTransformation->rotation);
	dataTransformation->globalTransformMatrix = float4x4::Scale(dataTransformation->scale, float3(0, 0, 0)) * dataTransformation->globalTransformMatrix;
	dataTransformation->globalTransformMatrix.float4x4::SetTranslatePart(dataTransformation->position.x, dataTransformation->position.y, dataTransformation->position.z);

	if (object != nullptr)
	{
		CTransformation* parentTrans = (CTransformation*)object->GetParent()->GetComponent(COMP_TRANSFORMATION);
	
		if (parentTrans != nullptr)
			dataTransformation->globalTransformMatrix = parentTrans->dataTransformation->globalTransformMatrix* dataTransformation->globalTransformMatrix;
		
	}

	object->UpdateTranformChilds();
	dataTransformation->SetUpdateTrans(false);
}

void CTransformation::UpdateTransFromParent(GameObject * parent)
{
	CTransformation* parentTrans = (CTransformation*)parent->GetComponent(COMP_TRANSFORMATION);
	if (parentTrans != nullptr)
	{
		dataTransformation->globalTransformMatrix = parentTrans->dataTransformation->localTransformMatrix * dataTransformation->globalTransformMatrix;
		float4x4 temp;
		dataTransformation->globalTransformMatrix.Decompose(dataTransformation->destiny, temp, dataTransformation->scale);
		dataTransformation->eulerAngles = temp.ToEulerXYZ();
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
			float3 localPos = dataTransformation->position - parentTrans->dataTransformation->position;
			Quat localRot = dataTransformation->rotation * parentTrans->dataTransformation->rotation.Conjugated();
			float3 localScale = dataTransformation->scale.Mul(parentTrans->dataTransformation->scale.Recip());

			dataTransformation->localTransformMatrix = float4x4::FromQuat(localRot);
			dataTransformation->globalTransformMatrix = float4x4::Scale(localScale, float3(0, 0, 0)) * dataTransformation->globalTransformMatrix;
			dataTransformation->globalTransformMatrix.float4x4::SetTranslatePart(localPos.x, localPos.y, localPos.z);
		}
		else
		{
			dataTransformation->localTransformMatrix = dataTransformation->globalTransformMatrix;
		}
	}
	else
	{
		dataTransformation->localTransformMatrix = dataTransformation->globalTransformMatrix;
	}
}

void CTransformation::SetPos(float3 pos)
{
	dataTransformation->position = pos;
}

float4x4 CTransformation::GetTransMatrix()
{
	return dataTransformation->globalTransformMatrix;
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
