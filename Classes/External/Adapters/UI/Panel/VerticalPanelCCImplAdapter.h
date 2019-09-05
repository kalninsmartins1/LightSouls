/*!
 * \Author	Martins Kalnins
 * \Date	20.07.2019
 * \Brief	Adapts cocos2d-x vertical panel to
 *				vertical panel implementation of core.
*/
#pragma once

#include "Classes/Core/UI/Panel/IVerticalPanelImpl.h"
#include "Classes/Core/String/String.h"

namespace cocos2d
{
	namespace ui 
	{
		class Layout;
	}
}

namespace UI
{
	class VerticalPanelCCImplAdapter : public IVerticalPanelImpl
	{
	public:
		virtual bool GetNormalizedPos(Vector2& outPos) const override;
		virtual bool GetPathToImage(String& outPath) const override;
		virtual bool Init(const ElementConfig& elementConfig) override;
		virtual void AddElement(IElement& element) override;

	private:
		cocos2d::ui::Layout* m_ccLayout;
		String				 m_pathToImage;
	};
}