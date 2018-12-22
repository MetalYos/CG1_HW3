#include "Geometry.h"

Geometry::~Geometry()
{
	// free polygons
	while (Polygons.size() > 0)
	{
		Poly* p = Polygons.back();
		Polygons.pop_back();
		delete p;
	}

	// free vertices
	for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
		delete (*it).second;
	Vertices.clear();

	// free edges
	for (auto it = Edges.begin(); it != Edges.end(); ++it)
		delete (*it);
	Edges.clear();

	// free bounding box vertices
	while (BBox.size() > 0)
	{
		Poly* p = BBox.back();
		BBox.pop_back();
		delete p;
	}
}

void Geometry::AddVertex(Vertex* v)
{
	Vertices[v->Pos] = v;
	// Calculate new min and max
	if (Vertices.size() == 1)
	{
		MinCoord = v->Pos;
		MaxCoord = v->Pos;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (v->Pos[i] < MinCoord[i])
				MinCoord[i] = v->Pos[i];
			if (v->Pos[i] > MaxCoord[i])
				MaxCoord[i] = v->Pos[i];
		}
	}
}

void Geometry::AddPolygon(Poly * p)
{
	Polygons.push_back(p);

	// Calculate center
	if (p->Vertices.size() > 0)
	{
		for (Vertex* v : p->Vertices)
		{
			p->Center += v->Pos;
		}
		p->Center /= p->Vertices.size();
	}
	
	// Build and add Edges
	for (unsigned int i = 0; i < p->Vertices.size(); i++)
	{
		PolyEdge* edge = new PolyEdge();
		// Save vertices in object space
		edge->A = p->Vertices[i];
		edge->B = p->Vertices[(i + 1) % p->Vertices.size()];
		edge->Polys.push_back(p);

		auto it = Edges.find(edge);
		if (it == Edges.end())
			Edges.insert(edge);
		else
		{
			(*it)->Polys.push_back(p);
			delete edge;
		}
	}
}

void Geometry::CalculateVerticesNormals()
{
	for (auto it = Vertices.begin(); it != Vertices.end(); ++it)
	{
		Vertex* v = (*it).second;
		
		Vec4 avg;
		for (Poly* p : v->Polys)
		{
			avg += p->CalcNormal;
		}
		avg /= v->Polys.size();
		avg[3] = 0.0;

		if (abs(avg[0]) < AL_DBL_EPSILON && abs(avg[1]) < AL_DBL_EPSILON && abs(avg[2]) < AL_DBL_EPSILON)
			v->CalcNormal = v->Polys.back()->CalcNormal;
		else
			v->CalcNormal = Vec4::Normalize3(avg);
	}
}

void Geometry::BuildBoundingBox()
{
	Vec4 a(MinCoord[0], MaxCoord[1], MaxCoord[2]);
	Vec4 b(MinCoord[0], MinCoord[1], MaxCoord[2]);
	Vec4 c(MaxCoord[0], MinCoord[1], MaxCoord[2]);
	Vec4 d(MaxCoord[0], MaxCoord[1], MaxCoord[2]);
	Vec4 e(MinCoord[0], MaxCoord[1], MinCoord[2]);
	Vec4 f(MinCoord[0], MinCoord[1], MinCoord[2]);
	Vec4 g(MaxCoord[0], MinCoord[1], MinCoord[2]);
	Vec4 h(MaxCoord[0], MaxCoord[1], MinCoord[2]);

	// Top Polygon
	Poly* top = new Poly();
	top->Vertices.push_back(new Vertex(a));
	top->Vertices.push_back(new Vertex(b));
	top->Vertices.push_back(new Vertex(c));
	top->Vertices.push_back(new Vertex(d));

	// Left Polygon
	Poly* left = new Poly();
	left->Vertices.push_back(new Vertex(b));
	left->Vertices.push_back(new Vertex(f));
	left->Vertices.push_back(new Vertex(g));
	left->Vertices.push_back(new Vertex(c));

	// Right Polygon
	Poly* right = new Poly();
	right->Vertices.push_back(new Vertex(a));
	right->Vertices.push_back(new Vertex(e));
	right->Vertices.push_back(new Vertex(h));
	right->Vertices.push_back(new Vertex(d));

	// Front Polygon
	Poly* front = new Poly();
	front->Vertices.push_back(new Vertex(b));
	front->Vertices.push_back(new Vertex(a));
	front->Vertices.push_back(new Vertex(e));
	front->Vertices.push_back(new Vertex(f));

	// Back Polygon
	Poly* back = new Poly();
	back->Vertices.push_back(new Vertex(c));
	back->Vertices.push_back(new Vertex(d));
	back->Vertices.push_back(new Vertex(h));
	back->Vertices.push_back(new Vertex(g));

	// Bottom Polygon
	Poly* bottom = new Poly();
	bottom->Vertices.push_back(new Vertex(e));
	bottom->Vertices.push_back(new Vertex(h));
	bottom->Vertices.push_back(new Vertex(g));
	bottom->Vertices.push_back(new Vertex(f));

	// Add polygons to bbox
	BBox.push_back(top);
	BBox.push_back(left);
	BBox.push_back(right);
	BBox.push_back(front);
	BBox.push_back(back);
	BBox.push_back(bottom);
}
