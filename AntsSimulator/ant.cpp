#include "ant.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include <tuple>

namespace
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> thetaDistr(-7.0, 7.0);
	std::uniform_real_distribution<> magnitudeDistr(0.0, 5.0);
	std::uniform_int_distribution<> nextThetaShiftDistr(1, 5);
	const double view_limit_x = 2560.0;
	const double view_limit_y = 1400.0;

}


double CylindricalX_to_CartesianX(double magnitude, double theta)
{
	double shiftX = magnitude * (cos(theta * (M_PI / 180.0)));
	return shiftX;
}

double CylindricalY_to_CartesianY(double magnitude, double theta)
{
	double shiftY = magnitude * (sin(theta * (M_PI / 180.0)));
	return shiftY;
}

Ant::Ant(std::vector<Trace> &traces, Nest& nest)
	: x( 0 )
	, y( 0 )
	, theta( 0 )
	, status( CarryingStatus::NotCarrying )
	, food( 0 )
	, nextThetaShift( 3 ) {
	traces.push_back(*std::make_unique<Trace>(x, y, FoodStatus::NoFood));
	traceNumber = traces.size() - 1;
	*(this->nest) = nest;
}

Ant::Ant(double xGiven, double yGiven,double thetaGiven, std::vector<Trace>& traces, Nest& nest_given)
	: x( xGiven )
	, y( yGiven )
	, theta(thetaGiven)
	, status( CarryingStatus::NotCarrying )
	, food( 0 ) 
	, nextThetaShift( 3 ) {
	traces.push_back(*std::make_unique<Trace>(x, y, FoodStatus::NoFood));
	traceNumber = traces.size() - 1;
	nest = std::unique_ptr<Nest>(&nest_given);
}

Ant::Ant(const Ant& other)
	: x(other.x)
	, y(other.y)
	, theta(other.theta)
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
		if (traces[traceNumber].Is_Path_To_Food(x, y, FoodStatus::Food))
		{
			nextCoordinates = traces[traceNumber].Next(x, y, FoodStatus::Food);
			theta = Theta_Shift_Calculation(x, y, nextCoordinates.first, nextCoordinates.second);
			x = nextCoordinates.first;
			y = nextCoordinates.second;
			return;
		} 
		else 
		{
			for (auto it = foods.begin(); it != foods.end(); it++) 
			{
				if (it->At_Food(x, y)) 
				{
					TakeFood(*it);
					if (food < 5)
						foods.erase(it);
					traces[traceNumber].Change_Mark(x, y, FoodStatus::Food);
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
				nearTrace = it->Near_Path_To_Food(x, y, FoodStatus::Food);
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
		if (nextThetaShift == 0)
		{
			thetaShift = thetaDistr(gen);
			theta = theta + thetaShift;
			nextThetaShift = nextThetaShiftDistr(gen);
		}
		nextThetaShift--;
		magnitude = magnitudeDistr(gen);
		shiftX = CylindricalX_to_CartesianX(magnitude, theta);
		shiftY = CylindricalY_to_CartesianY(magnitude, theta);
		if (x + shiftX >= view_limit_x || x + shiftX <= 0.0 || y + shiftY >= view_limit_y || y + shiftY <= 0.0)
		{
			theta += 90.0;
			shiftX = CylindricalX_to_CartesianX(magnitude, theta);
			shiftY = CylindricalY_to_CartesianY(magnitude, theta);
		}
		x = x + shiftX;
		y = y + shiftY;
		traces[traceNumber].Add(x,y);
	} 
	else 
	{
		if (traces[traceNumber].Is_Path_To_Nest(x, y, FoodStatus::Food)) {
			nextCoordinates = traces[traceNumber].Previous(x, y, FoodStatus::Food);
			theta = Theta_Shift_Calculation(x, y, nextCoordinates.first, nextCoordinates.second);
			x = nextCoordinates.first;
			y = nextCoordinates.second;
			traces[traceNumber].Change_Mark(x, y, FoodStatus::Food);
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

float Ant::GetTheta() const
{
	return theta;
}

double Ant::Theta_Shift_Calculation(double x1, double y1, double x2, double y2) const
{
	double shiftX = x2 - x1;
	double shiftY = y2 - y1;
	double theta = atan2(shiftY,shiftX) * (180.0 / M_PI);
	return theta;
}