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

const float Gf = 6.67259e-11f;
const double Gd = 6.67259e-11;
const float twopi = 6.28318530718f;
//const int reportfreq = 100000;
const int listsize = 2;
float timestep = 0.000001f;
float currentTime = 0.f;
int numberofsteps = 100000000;
ofstream myfile;
vector2f startPos(1.f, 0.f);
vector2f startVel(0.f, twopi);
fBody bodyList[listsize];

float distanceBetween(fBody a, fBody b) {
	float xDif = a.position.X - b.position.X;
	float yDif = a.position.Y - b.position.Y;
	return sqrt(xDif * xDif + yDif * yDif);
}
float distanceBetween(vector2f a, vector2f b) {
	float xDif = a.X - b.X;
	float yDif = a.Y - b.Y;
	return sqrt(xDif * xDif + yDif * yDif);
}

void computeGravityStep() {
	fBody otherB = bodyList[0];
	float constante = -Gf * otherB.mass;
	vector2f acc1, acc2, acc3, acc4;
	vector2f vel1, vel2, vel3, vel4;

	// K1
	float r = distanceBetween(otherB, bodyList[1]);
	acc1 = (bodyList[1].position - otherB.position) * constante / (r * r * r);
	vel1 = bodyList[1].velocity;
	// K2
	vector2f pos = bodyList[1].position + vel1 * 0.5f * timestep;
	r = distanceBetween(otherB.position, pos);
	acc2 = (pos - otherB.position) * constante / (r * r * r);
	vel2 = bodyList[1].velocity + acc1 * 0.5f * timestep;
	// K3
	pos = bodyList[1].position + vel2 * 0.5f * timestep;
	r = distanceBetween(otherB.position, pos);
	acc3 = (pos - otherB.position) * constante / (r * r * r);
	vel3 = bodyList[1].velocity + acc2 * 0.5f * timestep;
	// K4
	pos = bodyList[1].position + vel3 * timestep;
	r = distanceBetween(otherB.position, pos);
	acc4 = (pos - otherB.position) * constante / (r * r * r);
	vel4 = bodyList[1].velocity + acc3 * timestep;

	bodyList[1].position = bodyList[1].position + (vel1 + vel2 * 2.f + vel3 * 2.f + vel4) * timestep / 6.f;
	bodyList[1].velocity = bodyList[1].velocity + (acc1 + acc2 * 2.f + acc3 * 2.f + acc4) * timestep / 6.f;
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

vector2f correctPosition(float r, float distTraveled) {
	float rads = distTraveled / r;
	return vector2f(cos(rads), sin(rads)) * r;
}

void reset() {
	bodyList[1].position = startPos;
	bodyList[1].velocity = startVel;
	currentTime = 0.f;
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

void runConstantTimeSim(int numberOfSteps, float simTime, int n) {
	numberofsteps = numberOfSteps;
	timestep = simTime / (float)numberOfSteps;
	float r = distanceBetween(bodyList[1].position, bodyList[0].position);
	myfile.open("out_float\\" + time() + ' ' + to_string(n) + ".txt");
	myfile << "float\n"
		   << bodyList[0].position.X << " " << bodyList[0].position.Y << " //starting position bigboi\n"
		   << bodyList[0].mass << " //mass bigboi\n"
		   << bodyList[1].position.X << " " << bodyList[1].position.Y << " //starting position satellite\n"
		   << bodyList[1].velocity.X << " " << bodyList[1].velocity.Y << " //starting velocity satellite\n"
		   << timestep << " //timestep\n"
		   << numberofsteps << " //number of steps\n\n";

	cout << "Running...\n";
	auto start = chrono::system_clock::now();
	runSimulation();
	auto end = chrono::system_clock::now();
	chrono::duration<float> diff = end - start;
	float correctDistanceTraveled = abs(simTime*startVel.Y);
	//float correctDistanceTraveled = simTime * twopi * r;
	vector2f corrPos = correctPosition(r, correctDistanceTraveled);
	myfile << diff.count() << " //total duration\n"
		   << corrPos.X << ' ' << corrPos.Y << " //correct position\n"
		   << distanceBetween(corrPos, bodyList[1].position) / correctDistanceTraveled << " //error\n";
	//cout << distanceBetween(corrPos, bodyList[1].position);
	myfile.close();
	reset();
}

float fRand(float fMin, float fMax) {
	float f = (float)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

float calcMass(float r) {
	float v = startVel.Length();
	return r * v * v / Gf;
}

void runMultipleRandomSims(int n, int numberOfSteps = 500000) {
	for (int i = 0; i < n; i++) {
		float dist = startPos.X;
		float variance = dist / 100.0f;
		bodyList[1].position.X = dist + fRand(-variance, variance);
		bodyList[0].mass = calcMass(bodyList[1].position.X);
		runConstantTimeSim(numberOfSteps, 1, numberOfSteps*1000+1000+i);
	}
}

int main() {
	srand(time(NULL));
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

	fBody bigboi(591650582522.79f, vector2f(0.f, 0.f), vector2f(0.f, 0.f));
	fBody sattelite(1.f, startPos, startVel);

	bodyList[0] = bigboi.giveID(0);
	bodyList[1] = sattelite.giveID(1);

	//runMultipleRandomSims(1000);
	for (int n = 1000; n >= 10; n -= 10) {
		runMultipleRandomSims(100, n);
	}

	// Call this at the end of the program
	system("pause");
	return 0;
}