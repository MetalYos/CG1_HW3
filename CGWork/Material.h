#pragma once

#include "ALMath.h"

class Material
{
public:
	Vec4 Ka; // Between 0.0 - 1.0
	Vec4 Kd; // Between 0.0 - 1.0
	Vec4 Ks; // Between 0.0 - 1.0
	int Specular; // Between 1.0 - 1024.0

	Material() : Ka(Vec4(0.2)), Kd(Vec4(0.8)), Ks(Vec4(1.0)), Specular(32) {}
};