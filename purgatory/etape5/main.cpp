#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
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
    // this is supposed to throw but if it doesn't the assert will fail
    assert(s0[e2] == "");
  } catch (const std::out_of_range &oor) {
    (void)oor;
  }
  assert(s1[e2] == s2.getName());
  std::cout << "State passed unit tests" << std::endl;
}

void	unitTestFSA(void)
{
  FSA	fsa;
  State state("FIRST", true);

  try {
    // this is supposed to throw but if it doesn't the assert will fail
    assert(fsa["S0"].getName() == "S1");
  } catch (const std::out_of_range &oor) {
    (void)oor;
  }
  // testing a single final state FSA
  fsa.addState(state);
  assert(fsa["FIRST"].getName() == "FIRST");
  fsa.setInitialState(state);
  assert(fsa.getInitialState() == "FIRST");
  assert(fsa.getState() == "FIRST");
  assert(fsa.consumeEdge('c') == false);
  assert(fsa.getState() == "FIRST");
  assert(fsa.consumeEdge('d') == false);
  assert(fsa.getState() == "FIRST");
  // testing a three state FSA
  State	state2("SECOND", false);
  State	state3("THIRD", true);

  fsa["FIRST"].setFinal(false);
  fsa["FIRST"].link(Edge('c'), state2);
  state2.link(Edge('d'), state3);
  fsa.addState(state2);
  fsa.addState(state3);
  fsa.reset();
  assert(fsa.getState() == "FIRST");
  assert(fsa.consumeEdge('c') == false);
  assert(fsa.getState() == "SECOND");
  assert(fsa.consumeEdge('d') == true);
  assert(fsa.getState() == "THIRD");
  std::cout << "FSA passed unit tests" << std::endl;
}

void	unitTestFSAGenerate(FSA &fsa)
{
  std::string	needle("needle");

  // this function resets State's internal counter for unique state names
  State::resetNames();
  fsa = FSA::generateFromNeedle(needle);
  try {
    assert(fsa["S0"]['n'] == "S1");
    assert(fsa["S1"]['e'] == "S2");
    assert(fsa["S2"]['e'] == "S3");
    assert(fsa["S3"]['d'] == "S4");
    assert(fsa["S4"]['l'] == "S5");
    assert(fsa["S5"]['e'] == "S6");
    assert(fsa["S6"].isFinal());
    assert(fsa.getInitialState() == "S0");
    assert(fsa.getState() == "S0");
  } catch (const std::out_of_range &oor) {
    std::cerr << oor.what() << std::endl << "FSA improperly generated, aborting..." << std::endl;
    abort();
  }
  std::cout << "FSA generation passed unit tests" << std::endl;
}

void	unitTestMatcher(FSA &fsa)
{
  std::string	haystack("qereneederqrrrqerneqwrwrwerneedlqewrqrwrneedlerqereeeneedlemqrrqwerneedle");
  Matcher	matcher(fsa);
  int		matches;

  assert(matcher.find(haystack, matches));
  assert(matches == 3);

  std::cout << "Matcher passed unit tests" << std::endl;
}

