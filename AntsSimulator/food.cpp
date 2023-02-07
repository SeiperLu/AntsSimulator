#include "food.h"
#include<iostream>

Food::Food() :X(0), Y(0), Size(0), Count(0) {};

Food::Food(double X, double Y, int Size, double Count) :X(X), Y(Y), Size(Size), Count(Count) {};

double Food::Reduce_Food()
{
	if (Count >= 5)
	{
		Count -= 5;
		std::cout << "Jedzenie zabrane!\n";
		return 5.0;
	}
	else
	{
		double Temp = Count;
		Count = 0;
		return Temp;
	}
}

void Food::Re_Size()
{
	Size++;
}

bool Food::At_Food(double X_Given, double Y_Given)
{
	if (X_Given <= X+Size && X_Given >= X-Size  && Y_Given <= Y +Size && Y_Given >= Y-Size)
		return true;
	else
		return false;
}