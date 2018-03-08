// RungeKutta.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

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
	double X, Y;
	inline vector2d(void) {}
	inline vector2d(const double x, const double y) { X = x; Y = y; }
	inline vector2d operator + (const vector2d& A) const { return vector2d(X + A.X, Y + A.Y); }
	inline vector2d operator + (const double A) const { return vector2d(X + A, Y + A); }
	inline double Dot(const vector2d& A) const { return A.X*X + A.Y*Y; }
};

struct fBody {
	int id;
	float mass;
	vector2f position;
	vector2f velocity;
	inline fBody(void) {}
	inline fBody(const float m, const vector2f x, const vector2f v) { mass = m; position = x; velocity = v; }
};

struct dBody {
	int id;
	double mass;
	vector2d position;
	vector2d velocity;
	inline dBody(void) {}
	inline dBody(const double m, const vector2d x, const vector2d v) { mass = m; position = x; velocity = v; }
};

int listsize;
fBody* bodyList;
const float Gf = 6.67259e-11;
const double Gd = 6.67259e-11;
const float h = 0.1f;
float ts = 10.f;

int main() {
	listsize = 2;
	bodyList = (fBody*)calloc(sizeof(fBody), listsize);
	
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

vector2f partialStep(vector2f p1, vector2f p2, float timestep) {
	vector2f point;
	point.X = p1.X + p2.X * timestep;
	point.Y = p1.Y + p2.Y * timestep;
	return point;
}

vector2f calculateAcceleration(fBody b) {
	vector2f acceleration;
	for (int i = 0; i < listsize; i++) {
		if (bodyList[i].id != b.id) {
			fBody otherB = bodyList[i];
			float r = (b.position.X - otherB.position.X) * (b.position.X - otherB.position.X) + (b.position.Y - otherB.position.Y) * (b.position.Y - otherB.position.Y);
			r = sqrt(r);
			float constante = Gf * otherB.mass / (r * r * r);
			vector2f k1;
			vector2f k2;
			vector2f k3;
			vector2f k4;

			// K1
			k1.X = constante * (b.position.X - otherB.position.X);
			k1.Y = constante * (b.position.Y - otherB.position.Y);
			// K2
			vector2f vel = partialStep(otherB.velocity, k1, 0.5f);
			vector2f loc = partialStep(otherB.position, vel, 0.5f * ts); // timestep goed?
			k2.X = (otherB.position.X - loc.X) * constante;
			k2.Y = (otherB.position.Y - loc.Y) * constante;
			// K3
			vel = partialStep(otherB.velocity, k2, 0.5f);
			loc = partialStep(otherB.position, vel, 0.5f * ts); // timestep goed?
			k3.X = (otherB.position.X - loc.X) * constante;
			k3.Y = (otherB.position.Y - loc.Y) * constante;
			// K4
			vel = partialStep(otherB.velocity, k3, 1.f);
			loc = partialStep(otherB.position, vel, ts); // timestep goed?
			k4.X = (otherB.position.X - loc.X) * constante;
			k4.Y = (otherB.position.Y - loc.Y) * constante;

			acceleration.X += (k1.X + k2.X * 2 + k3.X * 2 + k4.X) / 6;
			acceleration.Y += (k1.X + k2.X * 2 + k3.X * 2 + k4.X) / 6;
		}
	}
	return acceleration;
}

/*
float f(fBody b, float tn, float yn ) {
	return 0; 
	// TODO: dit aanpassen
}

// calculate Runge Kutta
float calculateRK(fBody b, float tn, float yn) {
	calculateK4(b, tn, yn);
}

// calculate K1
float calculateK1(fBody b, float tn, float yn) {
	return f(b, tn, yn);
}
// calculate K2
float calculateK2(fBody b, float tn, float yn) {
	return f(b, tn + (h / 2), yn + h * (calculateK1(b, tn, yn) / 2));
}
// calculate K3
float calculateK3(fBody b, float tn, float yn) {
	return f(b, tn + (h / 2), yn + h * (calculateK2(b, tn, yn) / 2));
}
// calculate K4
float calculateK4(fBody b, float tn, float yn) {
	return f(b, tn + h, yn + h * calculateK3(b, tn, yn));
}
*/
