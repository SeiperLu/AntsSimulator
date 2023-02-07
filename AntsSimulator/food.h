#pragma once

class Food
{
public:
	Food();
	Food(double X, double Y, int Size, double Count);
	double Reduce_Food();
	void Re_Size();
	bool At_Food(double X_Given, double Y_Given);

private:
	double Count;
	int Size;
	double X;
	double Y;
};