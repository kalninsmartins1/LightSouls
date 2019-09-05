#include "String.h"

String::String()
{

}

String::String(const char* cStr)
	: m_str(cStr)
{

}

String::String(std::string str)
	: m_str(str)
{

}

String& String::operator+(const char* cStr)
{
	m_str.append(cStr);
	return *this;
}

String& String::operator+=(const char symbol)
{
	m_str += symbol;
	return *this;
}

String String::operator+(const char* cStr) const
{
	return String(m_str + cStr);
}

bool String::operator<(const String& other) const
{
	return m_str < other.m_str;
}

bool String::operator==(const char* cStr) const
{
	return m_str == cStr;
}

bool String::operator==(const String& other) const
{
	return m_str == other.m_str;
}

const char String::operator[](unsigned int index) const
{
	return m_str[index];
}

const char* String::GetCStr() const
{
	return m_str.c_str();
}

unsigned int String::GetLength() const
{
	return m_str.length();
}

bool String::IsEmpty() const
{
	return m_str.empty();
}
