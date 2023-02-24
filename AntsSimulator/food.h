#pragma once

class Food
{
public:
	Food()=default;
	Food(double X, double Y, int Size, double Count);
	double Reduce_Food();
	void Re_Size();
	bool At_Food(double X_Given, double Y_Given);

private:
	double Count=0;
	int Size=0;
	double X=0;
	double Y=0;
};