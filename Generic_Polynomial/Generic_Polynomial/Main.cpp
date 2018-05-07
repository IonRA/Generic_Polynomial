#include <iostream>
#include <stdexcept>
#include "Polynomial.h"

int main()
{
	int i;
	polynomial<double> p, p2;
	irred_polynomial<int> q;
	pairs<double> x, y;
	try
	{
		std::cin >> q;
		std::cout << q;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	std::cin >> i;
	return 0;
}