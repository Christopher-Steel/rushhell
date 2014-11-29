#ifndef	EDGE_H
# define EDGE_H

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

private:
  char	_c;
};

#endif /* EDGE_H */
