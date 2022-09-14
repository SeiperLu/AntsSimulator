#include "trace.h"
#include <tuple>

enum class Food_Status { No_Foods, Foods };

Trace::Trace(double X, double Y, int Food)
{
	if (Food == (int)Food_Status::No_Foods || Food == (int)Food_Status::Foods)
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