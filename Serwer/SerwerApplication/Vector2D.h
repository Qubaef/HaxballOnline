#pragma once

class Vector2D
{
private:
	double x;
	double y;

public:
	Vector2D(double x, double y);
	~Vector2D();
	double getX() const;
	double getY() const;
	void SetVector(double x, double y);
	void SetVector(Vector2D);
	void SetX(double x);
	void SetY(double y);
	double length() const;
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
	
	Vector2D operator*(const double& scalar) const;
	//if scalar =0, return (0,0) vector
	Vector2D operator/(const double& scalar) const;

	//special operations
	static double Dot(const Vector2D&, const Vector2D&);
	static double CrossProduct(const Vector2D&, const Vector2D&);

	//Return the unit vector of the input
	static Vector2D Normal(const Vector2D&);
};