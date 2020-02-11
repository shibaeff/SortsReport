/*
 * compare sorts on random arrays
 * Sorting methods implemented:
 *  - simple insertion sort
 *  - recursive QSort
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

/*
 * Generate random double
 */
enum {BOUND = 10000};
double
rand_double(void)
{
  double r = rand () % BOUND;
  double q = (double) 1 / (rand() % BOUND);
  // return some pseudorandom int + some pseudorandom fraction
  return r + q;
}

/*
 * Generates a sorted array of 0.0, 1.0, 2.0, ... n - 1
 */
double*
generate_sorted(size_t n)
{
  double* arr = (double*) malloc(sizeof(double) * n);
  for (size_t i = 0; i < n; ++i)
    {
      arr[i] = i;
    }
  return arr;
}

/*
 * Generates a reverse sorted array of n - 1, n - 2, ..., 0.0
 */
double*
generate_reverse(size_t n)
{
  double* arr = (double*) malloc(sizeof(double) * n);
  for (size_t i = 0; i < n; ++i)
    {
      arr[i] = (double)n - i;
    }
  return arr;
}

/*
 * Generates random array of size t
 */
double*
generate_random(size_t n)
{
  double* arr = (double*) malloc(sizeof(double) * n);
  for (size_t i = 0; i < n; ++i)
    {
      arr[i] = rand_double ();
    }
  return arr;
}

/*
 * Allocates memory and copies the src array to it.
 * Returns array pointer
 */
double*
deep_copy(const double *src, size_t n)
{
  double *dest = (double*) malloc(sizeof(double) * n);
  for (size_t i = 0; i < n; ++i)
    {
      dest[i] = src[i];
    }
  return dest;
}

void
print_arr(double *arr, size_t n)
{
  for (size_t i = 0; i < n; ++i)
    {
      printf("%f ", arr[i]);
    }
  printf("\n");
}



int
main (void)
{
  srand ((unsigned)clock());
  print_arr (generate_sorted (10), 10);
  print_arr (generate_reverse (10), 10);
  print_arr (generate_random (10), 10);
  return 0;
}