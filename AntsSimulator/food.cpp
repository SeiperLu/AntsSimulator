#include "food.h"
#include<iostream>

Food::Food(double X, double Y, int Size, double Count) :X(X), Y(Y), Size(Size), Count(Count) {};
double Food::Reduce_Food()
{
	if (Count >= 5)
	{
		Count -= 5;
		return 5.0;
	}
	else
	{
		double temp = Count;
		Count = 0.0;
		return temp;
	}
}

void Food::Re_Size()
{
	Size++;
}

bool Food::At_Food(double X_Given, double Y_Given, double addedRange)
{
	return (X_Given <= X + Size +addedRange && X_Given >= X - Size - addedRange && Y_Given <= Y + Size + addedRange && Y_Given >= Y - Size - addedRange);
		
}

double Food::GetX() const
{
	return X;
}

double Food::GetY() const
{
	return Y;
}