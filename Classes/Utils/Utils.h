#pragma once

#include "cocos2d.h"

namespace tinyxml2 { class XMLNode; }

class Utils
{
private:
	Utils();
public:
	// Color for displaying graphic errors
	static const cocos2d::Color3B COLOR_PINK;

	static std::vector<float> stringToVector(const std::string& str);
	static cocos2d::Color3B stringToColor(std::string str, const char* delimiter);
	static std::vector<std::string> splitStringByDelimiter(std::string str, const char* delimiter);
	
	static std::string appendFullPathToAssetsPath(const char* pathWithinAssets);
	static std::string appendFullPathToAssetsPath(std::string pathWithinAssets);
	
	static std::string getFirstChildText(tinyxml2::XMLNode* pNode);
	static bool initFromXML(cocos2d::Sprite& pSprite, const char* pathToXML);

	static bool isEqual(const std::string& a, const std::string& b);
	static void logVec2(const cocos2d::Vec2& v);
	static cocos2d::Vec2 getSpriteHeading(const cocos2d::Sprite* sprite);
private:
	// Extracts values from XMLNode attributes
	static cocos2d::Vec3 getVec3FromAttributes(const tinyxml2::XMLNode* pNode);
	static cocos2d::PhysicsMaterial getPhysicsMaterialFromAttributes(const tinyxml2::XMLNode* pNode);
	static cocos2d::PhysicsBody* getPhysicsBodyFromAttributes(const tinyxml2::XMLNode* pNode);

private:
	static const char* ASSETS_PATH;
	
};