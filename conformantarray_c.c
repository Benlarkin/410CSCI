#include <stdlib.h>

void inner(size_t n, int arr[]) {
  for (int i = 0; i < n; i++)
    arr[i] = 7;
}

void main() {
  int a[100];
  inner(100, a);
}
