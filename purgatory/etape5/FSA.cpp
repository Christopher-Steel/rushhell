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
  , _acceptingStates(rhs._acceptingStates)
{

}

FSA		&FSA::operator=(const FSA &rhs)
{
  _states = rhs._states;
  _initialState = rhs._initialState;
  _currentState = rhs._currentState;
  _acceptingStates = rhs._acceptingStates;
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
  if (state.isFinal()) {
    _acceptingStates.insert(state.getName());
  }
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
  StateSetBuilder::iterator		itFound;
  std::set<Edge>::const_iterator	alphaIt;
  std::vector<std::string>		move;
  size_t				itOffset;

  State::resetNames();
  brick.marked = false;
  this->closure(_initialState, brick.states);
  brick.status = State::create();
  ssb.push_back(brick);
  while (true) {
    for (it = ssb.begin(); it != ssb.end(); ++it) {
      if (StateSetBuildingBrick::isUnmarked(*it)) {
	break;
      }
    }
    if (it == ssb.end()) {
      break;
    }
    (*it).marked = true;
    brick.marked = false;
    for (alphaIt = _alphabet.begin(); alphaIt != _alphabet.end(); ++alphaIt) {
      move.clear();
      this->move((*it).states, *alphaIt, move);
      brick.states.clear();
      this->closure(move, brick.states);
      std::sort(brick.states.begin(), brick.states.end());
      if ((itFound = std::find(ssb.begin(), ssb.end(), brick)) != ssb.end()) {
	it->status.link(*alphaIt, itFound->status.getName());
      } else {
	brick.status = State::create();
	itOffset = it - ssb.begin();
	ssb.push_back(brick);
	it = ssb.begin() + itOffset;
	it->status.link(*alphaIt, brick.status.getName());
      }
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

FSA		FSA::append(const FSA &lhs, const FSA &rhs)
{
  FSA		newFSA = lhs;
  std::string	appendix("a");
  std::set<std::string>::const_iterator		it;

  newFSA._appendixify(rhs, appendix);
  newFSA._redirectAcceptors(Edge::Lambda, lhs, rhs._initialState + appendix);
  for (it = rhs._acceptingStates.begin(); it != rhs._acceptingStates.end(); ++it) {
    newFSA._acceptingStates.insert(*it + appendix);
  }
  return newFSA;
}

FSA		FSA::merge(const FSA &lhs, const FSA &rhs, bool alternative)
{
  FSA		newFSA = lhs;
  std::string	appendix("m");
  State		init(newFSA._initialState + "_" + rhs._initialState + appendix);

  init.link(Edge::Lambda, lhs._initialState);
  init.link(Edge::Lambda, rhs._initialState + appendix);
  newFSA.addState(init);
  newFSA._appendixify(rhs, appendix);
  if (alternative) {
    State	acceptor("Final" + init.getName(), true);

    newFSA._redirectAcceptors(Edge::Lambda, newFSA, acceptor.getName());
    newFSA.addState(acceptor);
  }
  return newFSA;
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
  fsa.setInitialState(_initialState);
  return fsa;
}

void		FSA::_appendixify(const FSA &rhs, const std::string &appendix)
{
  std::map<std::string, State>::const_iterator	it;

  for (it = rhs._states.begin(); it != rhs._states.end(); ++it) {
    this->addState(it->second.cloneAppend(appendix));
  }
}

void		FSA::_redirectAcceptors(const Edge &edge, const FSA &source, const std::string &target)
{
  std::set<std::string>::const_iterator		it;

  for (it = source._acceptingStates.begin(); it != source._acceptingStates.end(); ++it) {
    State	&st = _states[*it];

    st.setFinal(false);
    st.link(edge, target);
  }
  _acceptingStates.clear();
}

// StateSetBuildingBrick

bool	FSA::StateSetBuildingBrick::operator==(const StateSetBuildingBrick &rhs) const
{
  return (states == rhs.states);
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
