#include <cassert>
#include <stdexcept>

#include "FSA.hpp"

FSA::FSA()
{

}

FSA::~FSA(void)
{

}

FSA::FSA(const FSA &rhs)
  : _states(rhs._states)
  , _initialState(rhs._initialState)
  , _currentState(rhs._currentState)
{

}

FSA		&FSA::operator=(const FSA &rhs)
{
  _states = rhs._states;
  _initialState = rhs._initialState;
  _currentState = rhs._currentState;
  return *this;
}

State		&FSA::operator[](const std::string &name)
{
  std::map<std::string, State>::iterator it;

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

  assert((it = _states.find(state)) != _states.end());
  _initialState = state;
  _currentState = state;
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

  assert(_initialState.empty() == false);
  assert(_currentState.empty() == false);
  try {
    if (_states[_currentState].isFinal()) {
      _currentState = _initialState;
    }
    _currentState = _states[_currentState][edge];
    if (_states[_currentState].isFinal()) {
      reset = true;
    }
  } catch (const std::out_of_range &oor) {
    _currentState = _initialState;
    (void)oor;
    // if the state was reset because of an invalid edge it is possible
    // that the edge that was used was invalid at the currentState but
    // is valid for the reset state. if this isn't tested for then we
    // wouldn't be able to detect "cd" in "dccdd"
    try {
      _currentState = _states[_currentState][edge];
    } catch (const std::out_of_range &oor2) {
      (void)oor2;
    }
  }
  return reset;
}

void		FSA::reset(void)
{
  _currentState = _initialState;
}

FSA		FSA::generateFromNeedle(const std::string &needle)
{
  FSA		fsa;
  State		prev;
  State		current = State::create();

  for (unsigned i = 0; i < needle.length(); ++i) {
    prev = current;
    current = State::create((i == needle.length() - 1));
    prev.link(Edge(needle[i]), current);
    fsa.addState(prev);
    if (i == 0) {
      fsa.setInitialState(prev);
    }
  }
  fsa.addState(current);
  return fsa;
}
