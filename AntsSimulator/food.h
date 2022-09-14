#pragma once

class Food
{
public:
	// cialo konstruktorow do cpp
	Food() :X(0), Y(0), Size(0), Count(0) {};
	Food(double X, double Y, int Size, double Count) :X(X), Y(Y), Size(Size), Count(Count) {};
	double Reduce_Food();
	void Re_Size();
	bool At_Food(double X_Given, double Y_Given);

private:
	double Count;
	int Size;
	double X;
	double Y;
};