#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <algorithm>
#include <utility>

enum { NoFoods, Foods };
class Trace
{
public:
	Trace() = default;

	Trace(double x, double y, int food);
	Trace(const Trace& tr);
	Trace& operator=(const Trace& tr);
	Trace(Trace&&) = default;
	Trace& operator=(Trace&&) = default;
	~Trace() = default;

	void add(double x, double y);
	void changeMark(double x, double y, int trace_type);
	bool is_path_to_nest(double x, double y, int on_food) const;
	bool is_path_to_food(double x, double y, int on_food) const;
	std::pair<double, double> next(double x, double y, int on_food) const;
	std::pair<double, double> previous(double x, double y, int on_food) const;
	void remove_last();
	
private:
	std::vector<std::tuple<double, double, int>> trace;
};

class Nest
{
public:
	Nest() :x(0), y(0), count(0) {};
	Nest(double x, double y, double count) :x(x), y(y), count(count) {};
	void take(double count_given);

private:
	double x;
	double y;
	double count;
};

class Food
{
public:
	Food() :x(0), y(0), size(0), count(0) {};
	Food(double x, double y, int size, double count) :x(x), y(y), size(size), count(count) {};
	double reduceFood();
	void reSize();
	bool atFood(double x_given, double y_given);

private:
	double count;
	int size;
	double x;
	double y;
};

class Ant
{
public:
	Ant(std::vector<Trace>& traces, Nest& ne);
	Ant(int x_given, int y_given, std::vector<Trace>& traces, Nest& ne);
	void move(std::vector<Trace>& traces, std::vector<Food>& foods);
	void takeFood(Food& fd);
	void giveFood();
	~Ant();
private:
	enum {NoCarrying,Carry};


	double x;
	double y;
	int carry;
	double food;
	Trace *traked_trace;
	Nest *traked_nest;
};



Ant::Ant(std::vector<Trace>& traces,Nest &ne)
{
	x = 0;
	y = 0;
	carry = NoCarrying;
	food = 0;
	traked_trace = new Trace(x,y, NoFoods);
	traces.push_back(*traked_trace);
	traked_nest = new Nest();
	*traked_nest = ne;
}

Ant::Ant(int x_given, int y_given, std::vector<Trace>& traces, Nest &ne)
{
	x = x_given;
	y = y_given;
	carry = NoCarrying;
	food = 0;
	traked_trace = new Trace(x, y, NoFoods);
	traces.push_back(*traked_trace);
	traked_nest = new Nest();
	*traked_nest = ne;
}


void Ant::takeFood(Food& fd)
{
	carry = Carry;
	food += fd.reduceFood();
}

void Ant::giveFood()
{
	traked_nest->take(food);
	food = 0;
	carry = NoCarrying;
}

void Ant::move(std::vector<Trace> &traces, std::vector<Food> &foods)
{
	double shift_x, shift_y;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(-2, 2);
	shift_x = distr(gen);
	shift_y = distr(gen);
	if (carry == NoCarrying)
	{
		if (traked_trace->is_path_to_food(x, y, Foods))
		{
			std::pair<double, double> next_coordinates = traked_trace->next(x, y, Foods);
			x = next_coordinates.first;
			y = next_coordinates.second;
			return;
		}
		else
		{
			for (auto it = foods.begin(); it != foods.end(); it++)
			{
				if (it->atFood(x, y))
				{
					takeFood(*it);
					traked_trace->changeMark(x, y,Foods);
					return;
				}
			}
			for (auto it = traces.begin(); it != traces.end(); it++)
			{
				if (it->is_path_to_food(x, y, Foods))
				{
					*traked_trace = *it;
					return;
				}
				
			}
		}
		Trace temporal_trace(*traked_trace);
		traked_trace->remove_last();
		*traked_trace = temporal_trace;
		traked_trace->add(x + shift_x, y + shift_y);
	}
	else
	{
		if (traked_trace->is_path_to_nest(x, y, Foods))
		{
			std::pair<double, double> next_coordinates = traked_trace->previous(x, y, Foods);
			x = next_coordinates.first;
			y = next_coordinates.second;
			traked_trace->changeMark(x, y, Foods);
			return;
		}
		else
			giveFood();
	}

}

Trace::Trace(double x, double y, int food)
{
	if(food==NoFoods || food==Foods)
		trace.push_back(std::make_tuple(x, y, food));
	else
		trace.push_back(std::make_tuple(x, y, NoFoods));
}

void Trace::add(double x, double y)
{
	trace.push_back(std::make_tuple(x, y, NoFoods));
}

void Trace::changeMark(double x, double y, int trace_type)
{
	auto it = std::find(trace.begin(), trace.end(), std::make_tuple(x, y, NoFoods));
	if (it != trace.end())
	{
		*it = std::make_tuple(x, y, trace_type);
		return;
	}
	auto it2 = std::find(trace.begin(), trace.end(), std::make_tuple(x, y, Foods));
	if (it2 != trace.end())
		*it2 = std::make_tuple(x, y, trace_type);		
}

bool Trace::is_path_to_nest(double x, double y, int on_food) const
{
	auto position = std::find(trace.begin(), trace.end(), std::make_tuple(x, y, on_food));
	if (position != trace.begin() && position != trace.end())
		return true;
	else
		return false;
}

bool Trace::is_path_to_food(double x, double y, int on_food) const
{
	auto position = std::find(trace.begin(), trace.end(), std::make_tuple(x, y, on_food));
	if (position != trace.end() && position+1 != trace.end())
		return true;
	else
		return false;
}

std::pair<double, double> Trace::next(double x, double y, int on_food) const
{
	auto position = std::find(trace.begin(), trace.end(), std::make_tuple(x, y, on_food));
	if (position != trace.end() && position + 1 != trace.end())
		return std::make_pair(std::get<0>(*(position + 1)), (std::get<1>(*(position + 1))));
}

std::pair<double, double> Trace::previous(double x, double y, int on_food) const
{
	auto position = std::find(trace.begin(), trace.end(), std::make_tuple(x, y, on_food));
	if (position != trace.end() && position != trace.begin())
		return std::make_pair(std::get<0>(*(position - 1)), (std::get<1>(*(position - 1))));
}

void Trace::remove_last()
{
	trace.pop_back();
}

Trace::Trace(const Trace& tr)
{
	trace.assign(tr.trace.begin(), tr.trace.end());
	for (auto i = trace.begin(); i != trace.end(); i++)
		std::get<2>(*i) = NoFoods;
}

Trace& Trace::operator=(const Trace& tr)
{
	if (&tr == this)
		return *this;
	trace.assign(tr.trace.begin(), tr.trace.end());
	return *this;

}

double Food::reduceFood()
{
	if (count >= 5)
	{
		count -= 5;
		return 5.0;
	}
	else
	{
		double temp = count;
		count = 0;
		return temp;
	}
}

void Food::reSize()
{
	size++;
}

bool Food::atFood(double x_given, double y_given)
{
	if (x == x_given && y == y_given)
		return true;
	else
		return false;
}

void Nest::take(double count_given)
{
	count += count_given;
}

Ant::~Ant()
{
	delete traked_nest;
	delete traked_trace;
}

