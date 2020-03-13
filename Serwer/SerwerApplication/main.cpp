#include "stdafx.h"

using namespace  std;


float main()
{
	Vector2D vector = Vector2D(2, 3);
	Vector2D vector2 = vector * 3;

	cout << vector2.getX() <<" "<< vector2.getY() << endl;
	Vector2D vector3 = vector2 /vector;

	cout << vector3.getX() << " " << vector3.getY() << endl;
	vector3 = Vector2D::Normal(vector2);

	cout << vector3.getX() << " " << vector3.getY() << " " << vector3.length()<< endl;
}