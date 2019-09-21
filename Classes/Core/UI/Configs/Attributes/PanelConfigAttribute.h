/*!
 * \Author	Martins Kalnins
 * \Date	14.09.2019
 * \Brief	Abstract base class for all
 *			 container element configs.
*/
#pragma once

#include <vector>
#include "Classes/Core/UI/Configs/Attributes/IConfigAttribute.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"

namespace UI
{
	class PanelConfigAttribute : public IConfigAttribute
	{
	public:
		virtual int							GetPriority() const override;
		const std::vector<ElementConfig>&	GetElementConfigs() const;
		
		void SetContent(const std::vector<ElementConfig>& content);

		void AddContent(const ElementConfig& config);

	private:
		std::vector<ElementConfig>	m_content;
	};
}