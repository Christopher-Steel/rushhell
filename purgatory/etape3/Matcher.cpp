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
  for (unsigned i = 0; i < haystack.length(); ++i) {
    if (_fsa.consumeEdge(haystack[i])) {
      ++count;
    }
  }
  return (count > 0);
}
