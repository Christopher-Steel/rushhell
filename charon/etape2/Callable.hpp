#pragma once

#include <iostream>

/*
** Une classe callable pour tester l'�tape I.2
*/

class													Callable
{
public:
	Callable()
	{
	}

	~Callable()
	{
	}

	int													operator()()
	{
		std::cout << "callable" << std::endl;
		return (0);
	}
};