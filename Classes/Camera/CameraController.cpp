#include "CameraController.h"
#include "../GameConsts.h"

using namespace cocos2d;

bool CameraController::init()
{
	return true;
}
void CameraController::moveCameraTo(cocos2d::Vec2 worldPosition,
	float movementSpeed)
{
	// Move camera to input position
	Camera* defaultCamera = Camera::getDefaultCamera();
	Vec3 cameraPosition = defaultCamera->getPosition3D();
	Action* moveCamera = MoveTo::create(movementSpeed, 
		Vec3(worldPosition.x, worldPosition.y,
		cameraPosition.z));
	moveCamera->setTag(ACTION_MOVE_TAG);

	// First stop any runing moving actions
	defaultCamera->stopActionByTag(ACTION_MOVE_TAG);
	
	// Then start new action
	defaultCamera->runAction(moveCamera);
}
