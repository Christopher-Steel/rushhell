#ifndef	STATE_H
# define STATE_H

# include <map>
# include <string>

# include "Edge.hpp"

class State
{
public:
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

  void			unlink(const Edge &edge);

  static State		create(bool final = false);

private:
  std::string		_name;
  bool			_final;

  std::map<Edge, std::string>	_links;

  static unsigned	_nameCounter;
};

#endif /* STATE_H */
