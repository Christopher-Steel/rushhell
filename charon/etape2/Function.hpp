#pragma once

/* BASE CLASS */

template <typename T>
class				Function : public T
{
public:
	Function()
	{
	}

	Function(const T &rhs)
	{
	}

	Function					&operator=(const T &rhs)
	{
		return (*this);
	}
};

/* 0 ARG SPECIALIZATION */

template <typename T>
class				Function <T(void)>
{
	struct			IFunctor
	{
		virtual T operator()() = 0;
		virtual ~IFunctor() {}
	};

	template <typename F>
	struct			Functor : public IFunctor
	{
		F			func;

		Functor(F _func) : func(_func)
		{
		}
		virtual T	operator()()
		{
			return (func());
		}
	};

public:
	template <typename F>
	Function(F f)
	{
		func = new Functor<F>(f);
	}
	~Function()
	{
		delete (func);
	}
	T				operator()()
	{
		return ((*func)());
	};

private:
	IFunctor		*func;
};

/* 1 ARG SPECIALIZATION */

template <typename T, typename U>
class				Function <T(U)>
{
	struct			IFunctor
	{
		virtual T operator()(U) = 0;
		virtual ~IFunctor() {}
	};

	template <typename F>
	struct			Functor : public IFunctor
	{
		F			func;

		Functor(F _func) : func(_func)
		{
		}
		virtual T	operator()(U arg)
		{
			return (func(arg));
		}
	};

public:
	template <typename F>
	Function(F f)
	{
		func = new Functor<F>(f);
	}
	~Function()
	{
		delete (func);
	}
	T				operator()(U arg)
	{
		return ((*func)(arg));
	};

private:
	IFunctor		*func;
};

/* 2 ARGS SPECIALIZATION */

template <typename T, typename U, typename V>
class				Function <T(U, V)>
{
	struct			IFunctor
	{
		virtual T operator()(U, V) = 0;
		virtual ~IFunctor() {}
	};

	template <typename F>
	struct			Functor : public IFunctor
	{
		F			func;

		Functor(F _func) : func(_func)
		{
		}
		virtual T	operator()(U arg1, V arg2)
		{
			return (func(arg1, arg2));
		}
	};

public:
	template <typename F>
	Function(F f)
	{
		func = new Functor<F>(f);
	}
	~Function()
	{
		delete (func);
	}
	T				operator()(U arg1, V arg2)
	{
		return ((*func)(arg1, arg2));
	};

private:
	IFunctor		*func;
};

/* 3 ARGS SPECIALIZATION */

template <typename T, typename U, typename V, typename W>
class				Function <T(U, V, W)>
{
	struct			IFunctor
	{
		virtual T operator()(U, V, W) = 0;
		virtual ~IFunctor() {}
	};

	template <typename F>
	struct			Functor : public IFunctor
	{
		F			func;

		Functor(F _func) : func(_func)
		{
		}
		virtual T	operator()(U arg1, V arg2, W arg3)
		{
			return (func(arg1, arg2, arg3));
		}
	};

public:
	template <typename F>
	Function(F f)
	{
		func = new Functor<F>(f);
	}
	~Function()
	{
		delete (func);
	}
	T				operator()(U arg1, V arg2, W arg3)
	{
		return ((*func)(arg1, arg2, arg3));
	};

private:
	IFunctor		*func;
};

/* 4 ARGS SPECIALIZATION */

template <typename T, typename U, typename V, typename W, typename X>
class				Function <T(U, V, W, X)>
{
	struct			IFunctor
	{
		virtual T operator()(U, V, W, X) = 0;
		virtual ~IFunctor() {}
	};

	template <typename F>
	struct			Functor : public IFunctor
	{
		F			func;

		Functor(F _func) : func(_func)
		{
		}
		virtual T	operator()(U arg1, V arg2, W arg3, X arg4)
		{
			return (func(arg1, arg2, arg3, arg4));
		}
	};

public:
	template <typename F>
	Function(F f)
	{
		func = new Functor<F>(f);
	}
	~Function()
	{
		delete (func);
	}
	T				operator()(U arg1, V arg2, W arg3, X arg4)
	{
		return ((*func)(arg1, arg2, arg3, arg4));
	};

private:
	IFunctor		*func;
};