#pragma once

#include "ALMath.h"
#include "Geometry.h"
#include "Material.h"

class Model
{
private:
	std::vector<Geometry*> geos;
	Material* material;
	Mat4 transform;
	Mat4 normalTransform;
	Vec4 color;
	Vec4 normalColor;

	// BBox parameters
	std::vector<Poly*> bbox;
	Vec4 minCoord;
	Vec4 maxCoord;

public:
	Model() : material(new Material()), color(AL_WHITE), normalColor(AL_RED) { }
	~Model();

	void AddGeometry(Geometry* geo);

	const Mat4& GetTransform() const;
	const Mat4& GetNormalTransform() const;
	void Translate(const Mat4& T);
	void Scale(const Mat4& S);
	void Rotate(const Mat4& R);

	const std::vector<Geometry*>& GetGeometries() const;
	const std::vector<Poly*>& GetBBox() const;

	void SetColor(int r, int g, int b);
	void SetColor(const Vec4& color);
	const Vec4& GetColor() const;

	void SetNormalColor(int r, int g, int b);
	void SetNormalColor(const Vec4& color);
	const Vec4& GetNormalColor() const;

	void BuildBoundingBox();
	Vec4 GetBBoxDimensions() const;
	Vec4 GetBBoxCenter() const;

	Material* GetMaterial();
	
	void DeleteGeometries();

	//void Draw();
};

