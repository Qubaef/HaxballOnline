#include "stdafx.h"

Vector2D::Vector2D(double x, double y)
	:x(x), y(y)
{
}

Vector2D::~Vector2D()
{
}

void Vector2D::SetX(double x)
{
	this->x = x;
}

void Vector2D::SetY(double y)
{
	this->y = y;
}

void Vector2D::SetVector(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Vector2D::getX() const
{
	return this->x;
}

double Vector2D::getY() const
{
	return this->y;
}

bool Vector2D::operator!=(const Vector2D& vector) const
{
	return (this->x != vector.x || this->y != vector.y);
}

double Vector2D::length() const
{
	return sqrt(this->x * this->x + this->y * this->y);
}

Vector2D Vector2D::operator*(const double& scalar) const
{
	return Vector2D(this->x * scalar, this->y * scalar);
}

Vector2D Vector2D::operator/(const double& scalar) const
{
	if(scalar==0) {
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
	return Vector2D(this->x * vector.x, this->y * vector.y);
}

Vector2D Vector2D::operator/(const Vector2D& vector) const
{
	return Vector2D(this->x / vector.x, this->y / vector.y);
}

Vector2D Vector2D::operator+(const Vector2D& vector) const
{
	return Vector2D(this->x + vector.x, this->y + vector.y);
}

 Vector2D Vector2D::operator-(const Vector2D& vector) const
{
	return Vector2D(this->x - vector.x, this->y - vector.y);
}

bool Vector2D::operator<(const Vector2D& vector) const
{
	return (this->length() < vector.length());
}

bool Vector2D::operator<=(const Vector2D& vector) const
{
	return (this->length() <= vector.length());
}

bool Vector2D::operator==(const Vector2D& vector) const
{
	return (this->x == vector.x && this->y == vector.y);
}

bool Vector2D::operator>(const Vector2D& vector) const
{
	return (this->length() > vector.length());
}

bool Vector2D::operator>=(const Vector2D& vector) const
{
	return (this->length() >= vector.length());
}

double Vector2D::Dot(const Vector2D& a, const Vector2D& b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

double Vector2D::CrossProduct(const Vector2D& a, const Vector2D& b)
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