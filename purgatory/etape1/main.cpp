#include <cassert>
#include <iostream>

#include "Machine.hpp"

static std::string	g_alphabet("mechant");

bool	moveAppend(eState& current, std::string& built, unsigned cursor)
{
  assert(cursor < g_alphabet.length());
  built.push_back(g_alphabet[cursor]);
  current = gStateTable[current][cursor];
  return false;
}

bool	haltReset(eState& current, std::string& built, unsigned cursor)
{
  (void)cursor;
  current = S0;
  built = "";
  return true;
}

bool	actionError(eState& current, std::string& built, unsigned cursor)
{
  (void)cursor;
  current = S0;
  built = "";
  return false;
}

bool	do_action(eState& current, std::string& built, unsigned cursor)
{
  bool		(*actions[])(eState&, std::string&, unsigned) =
    {
      &moveAppend,
      &haltReset,
      &actionError
    };
  eAction	action = gActionTable[current][cursor];
  bool		validChar = (action == MA);

  while ((actions[action])(current, built, cursor)) {
    if ((action = gActionTable[current][cursor]) == MA) {
      validChar = true;
    }
  }
  return validChar;
}

int	search_for_string(const std::string& str)
{
  eState		currentState = S0;
  std::string		builtString;
  unsigned		alphaCursor;

  for (unsigned i = 0; i < str.length(); ++i) {
    for (alphaCursor = 0; alphaCursor < g_alphabet.length(); ++alphaCursor) {
      if (g_alphabet[alphaCursor] == str[i]) {
	break;
      }
    }
    if (do_action(currentState, builtString, alphaCursor)) {
      std::cout << "\033[36m";
    }
    std::cout << str[i] << "\033[0m";
    if (currentState == S7) {
      std::cout << "\033[32m!\033[0m";
    }
  }
  std::cout << std::endl;
  return 0;
}

void	print_intro(void)
{
  std::cout << "+---------------------------------------------------------------------+" << std::endl
	    << "| Welcome to test_fsa ! Please consider the following information :   |" << std::endl
	    << "|  - a white character is a non recognized character => ACTION_ERROR  |" << std::endl
	    << "|  - a cyan character means the state machine accepted it => MA       |" << std::endl
	    << "|  - \033[32m!\033[0m a green bang means the state machine completed => HR           |" << std::endl
	    << "+---------------------------------------------------------------------+" << std::endl
	    << std::endl;
}

int	main(int ac, char *av[])
{
  if (ac > 1) {
    print_intro();
    return search_for_string(std::string(av[1]));
  }
  return 1;
}
