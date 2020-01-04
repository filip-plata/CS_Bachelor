#include <assert.h>
#include <tuple>
#include <cstdio>
#include <vector>
#include <climits>
#define main notmain
#include "../citizen.h"
#include "../smalltown.h"
#include "../monster.h"
#undef main

#define M Mummy<int>
#define V Vampire<int>
#define Z Zombie<int>

#define MX(x, y) Mummy<int>(x, y)
#define ZX(x, y) Zombie<int>(x, y)
#define VX(x, y) Vampire<int>(x, y)

#define A Adult<int>
#define S Sheriff<int>

#define AX(x) Adult<int>(x, 20)
#define SX(x, y) Sheriff<int>(x, 20, y)

using namespace std;

#define MAX 1000

vector<int> alive;

bool isFib(int v) {
  int a = 0, b = 1, tmp;

  while (a <= v) {
    if (a == v) return true;
    tmp = a + b;
    a = b;
    b = tmp;
  }

  return false;
}

void test(string s) {
  printf("Test %s\n", s.c_str());
}

void end() {
  printf("Passed");
}

void nextRound(string s) {
  static int count = 0;
  count++;
  
  printf("\nROUND %d\n\n%s\n", count, s.c_str());
}

void monsterTest() {
  Mummy<int> mummy(10, 10);
  Vampire<float> vampire(5.0f, 1.0f);
  Zombie<char> zombie(5, 20);
}

void citizenTest() {
  test("CitizenTest");
  Teenager<int> teenager(10, 12);
  Adult<float> adult(80.0f, 50.0f);
  Sheriff<char> sheriff(50, 20, 10);
  end();
}

/*void makeAlives(int cnt, int start, int max, int tick, int[] kills) {
  
  }*/

void townTest() {
  printf("TownTest\n");

  nextRound("Monster kills everyone. \"MONSTER WON\" once.");

  auto town = SmallTown<M, int, 1, 8, A, A, A>(
					       M(5, 5),
					       AX(10),
					       AX(15),
					       AX(20)
  );
  int alive[] = {3, 2, 1, 1, 0};
  for (int i = 0; i < 3; i++) {
    town.tick(1);

    auto status = town.getStatus();
    fprintf(stderr, "Monster health: %d, citizens alive: %d\n",
	    (int)get<1>(status), (int)get<2>(status));
    assert(get<1>(status) == 5);
    assert(get<2>(status) == alive[i]);
  }

  town.tick(1);
    
  printf("\nROUND 2\n\nMonster is killed after first round but kills one of the citizens (not first in the list) should display \"CITIZEN WON\" twice.\n");
  auto town2 = SmallTown<M, int, 1, 8, S, A, A, A>(
					       M(5, 5),
					       SX(10, 100),
					       AX(5),
					       AX(15),
					       AX(20)
  );
  
  // Monster is killed in first round, 
  for (int i = 2; i >= 0; i--) {
    town2.tick(1);

    auto status = town2.getStatus();
    fprintf(stderr, "Monster health: %d, citizens alive: %d\n",
	    (int)get<1>(status), (int)get<2>(status));
    assert(get<1>(status) == 0);
    assert(get<2>(status) == 3);
  }

  printf("\nROUND 3\n\nMonster kills citizens in ~22 rounds, fibonnacci test\n");

  auto town3 = SmallTown<M, int, 1, 50, A, A, A, A, A, A, A>(
					       M(1, 1),
					       AX(1),
					       AX(2),
					       AX(3),
					       AX(4),
					       AX(5),
					       AX(6),
					       AX(7)				     
  );
  // Alive citizens count depending on fibo.
  int alives[] = {7, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    for (int i = 1; i < 22; i++) {
    town3.tick(1);
    fprintf(stderr, "Hour %d\n", i);

    auto status = town3.getStatus();
    fprintf(stderr, "Monster health: %d, citizens alive: %d\n",
	    (int)get<1>(status), (int)get<2>(status));
    assert(get<1>(status) == 1);
    assert(get<2>(status) == alives[i]);
  }

    // Print MONSTER WON
  town3.tick(1);

  nextRound("One citizen should be alive, fibo test");
  
  auto town4 = SmallTown<M, int, 10, 100, A, A, A, A, A, A>(
					       M(1, 1),
					       AX(1),
					       AX(2),
					       AX(3),
					       AX(4),
					       AX(5),
					       AX(6)			     	       );

  for (int i = 10; i < 99; i++) {
    town4.tick(1);
    fprintf(stderr, "Hour %d\n", i);

    auto status = town4.getStatus();
    assert(get<1>(status) == 1);
  }

  auto status = town4.getStatus();
  assert(get<2>(status) == 1);
  
  town4.tick(1);
  
}
void fiboTest() {
  test("FiboTest");
  const int num = 45;
  //printf("Big fibo: %d\n", fibonacci<num>());
  
  end();
}

int main() {
  monsterTest();
  citizenTest();
  fiboTest();
  townTest();
}
