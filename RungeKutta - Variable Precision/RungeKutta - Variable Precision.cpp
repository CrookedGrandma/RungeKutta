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
const double twopi = 6.28318530718;
//const int reportfreq = 100000;
const int listsize = 2;
double timestep = 0.000001;
double currentTime = 0;
int numberofsteps = 100000000;
int digits = 64;
unsigned long long bitmask = ~0;
ofstream myfile;
vector2d startPos(1, 0);
vector2d startVel(0, twopi);
dBody bodyList[listsize];

unsigned long long bitMask(int numberOfDigits) {
	unsigned long long mask = 0;
	mask = ~mask;
	mask <<= 64 - numberOfDigits;
	return mask;
}

double applyBitMask(double* d) {
	long long t = (*(long long *)d) & bitmask;
	return *(double*)&t;
}
vector2d applyBitMask(vector2d vector) {
	vector.X = applyBitMask(&vector.X);
	vector.Y = applyBitMask(&vector.Y);
	return vector;
}

double distanceBetween(dBody a, dBody b) {
	double xDif = a.position.X - b.position.X;
	double yDif = a.position.Y - b.position.Y;
	xDif = applyBitMask(&xDif);
	yDif = applyBitMask(&yDif);
	double root = sqrt(xDif * xDif + yDif * yDif);
	return applyBitMask(&root);
}
double distanceBetween(vector2d a, vector2d b) {
	double xDif = a.X - b.X;
	double yDif = a.Y - b.Y;
	xDif = applyBitMask(&xDif);
	yDif = applyBitMask(&yDif);
	double root = sqrt(xDif * xDif + yDif * yDif);
	return applyBitMask(&root);
}

void computeGravityStep() {
	dBody otherB = bodyList[0];
	double constante = -Gd * otherB.mass;
	vector2d acc1, acc2, acc3, acc4;
	vector2d vel1, vel2, vel3, vel4;

	// K1
	double r = distanceBetween(otherB, bodyList[1]);
	acc1 = (bodyList[1].position - otherB.position) * constante / (r * r * r);
	vel1 = bodyList[1].velocity;
	acc1 = applyBitMask(acc1);
	vel1 = applyBitMask(vel1);
	// K2
	vector2d pos = bodyList[1].position + vel1 * 0.5 * timestep;
	r = distanceBetween(otherB.position, pos);
	acc2 = (pos - otherB.position) * constante / (r * r * r);
	vel2 = bodyList[1].velocity + acc1 * 0.5 * timestep;
	acc2 = applyBitMask(acc2);
	vel2 = applyBitMask(vel2);
	// K3
	pos = bodyList[1].position + vel2 * 0.5 * timestep;
	r = distanceBetween(otherB.position, pos);
	acc3 = (pos - otherB.position) * constante / (r * r * r);
	vel3 = bodyList[1].velocity + acc2 * 0.5 * timestep;
	acc3 = applyBitMask(acc3);
	vel3 = applyBitMask(vel3);
	// K4
	pos = bodyList[1].position + vel3 * timestep;
	r = distanceBetween(otherB.position, pos);
	acc4 = (pos - otherB.position) * constante / (r * r * r);
	vel4 = bodyList[1].velocity + acc3 * timestep;
	acc4 = applyBitMask(acc4);
	vel4 = applyBitMask(vel4);

	vector2d endpos = bodyList[1].position + (vel1 + vel2 * 2 + vel3 * 2 + vel4) * timestep / 6;
	vector2d vel = bodyList[1].velocity + (acc1 + acc2 * 2 + acc3 * 2 + acc4) * timestep / 6;
	bodyList[1].position = applyBitMask(endpos);
	bodyList[1].velocity = applyBitMask(vel);
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

vector2d correctPosition(double r, double distTraveled) {
	double rads = distTraveled / r;
	return vector2d(cos(rads), sin(rads)) * r;
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

void runConstantTimeSim(int numberOfSteps, double simTime, int fileIndex) {
	numberofsteps = numberOfSteps;
	timestep = simTime / (double)numberOfSteps;
	double r = distanceBetween(bodyList[1].position, bodyList[0].position);
	myfile.open("out_variable\\" + time() + ' ' + to_string(digits) + ' ' + to_string(fileIndex) + ".txt");
	myfile << "double\n"
		   << bodyList[0].position.X << " " << bodyList[0].position.Y << " //starting position bigboi\n"
		   << bodyList[0].mass << " //mass bigboi\n"
		   << bodyList[1].position.X << " " << bodyList[1].position.Y << " //starting position satellite\n"
		   << bodyList[1].velocity.X << " " << bodyList[1].velocity.Y << " //starting velocity satellite\n"
		   << timestep << " //timestep\n"
		   << numberofsteps << " //number of steps\n"
		   << digits << " //number of digits used\n\n";

	cout << "Running...\n";
	auto start = chrono::system_clock::now();
	runSimulation();
	auto end = chrono::system_clock::now();
	chrono::duration<double> diff = end - start;
	double correctDistanceTraveled = abs(simTime*startVel.Y);
	//double correctDistanceTraveled = simTime * twopi * r;
	vector2d corrPos = correctPosition(r, correctDistanceTraveled);
	myfile << diff.count() << " //total duration\n"
		   << corrPos.X << ' ' << corrPos.Y << " //correct position\n"
		   << distanceBetween(corrPos, bodyList[1].position) / correctDistanceTraveled << " //error\n";
	//cout << distanceBetween(corrPos, bodyList[1].position);
	myfile.close();
	reset();
}

double fRand(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

double calcMass(double r) {
	double v = startVel.Length();
	return r * v * v / Gd;
}

void runMultipleRandomSims(int n, int numberOfSteps = 500000) {
	for (int i = 0; i < n; i++) {
		double dist = startPos.X;
		double variance = dist / 100.0;
		bodyList[1].position.X = dist + fRand(-variance, variance);
		bodyList[0].mass = calcMass(bodyList[1].position.X);
		runConstantTimeSim(numberOfSteps, 1, i);
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

	dBody bigboi(591650582522.79, vector2d(0., 0.), vector2d(0., 0.));
	dBody sattelite(1., startPos, startVel);

	bodyList[0] = bigboi.giveID(0);
	bodyList[1] = sattelite.giveID(1);

	//runMultipleRandomSims(1000);
	for (int d = 35; d <= 64; d++) {
		digits = d;
		bitmask = bitMask(d);
		runMultipleRandomSims(100, 500);
	}

	// Call this at the end of the program
	system("pause");
	return 0;
}