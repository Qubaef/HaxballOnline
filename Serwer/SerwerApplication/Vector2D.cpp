#include "Vector2D.h"
#include <cmath>

Vector2D::Vector2D(float x, float y)
	:x(x), y(y)
{

}

void Vector2D::SetX(float x)
{
	this->x = x;
}

void Vector2D::SetY(float y)
{
	this->y = y;
}

void Vector2D::SetVector(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vector2D::getX() const
{
	return this->x;
}

float Vector2D::getY() const
{
	return this->y;
}

bool Vector2D::operator!=(const Vector2D& vector) const
{
	if (this->x != vector.x || this->y != vector.y)
		return true;
	return false;
}

float Vector2D::length() const
{
	return sqrt(this->x * this->x + this->y * this->y);
}


Vector2D Vector2D::operator*(const float& scalar) const
{
	return Vector2D(this->x * scalar, this->y * scalar);
}


Vector2D Vector2D::operator/(const float& scalar) const
{
	if(scalar==0)
	{
		return  Vector2D(0, 0);
	}
	return Vector2D(this->x / scalar, this->y / scalar);
}

Vector2D Vector2D::operator-() const
{
	return Vector2D(-this->x, -this->y);
}

Vector2D Vector2D::operator*(const Vector2D& vector) const
{
	return Vector2D(this->x * vector.x, this->x * vector.x);
}

Vector2D Vector2D::operator/(const Vector2D& vector) const
{
	return Vector2D(this->x / vector.x, this->x / vector.x);
}

Vector2D Vector2D::operator+(const Vector2D& vector) const
{
	return Vector2D(this->x + vector.x, this->x + vector.x);
}

 Vector2D Vector2D::operator-(const Vector2D& vector) const
{
	return Vector2D(this->x - vector.x, this->x - vector.x);
}

bool Vector2D::operator<(const Vector2D& vector) const
{
	if (this->length() < vector.length())
		return  true;
	return false;
}

bool Vector2D::operator<=(const Vector2D& vector) const
{
	if (this->length() <= vector.length())
		return  true;
	return false;
}

bool Vector2D::operator==(const Vector2D& vector) const
{
	if (this->x == vector.x && this->y == vector.y)
		return true;
	return false;
}

bool Vector2D::operator>(const Vector2D& vector) const
{
	if (this->length() >vector.length())
		return  true;
	return false;
}

bool Vector2D::operator>=(const Vector2D& vector) const
{
	if (this->length() >= vector.length())
		return  true;
	return false;
}


float Vector2D::Dot(const Vector2D& a, const Vector2D& b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

float Vector2D::CrossProduct(const Vector2D& a, const Vector2D& b)
{
	return ((a.x * b.y) - (a.y * b.x));
}



Vector2D Vector2D::Normal(const Vector2D& v)
{
	return Vector2D(v.x / v.length(), v.y / v.length());
}

void Vector2D::SetVector(Vector2D vector)
{
	this->x = vector.x;
	this->y = vector.y;
}



