#pragma once

#include "ALMath.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Vec4Hash
{
public:
	std::size_t operator()(const Vec4& v) const
	{
		std::hash<double> hashVal;
		return hashVal(v[0]);
	}
};

class Vec4Comparer
{
public:
	bool operator()(const Vec4& u, const Vec4& v) const
	{
		return (Vec4::Distance3(u, v) < AL_DBL_EPSILON);
	}
};

struct Poly; // Forward Decleration

struct Vertex
{
	Vec4 Pos;
	Vec4 Normal;
	Vec4 CalcNormal;
	std::vector<Poly*> Polys;

	Vertex() { }
	explicit Vertex(const Vec4& v) : Pos(v) { }
};

struct Poly
{
	std::vector<Vertex*> Vertices;
	Vec4 Normal;
	Vec4 CalcNormal;
	Vec4 Center;
	bool IsSelected;
	// TODO: if needed, add a vector of Edges

	Poly() : IsSelected(false) {}
};

struct PolyEdge
{
	Vertex* A;
	Vertex* B;

	std::vector<Poly*> Polys;
};

class PolyEdgeComparer
{
public:
	bool operator()(PolyEdge const* u, PolyEdge const* v) const
	{
		/*/
		bool result1 = false;
		result1 = Vec4::Distance3(u->A->Pos, v->A->Pos) < AL_DBL_EPSILON;
		result1 = result1 && Vec4::Distance3(u->B->Pos, v->B->Pos) < AL_DBL_EPSILON;

		bool result2 = false;
		result2 = Vec4::Distance3(u->A->Pos, v->B->Pos) < AL_DBL_EPSILON;
		result2 = result2 && Vec4::Distance3(u->B->Pos, v->A->Pos) < AL_DBL_EPSILON;

		return result1 || result2;
		*/

		bool result = ((u->A->Pos == v->A->Pos) && (u->B->Pos == v->B->Pos)) || 
			((u->A->Pos == v->B->Pos) && (u->B->Pos == v->A->Pos));
		return result;
	}
};

class PolyEdgeHash
{
public:
	std::size_t operator()(const PolyEdge* e) const
	{
		std::hash<double> hashVal;
		Vec4 sum = e->A->Pos + e->B->Pos;
		return hashVal(sum[0] + sum[1] + sum[2]);
	}
};

class Geometry
{
public:
	std::unordered_map<Vec4, Vertex*, Vec4Hash, Vec4Comparer> Vertices;
	std::unordered_set<PolyEdge*, PolyEdgeHash, PolyEdgeComparer> Edges;
	std::vector<Poly*> Polygons;

	Vec4 Color;

	// BBox parameters
	std::vector<Poly*> BBox;
	Vec4 MinCoord;
	Vec4 MaxCoord;

public:
	Geometry() : Color(AL_WHITE) { }
	~Geometry();

	void AddVertex(Vertex* v);
	void AddPolygon(Poly* p);
	void CalculateVerticesNormals();

	void BuildBoundingBox();
};

