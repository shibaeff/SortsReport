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
#include <stdbool.h>
#include <math.h>

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
      arr[i] = i;
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
      arr[i] = (double) n - i - 1;
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

  volatile double scale = max (a, b) * EPS;

  return fabs (other - another) <= scale;
}

struct tree {
    struct tree *l, *r;
    size_t count; // the number of times key was encountered
    double data;
};

typedef struct tree tree;

void
insert (tree *root, double val)
{
  if (!root)
    {
      root = malloc (sizeof (tree));
      root->data = val;
      return;
    }

  if (equals (root->data, val))
    {
      ++root->count;
      return;
    }
  if (cmp (root->data, val))
    {
      insert (root->l, val); // go left if predicate is true
    }
  else
    {
      insert (root->r, val); // go right else
    }
}

/*
 * If values is NOT in the tree or counter is 0, return -1. Some error was encounter.
 * If counter is >=1, decrease is by 1. Return current count
 */
int
present (tree *root, double val)
{
  if (equals (root->data, val))
    {
      return --root->count;
    }
  if (cmp (root->data, val))
    {
      return present (root->l, val);
    }
  return present (root->r, val);
}


tree*
min_node(tree* root)
{
  if (!root->l)
    return root;
  return min_node (root->l);
}
/*
 * Deletes nodes from the binary tree
 */
tree *
delete (tree *root, double val)
{
  if (!root)
    return root;

  if (cmp (root->data, val))
    {
      if (!root->l)
        {
          tree *temp = root->r;
          free (root);
          return temp;
        }
      else if (!root->r)
        {
          tree *temp = root->l;
          free(root);
          return temp;
        }
      tree* temp = min_node(root->r);
      root->data = temp->data;
      root->r = delete (root->r, temp->data);
    }
  else if (cmp (root->data, val))
    root->l = delete (root->l, val);
  else
    root->r = delete (root->r, val);
  return root;
}

/*
 * Simple check for trivial case when src is sorted
 */
bool
check_trivial(const double *src, const double *sorted, size_t n)
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
check_reverse(const double *src, const double *sorted, size_t n)
{
  for (size_t i = 0; i < n; ++i)
    {
      if (!equals (src[i], sorted[n - i - 1]))
        return false;
    }
  return true;
}


/*
 * Assures whether the src array is the permutation of sorted's elements
 */
bool
check_correct (double *src, double *sorted, size_t n)
{
  // construct a tree from the src elements;
  tree *t = 0;
  for (size_t i = 0; i < n; ++i)
    {
      insert (t, src[i]);
    }
   // remove elements one by one
    for (size_t i = 0; i < n; ++i)
      {
        int r = present (t, sorted[i]);
        if (r == 0)
          {
            delete(t, sorted[r]);
          }
      }
  return !t;
}

int
main (void)
{
  double *a1 = generate_random (9);
  double *a2 = deep_copy (a1, 9);
  printf("%d", check_correct (a2, a1, 9));
  return 0;
}