#pragma once

#include "Classes/External/CocosEngine.h"

class String;

namespace LS
{
	class Camera : public cc::Camera
	{
	public:
		static Camera* Create(const String& pathToXML);

	private:
		bool Init(const String& pathToXML);
	};
};


