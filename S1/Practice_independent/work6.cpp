#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

double
sequence_sum(double *seq, size_t size)
{
  double sum = 0;
  for (size_t i = 0; i < size; ++i)
    sum += *(seq + i);

  return sum;
}

double
chunk_compute(double *seq, size_t seq_size, const size_t &chunk_size)
{
  if (chunk_size >= seq_size)
    return sequence_sum(seq, seq_size);

  double max_sum = 0,
         tmp_sum = 0;

  for(size_t i = 0; i < (seq_size - chunk_size); ++i)
  {
    tmp_sum = sequence_sum(seq + i, chunk_size);
    if (tmp_sum > max_sum)
      max_sum = tmp_sum;
  }

  return max_sum;
}

int
main()
{
  const size_t CHUNK_SIZE = 4;
  double arr[]{1, -1, 2, 3, -3, 5, 6, 7, -5};

  cout << chunk_compute(arr, sizeof(arr)/sizeof(*arr), CHUNK_SIZE);
  return 0;
}