void	testNFAtoDFA(void)
{
  // Building NFA for expression (a|b)*abb
  FSA		fsa;
  State		states[11];
  unsigned	i;

  State::resetNames();
  for (i = 0; i < 11; ++i) {
    states[i] = State::create((i == 10));
    std::cout << "Created state " << states[i].getName() << std::endl;
  }
  states[0].link(Edge::Lambda, std::string("S1"));
  states[0].link(Edge::Lambda, std::string("S7"));
  states[1].link(Edge::Lambda, std::string("S2"));
  states[1].link(Edge::Lambda, std::string("S4"));
  states[2].link('a', std::string("S3"));
  states[3].link(Edge::Lambda, std::string("S6"));
  states[4].link('b', std::string("S5"));
  states[5].link(Edge::Lambda, std::string("S6"));
  states[6].link(Edge::Lambda, std::string("S1"));
  states[6].link(Edge::Lambda, std::string("S7"));
  states[7].link('a', std::string("S8"));
  states[8].link('b', std::string("S9"));
  states[9].link('b', std::string("S10"));
  for (i = 0; i < 11; ++i) {
    fsa.addState(states[i]);
  }
  fsa.setInitialState(std::string("S0"));
  // Closure test
  std::vector<std::string>	closure;
  fsa.closure(closure);
  assert(closure.size() == 5);
  assert(closure[0] == "S0");
  assert(closure[1] == "S1");
  assert(closure[2] == "S2");
  assert(closure[3] == "S4");
  assert(closure[4] == "S7");
  // Move test
  std::vector<std::string>	move;
  fsa.move(closure, Edge('a'), move);
  assert(move.size() == 2);
  assert(move[0] == "S3");
  assert(move[1] == "S8");
  std::cout << "Move passed unit tests" << std::endl;
  closure.clear();
  fsa.closure(move, closure);
  assert(closure.size() == 7);
  std::sort(closure.begin(), closure.end());
  assert(closure[0] == "S1");
  assert(closure[1] == "S2");
  assert(closure[2] == "S3");
  assert(closure[3] == "S4");
  assert(closure[4] == "S6");
  assert(closure[5] == "S7");
  assert(closure[6] == "S8");
  std::cout << "Closure passed unit tests" << std::endl;
}

void	testDOT(FSA &fsa)
{
  std::string	path("./matcher.dot");
  std::ofstream	ofs(path.c_str());

  if (!ofs) {
    std::cerr << "Failed to open output file " << path << std::endl;
    std::cout << "DOT conversion could not pass tests" << std::endl;
    return;
  }
  ofs << fsa;
}

void	testNFAtoDFADOT(void)
{
  std::string	path("./NFA.dot");
  std::ofstream	ofs(path.c_str());

  if (!ofs) {
    std::cerr << "Failed to open output file " << path << std::endl;
    std::cout << "NFA creation could not pass tests" << std::endl;
    return;
  }
  // Building NFA for expression (a|b)*abb
  FSA		fsa;
  State		states[11];
  unsigned	i;

  State::resetNames();
  for (i = 0; i < 11; ++i) {
    states[i] = State::create((i == 10));
    std::cout << "Created state " << states[i].getName() << std::endl;
  }
  states[0].link(Edge::Lambda, std::string("S1"));
  states[0].link(Edge::Lambda, std::string("S7"));
  states[1].link(Edge::Lambda, std::string("S2"));
  states[1].link(Edge::Lambda, std::string("S4"));
  states[2].link('a', std::string("S3"));
  states[3].link(Edge::Lambda, std::string("S6"));
  states[4].link('b', std::string("S5"));
  states[5].link(Edge::Lambda, std::string("S6"));
  states[6].link(Edge::Lambda, std::string("S1"));
  states[6].link(Edge::Lambda, std::string("S7"));
  states[7].link('a', std::string("S8"));
  states[8].link('b', std::string("S9"));
  states[9].link('b', std::string("S10"));
  for (i = 0; i < 11; ++i) {
    fsa.addState(states[i]);
  }
  fsa.setInitialState(std::string("S0"));
  ofs << fsa;
  // Conversion to DFA
  ofs.close();
  path = "DFA.dot";
  ofs.open(path.c_str());
  if (!ofs) {
    std::cerr << "Failed to open output file " << path << std::endl;
    std::cout << "DFA creation could not pass tests" << std::endl;
    return;
  }
  FSA dfa = fsa.subset();
  ofs << dfa;
}

int	main(void)
{
  FSA	fsa;

  unitTestEdge();
  unitTestState();
  unitTestFSA();
  unitTestFSAGenerate(fsa);
  unitTestMatcher(fsa);
  testDOT(fsa);
  testNFAtoDFA();
  testNFAtoDFADOT();
  return 0;
}
