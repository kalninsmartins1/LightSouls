/*!
 * \Author	Martins Kalnins
 * \Date	17.09.2019
 * \Brief	Class for loading UI element configs
 *			 from given path.
*/

#pragma once

#include <vector>
#include <memory>
#include "Utils/XML/XMLTypes.h"

class String;

namespace UI
{
	class ElementConfig;
	class IAttributeLoader;

	class ElementConfigLoader
	{
	public:
		static void RemoveAllConfigLoaders();
		static void RegisterConfigLoader(const IAttributeLoader& loader);
		static bool LoadConfig(const String& configPath, ElementConfig& outConfig);

		bool Init(const String& configPath);

	private:
		const XMLElement*	GetRootElement() const;

		void LoadAttributes(const XMLElement& xmlElement, ElementConfig& config);

	private:
		static std::vector<std::unique_ptr<IAttributeLoader>> s_configLoaders;
		XMLDoc m_doc;
	};
}
