#include "ant.h"
#include<math.h>
#include <tuple>

enum class FoodStatus { 
	NoFood, 
	Food 
};

Ant::Ant(std::vector<Trace> &traces, Nest& nest)
	: x( 0 )
	, y( 0 )
	,theta(0)
	, status( CarryingStatus::NotCarrying )
	, food( 0 ) {
	traces.push_back(*std::make_unique<Trace>(x, y, (int)FoodStatus::NoFood));
	traceNumber = traces.size() - 1;
	*(this->nest) = nest;
}

Ant::Ant(double xGiven, double yGiven,double thetaGiven, std::vector<Trace>& traces, Nest& nest_given)
	: x( xGiven )
	, y( yGiven )
	,theta(thetaGiven)
	, status( CarryingStatus::NotCarrying )
	, food( 0 ) {
	traces.push_back(*std::make_unique<Trace>(x, y, (int)FoodStatus::NoFood));
	traceNumber = traces.size() - 1;
	nest = std::unique_ptr<Nest>(&nest_given);
}

Ant::Ant(const Ant& other)
	: x(other.x)
	, y(other.y)
	,theta(other.theta)
	, food(other.food)
	, status(other.status) {
	traceNumber = other.traceNumber;
	nest = std::make_unique<Nest>(*other.nest);
}

Ant& Ant::operator=(const Ant& other)
{
	if (this == &other) {
		return *this;
	}

	x = other.x;
	y = other.y;
	theta = other.theta;
	food = other.food;
	status = other.status;

	traceNumber = other.traceNumber;
	nest = std::make_unique<Nest>(*other.nest);

	return *this;
}


void Ant::TakeFood(class Food& Fd)
{
	status = CarryingStatus::Carrying;
	food += Fd.Reduce_Food();
}

void Ant::GiveFood()
{
	nest->Take(food);
	food = 0;
	status = CarryingStatus::NotCarrying;
}

void Ant::Move(std::vector<Trace>& traces, std::vector<class Food>& foods)
{

	if (status == CarryingStatus::NotCarrying) 
	{
		if (traces[traceNumber].Is_Path_To_Food(x, y, (int)FoodStatus::Food))
		{
			nextCoordinates = traces[traceNumber].Next(x, y, (int)FoodStatus::Food);
			x = nextCoordinates.first;
			y = nextCoordinates.second;
			return;
		} 
		else 
		{
			for (auto it = foods.begin(); it != foods.end(); it++) 
			{
				if (it->At_Food(x, y)) {
					TakeFood(*it);
					traces[traceNumber].Change_Mark(x, y, (int)FoodStatus::Food);
					return;
				}
			}
			i = 0;
			for (auto it = traces.begin(); it != traces.end(); it++)
			{
				if (i == traceNumber)
				{
					i++;
					continue;
				}	
				nearTrace = it->Near_Path_To_Food(x, y, (int)FoodStatus::Food);
				if (nearTrace != std::pair<double,double>(0.0,0.0)) 
				{
					traceNumber = i;
					x = nearTrace.first;
					y = nearTrace.second;
					return;
				}
				i++;
			}
		}
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> thetaDistr(-3.0, 3.0);
		std::uniform_real_distribution<> magnitudeDistr(0.0, 2.0);
		thetaShift = thetaDistr(gen);
		theta = theta + thetaShift;
		magnitude = magnitudeDistr(gen);
		shiftX = magnitude * (cos(theta * (3.14 / 180.0)));
		shiftY = magnitude * (sin(theta * (3.14 / 180.0)));
		if (x + shiftX >= 2560.0 || x + shiftX <= 0.0 || y + shiftY >= 1440.0 || y + shiftY <= 0.0)
		{
			theta += 90.0;
			shiftX = magnitude * (cos(theta * (3.14 / 180.0)));
			shiftY = magnitude * (sin(theta * (3.14 / 180.0)));
		}
		x = x + shiftX;
		y = y + shiftY;
		traces[traceNumber].Add(x,y);
	} 
	else 
	{
		if (traces[traceNumber].Is_Path_To_Nest(x, y, (int)FoodStatus::Food)) {
			nextCoordinates = traces[traceNumber].Previous(x, y, (int)FoodStatus::Food);
			x = nextCoordinates.first;
			y = nextCoordinates.second;
			traces[traceNumber].Change_Mark(x, y, (int)FoodStatus::Food);
			return;
		} else {
			GiveFood();
		}
	}
}

double Ant::GetX() const
{
	return x;
}

double Ant::GetY() const
{
	return y;
}
