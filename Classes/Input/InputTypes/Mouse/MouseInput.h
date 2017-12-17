#pragma once

namespace cocos2d {
	class EventMouse;
}

class MouseInput
{
public:
	bool init();

private:
	void onMouseButtonDown(cocos2d::EventMouse* pEvent);
	void onMouseButtonUp(cocos2d::EventMouse* pEvent);
};
