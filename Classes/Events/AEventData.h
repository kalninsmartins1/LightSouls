#pragma once

class AEventData
{
public:
	AEventData(unsigned int senderId);
	virtual ~AEventData();

	unsigned int GetSenderId() const;

private:
	unsigned int m_senderId; // Id of sender
};