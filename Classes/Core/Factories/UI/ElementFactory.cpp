#include "ElementFactory.h"
#include "Classes/Core/UI/Panel/IVerticalPanelImpl.h"

using namespace UI;

ElementFactory::ElementFactory(const IVerticalPanelImpl* impl)
	: m_verticalPanelImpl(impl)
{

}

ElementFactory::~ElementFactory()
{
	delete m_verticalPanelImpl;
	m_verticalPanelImpl = nullptr;
}

VerticalPanel ElementFactory::CreateVerticalPanel()
{
	IVerticalPanelImpl* impl = nullptr;
	if (m_verticalPanelImpl != nullptr)
	{
		impl = m_verticalPanelImpl->Clone();
	}

	return VerticalPanel(impl);
}
