#include "Utils.h"
#include "3rdPartyLibs/tinyxml2.h"
#include "GameConsts.h"

using namespace cocos2d;

const Color3B Utils::COLOR_PINK(255, 192, 203);
const char* Utils::ASSETS_PATH = "res/";

std::vector<float> Utils::stringToVector(const std::string& str)
{
	const char* cStr = str.c_str();
	std::vector<float> numbers;
	int rectIdx = 0;

	for (unsigned int index = 0; index < str.length(); index++)
	{
		if (cStr[index] != ']' && cStr[index] != '[')
		{
			// Reads the whole number
			std::string wholeNumber = "";
			while (cStr[index] != ',' && cStr[index] != ']')
			{
				wholeNumber += cStr[index];
				index++;
			}

			// Transforms numbers in string format to integer format
			numbers.push_back(atof(wholeNumber.c_str()));
			rectIdx++;
		}
	}
	return numbers;
}
Vec2 Utils::getSpriteHeading(const Sprite* sprite)
{
	Vec2 playerPos = sprite->getPosition();
	float rotationRad = sprite->getRotation() * PI / 180;
	return Vec2(playerPos.x + (rotationRad), playerPos.y + (rotationRad));
}
Color3B Utils::stringToColor(std::string str, const char* delimiter)
{
	// Split string by delimiter
	std::vector<std::string> colorValArray = splitStringByDelimiter(str, delimiter);

	// In case of error return PINK
	if (colorValArray.size() != 3) return COLOR_PINK;

	// Get the component values
	int r = stoi(colorValArray.at(0));
	int g = stoi(colorValArray.at(1));
	int b = stoi(colorValArray.at(2));

	// Return the read color
	return Color3B(r, g, b);
}
std::string Utils::appendFullPathToAssetsPath(const char* pathWithinAssets)
{
	return std::string(ASSETS_PATH) + pathWithinAssets;
}
std::string Utils::appendFullPathToAssetsPath(std::string pathWithinAssets)
{
	return std::string(ASSETS_PATH) + pathWithinAssets;
}

std::vector<std::string> Utils::splitStringByDelimiter(std::string str, const char* delimiter)
{
	std::vector<std::string> stringParts;
	int index = str.find_first_of(delimiter); // Return last index if no delimiter found

	/* 
	 * Split string and store parts in array while string
	 *  is not empty or delimiter not found
	 */
	while (!str.empty() && index != -1)
	{
		// Get the split off part
		std::string split = str.substr(0, index);

		// Save it in array
		stringParts.push_back(split);

		// Split the actual part off the string
		str = str.substr(index + 1, str.length()-1); // index+1 because we dont want the old delimiter

		// Look for the next delimiter
		index = str.find_first_of(delimiter);
	}

	// Need to add the last part of the string that did not have delimiter
	stringParts.push_back(str);

	return stringParts;
}

std::string Utils::getFirstChildText(tinyxml2::XMLNode* pNode)
{
	return pNode->FirstChild()->ToText()->Value();
}