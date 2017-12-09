#include "Utils.h"
#include "GameConsts.h"

using namespace cocos2d;

Utils::Utils()
{

}

Vec2 Utils::getSpriteHeading(const Sprite* sprite)
{
	Vec2 playerPos = sprite->getPosition();
	float rotationRad = sprite->getRotation() * PI / 180;
	return Vec2(playerPos.x + (rotationRad), playerPos.y + (rotationRad));
}

bool Utils::isStrEqual(const std::string& a, const std::string& b)
{
	return a.compare(b) == 0;
}

void Utils::logVec2(const cocos2d::Vec2& v)
{
	cocos2d::log("(X: %f, Y: %f)", v.x, v.y);
}