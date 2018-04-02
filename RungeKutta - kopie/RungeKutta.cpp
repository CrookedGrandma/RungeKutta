// RungeKutta.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <numeric>

using namespace std;

struct vector2f {
	float X, Y;
	inline vector2f(void) { X = 0.f; Y = 0.f; }
	inline vector2f(const float x, const float y) { X = x; Y = y; }
	inline vector2f operator + (const vector2f& A) const { return vector2f(X + A.X, Y + A.Y); }
	inline vector2f operator + (const float A) const { return vector2f(X + A, Y + A); }
	inline vector2f operator - (const vector2f& A) const { return vector2f(X - A.X, Y - A.Y); }
	inline vector2f operator - (const float A) const { return vector2f(X - A, Y - A); }
	inline vector2f operator * (const float A) const { return vector2f(X * A, Y * A); }
	inline vector2f operator / (const float A) const { return vector2f(X / A, Y / A); }
	inline float Dot(const vector2f& A) const { return A.X*X + A.Y*Y; }
	inline float Length() { return sqrt(X * X + Y * Y); }
};

struct vector2d {
	double X, Y;
	inline vector2d(void) { X = 0.0; Y = 0.0; }
	inline vector2d(const double x, const double y) { X = x; Y = y; }
	inline vector2d operator + (const vector2d& A) const { return vector2d(X + A.X, Y + A.Y); }
	inline vector2d operator + (const double A) const { return vector2d(X + A, Y + A); }
	inline vector2d operator - (const vector2d& A) const { return vector2d(X - A.X, Y - A.Y); }
	inline vector2d operator - (const double A) const { return vector2d(X - A, Y - A); }
	inline vector2d operator * (const double A) const { return vector2d(X * A, Y * A); }
	inline vector2d operator / (const double A) const { return vector2d(X / A, Y / A); }
	inline double Dot(const vector2d& A) const { return A.X*X + A.Y*Y; }
	inline double Length() { return sqrt(X * X + Y * Y); }
};

struct fBody {
	int id;
	float mass;
	vector2f position;
	vector2f velocity;
	inline fBody(void) {}
	inline fBody(const float m, const vector2f x, const vector2f v) { mass = m; position = x; velocity = v; }
	inline fBody giveID(int id_) { id = id_; return *this; }
	inline string str() { return to_string(position.X) + ' ' + to_string(position.Y) + "\n" + to_string(velocity.X) + ' ' + to_string(velocity.Y); }
};

struct dBody {
	int id;
	double mass;
	vector2d position;
	vector2d velocity;
	inline dBody(void) {}
	inline dBody(const double m, const vector2d x, const vector2d v) { mass = m; position = x; velocity = v; }
	inline dBody giveID(int id_) { id = id_; return *this; }
	inline string str() { return to_string(position.X) + ' ' + to_string(position.Y) + "\n" + to_string(velocity.X) + ' ' + to_string(velocity.Y); }
};

//fBody* bodyList;
const float Gf = 6.67259e-11f;
const double Gd = 6.67259e-11;
const double twopi = 6.28318530718;
//const int reportfreq = 100000;
const int listsize = 2;
double timestep = 0.000001;
double currentTime = 0;
int numberofsteps = 100000000;
ofstream myfile;
vector2d startPos(0, 1);
vector2d startVel(twopi, 0);
dBody bodyList[listsize];

double distanceBetween(dBody a, dBody b) {
	double xDif = a.position.X - b.position.X;
	double yDif = a.position.Y - b.position.Y;
	return sqrt(xDif * xDif + yDif * yDif);
}
double distanceBetween(vector2d a, vector2d b) {
	double xDif = a.X - b.X;
	double yDif = a.Y - b.Y;
	return sqrt(xDif * xDif + yDif * yDif);
}

vector2d partialStep(vector2d p1, vector2d p2, double timestep) {
	vector2d point;
	point.X = p1.X + p2.X * timestep;
	point.Y = p1.Y + p2.Y * timestep;
	return point;
}

vector2d calculateAcceleration(dBody b) {
	vector2d acceleration;
	//for (int i = 0; i < listsize; i++) {
	//	if (bodyList[i].id != b.id) {
			dBody otherB = bodyList[0];
			double constante = -Gd * otherB.mass;
			vector2d k1, k2, k3, k4;

			// K1
			double r = distanceBetween(otherB, b);
			k1 = (b.position - otherB.position) * constante / (r * r * r);
			// K2
			vector2d vel = b.velocity + k1 * (0.5 * timestep);//partialStep(b.velocity, k1, 0.5f * timestep);
			vector2d loc = b.position + vel * (0.5 * timestep);
			//vector2d loc = partialStep(b.position, vel, 0.5 * timestep); 
			r = distanceBetween(otherB.position, loc);
			k2 = (loc - otherB.position) * constante / (r * r * r);
			// K3
			vel = partialStep(b.velocity, k2, 0.5 * timestep);
			loc = partialStep(b.position, vel, 0.5 * timestep); 
			r = distanceBetween(otherB.position, loc);
			k3 = (loc - otherB.position) * constante / (r * r * r);
			// K4
			vel = partialStep(b.velocity, k3, timestep);
			loc = partialStep(b.position, vel, timestep); 
			r = distanceBetween(otherB.position, loc);
			k4 = (loc - otherB.position) * constante / (r * r * r);

			acceleration = (k1 + k2 /** 2 + k3 * 2 + k4*/) / 2;
	//	}
	//}
	return acceleration;
}

