#ifndef	FSA_H
# define FSA_H

# include <map>

# include "State.hpp"

class FSA
{
public:
  FSA();
  virtual ~FSA(void);
  FSA(const FSA& rhs);
  FSA		&operator=(const FSA &rhs);

  const State	&operator[](const std::string &name) const;

  void		addState(const State &state);
  void		setInitialState(const std::string &state);
  void		setInitialState(const State &state);

  const std::string	&getInitialState(void) const;
  const std::string	&getState(void) const;

  bool		consumeEdge(char c);
  bool		consumeEdge(const Edge &edge);

  static FSA	generateFromNeedle(const std::string &needle);

private:
  std::map<std::string, State>	_states;
  std::string			_initialState;
  std::string			_currentState;
};

#endif /* FSA_H */
