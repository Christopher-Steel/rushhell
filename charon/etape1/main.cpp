#include <cassert>
#include <iostream>

#include "FunctionSignature.hpp"

static int	zerofunction();
static int	onefunction(int);
static int	twofunction(int, int);
static int	threefunction(int, int, int);
static int	fourfunction(int, int, int, int);

/* Des tests pour l'étape I.1 */
int	main()
{
  FunctionSignature<int()>::type			f0 = &zerofunction;
  FunctionSignature<int(int)>::type			f1 = &onefunction;
  FunctionSignature<int(int, int)>::type		f2 = &twofunction;
  FunctionSignature<int(int, int, int)>::type		f3 = &threefunction;
  FunctionSignature<int(int, int, int, int)>::type	f4 = &fourfunction;

  assert(f0() == 0);
  assert(f1(1) == 1);
  assert(f2(1, 2) == 3);
  assert(f3(1, 2, 3) == 6);
  assert(f4(1, 2, 3, 4) == 10);
  std::cout << "FunctionSignature passed unit tests" << std::endl;
  return 0;
}

int	zerofunction()
{
  std::cout << "no parameters" << std::endl;
  return 0;
}

int	onefunction(int a)
{
  std::cout << a << std::endl;
  return a;
}

int	twofunction(int a, int b)
{
  std::cout << a << " " << b << std::endl;
  return a + b;
}

int	threefunction(int a, int b, int c)
{
  std::cout << a << " " << b << " " << c << std::endl;
  return a + b + c;
}

int	fourfunction(int a, int b, int c, int d)
{
  std::cout << a << " " << b << " " << c << " " << d << std::endl;
  return a + b + c + d;
}

int	fivefunction(int a, int b, int c, int d, int e)
{
  std::cout << a + b + c + d + e << std::endl;
  return a + b + c + d + e;
}
