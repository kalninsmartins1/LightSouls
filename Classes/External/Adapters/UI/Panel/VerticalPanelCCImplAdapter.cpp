#include "VerticalPanelCCImplAdapter.h"
#include "Classes/Core/UI/ElementConfig.h"
#include "Classes/Core/Math/Vector2.h"
#include "Classes/Core/UI/IElement.h"
#include "ui/CocosGUI.h"
#include "Classes/External/CocosEngine.h"

using namespace UI;
using namespace cocos2d;

bool VerticalPanelCCImplAdapter::Init(const ElementConfig& elementConfig)
{
	m_ccLayout = ui::Layout::create();
	if (m_ccLayout != nullptr)
	{
		const char* pathToImage = elementConfig.GetPathToImage().GetCStr();
		m_ccLayout->setBackGroundImage(pathToImage);
		m_pathToImage = pathToImage;

		const Vector2& normalizedPos = elementConfig.GetNormalizedPosition();
		m_ccLayout->setNormalizedPosition(Vec2(normalizedPos.GetX(), normalizedPos.GetY()));

		const Vector2& point = elementConfig.GetAnchorPosition();
		m_ccLayout->setAnchorPoint(Vec2(point.GetX(), point.GetY()));
		m_ccLayout->setLayoutType(ui::Layout::Type::VERTICAL);
	}

	return m_ccLayout != nullptr;
}

void VerticalPanelCCImplAdapter::AddElement(IElement& element)
{
	if (m_ccLayout != nullptr)
	{
		String path;
		element.GetPathToImage(path);
		m_ccLayout->addChild(Sprite::create(path.GetCStr()));
	}
}

bool VerticalPanelCCImplAdapter::GetNormalizedPos(Vector2& outPos) const
{
	if (m_ccLayout != nullptr)
	{
		Vec2 normPos = m_ccLayout->getNormalizedPosition();
		outPos.SetX(normPos.x);
		outPos.SetY(normPos.y);
		return true;
	}

	return false;
}

bool VerticalPanelCCImplAdapter::GetPathToImage(String& outPath) const
{
	outPath = m_pathToImage;
	return true;
}
