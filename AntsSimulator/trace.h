#pragma once
#include <vector>
#include <tuple>
enum class FoodStatus {
	NoFood,
	Food
};

class Trace
{
public:
	Trace(double X, double Y, FoodStatus Food);
	Trace(const Trace& Tr);
	Trace& operator=(const Trace& Tr);
	Trace(Trace&&) = default;
	Trace& operator=(Trace&&) = default;

	void Add(double X, double Y);
	void Change_Mark(double X, double Y, FoodStatus Trace_Type);
	bool Is_Path_To_Nest(double X, double Y, FoodStatus On_Food) const;
	bool Is_Path_To_Food(double X, double Y, FoodStatus On_Food) const;
	std::pair<double, double> Next(double X, double Y, FoodStatus On_Food) const;
	std::pair<double, double> Previous(double X, double Y, FoodStatus On_Food) const;
	std::pair<double, double> Near_Path_To_Food(double X, double Y, FoodStatus On_Food, std::vector<class Food>& foods) const;
	void Change_Mark_All_Trace(FoodStatus Trace_Type);
	void Remove_Last();
private:
	std::vector<std::tuple<double, double, FoodStatus>> trace;
};