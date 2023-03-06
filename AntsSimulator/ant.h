#pragma once
#include <memory>
#include <random>
#include "food.h"
#include "nest.h"
#include "trace.h"


class Ant {
public:
	Ant(std::vector<Trace> &Traces, Nest& Ne);
	Ant(double xGiven, double yGiven,double thetaGiven, std::vector<Trace>& Traces, Nest& Ne);
	Ant(const Ant&);
	Ant& operator=(const Ant&);
	Ant(Ant&&) = default;
	Ant& operator=(Ant&&) = default;
	~Ant() = default;

	void Move(std::vector<Trace>& Traces, std::vector<Food>& Foods);
	void TakeFood(Food& Fd);
	void GiveFood();
	double GetX() const;
	double GetY() const;
	float GetTheta() const;
private:
	enum class CarryingStatus { 
		NotCarrying, 
		Carrying 
	};
	double Theta_Shift_Calculation(double x1,double y1,double x2, double y2) const;
	double x;
	double y;
	double theta;/*phase angel*/
	double food;

	std::pair<double, double> nextCoordinates;
	std::pair<double, double> nearTrace;
	int i;
	int nextThetaShift;
	double thetaShift;
	double magnitude;
	double shiftX;
	double shiftY;
	CarryingStatus status;
	int  traceNumber;
	std::unique_ptr<Nest> nest;
};