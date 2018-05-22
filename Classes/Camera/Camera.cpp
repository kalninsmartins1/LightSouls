#include "Camera.h"
#include "Utils/XML/XMLLoader.h"

NS_LIGHTSOULS_BEGIN

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

NS_LIGHTSOULS_END