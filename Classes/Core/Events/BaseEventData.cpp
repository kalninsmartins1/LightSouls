#include "BaseEventData.h"

BaseEventData::BaseEventData(unsigned int senderId)
	: m_senderId(senderId)
{

}

BaseEventData::~BaseEventData()
{

}

unsigned int BaseEventData::GetSenderId() const
{
	return m_senderId;
}
