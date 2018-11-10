#pragma once

#include "LightSoulsTypes.h"

namespace LS
{
	class Camera : public cocos2d::Camera
	{
	public:
		static Camera* Create(const String& pathToXML);

	private:
		bool Init(const String& pathToXML);
	};
};


