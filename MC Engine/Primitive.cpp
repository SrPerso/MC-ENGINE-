
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"


#pragma comment (lib, "glut/glut32.lib")

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
}

Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
}

void Cube::InnerRender() const
{	
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f( sx, -sy, sz);
	glVertex3f( sx,  sy, sz);
	glVertex3f(-sx,  sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f( sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx,  sy, -sz);
	glVertex3f( sx,  sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy,  sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx,  sy, -sz);
	glVertex3f(sx,  sy,  sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy,  sz);
	glVertex3f(-sx,  sy,  sz);
	glVertex3f(-sx,  sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy,  sz);
	glVertex3f( sx, sy,  sz);
	glVertex3f( sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f( sx, -sy, -sz);
	glVertex3f( sx, -sy,  sz);
	glVertex3f(-sx, -sy,  sz);

	glEnd();
}

// SPHERE ============================================
PrimitiveSphere::PrimitiveSphere() : Primitive(), radius(1.0f), numStacks(0), numSides(0)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

PrimitiveSphere::PrimitiveSphere(float radius,  int numStacks, int numSides) : Primitive(), radius(radius), numStacks(numStacks),numSides(numSides)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void PrimitiveSphere::InnerRender() const
{

	GLfloat curRadius, curTheta, curRho, deltaTheta, deltaRho, curX, curY, curZ;
	int curStack, curSlice ;
	int numVerts = (numStacks -1)*numSides;
	vec3* points = new vec3[numVerts];
	int curVert = 0;
	int t;

	deltaTheta = (2 * M_PI) / numSides;
	deltaRho = M_PI / numStacks;

	for (curStack = 1; curStack<numStacks; curStack++)
	{
		curRho = (3.141 / 2.0) - curStack*deltaRho;
		curY = sin(curRho) * radius;
		curRadius = cos(curRho) * radius;
		for (curSlice = 0; curSlice<numSides; curSlice++)
		{
			curTheta = curSlice * deltaTheta;
			curX = curRadius * cos(curTheta);
			curZ = -curRadius * sin(curTheta);
			points[curVert++] = vec3{ curX,curY,curZ };
		}
	}
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, 1, 0);
	glVertex3d(0, radius, 0);
	for (t = 0; t<numSides; t++)
	{
		curX = points[t].x;
		curY = points[t].y;
		curZ = points[t].z;
		glNormal3d(curX, curY, curZ);
		glVertex3d(curX, curY, curZ);
	}
	curX = points[0].x;
	curY = points[0].y;
	curZ = points[0].z;
	glNormal3d(curX, curY, curZ);
	glVertex3d(curX, curY, curZ);
	glEnd();

	// part B - draw the 'sides' (quads)
	int vertIndex;
	for (curStack = 0; curStack<numStacks - 2; curStack++)
	{
		vertIndex = curStack * numSides;
		glBegin(GL_QUAD_STRIP);
		for (curSlice = 0; curSlice<numSides; curSlice++)
		{
			glNormal3d(points[vertIndex + curSlice].x, points[vertIndex + curSlice].y, points[vertIndex + curSlice].z);
			glVertex3d(points[vertIndex + curSlice].x, points[vertIndex + curSlice].y, points[vertIndex + curSlice].z);

			glNormal3d(points[vertIndex + numSides + curSlice].x, points[vertIndex + numSides + curSlice].y, points[vertIndex + numSides + curSlice].z);
			glVertex3d(points[vertIndex + numSides + curSlice].x, points[vertIndex + numSides + curSlice].y, points[vertIndex + numSides + curSlice].z);
		}
		glNormal3d(points[vertIndex].x, points[vertIndex].y, points[vertIndex].z);
		glVertex3d(points[vertIndex].x, points[vertIndex].y, points[vertIndex].z);
		glNormal3d(points[vertIndex + numSides].x, points[vertIndex + numSides].y, points[vertIndex + numSides].z);
		glVertex3d(points[vertIndex + numSides].x, points[vertIndex + numSides].y, points[vertIndex + numSides].z);
		glEnd();
	}

	// part C - draw the bottom 'lid' (tris)
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, -1, 0);
	glVertex3d(0, -radius, 0);
	for (t = 0; t<numSides - 1; t++)
	{
		curX = points[numVerts - 1 - t].x;
		curY = points[numVerts - 1 - t].y;
		curZ = points[numVerts - 1 - t].z;
		glNormal3d(curX, curY, curZ);
		glVertex3d(curX, curY, curZ);
	}
	curX = points[numVerts - 1].x;
	curY = points[numVerts - 1].y;
	curZ = points[numVerts - 1].z;
	glNormal3d(curX, curY, curZ);
	glVertex3d(curX, curY, curZ);
	glEnd();

	
}


// CYLINDER ============================================
PrimitiveCylinder::PrimitiveCylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

PrimitiveCylinder::PrimitiveCylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void PrimitiveCylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
PrimitiveLine::PrimitiveLine() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

PrimitiveLine::PrimitiveLine(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void PrimitiveLine::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
PrimitivePlane::PrimitivePlane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

PrimitivePlane::PrimitivePlane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void PrimitivePlane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}


