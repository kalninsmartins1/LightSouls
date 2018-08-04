#pragma once

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

#define NS_LIGHTSOULS_BEGIN namespace LightSouls {
#define NS_LIGHTSOULS_END };

NS_LIGHTSOULS_BEGIN

using Vector2 = cocos2d::Vec2;
using Vector3 = cocos2d::Vec3;
using String = std::string;

// XML stuff since whole project uses it to load data
using XMLElement = tinyxml2::XMLElement;
using XMLNode = tinyxml2::XMLNode;
using XMLElement = tinyxml2::XMLElement;
using XMLDoc = tinyxml2::XMLDocument;
using XMLError = tinyxml2::XMLError;

NS_LIGHTSOULS_END
