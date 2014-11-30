#ifndef	EDGE_H
# define EDGE_H

# include <iostream>

class Edge
{
public:
  Edge(void);
  Edge(char c);
  virtual ~Edge(void);
  Edge(const Edge &rhs);
  Edge	&operator=(const Edge &rhs);
  Edge	&operator=(char c);

  bool	operator==(const Edge &rhs) const;
  bool	operator!=(const Edge &rhs) const;
  bool	operator<(const Edge &rhs) const;
  bool	operator>(const Edge &rhs) const;
  bool	operator<=(const Edge &rhs) const;
  bool	operator>=(const Edge &rhs) const;

  bool	operator()(char c) const;

  static const Edge	Lambda;

private:
  friend std::ostream	&operator<<(std::ostream &, const Edge &);

  char	_c;
};
std::ostream	&operator<<(std::ostream &ostream, const Edge &edge);

#endif /* EDGE_H */
