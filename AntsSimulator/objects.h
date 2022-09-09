#pragma once

// tylko potrzbne includy
#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <algorithm>
#include <utility>

// enum class jest lpszy niz zwykly enum

enum class Food_Status { No_Foods, Foods };
// jedna para h+cpp dla klasy
class Trace
{
public:
    // glm::vector2 zamiast dwoch doubli
	Trace(double X, double Y, int Food);
	Trace(const Trace& Tr);
	Trace& operator=(const Trace& Tr);
	Trace(Trace&&) = default;
	Trace& operator=(Trace&&) = default;

	void Add(double X, double Y);
	void Change_Mark(double X, double Y, int Trace_Type);
	bool Is_Path_To_Nest(double X, double Y, int On_Food) const;
	bool Is_Path_To_Food(double X, double Y, int On_Food) const;
	std::pair<double, double> Next(double X, double Y, int On_Food) const;
	std::pair<double, double> Previous(double X, double Y, int On_Food) const;
	void Remove_Last();

private:
	std::vector<std::tuple<double, double, int>> trace;
};

class Nest
{
public:
    // lepiej default i inicjalizacja zminnych w klasie z {}
    Nest()= default;
	Nest(double X, double Y, double Count) :X(X), Y(Y), Count(Count) {};
	void Take(double Count_Given);

private:
	double X{0};
	double Y{0};
	double Count{0};
};

class Food
{
public:
    // cialo konstruktorow do cpp
	Food() :X(0), Y(0), Size(0), Count(0) {};
	Food(double X, double Y, int Size, double Count) :X(X), Y(Y), Size(Size), Count(Count) {};
	double Reduce_Food();
	void Re_Size();
	bool At_Food(double X_Given, double Y_Given);

private:
	double Count;
	int Size;
	double X;
	double Y;
};

class Ant
{
public:
	Ant(std::vector<Trace>& Traces, Nest& Ne);
	Ant(int X_Given, int Y_Given, std::vector<Trace>& Traces, Nest& Ne);
	void move(std::vector<Trace>& Traces, std::vector<Food>& Foods);
	void Take_Food(Food& Fd);
	void Give_Food();
	double Ant_X();
	double Ant_Y();
    // z uniqu ptr zbedny dtor
	~Ant();
private:
    // enum class
	enum class Carrying_Status { No_Carrying, Carrying };

	double X;
	double Y;
	Carrying_Status Carry;
	double Food;
    //unique ptr
	Trace* Traked_Trace;
	Nest* Traked_Nest;
};

