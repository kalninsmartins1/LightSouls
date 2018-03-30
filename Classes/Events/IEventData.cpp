#include "IEventData.h"

IEventData::IEventData(unsigned int receiversId)
	: m_receiversId(receiversId)
{

}

IEventData::~IEventData()
{

}

unsigned int IEventData::GetReceiversId() const
{
	return m_receiversId;
}
