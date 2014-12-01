#include <sstream>
#include <stdexcept>
#include <string>

#include "State.hpp"

unsigned		State::_nameCounter = 0;

State::State()
  : _name("")
  , _final(false)
{

}

State::State(const std::string &name, bool final)
  : _name(name)
  , _final(final)
{

}

State::~State(void)
{

}

State::State(const State &rhs)
  : _name(rhs._name)
  , _final(rhs._final)
  , _links(rhs._links)
{

}

State&			State::operator=(const State &rhs)
{
  _name = rhs._name;
  _final = rhs._final;
  _links = rhs._links;
  return *this;
}

bool			State::operator==(const State &rhs) const
{
  return (_name == rhs._name);
}

bool			State::operator!=(const State &rhs) const
{
  return (_name != rhs._name);
}

bool			State::operator<(const State &rhs) const
{
  return (_name < rhs._name);
}

bool			State::operator>(const State &rhs) const
{
  return (_name > rhs._name);
}

bool			State::operator<=(const State &rhs) const
{
  return (_name <= rhs._name);
}

bool			State::operator>=(const State &rhs) const
{
  return (_name >= rhs._name);
}

const std::string	&State::operator[](const Edge &edge) const
{
  return this->getLinks(edge)[0];
}

const std::string	&State::getName(void) const
{
  return _name;
}

bool			State::isFinal(void) const
{
  return _final;
}

void			State::setName(const std::string &name)
{
  _name = name;
}

void			State::setFinal(bool final)
{
  _final = final;
}

void			State::link(const Edge &edge, const std::string &state)
{
  _links[edge].push_back(state);
}

void			State::link(const Edge &edge, const State &state)
{
  this->link(edge, state.getName());
}

State::LinkSet		&State::getLinks(const Edge &edge)
{
  std::map<Edge, LinkSet>::iterator it;

  if ((it = _links.find(edge)) != _links.end()) {
    return it->second;
  }
  throw std::out_of_range("Edge not found in Status");
}

const State::LinkSet	&State::getLinks(const Edge &edge) const
{
  std::map<Edge, LinkSet>::const_iterator it;

  if ((it = _links.find(edge)) != _links.end()) {
    return it->second;
  }
  throw std::out_of_range("Edge not found in Status");
}

bool			State::hasEdge(const Edge &edge) const
{
  return (_links.find(edge) != _links.end());
}

void			State::fillAlphabet(std::set<Edge> &alphabet) const
{
  std::map<Edge, State::LinkSet>::const_iterator	it;

  for (it = _links.begin(); it != _links.end(); ++it) {
    if (it->first(-1) == false) {
      alphabet.insert(it->first);
    }
  }
}

State			State::cloneAppend(const std::string &suffix) const
{
  State			st(_name + suffix, _final);
  std::map<Edge, LinkSet>::const_iterator	it;
  LinkSet::const_iterator			linkIt;

  for (it = _links.begin(); it != _links.end(); ++it) {
    const LinkSet	&states = it->second;

    for (linkIt = states.begin(); linkIt != states.end(); ++linkIt) {
      st.link(it->first, *linkIt + suffix);
    }
  }
  return st;
}

State			State::create(bool final)
{
  std::stringstream	ss;

  ss << "S" << _nameCounter;
  ++_nameCounter;
  return State(ss.str(), final);
}

void			State::resetNames(void)
{
  _nameCounter = 0;
}

// DOT conversion

std::ostream	&operator<<(std::ostream &ostream, const State &state)
{
  std::map<Edge, State::LinkSet>::const_iterator	it;
  unsigned	i;

  if (state._final) {
    ostream << "  " << state._name  << " [style=\"filled\" color=\"red\"]"<< std::endl;
  }
  for (it = state._links.begin(); it != state._links.end(); ++it) {
    for (i = 0; i < it->second.size(); ++i) {
      ostream << "  " << state._name << " -> " << it->second[i]
	      << " [label=\"" << it->first << "\"];" << std::endl;
    }
  }
  return ostream;
}