void updateVelocities() {
	for (int i = 1; i < listsize; i++) {
		vector2d acceleration = calculateAcceleration(bodyList[i]);
		bodyList[i].velocity = bodyList[i].velocity + acceleration * timestep;
	}
}

void updatePositions() {
	for (int i = 1; i < listsize; i++) {
		bodyList[i].position = bodyList[i].position + bodyList[i].velocity * timestep;
	}
}

void computeGravityStep() {
	updateVelocities();
	updatePositions();
}

string time() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I.%M.%S", timeinfo);
	string str(buffer);

	return str;
}

void reset() {
	bodyList[1].position = startPos;
	bodyList[1].velocity = startVel;
	currentTime = 0;
}

void runSimulation(int numberOfSteps = numberofsteps) {
	for (int i = 0; i < numberOfSteps; i++) {
		if (i == numberOfSteps - 1) {
			myfile << to_string(currentTime) << "\n"
				   << bodyList[1].str() << "\n"
				   << to_string(distanceBetween(bodyList[0], bodyList[1])) << "\n\n";
		}
		computeGravityStep();
		currentTime += timestep;
	}
}

void runConstantTimeSim(int numberOfSteps, double simTime, int n) {
	reset();
	numberofsteps = numberOfSteps;
	timestep = simTime / (double)numberOfSteps;
	myfile.open(time() + ' ' + to_string(n) + ".txt");
	myfile << "double\n"
		   << bodyList[0].position.X << " " << bodyList[0].position.Y << " //starting position bigboi\n"
		   << bodyList[0].mass << " //mass bigboi\n"
		   << bodyList[1].position.X << " " << bodyList[1].position.Y << " //starting position satellite\n"
		   << bodyList[1].velocity.X << " " << bodyList[1].velocity.Y << " //starting velocity satellite\n"
		   << timestep << " //timestep\n"
		   << numberofsteps << " //number of steps\n\n";

	auto start = chrono::system_clock::now();
	cout << "Running...\n";
	runSimulation();
	auto end = chrono::system_clock::now();
	chrono::duration<double> diff = end - start;
	myfile << diff.count() << " //total duration\n"
		   << distanceBetween(*(new vector2d(0, 1)), bodyList[1].position) / (simTime * twopi * distanceBetween(*(new vector2d(0, 1)), bodyList[0].position)) << " //error\n";
	myfile.close();
}

int main() {
	//bodyList = (fBody*)calloc(sizeof(fBody), listsize);

	/*fBody sun(2e30, vector2f(0.f, 0.f), vector2f(0.f, 0.f));
	fBody mercury(3.285e23, vector2f(0.f, 5.0e10), vector2f(47000.f, 0.f));
	fBody venus(4.8e24, vector2f(0.f, 1.1e11), vector2f(35000.f, 0.f));
	fBody earth(6e24, vector2f(0.f, 1.5e11), vector2f(30000.f, 0.f));
	fBody mars(6.4e23, vector2f(0.f, 2.2e11), vector2f(24000.f, 0.f));
	fBody jupiter(1e28, vector2f(0.f, 7.7e11), vector2f(13000.f, 0.f));
	fBody saturn(5.7e26, vector2f(0.f, 1.4e12), vector2f(9000.f, 0.f));
	fBody uranus(8.7e25, vector2f(0.f, 2.8e12), vector2f(6835.f, 0.f));
	fBody neptune(4.5e12, vector2f(0.f, 4.5e12), vector2f(5477.f, 0.f));
	fBody pluto(1.3e22, vector2f(0.f, 3.7e12), vector2f(4748.f, 0.f));*/

	dBody bigboi(591650582522.79, vector2d(0., 0.), vector2d(0., 0.));
	dBody sattelite(1., startPos, startVel);

	bodyList[0] = bigboi.giveID(0);
	bodyList[1] = sattelite.giveID(1);

	for (int n = 500000; n >= 10000; n -= 10000) {
		runConstantTimeSim(n, 10, n);
	}
	/*runConstantTimeSim(10000, 100, 0);
	runConstantTimeSim(100000000, 100, 1);*/
	
	
	// Call this at the end of the program
	//free(bodyList);
	system("pause");
	return 0;
}