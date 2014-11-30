#include <algorithm>
#include <cassert>
#include <functional>
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
  state.fillAlphabet(_alphabet);
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

void		FSA::closure(const std::string &state, std::vector<std::string> &out) const
{
  std::map<std::string, State>::const_iterator	it;

  if ((it = _states.find(state)) == _states.end()) {
    return;
  }
  out.push_back(state);
  if (it->second.hasEdge(Edge::Lambda)) {
    const State::LinkSet	&links = it->second.getLinks(Edge::Lambda);

    for (unsigned i = 0; i < links.size(); ++i) {
      this->closure(links[i], out);
    }
  }
}

void		FSA::closure(std::vector<std::string> &out) const
{
  this->closure(_currentState, out);
}

void		FSA::closure(const std::vector<std::string> &states, std::vector<std::string> &out) const
{
  for (unsigned i = 0; i < states.size(); ++i) {
    this->closure(states[i], out);
  }
}

void		FSA::move(const std::string &state, const Edge &edge,
			  std::vector<std::string> &out) const
{
  std::map<std::string, State>::const_iterator	it;

  if ((it = _states.find(state)) == _states.end()) {
    return;
  }
  if (it->second.hasEdge(edge)) {
    const State::LinkSet	&links = it->second.getLinks(edge);

    out.insert(out.end(), links.begin(), links.end());
  }
}

void		FSA::move(const std::vector<std::string> &closure,
			  const Edge &edge, std::vector<std::string> &out) const
{
  for (unsigned i = 0; i < closure.size(); ++i) {
    this->move(closure[i], edge, out);
  }
}

void		FSA::move(const Edge &edge, std::vector<std::string> &out) const
{
  std::vector<std::string>	closure;

  this->closure(_currentState, closure);
  this->move(closure, edge, out);
}

FSA		FSA::subset(void) const
{
  StateSetBuilder			ssb;
  StateSetBuildingBrick			brick;
  StateSetBuilder::iterator		it;
  std::set<Edge>::const_iterator	alphaIt;
  std::vector<std::string>		move;

  State::resetNames();
  brick.marked = false;
  this->closure(_initialState, brick.states);
  ssb.push_back(brick);
  while ((it = std::find_if(ssb.begin(), ssb.end(), StateSetBuildingBrick::isUnmarked))
	 != ssb.end()) {
    it->marked = true;
    brick.marked = false;
    for (alphaIt = _alphabet.begin(); alphaIt != _alphabet.end(); ++alphaIt) {
      move.clear();
      this->move(it->states, *alphaIt, move);
      brick.states.clear();
      this->closure(move, brick.states);
      if (std::find(ssb.begin(), ssb.end(), brick) == ssb.end()) {
	brick.status = State::create();
	ssb.push_back(brick);
      }
      it->status.link(*alphaIt, brick.status.getName());
    }
  }
  return this->_generateFromBuilder(ssb);
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

FSA		FSA::_generateFromBuilder(StateSetBuilder& builder) const
{
  std::map<std::string, State>::const_iterator	it;

  FSA		fsa;
  unsigned	st;

  for (unsigned i = 0; i < builder.size(); ++i) {
    StateSetBuildingBrick	&brick = builder[i];

    for (st = 0; st < brick.states.size(); ++st) {
      if ((it = _states.find(brick.states[st])) == _states.end()) {
	continue;
      }
      if (it->second.isFinal()) {
	brick.status.setFinal(true);
	break;
      }
    }
    fsa.addState(brick.status);
  }
  return fsa;
}

// StateSetBuildingBrick

bool	FSA::StateSetBuildingBrick::operator==(const StateSetBuildingBrick &rhs) const
{
  return ((status.getName() == rhs.status.getName() && status.getName() != "")
	  || states == rhs.states);
}

bool	FSA::StateSetBuildingBrick::isUnmarked(const StateSetBuildingBrick &brick)
{
  return !brick.marked;
}

// DOT conversion
std::ostream	&operator<<(std::ostream &ostream, const FSA &fsa)
{
  std::map<std::string, State>::const_iterator	it;

  ostream << "digraph G {" << std::endl;
  for (it = fsa._states.begin(); it != fsa._states.end(); ++it) {
    ostream << it->second;
  }
  ostream << "}" << std::endl;
  return ostream;
}
