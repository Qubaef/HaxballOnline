#pragma once
class Vector2D
{
private:
	float x;
	float y;

public:
	Vector2D(float x, float y);
	float getX() const;
	float getY() const;
	void SetVector(float x, float y);
	void SetVector(Vector2D);
	void SetX(float x);
	void SetY(float y);
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
	static float Dot(const Vector2D&, const Vector2D&);
	static float CrossProduct(const Vector2D&, const Vector2D&);

	//Return the unit vector of the input
	static Vector2D Normal(const Vector2D&);
};