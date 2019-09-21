#include "ElementConfigLoader.h"
#include "IAttributeLoader.h"
#include "Classes/Core/UI/Configs/ElementConfig.h"
#include "Classes/Core/String/String.h"
#include "Utils/XML/XMLLoader.h"
#include "Utils/XML/XMLConsts.h"

using namespace UI;

std::vector<std::unique_ptr<IAttributeLoader>> ElementConfigLoader::s_configLoaders;

bool UI::ElementConfigLoader::Init(const String& configPath)
{
	return XMLLoader::LoadXMLFile(configPath, m_doc);
}

void UI::ElementConfigLoader::RegisterConfigLoader(const IAttributeLoader& loader)
{
	s_configLoaders.emplace_back(loader.Clone());
}

bool UI::ElementConfigLoader::LoadConfig(const String& configPath, ElementConfig& outConfig)
{
	ElementConfigLoader loader;
	bool isSuccessful = false;

	if (loader.Init(configPath))
	{
		auto root = loader.GetRootElement();
		if (root != nullptr)
		{
			const String& type = root->Attribute(XMLConsts::TYPE_ATTR);
			if (!type.IsEmpty())
			{
				outConfig.SetType(type);
				loader.LoadAttributes(*root, outConfig);
				isSuccessful = true;
			}
			else
			{
				CC_ASSERT("UI Element defined without type specification !");
			}
		}
	}

	return isSuccessful;
}

const XMLElement* UI::ElementConfigLoader::GetRootElement() const
{
	return m_doc.RootElement();
}

void UI::ElementConfigLoader::LoadAttributes(const XMLElement& xmlElement, ElementConfig& config)
{
	for (auto& loader : s_configLoaders)
	{
		if (loader.get() != nullptr)
		{
			loader->LoadAttribute(xmlElement, config);
		}
	}
}
