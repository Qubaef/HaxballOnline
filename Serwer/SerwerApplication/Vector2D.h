#pragma once
class Vector2D
{
private:
	int x;
	int y;

public:
	Vector2D(int x, int y);
	int getX();
	int getY();
	void SetX();
	void SetY();
	int length();
	//vector operations operatort
	const Vector2D operator+(const Vector2D& vector) const;
	const Vector2D operator-(const Vector2D& vector) const;
	const Vector2D operator*(const Vector2D& vector) const;
	const Vector2D operator/(const Vector2D& vector) const;
	//bool operations 
	bool operator==(const Vector2D& vector) const;
	bool operator!=(const Vector2D& vector) const;
	bool operator>=(const Vector2D& vector) const;
	bool operator<=(const Vector2D& vector) const;
	bool operator>(const Vector2D& vector) const;
	bool operator<(const Vector2D& vector) const;
	//negate vector
	Vector2D operator-() const;
	//Apply scalar operations.
	Vector2D operator*(const float&) const;
	Vector2D operator/(const float&) const;
};