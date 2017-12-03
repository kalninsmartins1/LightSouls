#include <cmath>
#include "Enemy.h"

using namespace cocos2d;

Enemy::Enemy()
:	m_Level(0), 
	m_Health(0), 
	m_MoveSpeed(0), 
	m_ActiveRadius(0), 
	m_DamageDealt(0)
{
	m_pStateMachine = nullptr;
}

Enemy::~Enemy()
{

}

//bool Enemy::init(const char* pathToXML)
//{
//	// First read all things from XML
//	bool success = initFromXML(pathToXML);
//
//	// Then init other things
//	m_pStateMachine = new StateMachine();
//	m_BasePosition = Vec2(getPosition());
//	m_DamageDealt = m_Level + log10(m_Level);
//	m_SpellList.push_back(new Spell());
//
//	/* Need to create new instanse of sprite for spell because otherwise
//	 * enemy sprite will be affected by speel changes to its sprite. POINTERS !
//	*/
//	Sprite* pSpellSprite = Sprite::createWithTexture(getTexture());
//	m_SpellList.at(0)->init(this, FireBolt, pSpellSprite, 50, 100);
//
//	return m_pStateMachine->init(this) && success;
//}

void Enemy::update(float deltaTime)
{
//	m_pStateMachine->update(deltaTime);
}
float Enemy::getActiveRadius() const
{
	return m_ActiveRadius;
}
Vec2 Enemy::getBasePosition() const
{
	return m_BasePosition;
}
float Enemy::getMovementSpeed() const
{
	return m_MoveSpeed;
}
bool Enemy::initFromXML(const char* pathToXML)
{
	// Load the file
//	tinyxml2::XMLDocument doc;
//	tinyxml2::XMLError err = doc.LoadFile(Utils::appendFullPathToAssetsPath(pathToXML).c_str());
//	if (err)
//	{
//		cocos2d::log("Enemy: XML file not found: %s", pathToXML);
//	}
//	// Get the data
//	tinyxml2::XMLNode* pData = doc.RootElement();
//	tinyxml2::XMLNode* pXMLSpritePath = pData->FirstChildElement(XML_BASE_SPRITE)->FirstChild();
//	tinyxml2::XMLNode* pXMLLevel = pData->FirstChildElement(XML_LEVEL)->FirstChild();
//	tinyxml2::XMLNode* pXMLMoveSpeed = pData->FirstChildElement(XML_MOVE_SPEED)->FirstChild();
//	tinyxml2::XMLNode* pXMLRadius = pData->FirstChildElement(XML_RADIUS)->FirstChild();
//
//	// Initialize enemy sprite
//	auto spritePath = Utils::appendFullPathToAssetsPath(
//		pXMLSpritePath->ToText()->Value());
//	
//	if (!initWithFile(spritePath))
//	{
//		// Log info if sprite not found 
//		cocos2d::log("Enemy: Failed to init from file: %s ", spritePath);
//	}
//	setContentSize(Size(BUNNY_SIZE, BUNNY_SIZE));
//	
//
//	// Init enemy level
//	m_Level = std::stoi(pXMLLevel->ToText()->Value());
//
//	// Init radius
//	m_ActiveRadius = std::stof(pXMLRadius->ToText()->Value());
//
//	// Init move speed
//	m_MoveSpeed = std::stof(pXMLMoveSpeed->ToText()->Value());

	return getTexture() != nullptr;
}