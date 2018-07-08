#pragma once

class AEventData
{
public:
	AEventData(unsigned int receiverId);
	virtual ~AEventData();

	unsigned int GetReceiversId() const;

private:
	unsigned int m_receiversId;
};