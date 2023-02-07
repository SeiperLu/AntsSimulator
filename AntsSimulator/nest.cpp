#include "nest.h"
#include <iostream>
void Nest::Take(double Count_Given)
{
	Count += Count_Given;
	std::cout << "Jedzenie odebrane!\n";
}