// Verificacion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iomanip>
#include<iostream>
#include <math.h>
#include <vector>

using namespace std;

#include "Point3D.h"

Point3D getFinalPoint(Point3D &previous, double magnitude, double pitch, double yaw) {
	Point3D next = next.getVector(magnitude, pitch, yaw);
	previous += next;
	return previous;
}

int main(){

	Point3D result, previous;
	double mag = 5.83, theta = 45, phi = 46.6861;

	cout << "PREVIOUS: " << previous.getX() << " " << previous.getY() << " " << previous.getZ() << endl;

	result = getFinalPoint(previous, mag, phi, theta);

	cout << "RESULT: " << result.getX() << " " << result.getY() << " " << result.getZ() << endl;

	cout << endl << "PROGRAM HAS BEEN TERMINATED! " << endl;
	int opcion;
	cin >> opcion;

    return 0;
}

