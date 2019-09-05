/*!
 * \Author	Martins Kalnins
 * \Date	20.07.2019
 * \Brief	Panel specialization that orders its child
 *				elements vertically.
*/
#pragma once

#include <memory>

#include "Classes/Core/UI/Panel/IPanel.h"

class Vector2;
class String;

namespace UI
{
	class IVerticalPanelImpl;

	class VerticalPanel : public IPanel
	{
	public:
		VerticalPanel(IVerticalPanelImpl* impl);

		virtual bool GetNormalizedPos(Vector2& outPos) const;
		virtual bool GetPathToImage(String& outPathToImage) const;
		virtual bool Init(const UI::ElementConfig& elementConfig);
		virtual void AddElement(UI::IElement& element);

	private:
		std::unique_ptr<IVerticalPanelImpl> m_verticalPanelImpl;
	};
}