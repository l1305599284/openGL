#pragma once
#include "Box.h"
#include <vector>
class Scene
{
	public:
		std::vector<Box> children;
		void add(Box inObject3D);
		void clear();
	private:
		std::string type = "Scene";
};

