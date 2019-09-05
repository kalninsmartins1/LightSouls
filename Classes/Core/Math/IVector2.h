/*!
 * \Author	Martins Kalnins
 * \Date	27.07.2019
 * \Brief	Interface for representing Vector2
 *			 with x and y components.
*/
#pragma once

class IVector2
{
public:
	virtual const float	GetX() const = 0;
	virtual const float	GetY() const = 0;
	virtual const float	GetLenghtSquared() const = 0;
	virtual const float GetLenght() const = 0;

	virtual void SetX(float x) = 0;
	virtual void SetY(float y) = 0;
	virtual void Set(float x, float y) = 0;

	virtual void Normalize() = 0;
};