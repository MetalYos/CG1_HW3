#include "Scene.h"
#include "StdAfx.h"


Scene::Scene() : camera(new Camera()), isBBoxOn(false), vertexNormals(false), 
	polyNormals(false), isCalcNormal(true), bgColor((AL_BLACK)), fineNess(20)
{
}


Scene::~Scene()
{
	while (models.size() > 0)
	{
		Model* model = models.back();
		models.pop_back();
		delete model;
	}
	delete camera;
}

void Scene::CreateModel()
{
	Model* model = new Model();
	models.push_back(model);
}

const std::vector<Model*>& Scene::GetModels() const
{
	return models;
}

Camera * Scene::GetCamera() const
{
	return camera;
}

void Scene::SetBBoxState(bool isBBoxOn)
{
	this->isBBoxOn = isBBoxOn;
}

bool Scene::GetBBoxState() const
{
	return isBBoxOn;
}

void Scene::SetNormals(bool vertexNormals, bool polyNormals)
{
	this->vertexNormals = vertexNormals;
	this->polyNormals = polyNormals;
}

bool Scene::AreVertexNormalsOn() const
{
	return vertexNormals;
}

bool Scene::ArePolyNormalsOn() const
{
	return polyNormals;
}

void Scene::SetCalcNormalState(bool isCalcNorm)
{
	this->isCalcNormal = isCalcNorm;
}

bool Scene::GetCalcNormalState() const
{
	return isCalcNormal;
}

void Scene::SetFineNess(int fineNess)
{
	this->fineNess = fineNess;
}

int Scene::GetFineNess() const
{
	return fineNess;
}

void Scene::DeleteModels()
{
	while (models.size() > 0)
	{
		Model* model = models.back();
		models.pop_back();
		delete model;
	}
}
