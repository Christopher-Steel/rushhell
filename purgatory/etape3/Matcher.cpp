#include <iostream>
#include <string>

#include "Matcher.hpp"

Matcher::Matcher(FSA &fsa)
  : _fsa(fsa)
{

}

Matcher::~Matcher(void)
{

}

Matcher::Matcher(const Matcher &rhs)
  : _fsa(rhs._fsa)
{

}

bool	Matcher::find(const std::string &haystack)
{
  int	unused;

  return this->find(haystack, unused);
}

bool	Matcher::find(const std::string &haystack, int &count)
{
  count = 0;
  std::cout << "searching in a haystack => \"" << haystack << "\"" << std::endl;
  for (unsigned i = 0; i < haystack.length(); ++i) {
    std::cout << "testing edge " << haystack[i] << std::endl;
    if (_fsa.consumeEdge(haystack[i])) {
      ++count;
    }
  }
  return (count > 0);
}
