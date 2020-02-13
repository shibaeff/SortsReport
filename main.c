/*
 * compare sorts on random arrays
 * Sorting methods implemented:
 *  - simple insertion sort
 *  - recursive QSort
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

/*
 * Generate random double
 */
enum {
    BOUND = 10000
};
double
rand_double (void)
{
  double r = rand () % BOUND;
  double q = (double) 1 / (rand () % BOUND);
  // return some pseudorandom int + some pseudorandom fraction
  return r + q;
}

/*
 * Generates a sorted array of 0.0, 1.0, 2.0, ... n - 1
 */
double *
generate_sorted (size_t n)
{
  double *arr = (double *) malloc (sizeof (double) * n);
  for (size_t i = 0; i < n; ++i)
    {
      arr[i] = (double) n - i - 1;
    }
  return arr;
}

/*
 * Generates a reverse sorted array of n - 1, n - 2, ..., 0.0
 */
double *
generate_reverse (size_t n)
{
  double *arr = (double *) malloc (sizeof (double) * n);
  for (size_t i = 0; i < n; ++i)
    {
      arr[i] = i;
    }
  return arr;
}

/*
 * Generates random array of size t
 */
double *
generate_random (size_t n)
{
  double *arr = (double *) malloc (sizeof (double) * n);
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
double *
deep_copy (const double *src, size_t n)
{
  double *dest = (double *) malloc (sizeof (double) * n);
  for (size_t i = 0; i < n; ++i)
    {
      dest[i] = src[i];
    }
  return dest;
}

void
print_arr (double *arr, size_t n)
{
  for (size_t i = 0; i < n; ++i)
    {
      printf ("%f ", arr[i]);
    }
  printf ("\n");
}

/*
 * Returns true if absolute value does NOT increase
 */
bool
cmp (double other, double another)
{
  return fabs (other) >= fabs (another);
}

/*
 * Returns true, if doubles are really close
 */
const double EPS = 0.00000005;
double max (double a, double b)
{
  return (a > b) ? a : b;
}


/*
 * Relative comparison of two duobles
 */
bool
equals (double other, double another)
{
  double a = fabs (other);
  double b = fabs (another);

  double scale = max (a, b) * EPS;

  return fabs (other - another) <= scale;
}


/*
 * Simple check for trivial case when src is sorted
 */
bool
check_trivial (const double *src, const double *sorted, size_t n)
{
  for (size_t i = 0; i < n; ++i)
    {
      if (src[i] != sorted[i])
        return false;
    }
  return true;
}

/*
 * Just the same but for the reversed src case
 */
bool
check_reverse (const double *src, const double *sorted, size_t n)
{
  for (size_t i = 0; i < n; ++i)
    {
      if (!equals (src[i], sorted[n - i - 1]))
        return false;
    }
  return true;
}

bool
permucheck(double *src, double *sorted, size_t n)
{
  for (size_t i = 0; i < n; ++i)
    {
      int skip = false;
      for (size_t j = 0; j < n; ++j)
        {
          if (equals (src[i], sorted[j]))
            {
              skip = 1;
              break;
            }
        }
      if (!skip)
        return false;
    }
  return true;
}

bool
is_sorted(double *sorted, size_t n)
{
  for (size_t i = 1; i < n; ++i)
    {
      if (!cmp(sorted[i - 1], sorted[i]))
        return false;
    }
  return true;
}
/*
 * Assures whether the src array is the permutation of sorted's elements && and sorted is actually sorted
 */
bool
check_correct (double *src, double *sorted, size_t n)
{
  return is_sorted (sorted, n) && permucheck (src, sorted, n);
}

// HELPER FUNCTIONS FOR SORTING
size_t gNumSwaps = 0;
/*
 * Swap values and increase counter
 */
void
swap_with_count (double *other, double *another)
{
  double temp = *other;
  *other = *another;
  *another = temp;
  ++gNumSwaps;
}

/*
 *  Compare two numbers and increase counter
 */
// total number of comparisons
size_t gNumCmp = 0;
bool
cmp_with_count (double other, double another)
{
  ++gNumCmp;
  return cmp (other, another);
}

double
avg (const int arr[], int n)
{
  double res = 0.0;
  for (int i = 0; i < n; ++i)
    res += arr[i];
  return res / n;
}

/*
 * print stats for specific case
 */
void
print_case (int size, int swaps[], int cmps[])
{
  printf ("Printing stats for size %d\n", size);
  printf ("Number of swaps %d, %d, %d, %d ---> %f\n", swaps[0], swaps[1],
          swaps[2], swaps[3], avg (swaps, 4));
  printf ("Number of comparisons %d, %d, %d, %d ---> %f\n", cmps[0], cmps[1],
          cmps[2], cmps[3], avg (cmps, 4));
}

/*
 * Sets global parameters to zero
 */
void
zero (void)
{
  gNumSwaps = gNumCmp = 0;
}

/*
 * Handles all the assessment routine
 */
// sizes of arrays to be tested
const int kRanges[] = {10, 100, 1000, 10000};
void
driver (void (*method) (double*, size_t,
                        bool (*) (double, double),
                        void (*) (double *, double *)
))
{
  int cmps[4];
  int swaps[4];

  for (int i = 0; i < 4; ++i)
    {
      int sz = kRanges[i];

      // Trivial
      double *src = generate_sorted (sz);
      double *old = deep_copy (src, sz);
      method (src, sz, cmp_with_count, swap_with_count);
      assert (check_trivial (old, src, sz));
      cmps[0] = gNumCmp;
      swaps[0] = gNumSwaps;
      free (src);
      free (old);
      zero ();

      // Reversed
      src = generate_reverse (sz);
      old = deep_copy (src, sz);
      method (src, sz, cmp_with_count, swap_with_count);
      assert (check_reverse (old, src, sz));
      cmps[1] = gNumCmp;
      swaps[1] = gNumSwaps;
      free (src);
      free (old);
      zero ();

      // Random 1
      src = generate_random (sz);
      old = deep_copy (src, sz);
      method (src, sz, cmp_with_count, swap_with_count);
      assert (check_correct (old, src, sz));
      cmps[2] = gNumCmp;
      swaps[2] = gNumSwaps;
      free (src);
      free (old);
      zero ();

      //  Random 2
      src = generate_random (sz);
      old = deep_copy (src, sz);
      method (src, sz, cmp_with_count, swap_with_count);
      assert(check_correct (old, src, sz));
      cmps[3] = gNumCmp;
      swaps[3] = gNumSwaps;
      free (src);
      free (old);
      zero ();

      print_case (sz, swaps, cmps);
    }
}

/*
 * Simple selection sort
 */
void
selection (double *arr, size_t sz,
           bool (*cmp) (double, double),
           void (*swap) (double *, double *)
)
{
  for (size_t i = 0; i < sz; ++i)
    {
      size_t min = i;
      for (size_t j = i + 1; j < sz; ++j)
        {
          if (cmp(arr[j], arr[min]))
            min = j;
        }
      if (min != i)
        {
          swap (arr + i, arr + min);
        }
    }
}

/*
 * Partition for the quicksort
 */
size_t
partition(double* arr, size_t l, size_t r, bool (*cmp)(double, double), void (*swap)(double*, double*))
{
  size_t pivot_i = rand() % (r - l + 1);
  swap_with_count (arr + r, arr + pivot_i);
  size_t i = l;
  double pivot = arr[r];
  for (size_t j = l; j <= r; ++j)
    {
      if (!cmp(arr[i], pivot)) {
        swap(arr + i, arr + j);
        ++i;
      }
    }
  swap(arr + i, arr + r);
  return i;
}

void
quick_sort_r(double* arr, size_t l, size_t r, bool (*cmp)(double, double), void (*swap)(double*, double*))
{
  if (l < r)
    {
      size_t p = partition (arr, l, r, cmp, swap);
      quick_sort_r (arr, l, p - 1, cmp, swap);
      quick_sort_r (arr, p + 1, r, cmp, swap);
    }
}

void quick_sort(double *arr, size_t sz,
                bool (*cmp) (double, double),
                void (*swap) (double *, double *)
)
{
  quick_sort_r (arr, 0, sz - 1, cmp, swap);
}

int
main (void)
{
  srand (1);
  printf("Performing runs for SELECTION SORT\n");
  driver(selection);
  printf("COMPLETE!\n");

  printf("Performing runs for RECURSIVE QSORT\n");
  driver(quick_sort);
  printf("COMPLETE!\n");
  return 0;
}