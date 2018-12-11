#include "Model.h"

Model::~Model()
{
	DeleteGeometries();
}

void Model::AddGeometry(Geometry * geo)
{
	geos.push_back(geo);

	// Calculate Min and Max coords
	if (geos.size() == 1)
	{
		minCoord = geo->MinCoord;
		maxCoord = geo->MaxCoord;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (geo->MinCoord[i] < minCoord[i])
				minCoord[i] = geo->MinCoord[i];
			if (geo->MaxCoord[i] > maxCoord[i])
				maxCoord[i] = geo->MaxCoord[i];
		}
	}
}

const Mat4& Model::GetTransform() const
{
	return transform;
}

const Mat4 & Model::GetNormalTransform() const
{
	return normalTransform;
}

void Model::Translate(const Mat4 & T)
{
	transform = T * transform;
}

void Model::Scale(const Mat4 & S)
{
	transform = S * transform;
	normalTransform = Mat4::Scale(1.0 / S[0][0], 1.0 / S[1][1], 1.0 / S[2][2]) * transform;
}

void Model::Rotate(const Mat4 & R)
{
	transform = R * transform;
	normalTransform = R * normalTransform;
}

const std::vector<Geometry*>& Model::GetGeometries() const
{
	return geos;
}

const std::vector<Poly*>& Model::GetBBox() const
{
	return bbox;
}

void Model::SetColor(int r, int g, int b)
{
	color = Vec4(r, g, b);
}

void Model::SetColor(const Vec4 & color)
{
	this->color = color;
}

const Vec4 & Model::GetColor() const
{
	return color;
}

void Model::SetNormalColor(int r, int g, int b)
{
	normalColor = Vec4(r, g, b);
}

void Model::SetNormalColor(const Vec4 & color)
{
	normalColor = color;
}

const Vec4 & Model::GetNormalColor() const
{
	return normalColor;
}

void Model::BuildBoundingBox()
{
	Vec4 a(minCoord[0], maxCoord[1], maxCoord[2]);
	Vec4 b(minCoord[0], minCoord[1], maxCoord[2]);
	Vec4 c(maxCoord[0], minCoord[1], maxCoord[2]);
	Vec4 d(maxCoord[0], maxCoord[1], maxCoord[2]);
	Vec4 e(minCoord[0], maxCoord[1], minCoord[2]);
	Vec4 f(minCoord[0], minCoord[1], minCoord[2]);
	Vec4 g(maxCoord[0], minCoord[1], minCoord[2]);
	Vec4 h(maxCoord[0], maxCoord[1], minCoord[2]);

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
	bbox.push_back(top);
	bbox.push_back(left);
	bbox.push_back(right);
	bbox.push_back(front);
	bbox.push_back(back);
	bbox.push_back(bottom);
}

Vec4 Model::GetBBoxDimensions() const
{
	Vec4 result = maxCoord - minCoord;
	for (int i = 0; i < 3; i++)
	{
		result[0] = abs(result[0]);
	}
	return result;
}

Vec4 Model::GetBBoxCenter() const
{
	Vec4 result = minCoord + (maxCoord - minCoord) / 2.0;
	result[3] = 1.0;
	return result;
}

void Model::DeleteGeometries()
{
	// free geometries
	while (geos.size() > 0)
	{
		Geometry* geo = geos.back();
		geos.pop_back();
		delete geo;
	}

	// free bounding box vertices
	while (bbox.size() > 0)
	{
		Poly* p = bbox.back();
		bbox.pop_back();
		delete p;
	}
}
