#ifndef	STATE_H
# define STATE_H

# include <map>
# include <set>
# include <string>
# include <vector>

# include "Edge.hpp"

class State
{
public:
  typedef std::vector<std::string>	LinkSet;

  State();
  State(const std::string &name, bool final = false);
  State(bool final);
  virtual ~State(void);
  State(const State &rhs);
  State			&operator=(const State &rhs);

  bool			operator==(const State &rhs) const;
  bool			operator!=(const State &rhs) const;
  bool			operator<(const State &rhs) const;
  bool			operator>(const State &rhs) const;
  bool			operator<=(const State &rhs) const;
  bool			operator>=(const State &rhs) const;

  const std::string	&operator[](const Edge &edge) const;

  const std::string	&getName(void) const;
  bool			isFinal(void) const;

  void			setName(const std::string &name);
  void			setFinal(bool final);

  void			link(const Edge &edge, const std::string &state);
  void			link(const Edge &edge, const State &state);

  LinkSet		&getLinks(const Edge &edge);
  const LinkSet		&getLinks(const Edge &edge) const;

  bool			hasEdge(const Edge &edge) const;

  void			fillAlphabet(std::set<Edge> &alphabet) const;

  static State		create(bool final = false);
  static void		resetNames(void);

private:
  friend std::ostream	&operator<<(std::ostream &, const State &);

  std::string		_name;
  bool			_final;

  std::map<Edge, LinkSet>	_links;

  static unsigned	_nameCounter;
};
std::ostream	&operator<<(std::ostream &ostream, const State &state);

#endif /* STATE_H */
