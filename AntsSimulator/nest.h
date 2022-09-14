#pragma once

class Nest
{
public:
	// lepiej default i inicjalizacja zminnych w klasie z {}
	Nest() = default;
	Nest(double X, double Y, double Count) :X(X), Y(Y), Count(Count) {};
	void Take(double Count_Given);

private:
	double X{ 0 };
	double Y{ 0 };
	double Count{ 0 };
};