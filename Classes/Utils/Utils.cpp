#include "Utils.h"
#include "GameConsts.h"

using namespace cocos2d;

Utils::Utils()
{

}

bool Utils::isStrEqual(const std::string& a, const std::string& b)
{
	return a.compare(b) == 0;
}

void Utils::logVec2(const cocos2d::Vec2& v)
{
	cocos2d::log("(X: %f, Y: %f)", v.x, v.y);
}