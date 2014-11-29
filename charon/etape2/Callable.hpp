#pragma once

#include <iostream>

/*
** Une classe callable pour tester l'étape I.2
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