#include "food.h"
#include<iostream>

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
		return Count;
}

void Food::Re_Size()
{
	Size++;
}

bool Food::At_Food(double X_Given, double Y_Given)
{
	return (X_Given <= X + Size && X_Given >= X - Size && Y_Given <= Y + Size && Y_Given >= Y - Size);
		
}