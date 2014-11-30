#include "Edge.hpp"

const Edge	Edge::Lambda(-1);

Edge::Edge(void)
  : _c(0)
{

}

Edge::Edge(char c)
  : _c(c)
{

}

Edge::~Edge()
{

}

Edge::Edge(const Edge &rhs)
  : _c(rhs._c)
{

}

Edge&	Edge::operator=(const Edge &rhs)
{
  _c = rhs._c;
  return *this;
}

Edge&	Edge::operator=(char c)
{
  _c = c;
  return *this;
}

bool	Edge::operator==(const Edge &rhs) const
{
  return (_c == rhs._c);
}

bool	Edge::operator!=(const Edge &rhs) const
{
  return (_c != rhs._c);
}

bool	Edge::operator<(const Edge &rhs) const
{
  return (_c < rhs._c);
}

bool	Edge::operator>(const Edge &rhs) const
{
  return (_c > rhs._c);
}

bool	Edge::operator<=(const Edge &rhs) const
{
  return (_c <= rhs._c);
}

bool	Edge::operator>=(const Edge &rhs) const
{
  return (_c >= rhs._c);
}

bool	Edge::operator()(char c) const
{
  return (_c == c);
}

// DOT conversion
std::ostream	&operator<<(std::ostream &ostream, const Edge &edge)
{
  if (edge._c == -1) {
    ostream << "lambda";
  } else {
    ostream << edge._c;
  }
  return ostream;
}
