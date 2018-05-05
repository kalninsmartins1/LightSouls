#include "AEventData.h"

AEventData::AEventData(unsigned int receiversId)
	: m_receiversId(receiversId)
{

}

AEventData::~AEventData()
{

}

unsigned int AEventData::GetReceiversId() const
{
	return m_receiversId;
}
