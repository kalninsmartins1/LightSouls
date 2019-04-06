#include "AEventData.h"

AEventData::AEventData(unsigned int senderId)
	: m_senderId(senderId)
{

}

AEventData::~AEventData()
{

}

unsigned int AEventData::GetSenderId() const
{
	return m_senderId;
}
