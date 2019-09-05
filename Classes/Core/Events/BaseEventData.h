#pragma once

class BaseEventData
{
public:
	BaseEventData(unsigned int senderId);
	virtual ~BaseEventData();

	unsigned int GetSenderId() const;

private:
	unsigned int m_senderId; // Id of sender
};