#include "trace.h"
#include "food.h"
#include <iostream>
Trace::Trace(double X, double Y, FoodStatus Food)
{
	if (Food == FoodStatus::NoFood || Food == FoodStatus::Food)
		trace.push_back(std::make_tuple(X, Y, Food));
	else
		trace.push_back(std::make_tuple(X, Y, FoodStatus::NoFood));
}

void Trace::Add(double X, double Y)
{
	trace.push_back(std::make_tuple(X, Y, FoodStatus::NoFood));
}

void Trace::Change_Mark(double X, double Y, FoodStatus Trace_Type)
{
	auto it = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, FoodStatus::NoFood));
	if (it != trace.end())
	{
		*it = std::make_tuple(X, Y, Trace_Type);
		return;
	}
	auto it2 = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, FoodStatus::Food));
	if (it2 != trace.end())
		*it2 = std::make_tuple(X, Y, Trace_Type);
}

bool Trace::Is_Path_To_Nest(double X, double Y, FoodStatus On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.begin() && Position != trace.end())
		return true;
	else
		return false;
}

bool Trace::Is_Path_To_Food(double X, double Y, FoodStatus On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.end() && Position + 1 != trace.end())
		return true;
	else
		return false;
}

std::pair<double, double> Trace::Near_Path_To_Food(double X, double Y, FoodStatus On_Food, std::vector<class Food>& foods) const
{
	if(std::get<0>(*(trace.end()-1)) == X && std::get<1>(*(trace.end() - 1)) == Y)
		return std::pair<double, double>(0.0, 0.0);
	for (auto it = trace.begin(); it != trace.end(); it++)
	{
		if (std::get<2>(*it) == On_Food && X < std::get<0>(*it) + 15.0 && X > std::get<0>(*it) - 15.0 && Y < std::get<1>(*it) + 15.0 && Y > std::get<1>(*it) - 15.0)
			for (auto itFood = foods.begin(); itFood != foods.end(); it++)
			{
				if (itFood->At_Food(std::get<0>(*(trace.end() - 1)), std::get<1>(*(trace.end() - 1))))
					return std::pair<double, double>(std::get<0>(*it), std::get<1>(*it));
			}
	}
	return std::pair<double,double>(0.0,0.0);
}

std::pair<double, double> Trace::Next(double X, double Y, FoodStatus On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.end() && Position + 1 != trace.end())
		return std::make_pair(std::get<0>(*(Position + 1)), (std::get<1>(*(Position + 1))));
	return std::make_pair<double, double>(0.0, 0.0);
}

std::pair<double, double> Trace::Previous(double X, double Y, FoodStatus On_Food) const
{
	auto Position = std::find(trace.begin(), trace.end(), std::make_tuple(X, Y, On_Food));
	if (Position != trace.end() && Position != trace.begin())
		return std::make_pair(std::get<0>(*(Position - 1)), (std::get<1>(*(Position - 1))));
	return std::make_pair<double, double>(0.0f, 0.0f);
}

void Trace::Remove_Last()
{
	trace.pop_back();
}

Trace::Trace(const Trace& Tr)
{
	trace.assign(Tr.trace.begin(), Tr.trace.end());
	for (auto i = trace.begin(); i != trace.end(); i++)
		std::get<2>(*i) = FoodStatus::NoFood;
}

Trace& Trace::operator=(const Trace& Tr)
{
	if (&Tr == this)
		return *this;
	trace.assign(Tr.trace.begin(), Tr.trace.end());
	return *this;
}

void Trace::Change_Mark_All_Trace(FoodStatus Trace_Type)
{
	for (auto it = trace.begin(); it != trace.end(); it++)
		std::get<2>(*it) = Trace_Type;
}
