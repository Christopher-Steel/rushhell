#include <cassert>
#include <iostream>
#include <stdexcept>

#include "Edge.hpp"
#include "State.hpp"
#include "FSA.hpp"
#include "Matcher.hpp"

void	unitTestEdge(void)
{
  Edge	e;
  Edge	e2('2');
  Edge	e3(e2);

  e = '1';
  assert(e('1'));
  e = e2;
  assert(e('2'));
  assert(e == e2);
  assert(e != Edge('3'));
  assert(e <= e2);
  assert(e3 >= e2);
  e3 = '3';
  assert(e3 > e2);
  assert(e2 < e3);
  std::cout << "Edge passed unit tests" << std::endl;
}

void	unitTestState(void)
{
  State	s;
  State sNamed(std::string("name"));
  State sNamedFinal("name", true);
  State sFinal("", true);

  /*
  ** Constructor and getter tests
  */
  assert(s.getName() == "");
  assert(s.isFinal() == false);

  assert(sNamed.getName() == "name");
  assert(sNamed.isFinal() == false);

  assert(sNamedFinal.getName() == "name");
  assert(sNamedFinal.isFinal());

  assert(sFinal.getName() == "");
  assert(sFinal.isFinal() == true);

  /*
  ** Comparison tests
  */
  assert(s == sFinal);
  assert(sNamed == sNamedFinal);
  assert(s < sNamed);
  assert(sNamedFinal > sFinal);
  assert(s >= sFinal);
  assert(s <= sFinal);

  /*
  ** Create tests
  */
  State s0 = State::create();
  State s1 = State::create();
  State s2 = State::create(true);

  assert(s0.getName() == "S0");
  assert(s1.getName() == "S1");
  assert(s2.getName() == "S2");

  /*
  ** link/unlink and access tests
  */
  Edge	e1('c');
  Edge	e2('4');

  s0.link(e1, s1.getName());
  s1.link(e2, s2);
  assert(s0[e1] == s1.getName());
  try {
    assert(s0[e2] == "");
  } catch (const std::out_of_range &oor) {
  }
  assert(s1[e2] == s2.getName());
  s1.unlink(e2);
  try {
    assert(s1[e2] == "");
  } catch (const std::out_of_range &oor) {
  }
  std::cout << "State passed unit tests" << std::endl;
}

void	unitTestFSAandMatcher(void)
{
  std::string	haystack("qereneederqrrrqerneqwrwrwerneedlqewrqrwrneedlerqereeeneedlemqrrqwerneedle");
  std::string	needle("needle");
  FSA		fsa = FSA::generateFromNeedle(needle);
  Matcher	matcher(fsa);
  int		matches;

  assert(matcher.find(haystack, matches));
  assert(matches == 3);
}

int	main(void)
{
  unitTestEdge();
  unitTestState();
  unitTestFSAandMatcher();
  return 0;
}
