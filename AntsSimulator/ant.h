#pragma once
#include <vector>
#include <tuple>
#include "food.h"
#include "nest.h"
#include"trace.h"

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