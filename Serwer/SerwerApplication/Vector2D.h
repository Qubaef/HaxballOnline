#pragma once
class Vector2D
{
private:
	int x;
	int y;

public:
	Vector2D(int x, int y);
	int getX() const;
	int getY() const;
	void SetVector(int x, int y);
	void SetVector(Vector2D);
	void SetX(int x);
	void SetY(int y);
	float length() const;
	//vector operations operators
	Vector2D operator+(const Vector2D& vector) const;
	Vector2D operator-(const Vector2D& vector) const;
	Vector2D operator*(const Vector2D& vector) const;
	Vector2D operator/(const Vector2D& vector) const;
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
	
	Vector2D operator*(const float& scalar) const;
	//if scalar =0, return (0,0) vector
	Vector2D operator/(const float& scalar) const;

	//special operations
	float dot() const;
	Vector2D Normalize() const;
};