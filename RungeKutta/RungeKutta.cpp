// RungeKutta.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#include "stdafx.h"

struct vector3f {
	float X, Y, Z;
	inline vector3f(void) {}
	inline vector3f(const float x, const float y, const float z) { X = x; Y = y; Z = z; }
	inline vector3f operator + (const vector3f& A) const { return vector3f(X + A.X, Y + A.Y, Z + A.Z); }
	inline vector3f operator + (const float A) const { return vector3f(X + A, Y + A, Z + A); }
	inline float Dot(const vector3f& A) const { return A.X*X + A.Y*Y + A.Z*Z; }
};

struct vector3d {
	float X, Y, Z;
	inline vector3d(void) {}
	inline vector3d(const float x, const float y, const float z) { X = x; Y = y; Z = z; }
	inline vector3d operator + (const vector3d& A) const { return vector3d(X + A.X, Y + A.Y, Z + A.Z); }
	inline vector3d operator + (const float A) const { return vector3d(X + A, Y + A, Z + A); }
	inline float Dot(const vector3d& A) const { return A.X*X + A.Y*Y + A.Z*Z; }
};

struct Body {
	float mass;
	vector3f position;
	vector3f velocity;
	inline Body(void) {}
	inline Body(const float m, const vector3f x, const vector3f v) { mass = m; position = x; velocity = v; }
};


int main() {
	Body earth(5.f, vector3f(0, 0, 0), vector3f(0, 0, 0));
	earth.mass = 2.f;
	
	return 0;
}

const float g = 0;

// TODO: methode voor gravitatiekrachtvector tussen 2 bodies formule
float calculateGravitationPower(float m1, float m2, float r) {
	return (g * m1 * m2) / (r * r);
}
// TODO: methode voor het berekenen van de vector in bewegingsrichting orbitplaneet.
// TODO: resulterende krachtvector berekenen

// calculate Runge Kutta
/*float calculateRK(float t, float h) {

}

// calculate the K's for Runge Kutta
float calculateK1(float x, float t) {
	//return x / t;
}
float calculateK2() {

}
float calculateK3() {

}
float calculateK4() {

}*/

