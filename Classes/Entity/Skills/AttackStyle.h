#pragma once

#include <string>

enum AttackStyle
{
	None,
	CloseRange,
	LongRange,
};

/** Function compares given string to predefined string to find the appropriate
 * SpellType which is then returned.
*/
static AttackStyle strToSpellType(std::string spellName)
{
	/*
	 * std::string::compare 
	 * 0 - equal
	 * other - does not equal
	 * More info at : http://www.cplusplus.com/reference/string/string/compare/
	 */
	AttackStyle result = None;
//	if(spellName.compare(XML_FIRE_BOLT) == 0)
//	{
//		result = FireBolt;
//	}
//	else if(spellName.compare(XML_ICE_BOLT) == 0)
//	{
//		result = IceBolt;
//	}
//	else if(spellName.compare(XML_GROUND_BOLT) == 0)
//	{
//		result = GroundBolt;
//	}

	return result;
}