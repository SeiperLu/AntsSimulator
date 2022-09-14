#include "food.h"

double Food::Reduce_Food()
{
	if (Count >= 5)
	{
		Count -= 5;
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
	if (X == X_Given && Y == Y_Given)
		return true;
	else
		return false;
}