#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int main() {
  long long res;
  string desc;
  getline(cin, desc);
  char prev = '\0';
  long long lowest = desc.length();
  long long dist = 0;

  for (char c : desc) {
    dist++;
    if (c == '*')
      continue;
    if (prev == '\0') {
      prev = c;
      dist = 0;
      continue;
    }
    if (c != prev) {
      lowest = min({dist, lowest});
      prev = c;
      dist = 0;
    } else {
      dist = 0;
    }
  }

  res = desc.length() - lowest + 1;
  cout << res << endl;
  
  return 0;
}
