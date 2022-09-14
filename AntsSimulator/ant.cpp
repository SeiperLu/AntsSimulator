#include "ant.h"
#include <random>

enum class Food_Status { No_Foods, Foods };
Ant::Ant(std::vector<Trace>& Traces, Nest& Ne)
{
	X = 0;
	Y = 0;
	Carry = Carrying_Status::No_Carrying;
	Food = 0;
	Traked_Trace = new Trace(X, Y, (int)Food_Status::No_Foods);
	Traces.push_back(*Traked_Trace);
	Traked_Nest = new Nest();
	*Traked_Nest = Ne;
}

Ant::Ant(int X_Given, int Y_Given, std::vector<Trace>& Traces, Nest& Ne)
{
	X = Y_Given;
	Y = Y_Given;
	Carry = Carrying_Status::No_Carrying;
	Food = 0;
	Traked_Trace = new Trace(X, Y, (int)Food_Status::No_Foods);
	Traces.push_back(*Traked_Trace);
	Traked_Nest = new Nest();
	*Traked_Nest = Ne;
}


void Ant::Take_Food(class Food& Fd)
{
	Carry = Carrying_Status::Carrying;
	Food += Fd.Reduce_Food();
}

void Ant::Give_Food()
{
	Traked_Nest->Take(Food);
	Food = 0;
	Carry = Carrying_Status::No_Carrying;
}

void Ant::move(std::vector<Trace>& Traces, std::vector<class Food>& foods)
{
	double Shift_X, Shift_Y;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> Distr(-2, 2);
	Shift_X = Distr(gen);
	Shift_Y = Distr(gen);
	if (Carry == Carrying_Status::No_Carrying)
	{
		if (Traked_Trace->Is_Path_To_Food(X, Y, (int)Food_Status::Foods))
		{
			std::pair<double, double> Next_Coordinates = Traked_Trace->Next(X, Y, (int)Food_Status::Foods);
			X = Next_Coordinates.first;
			Y = Next_Coordinates.second;
			return;
		}
		else
		{
			for (auto it = foods.begin(); it != foods.end(); it++)
			{
				if (it->At_Food(X, Y))
				{
					Take_Food(*it);
					Traked_Trace->Change_Mark(X, Y, (int)Food_Status::Foods);
					return;
				}
			}
			for (auto it = Traces.begin(); it != Traces.end(); it++)
			{
				if (it->Is_Path_To_Food(X, Y, (int)Food_Status::Foods))
				{
					*Traked_Trace = *it;
					return;
				}

			}
		}
		Trace Temporal_Trace(*Traked_Trace);
		Traked_Trace->Remove_Last();
		*Traked_Trace = Temporal_Trace;
		Traked_Trace->Add(X + Shift_X, Y + Shift_Y);
	}
	else
	{
		if (Traked_Trace->Is_Path_To_Nest(X, Y, (int)Food_Status::Foods))
		{
			std::pair<double, double> Next_Coordinates = Traked_Trace->Previous(X, Y, (int)Food_Status::Foods);
			X = Next_Coordinates.first;
			Y = Next_Coordinates.second;
			Traked_Trace->Change_Mark(X, Y, (int)Food_Status::Foods);
			return;
		}
		else
			Give_Food();
	}

}

double Ant::Ant_X()
{
	return X;
}
double Ant::Ant_Y()
{
	return Y;
}

Ant::~Ant()
{
	delete Traked_Nest;
	delete Traked_Trace;
}

