#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <algorithm>
#include <utility>
#include "objects.h"

Ant::Ant(std::vector<Trace>& Traces,Nest &Ne)
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

Ant::Ant(int X_Given, int Y_Given, std::vector<Trace>& Traces, Nest &Ne)
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


void Ant::Take_Food(class Food &Fd)
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

void Ant::move(std::vector<Trace> &Traces, std::vector<class Food> &foods)
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

Trace::Trace(double X, double Y, int Food)
{
	if(Food == (int)Food_Status::No_Foods || Food == (int)Food_Status::Foods)
		trace.push_back(std::make_tuple(X, Y, Food));
	else
		trace.push_back(std::make_tuple(X, Y, (int)Food_Status::No_Foods));
}

void Trace::Add(double X, double Y)
{
	trace.push_back(std::make_tuple(X, Y, (int)Food_Status::No_Foods));
}

void Trace::Change_Mark(double X, double Y, int Trace_Type)
{
	auto it = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, (int)Food_Status::No_Foods));
	if (it != trace.end())
	{
		*it = std::make_tuple(X, Y, Trace_Type);
		return;
	}
	auto it2 = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, (int)Food_Status::Foods));
	if (it2 != trace.end())
		*it2 = std::make_tuple(X, Y, Trace_Type);		
}

bool Trace::Is_Path_To_Nest(double X, double Y, int On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.begin() && Position != trace.end())
		return true;
	else
		return false;
}

bool Trace::Is_Path_To_Food(double X, double Y, int On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.end() && Position + 1 != trace.end())
		return true;
	else
		return false;
}

std::pair<double, double> Trace::Next(double X, double Y, int On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.end() && Position + 1 != trace.end())
		return std::make_pair(std::get<0>(*(Position + 1)), (std::get<1>(*(Position + 1))));
}

std::pair<double, double> Trace::Previous(double X, double Y, int On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.end() && Position != trace.begin())
		return std::make_pair(std::get<0>(*(Position - 1)), (std::get<1>(*(Position - 1))));
}

void Trace::Remove_Last()
{
	trace.pop_back();
}

Trace::Trace(const Trace& Tr)
{
	trace.assign(Tr.trace.begin(), Tr.trace.end());
	for (auto i = trace.begin(); i != trace.end(); i++)
		std::get<2>(*i) = (int)Food_Status::No_Foods;
}

Trace& Trace::operator=(const Trace& Tr)
{
	if (&Tr == this)
		return *this;
	trace.assign(Tr.trace.begin(), Tr.trace.end());
	return *this;

}

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

void Nest::Take(double Count_Given)
{
	Count += Count_Given;
}

Ant::~Ant()
{
	delete Traked_Nest;
	delete Traked_Trace;
}

