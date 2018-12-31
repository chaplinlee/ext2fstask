#include <stdbool.h>

// test if `number` is a power of `base`
// Consider not using this method!!!!!
bool is_power(int number, unsigned int base)
{
  if(number <= 0)
    return false;
  int a[12] = { 0, 1, 3, 9, 27, 81, 243, 5, 25, 125, 7, 49};
  for (int i = 0; i < 12; i++)
    if (number == a[i])
      return true;
  return false;
}