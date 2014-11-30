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
  std::map<Edge, std::string>::const_iterator it;

  if ((it = _links.find(edge)) != _links.end()) {
    return it->second;
  }
  throw std::out_of_range("Edge not found in Status");
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
  _links[edge] = state;
}

void			State::link(const Edge &edge, const State &state)
{
  _links[edge] = state.getName();
}

void			State::unlink(const Edge &edge)
{
  _links.erase(edge);
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
