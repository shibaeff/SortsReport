/**
 * @brief
 * compare sorts on random arrays
 * Sorting methods implemented:
 *  - simple insertion sort
 *  - recursive QSort
 */

// UNCOMMENT TO GET SPECIFIC MODES INCLUDED IN THE BUILD
#define RUN_WITH_PRINT
// #define RUN_WITH_STATS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <time.h>

enum {
    BOUND = 10000
};

/**
 * @brief
 * Generate random double
 */
double
rand_double (void)
{
  // return ((double) rand()) / RAND_MAX;
  // return (double) BOUND + rand () / ((double) RAND_MAX / (2 * BOUND));
  double r = rand () % BOUND;
  r *= (rand () % 2) ? -1 : 1;
  double q = (double) 1 / (rand () % BOUND);
  // return some pseudorandom int + some pseudorandom fraction
  return r + q;
}


/**
 * @brief
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

/**
 * @brief
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

/**
 * @brief
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

/**
 * @brief
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

/**
 * @brief
 * Returns true if absolute value does NOT increase
 */
bool
cmp (double other, double another)
{
  return fabs (other) >= fabs (another);
}

/**
 * Returns true, if doubles are really close
 */
const double EPS = 0.00000005;
double max (double a, double b)
{
  return (a > b) ? a : b;
}


/** @brief
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


/** @brief
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

/** @brief
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
permucheck (double *src, double *sorted, size_t n)
{
  bool *marks = malloc (n * sizeof (bool));
  for (size_t j = 0; j < n; ++j)
    {
      for (size_t i = 0; i < n; ++i)
        {
          if (equals (src[i], sorted[j]) && !marks[j])
            {
              marks[j] = 1;
              break;
            }
        }
    }
  for (size_t i = 0; i < n; ++i)
    {
      if (!marks[i])
        return false;
    }
  free (marks);
  return true;
}

bool
is_sorted (double *sorted, size_t n)
{
  for (size_t i = 1; i < n; ++i)
    {
      if (!cmp (sorted[i - 1], sorted[i]))
        return false;
    }
  return true;
}
/** @brief
 * Assures whether the src array is the permutation of sorted's elements && and sorted is actually sorted
 */
bool
check_correct (double *src, double *sorted, size_t n)
{
  return is_sorted (sorted, n) && permucheck (src, sorted, n);
}

// HELPER FUNCTIONS FOR SORTING
size_t gNumSwaps = 0;
/** @brief
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

/** @brief
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

/** @brief
 * print stats for specific case
 */
enum {
    NCASES = 4
};
void
print_case (int size, int swaps[], int cmps[])
{
  printf ("Printing stats for size %d\n", size);
  printf ("Number of swaps %d & %d & %d & %d & %f\n", swaps[0], swaps[1],
          swaps[2], swaps[3], avg (swaps, NCASES));
  printf ("Number of comparisons %d & %d & %d & %d & %f\n", cmps[0], cmps[1],
          cmps[2], cmps[3], avg (cmps, NCASES));
}

/** @brief
 * Sets global parameters to zero
 */
void
zero (void)
{
  gNumSwaps = gNumCmp = 0;
}

/** @brief
 * Handles all the assessment routine
 */
// sizes of arrays to be tested
const int kRanges[] = {10, 100, 1000, 10000, 100000};
void
driver (void (*method) (double *, size_t,
                        bool (*) (double, double),
                        void (*) (double *, double *)
))
{
  int cmps[NCASES];
  int swaps[NCASES];

  for (int i = 0; i < NCASES; ++i)
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

/** @brief
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
          if (cmp (arr[j], arr[min]))
            min = j;
        }
      if (min != i)
        {
          swap (arr + i, arr + min);
        }
    }
}

/** @brief
 * Partition for the quicksort
 */
size_t
partition (double *arr, size_t l, size_t r, bool (*cmp) (double, double), void (*swap) (double *, double *))
{
  size_t i = l;
  size_t j = r + 1;

  double v = arr[i];
  for (;;)
    {
      while (cmp (arr[++i], v))
        {
          if (i == r)
            break;
        }
      while (cmp (v, arr[--j]))
        {
          if (j == l)
            break;
        }
      if (i >= j)
        break;
      swap (arr + i, arr + j);
    }
  swap (arr + l, arr + j);
  return j;
}

void
quick_sort_r (double *arr, int l, int r, bool (*cmp) (double, double), void (*swap) (double *, double *))
{
  if (l < r)
    {
      int p = partition (arr, l, r, cmp, swap);
      quick_sort_r (arr, l, p - 1, cmp, swap);
      quick_sort_r (arr, p + 1, r, cmp, swap);
    }
}

void quick_sort (double *arr, size_t sz,
                 bool (*cmp) (double, double),
                 void (*swap) (double *, double *)
)
{
  quick_sort_r (arr, 0, sz - 1, cmp, swap);
}

void
run_with_print (void)
{
  double *arr = generate_random (kRanges[0]);
  print_arr (arr, kRanges[0]);
  selection (arr, kRanges[0], cmp, swap_with_count);
  print_arr (arr, kRanges[0]);
  free (arr);

  arr = generate_random (kRanges[0]);
  print_arr (arr, kRanges[0]);
  quick_sort (arr, kRanges[0], cmp, swap_with_count);
  print_arr (arr, kRanges[0]);
  free (arr);
}

int
main (void)
{
  srand(time(0));
#ifdef RUN_WITH_PRINT
  run_with_print ();
#endif

#ifdef RUN_WITH_STATS
  printf ("Performing runs for SELECTION SORT\n");
  driver (selection);
  printf ("COMPLETE!\n");

  printf ("Performing runs for RECURSIVE QSORT\n");
  driver (quick_sort);
  printf ("COMPLETE!\n");
#endif

  return 0;
}