
#pragma once
#include "glmath.h"
#include "Color.h"
#include "p2List.h"
enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Capsule,
	Primitive_Cube1,
	Primitive_Cube2
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

class Cube1 : public Primitive
{
public:
	Cube1();
	Cube1(vec3 size);
	void InnerRender() const;
public:
	vec3 size;
	uint my_id = 0;

	
};

class Cube2 : public Primitive
{
public:
	Cube2();
	Cube2(vec3 size);
	void InnerRender() const;
public:
	vec3 size;
	uint my_id_v=0;
	uint my_id=0;
	
	


};


// ============================================
class PrimitiveSphere : public Primitive
{
public:
	PrimitiveSphere();
	PrimitiveSphere(float radius, const int numStacks, const int numSides);
	void InnerRender() const;
public:
	float radius;
	int numStacks;
	int numSides;
};

// ============================================
class PrimitiveCylinder : public Primitive
{
public:
	PrimitiveCylinder();
	PrimitiveCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class PrimitiveLine : public Primitive
{
public:
	PrimitiveLine();
	PrimitiveLine(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class PrimitivePlane : public Primitive
{
public:
	PrimitivePlane();
	PrimitivePlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};


