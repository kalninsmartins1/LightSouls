/*!
 * \Author	Martins Kalnins
 * \Date	27.07.2019
 * \Brief	Vector2 class with x and y components.
*/
#pragma once

#include "Classes/Core/Math/IVector2.h"

class Vector2 : public IVector2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& other);

	Vector2& operator*(float scale);
	Vector2& operator/(float divider);
	Vector2& operator+(const Vector2& otherVec);
	Vector2& operator-(const Vector2& otherVec);
	Vector2& operator=(const Vector2& otherVec);

	Vector2 operator/(float divider) const;
	Vector2 operator*(float scale) const;
	Vector2 operator+(const Vector2& otherVec) const;
	Vector2 operator-(const Vector2& otherVec) const;

public:
	static Vector2 GetZero();
	static Vector2 GetOneX();
	static Vector2 GetOneY();

	virtual const float GetX() const override;
	virtual const float GetY() const override;
	virtual const float GetLenghtSquared() const override;
	virtual const float GetLenght() const override;
	Vector2 GetNormalized() const;

	virtual void SetX(float x) override;
	virtual void SetY(float y) override;
	virtual void Set(float x, float y) override;

	virtual void Normalize() override;

private:
	float m_x;
	float m_y;
};