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
	inline vector2f(void) { X = 0.f; Y = 0.f; }
	inline vector2f(const float x, const float y) { X = x; Y = y; }
	inline vector2f operator + (const vector2f& A) const { return vector2f(X + A.X, Y + A.Y); }
	inline vector2f operator + (const float A) const { return vector2f(X + A, Y + A); }
	inline vector2f operator * (const float A) const { return vector2f(X * A, Y * A); }
	inline float Dot(const vector2f& A) const { return A.X*X + A.Y*Y; }
};

struct vector2d {
	double X, Y;
	inline vector2d(void) { X = 0.0; Y = 0.0; }
	inline vector2d(const double x, const double y) { X = x; Y = y; }
	inline vector2d operator + (const vector2d& A) const { return vector2d(X + A.X, Y + A.Y); }
	inline vector2d operator + (const double A) const { return vector2d(X + A, Y + A); }
	inline vector2d operator * (const double A) const { return vector2d(X * A, Y * A); }
	inline double Dot(const vector2d& A) const { return A.X*X + A.Y*Y; }
};

struct fBody {
	int id;
	float mass;
	vector2f position;
	vector2f velocity;
	inline fBody(void) {}
	inline fBody(const float m, const vector2f x, const vector2f v) { mass = m; position = x; velocity = v; }
	inline fBody giveID(int id_) { id = id_; return *this; }
	inline string str() { return to_string(id) + " | " + to_string(mass) + " | " + to_string(position.X) + ' ' + to_string(position.Y) + " | " + to_string(velocity.X) + ' ' + to_string(velocity.Y) + "\n"; }
};

struct dBody {
	int id;
	double mass;
	vector2d position;
	vector2d velocity;
	inline dBody(void) {}
	inline dBody(const double m, const vector2d x, const vector2d v) { mass = m; position = x; velocity = v; }
	inline dBody giveID(int id_) { id = id_; return *this; }
	inline string str() { return to_string(id) + " | " + to_string(mass) + " | " + to_string(position.X) + ' ' + to_string(position.Y) + " | " + to_string(velocity.X) + ' ' + to_string(velocity.Y) + "\n"; }
};

const int listsize = 2;
//fBody* bodyList;
fBody bodyList[listsize];
const float Gf = 6.67259e-11;
const double Gd = 6.67259e-11;
const float h = 0.1f;
const float timestep = 0.00001f;

// TODO: methode voor gravitatiekrachtvector tussen 2 bodies formule
float Fz(float m1, float m2, float r) {
	return (Gf * m1 * m2) / (r * r);
}
double Fz(double m1, double m2, double r) {
	return (Gd * m1 * m1) / (r * r);
}

float distanceBetween(fBody a, fBody b) {
	float xDif = a.position.X - b.position.X;
	float yDif = a.position.Y - b.position.Y;
	return sqrt(xDif * xDif + yDif * yDif);
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
			float r = distanceBetween(b, otherB);
			float constante = Gf * otherB.mass / (r * r * r);
			vector2f k1, k2, k3, k4;

			// K1
			k1.X = constante * (b.position.X - otherB.position.X);
			k1.Y = constante * (b.position.Y - otherB.position.Y);
			// K2
			vector2f vel = partialStep(otherB.velocity, k1, 0.5f);
			vector2f loc = partialStep(otherB.position, vel, 0.5f * timestep); // timestep goed?
			k2.X = (otherB.position.X - loc.X) * constante;
			k2.Y = (otherB.position.Y - loc.Y) * constante;
			// K3
			vel = partialStep(otherB.velocity, k2, 0.5f);
			loc = partialStep(otherB.position, vel, 0.5f * timestep); // timestep goed?
			k3.X = (otherB.position.X - loc.X) * constante;
			k3.Y = (otherB.position.Y - loc.Y) * constante;
			// K4
			vel = partialStep(otherB.velocity, k3, 1.f);
			loc = partialStep(otherB.position, vel, timestep); // timestep goed?
			k4.X = (otherB.position.X - loc.X) * constante;
			k4.Y = (otherB.position.Y - loc.Y) * constante;

			acceleration.X += (k1.X + k2.X * 2 + k3.X * 2 + k4.X) / 6;
			acceleration.Y += (k1.Y + k2.Y * 2 + k3.Y * 2 + k4.Y) / 6;
			int ikwilhiereenbreakpointplease;
		}
	}
	return acceleration;
}

void updateVelocities() {
	for (int i = 0; i < listsize; i++) {
		vector2f acceleration = calculateAcceleration(bodyList[i]);
		bodyList[i].velocity = bodyList[i].velocity + acceleration * timestep;
	}
}

void updatePositions() {
	for (int i = 0; i < listsize; i++) {
		bodyList[i].position = bodyList[i].position + bodyList[i].velocity * timestep;
	}
}

void computeGravityStep() {
	updateVelocities();
	updatePositions();
}

void runSimulation(int numberOfSteps = 10000, int reportFreq = 100) {
	for (int i = 0; i < numberOfSteps; i++) {
		if (i % reportFreq == 0) {
			cout << bodyList[1].str();
		}
		computeGravityStep();
	}
}


int main() {
	//bodyList = (fBody*)calloc(sizeof(fBody), listsize);

	fBody sun(2e30, vector2f(0.f, 0.f), vector2f(0.f, 0.f));
	fBody mercury(3.285e23, vector2f(0.f, 5.0e10), vector2f(47000.f, 0.f));
	fBody venus(4.8e24, vector2f(0.f, 1.1e11), vector2f(35000.f, 0.f));
	fBody earth(6e24, vector2f(0.f, 1.5e11), vector2f(30000.f, 0.f));
	fBody mars(6.4e23, vector2f(0.f, 2.2e11), vector2f(24000.f, 0.f));
	fBody jupiter(1e28, vector2f(0.f, 7.7e11), vector2f(13000.f, 0.f));
	fBody saturn(5.7e26, vector2f(0.f, 1.4e12), vector2f(9000.f, 0.f));
	fBody uranus(8.7e25, vector2f(0.f, 2.8e12), vector2f(6835.f, 0.f));
	fBody neptune(4.5e12, vector2f(0.f, 4.5e12), vector2f(5477.f, 0.f));
	fBody pluto(1.3e22, vector2f(0.f, 3.7e12), vector2f(4748.f, 0.f));

	fBody bigboi(591650582522.79f, vector2f(0.f, 0.f), vector2f(0.f, 0.f));
	fBody sattelite(1.f, vector2f(0.f, 1.f), vector2f(6.28318531f, 0.f));

	bodyList[0] = bigboi.giveID(0);
	bodyList[1] = sattelite.giveID(1);

	for (int i = 0; i < listsize; i++) {
		runSimulation();
	}

	// Call this at the end of the program
	//free(bodyList);
	system("pause");
	return 0;
}