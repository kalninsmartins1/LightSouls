/*!
 * \Author	Martins Kalnins
 * \Date	02.09.2019
 * \Brief	String class adapter for String used across project.
*/
#pragma once

#include <string>

class String
{
public:
	String();
	String(const char* cStr);
	String(const std::string& str);
	
	String&		operator+(const char* cStr);
	String&		operator+=(const char symbol);

	const char	operator[](unsigned int index) const;
	String		operator+(const char* cStr) const;

	bool		operator<(const String& other) const;
	bool		operator==(const char* cStr) const;
	bool		operator==(const String& other) const;

public:
	const char*		GetCStr() const;
	unsigned int	GetLength() const;
	bool			IsEmpty() const;

private:
	std::string m_str;
};