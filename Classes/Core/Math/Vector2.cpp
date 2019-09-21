#include "Vector2.h"
#include <math.h> 

Vector2::Vector2()
	: m_x(0.0f)
	, m_y(0.0f)
{
}

Vector2::Vector2(float x, float y)
	: m_x(x)
	, m_y(y)
{

}

Vector2::Vector2(const Vector2& other)
	: m_x(other.m_x)
	, m_y(other.m_y)
{

}

Vector2 Vector2::operator+(const Vector2& otherVec) const
{
	return Vector2(m_x + otherVec.m_x, m_y + otherVec.m_y);
}

Vector2 Vector2::operator-(const Vector2& otherVec) const
{
	return Vector2(m_x - otherVec.m_x, m_y - otherVec.m_y);
}

Vector2& Vector2::operator=(const Vector2& otherVec)
{
	m_x = otherVec.m_x;
	m_y = otherVec.m_y;
	return *this;
}

Vector2 Vector2::operator*(float scale) const
{	
	return Vector2(m_x * scale, m_y * scale);
}

Vector2 Vector2::operator/(float divider) const
{
	Vector2 result;
	if (divider > 0)
	{
		result.Set(m_x / divider, m_y / divider);
	}

	return result;
}

Vector2 Vector2::GetZero()
{
	return Vector2(0.0f, 0.0f);
}

Vector2 Vector2::GetOneX()
{
	return Vector2(1.0f, 0.0f);
}

Vector2 Vector2::GetOneY()
{
	return Vector2(0.0f, 1.0f);
}

const float Vector2::GetX() const
{
	return m_x;
}

const float Vector2::GetY() const
{
	return m_y;
}

void Vector2::SetX(float x)
{
	m_x = x;
}

void Vector2::SetY(float y)
{
	m_y = y;
}

void Vector2::Set(float x, float y)
{
	m_x = x;
	m_y = y;
}

void Vector2::Normalize()
{
	const float length = GetLenght();
	if (length > 0)
	{
		m_x /= length;
		m_y /= length;
	}
}

const float Vector2::GetLenghtSquared() const
{
	return (m_x * m_x) + (m_y * m_y);
}

const float Vector2::GetLenght() const
{
	return sqrt(GetLenghtSquared());
}

Vector2 Vector2::GetNormalized() const
{
	Vector2 v(m_x, m_y);
	v.Normalize();
	return v;
}
