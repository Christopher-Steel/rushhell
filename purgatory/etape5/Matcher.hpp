#ifndef	MATCHER_H
# define MATCHER_H

# include "FSA.hpp"

class Matcher
{
public:
  Matcher(FSA &fsa);
  virtual ~Matcher(void);
  Matcher(const Matcher& rhs);

  bool		find(const std::string &haystack);
  bool		find(const std::string &haystack, int &count);

private:
  FSA		&_fsa;
};

#endif /* MATCHER_H */
