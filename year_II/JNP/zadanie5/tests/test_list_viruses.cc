#include "../virus_genealogy.h"
#include <cassert>
#include <vector>
#include <random>
#include <limits>


class Virus {
  public:
    typedef  int id_type;
    Virus(id_type const &_id) : id(_id) {
    }
    id_type get_id() const {
      return id;
    }
  private:
    id_type id;
};

int main() {
    const int LIST_LENGTH = 10000;
    const int ROOT = -1;
    VirusGenealogy<Virus> gen(ROOT);
    Virus::id_type const id = gen.get_stem_id();

    for (int i = 0; i < LIST_LENGTH; i++) {
        gen.create(i, i-1);
    }

    assert(id == ROOT);

    // sanity checks
    for (int i = ROOT; i < LIST_LENGTH-1; i++) {
        assert(gen.get_children(i).size() == 1);
        assert(gen.get_parents(i+1).size() == 1);
        assert(gen.exists(i));
    }
    // random VirusNotFound
    for (int i = 0; i<100; i++) {
        bool caught = false;

        try {
            std::default_random_engine generator;
            std::uniform_int_distribution<int>
                dist(LIST_LENGTH + 1, std::numeric_limits<int>::max());
            gen[dist(generator)];
        } catch (VirusNotFound & e) {
            caught = true;
        }

        assert(caught);
    }

    for (int i = 0; i<100; i++) {
        bool caught = false;

        try {
            std::default_random_engine generator;
            std::uniform_int_distribution<int>
                dist(LIST_LENGTH + 1, std::numeric_limits<int>::max());
            gen.remove(dist(generator));
        } catch (VirusNotFound & e) {
            caught = true;
        }

        assert(caught);
    }

    gen.remove(3);

    try {
        gen[3];
        gen[4];
        // Exception should be thrown before
        assert(false);
    } catch (VirusNotFound & e) {
    }

    try {
        gen.connect(0, 3);
        // Exception should be thrown before
        assert(false);
    } catch (VirusNotFound & e) {
    }

    try {
        gen.connect(2, 0);
    } catch (VirusNotFound & e) {
        // Exception should not be thrown before
        assert(false);
    }

    gen.remove(1);
    try {
        gen[2];
    } catch (VirusNotFound & e) {
        // Exception should be not thrown before
        assert(false);
    }

    gen.remove(0);

    try {
        gen[2];
    } catch (VirusNotFound & e) {
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
