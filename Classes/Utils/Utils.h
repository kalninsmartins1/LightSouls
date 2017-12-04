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
	static cocos2d::Vec2 getSpriteHeading(const cocos2d::Sprite* sprite);
	static cocos2d::Color3B stringToColor(std::string str, const char* delimiter);
	static std::vector<std::string> splitStringByDelimiter(std::string str, const char* delimiter);
	static std::string appendFullPathToAssetsPath(const char* pathWithinAssets);
	static std::string appendFullPathToAssetsPath(std::string pathWithinAssets);
	static std::string getFirstChildText(tinyxml2::XMLNode* pNode);
	static bool initFromXML(cocos2d::Sprite& pSprite, const char* pathToXML);

private:
	// Extracts Vec3 x,y,z value from XMLNode attributes
	static cocos2d::Vec3 getVec3FromAttributes(tinyxml2::XMLNode* pNode);


private:
	static const char* ASSETS_PATH;
	
};