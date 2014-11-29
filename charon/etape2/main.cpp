#include <cassert>

#include <boost/bind.hpp>

#include "FunctionSignature.hpp"
#include "Function.hpp"
#include "Callable.hpp"

static int	zerofunction();
static int	onefunction(int);
static int	charfunction(char);

/* Des tests pour l'étape I.2 */
int main(void)
{
  Callable		callable;
  Function<int(int)>	f1(&onefunction);
  Function<int(int)>	f2 = &onefunction;
  Function<int()>	f3(&zerofunction);
  Function<int()>	f4 = boost::bind(&onefunction, 150);
  Function<int(char)>	cf = boost::bind(&charfunction, _1);

  assert(f1(50) == 50);
  assert(f2(100) == 100);
  assert(f3() == 0);
  assert(f4() == 150);
  assert(cf('c') == 'c');

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

int	charfunction(char c)
{
  std::cout << c << std::endl;
  return c;
}
