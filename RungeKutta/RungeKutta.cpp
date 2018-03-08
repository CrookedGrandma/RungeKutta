// RungeKutta.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct vector2f {
	float X, Y;
	inline vector2f(void) {}
	inline vector2f(const float x, const float y) { X = x; Y = y; }
	inline vector2f operator + (const vector2f& A) const { return vector2f(X + A.X, Y + A.Y); }
	inline vector2f operator + (const float A) const { return vector2f(X + A, Y + A); }
	inline float Dot(const vector2f& A) const { return A.X*X + A.Y*Y; }
};

struct vector2d {
	float X, Y;
	inline vector2d(void) {}
	inline vector2d(const double x, const double y) { X = x; Y = y; }
	inline vector2d operator + (const vector2d& A) const { return vector2d(X + A.X, Y + A.Y); }
	inline vector2d operator + (const double A) const { return vector2d(X + A, Y + A); }
	inline double Dot(const vector2d& A) const { return A.X*X + A.Y*Y; }
};

struct fBody {
	float mass;
	vector2f position;
	vector2f velocity;
	inline fBody(void) {}
	inline fBody(const float m, const vector2f x, const vector2f v) { mass = m; position = x; velocity = v; }
};

struct dBody {
	double mass;
	vector2d position;
	vector2d velocity;
	inline dBody(void) {}
	inline dBody(const double m, const vector2d x, const vector2d v) { mass = m; position = x; velocity = v; }
};


const float Gf = 6.67259e-11;
const double Gd = 6.67259e-11;

int main() {
	fBody* bodyList;
	bodyList = (fBody*)calloc(sizeof(fBody), 2);
	
	// Call this at the end of the program
	free(bodyList);
	return 0;
}

// TODO: methode voor gravitatiekrachtvector tussen 2 bodies formule
float Fz(float m1, float m2, float r) {
	return (Gf * m1 * m2) / (r * r);
}
double Fz(double m1, double m2, double r) {
	return (Gd * m1 * m1) / (r * r);
}

// TODO: methode voor het berekenen van de vector in bewegingsrichting orbitplaneet.



// TODO: resulterende krachtvector berekenen

// calculate Runge Kutta
float calculateRK(float tn, float yn) {
	calculateK4(tn, yn);
}

// calculate K1
float calculateK1(float tn, float yn, float h) {
	return functie(tn, yn);
}
// calculate K2
float calculateK2(float tn, float yn, float h) {
	return functie(tn + (h / 2), yn + h * (calculateK1(tn, yn) / 2));
}
// calculate K3
float calculateK3(float tn, float yn, float h) {
	return functie(tn + (h / 2), yn + h * (calculateK2(tn, yn) / 2));
}
// calculate K4
float calculateK4(float tn, float yn, float h) {
	return functie(tn + h, yn + h * calculateK3(tn, yn));
}

