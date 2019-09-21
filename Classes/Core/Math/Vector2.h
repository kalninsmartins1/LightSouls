/*!
 * \Author	Martins Kalnins
 * \Date	27.07.2019
 * \Brief	Vector2 class with x and y components.
*/
#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);

	Vector2& operator=(const Vector2& otherVec);
	Vector2 operator/(float divider) const;
	Vector2 operator*(float scale) const;
	Vector2 operator+(const Vector2& otherVec) const;
	Vector2 operator-(const Vector2& otherVec) const;

public:
	static Vector2 GetZero();
	static Vector2 GetOneX();
	static Vector2 GetOneY();

	virtual const float GetX() const;
	virtual const float GetY() const;
	virtual const float GetLenghtSquared() const;
	virtual const float GetLenght() const;
	Vector2 GetNormalized() const;

	virtual void SetX(float x);
	virtual void SetY(float y);
	virtual void Set(float x, float y);

	virtual void Normalize();

private:
	float m_x;
	float m_y;
};