#pragma once

/*
** ETAPE I.2
** Ca marche pour les pointeurs sur fonction
** J'ai pas encore trouve pour les callables
** J'ai pas testé avec boost::bind
*/

// base class

template <typename T>
class		Function : public T
{

};

// specialisation pour 0 argument (void)

template <typename T>
class		Function <T()>
{
  typedef T	(*type)();

public:
  Function(type func)
  {
    _func = func;
  }

  ~Function()
  {
  }

  Function	&operator=(type func)
  {
    _func = func;
    return (*this);
  }

  T		operator()()
  {
    return (_func());
  }
private:
  type		_func;
};

// specialisation pour 1 argument

template <typename T, typename U>
class							Function <T(U)>
{
  typedef T						(*type)(U);

public:
  Function(type func)
  {
    _func = func;
  }

  ~Function()
  {
  }

  Function					&operator=(type func)
  {
    _func = func;
    return (*this);
  }

  T							operator()(U arg)
  {
    return (_func(arg));
  }
private:
  type						_func;
};
