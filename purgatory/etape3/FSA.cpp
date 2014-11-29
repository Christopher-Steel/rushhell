#include <iostream>
#include <stdexcept>

#include "FSA.hpp"

FSA::FSA()
{

}

FSA::~FSA(void)
{

}

FSA		&FSA::operator=(const FSA &rhs)
{
  _states = rhs._states;
  _initialState = rhs._initialState;
  _currentState = rhs._currentState;
  return *this;
}

const State	&FSA::operator[](const std::string &name) const
{
  std::map<std::string, State>::const_iterator it;

  if ((it = _states.find(name)) != _states.end()) {
    return it->second;
  }
  throw std::out_of_range("Status not found in FSA");
}

void		FSA::addState(const State &state)
{
  _states[state.getName()] = state;
}

void		FSA::setInitialState(const std::string &state)
{
  std::map<std::string, State>::iterator it;

  if ((it = _states.find(state)) != _states.end()) {
    _initialState = state;
    _currentState = state;
  }
}

void		FSA::setInitialState(const State &state)
{
  this->setInitialState(state.getName());
}

const std::string	&FSA::getInitialState(void) const
{
  return _initialState;
}

const std::string	&FSA::getState(void) const
{
  return _currentState;
}

bool		FSA::consumeEdge(char c)
{
  Edge		edge(c);

  return this->consumeEdge(edge);
}

bool		FSA::consumeEdge(const Edge &edge)
{
  bool		reset = false;

  try {
    _currentState = _states[_currentState][edge];
    std::cout << "Switched to state " << _currentState << std::endl;
    if (_states[_currentState].isFinal()) {
      std::cout << "it's a final state" << std::endl;
      _currentState = _initialState;
      reset = true;
    }
  } catch (const std::out_of_range &oor) {
    _currentState = _initialState;
  }
  return reset;
}

FSA		FSA::generateFromNeedle(const std::string &needle)
{
  FSA		fsa;
  State		prev;
  State		current = State::create();

  fsa.setInitialState(current);
  for (unsigned i; i < needle.length(); ++i) {
    prev = current;
    current = State::create((i == needle.length() - 1));
    prev.link(Edge(needle[i]), current);
    fsa.addState(prev);
  }
  return fsa;
}
