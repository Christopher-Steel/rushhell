#ifndef	FSA_H
# define FSA_H

# include <map>
# include <set>
# include <vector>

# include "State.hpp"

class FSA
{
public:
  enum Type
    {
      NFA,
      DFA
    };

  FSA();
  virtual ~FSA(void);
  FSA(const FSA &rhs);
  FSA		&operator=(const FSA &rhs);

  State		&operator[](const std::string &name);

  void		addState(const State &state);
  void		setInitialState(const std::string &state);
  void		setInitialState(const State &state);

  const std::string	&getInitialState(void) const;
  const std::string	&getState(void) const;

  void		closure(const std::string &state, std::vector<std::string> &out) const;
  void		closure(std::vector<std::string> &out) const;
  void		closure(const std::vector<std::string> &states, std::vector<std::string> &out) const;

  void		move(const std::string &state, const Edge &edge,
		     std::vector<std::string> &out) const;
  void		move(const std::vector<std::string> &closure,
		     const Edge &edge, std::vector<std::string> &out) const;
  void		move(const Edge &edge, std::vector<std::string> &out) const;

  FSA		subset(void) const;

  bool		consumeEdge(char c);
  bool		consumeEdge(const Edge &edge);

  void		reset(void);

  static FSA	generateFromNeedle(const std::string &needle);

private:
  struct StateSetBuildingBrick
  {
    bool			marked;
    std::vector<std::string>	states;
    State			status;

    bool		operator==(const StateSetBuildingBrick &rhs) const;

    static bool		isUnmarked(const StateSetBuildingBrick &brick);
  };
  typedef std::vector<StateSetBuildingBrick>	StateSetBuilder;
  friend std::ostream &operator<<(std::ostream &, const FSA &);

  FSA		_generateFromBuilder(StateSetBuilder& builder) const;

  std::map<std::string, State>	_states;
  std::set<Edge>		_alphabet;
  std::string			_initialState;
  std::string			_currentState;
};
std::ostream	&operator<<(std::ostream &ostream, const FSA &fsa);

#endif /* FSA_H */
