#pragma once

class IEventData
{
public:
	IEventData(unsigned int receiversId);
	virtual ~IEventData();

	unsigned int GetReceiversId() const;

private:
	unsigned int m_receiversId;
};