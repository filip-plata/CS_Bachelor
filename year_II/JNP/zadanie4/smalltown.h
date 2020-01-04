#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include "monster.h"
#include "citizen.h"
#include <tuple>
#include <typeinfo>
#include <cstddef>
#include <utility>
#include <cxxabi.h>
#include <string>

// For each functions to iterate over tuple.
template <typename Tuple, typename F, std::size_t... Indices>
void for_each_impl(Tuple &&tuple, F &&f, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void)swallow{1, (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(),
                      int{})...};
}

template <typename Tuple, typename F> void for_each(Tuple &&tuple, F &&f) {
    constexpr std::size_t N =
        std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                  std::make_index_sequence<N>{});
}

// tail recursion, so constructing aray of fibonnaci numbers
// of size N will be N^2. Since N <= 100 most probably, it is ok
template <typename U> constexpr U fibonacci(U n, U val = 1, U prev = 0) {
    U ret = 0;
    if (n == 0) {
        ret = prev;
    } else {
        if (n == 1) {
            ret = val;
        } else {
            ret = fibonacci(n - 1, val + prev, val);
        }
    }
    return ret;
}

template <typename U> constexpr auto fibLimit(U time) {
    U limit = 0;
    while (fibonacci(limit) < time) {
        limit++;
    }
    return limit;
}

template <typename M, typename U, U t0, U t1, typename... C> class SmallTown {
    static_assert(t0 <= t1, "Time is wrong");
    static_assert(std::is_arithmetic<U>::value, "No arithemtic type");

private:
    // array of calculated length to fill
    typedef std::array<U, fibLimit(t1) + 1> A;

    template <U... i> static constexpr A fs() { return A{{fibonacci(i)...}}; }

    // Filling an array with Fibonacci numbers.
    template <U...> struct S;

    template <U... i> struct S<0, i...> {
        static constexpr A gs() { return fs<0, i...>(); }
    };

    template <U i, U... j> struct S<i, j...> {
        static constexpr A gs() { return S<i - 1, i, j...>::gs(); }
    };

    using myFibArray = S<fibLimit(t1)>;
    M _monster;
    U _time;
    long long _alive;
    std::tuple<C...> _citizens;
    long long _numberAlive() {
        long long alive = 0;
        for_each(_citizens, [&alive](auto &citizen) {
            if (citizen.getHealth() > 0) {
                alive++;
            }
        });
        return alive;
    }

    bool _checkIfFib(U time) {
        int i = 0;
        while (myFibArray::gs()[i] <= time) {
            if (time == myFibArray::gs()[i])
                return true;
            i++;
        }
        return false;
    }

public:
    SmallTown(M monster, C... args)
        : _monster(monster), _time(t0), _citizens(args...) {
        _alive = this->_numberAlive();
    };

    auto getStatus() {
        return std::make_tuple(_monster.getName(),
            _monster.getHealth(),
            _alive);
    }

    void tick(U timeStep) {
        bool alive = this->_alive > 0;
        bool monster = _monster.getHealth() > 0;

        if (alive && !monster) {
            std::cout << "CITIZENS WON" << std::endl;
        }

        else if (!alive && monster) {
            std::cout << "MONSTER WON" << std::endl;
        }

        else if (!alive && !monster) {
            std::cout << "DRAW" << std::endl;
        }

        else {
            if (_checkIfFib(_time) && monster) {
                for_each(_citizens, [this](auto &citizen) {
                    if (citizen.getHealth() > 0) {
                        attack(this->_monster, citizen);
                        if (citizen.getHealth() == 0) {
                            this->_alive--;
                        }
                    }
                });
            }
        }
        _time += timeStep;
        if (_time < 0) { // overflow
            U max = std::numeric_limits<U>::max();
            _time -= timeStep;
            _time = timeStep - (max - _time + 1);
        } else if (_time > t1) {
            _time %= (t1 + 1);
        }
    }
};

#endif /* SMALLTOWN_H */
