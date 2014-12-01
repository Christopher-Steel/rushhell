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

void	buildNFA(FSA &nfa)
{
  // Building NFA for expression (a|b)*abb
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
    nfa.addState(states[i]);
  }
  nfa.setInitialState(std::string("S0"));
}

void	testClosureAndMove(FSA &nfa)
{
  // Closure test
  std::vector<std::string>	closure;
  nfa.closure(closure);
  assert(closure.size() == 5);
  assert(closure[0] == "S0");
  assert(closure[1] == "S1");
  assert(closure[2] == "S2");
  assert(closure[3] == "S4");
  assert(closure[4] == "S7");
  // Move test
  std::vector<std::string>	move;
  nfa.move(closure, Edge('a'), move);
  assert(move.size() == 2);
  assert(move[0] == "S3");
  assert(move[1] == "S8");
  std::cout << "Move passed unit tests" << std::endl;
  closure.clear();
  nfa.closure(move, closure);
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

bool	openOFStream(const std::string path, std::ofstream &ofs)
{
  ofs.open(path.c_str());
  if (ofs) {
    return true;
  }
  std::cerr << "Failed to open output file " << path << std::endl;
  return false;
}

void	testDOT(FSA &fsa)
{
  std::ofstream	ofs;

  if (!openOFStream("./matcher.dot", ofs)) {
    std::cout << "DOT conversion could not pass tests" << std::endl;
    return;
  }
  ofs << fsa;
}

void	testNFAtoDFADOT(FSA &nfa, FSA &dfa)
{
  std::ofstream	ofs;

  if (!openOFStream("./NFA.dot", ofs)) {
    std::cout << "NFA creation could not pass tests" << std::endl;
    return;
  }
  ofs << nfa;
  // Conversion to DFA
  ofs.close();
  if (!openOFStream("./DFA.dot", ofs)) {
    std::cout << "DFA creation could not pass tests" << std::endl;
    return;
  }
  dfa = nfa.subset();
  ofs << dfa;
}

void	testAppend(FSA &lhs, FSA &rhs, FSA &appended)
{
  std::ofstream ofs;

  if (!openOFStream("./appended.dot", ofs)) {
    std::cout << "Append could not pass tests" << std::endl;
    return;
  }
  appended = FSA::append(lhs, rhs);
  ofs << appended;
  std::cout << "Append passed tests" << std::endl;
}

void	testMerge(FSA &lhs, FSA &rhs, bool methodTwo, FSA &merged)
{
  std::ofstream ofs;
  std::string	path = (!methodTwo ? "./merged1.dot" : "./merged2.dot");

  if (!openOFStream(path, ofs)) {
    std::cout << "Merge one could not pass tests" << std::endl;
    return;
  }
  merged = FSA::merge(lhs, rhs, methodTwo);
  ofs << merged;
  std::cout << "Merge passed tests" << std::endl;
}

int	main(void)
{
  FSA	fsa;
  FSA	nfa;
  FSA	dfa;
  FSA	appended;
  FSA	merged1;
  FSA	merged2;
  FSA	lexernfa;
  FSA	lexerdfa;

  unitTestEdge();
  unitTestState();
  unitTestFSA();
  unitTestFSAGenerate(fsa);
  unitTestMatcher(fsa);
  testDOT(fsa);
  buildNFA(nfa);
  testClosureAndMove(nfa);
  testNFAtoDFADOT(nfa, dfa);
  testAppend(fsa, dfa, appended);
  testMerge(dfa, fsa, false, merged1);
  testMerge(dfa, fsa, true, merged2);
  return 0;
}
