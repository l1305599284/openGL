#include "Scene.h"

void Scene::add(Box inObject3D)
{
	children.push_back(inObject3D);
	std::cout << "Ìí¼Ó³É¹¦" << std::endl;
}

void Scene::clear()
{
	children.clear();
}