#include <vector>
#include <algorithm>
#include <iostream>

std::size_t iParent(std::size_t n) { return (n-1)/2; }

std::size_t iLeftChild(std::size_t n) { return 2*n+1;}

std::size_t iRightChild(std::size_t n) { return 2*n+2;}

template <typename T>
void siftDown(std::vector<T> &data, std::size_t start,
	      std::size_t end) {
  std::size_t root = start, child;
  while ((child = iLeftChild(root)) <= end) {
    std::size_t swap(root);

    if (data[swap] < data[child])
      swap = child;
    if (child+1 <= end && data[swap] < data[child+1])
      swap = child + 1;
    if (swap == root)
      return;
    else {
      std::swap(data[root], data[swap]);
      root = swap;
    }
  }
}

template <typename T>
void heapify(std::vector<T> &data, std::size_t count) {
  long long start = iParent(count-1);
  
  while (start >= 0)
    siftDown<T>(data, start--, count-1);
}

template <typename T>
void heapsort(std::vector<T> &data) {
  heapify(data, data.size());

  std::size_t end = data.size() - 1;
  while (end > 0) {
    std::swap(data[0], data[end--]);
    siftDown(data, 0, end);
  }
}


int main(int argc, char *argv[]) {
  std::vector<int> data{10, 8, 5 , 4, -12};
  heapsort(data);
  for_each(data.begin(), data.end(), [](auto &x) {
      std::cout << x << " ";
    });
  std::cout << "\b\n" << std::flush;
  return 0;
}
