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

void Utils::startTimerWithCallback(Node* pNode, std::function<void()> callback,
	float time)
{
	Vector<FiniteTimeAction*> callbackTimerActions;
	callbackTimerActions.pushBack(DelayTime::create(time));
	callbackTimerActions.pushBack(CallFunc::create(callback));
	
	// Start timer with callback
	pNode->runAction(Sequence::create(callbackTimerActions));
}
