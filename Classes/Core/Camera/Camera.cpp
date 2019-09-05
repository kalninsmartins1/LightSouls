#include "Camera.h"
#include "Classes/Utils/Utils.h"
#include "Classes/Utils/XML/XMLLoader.h"

namespace LS
{
	Camera* Camera::Create(const String& pathToXML)
	{
		Camera* camera = new (std::nothrow) Camera();
		if (camera && camera->Init(pathToXML))
		{
			camera->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(camera);
		}

		return camera;
	}

	bool Camera::Init(const String& pathToXML)
	{
		setDepth(0.0f);
		return initDefault() && XMLLoader::InitializeCamera(*this, pathToXML);
	}
};



