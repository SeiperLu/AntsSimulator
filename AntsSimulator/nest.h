#pragma once

class Nest
{
public:
	Nest() = default;
	Nest(double X, double Y, double Count) :X(X), Y(Y), Count(Count) {};
	void Take(double Count_Given);

private:
	double X;
	double Y;
	double Count;
};