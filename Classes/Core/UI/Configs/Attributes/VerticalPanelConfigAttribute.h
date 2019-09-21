/*!
 * \Author	Martins Kalnins
 * \Date	18.09.2019
 * \Brief	Creation attribute for UI VerticalPanel.
*/
#pragma once

#include "Classes/Core/UI/Configs/Attributes/PanelConfigAttribute.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class Element;

	class VerticalPanelConfigAttribute : public PanelConfigAttribute
	{
	public:
		static const String&						GetType();

		float										GetSpacing() const;
		float										GetLeftPadding() const;
		float										GetTopPadding() const;
		virtual std::unique_ptr<IConfigAttribute>	Clone() const override;

		void SetSpacing(float spacing);
		void SetLeftPadding(float padding);
		void SetTopPadding(float padding);

		virtual void Apply(Element& target) const override;

	private:
		float GetTotalChildHeight(const std::vector<Element*>& childElements) const;
		float GetLargestChildWidth(const std::vector<Element*>& childElements) const;
		void  GetContainerSize(const std::vector<Element*>& childElements, Vector2& outSize) const;

		void InstantiateChildElements(Element& target) const;
		void ValidateChildPositions(Element& target) const;

	private:
		static String	s_type;
		float			m_spacing;
		float			m_leftPadding;
		float			m_topPadding;
	};
}