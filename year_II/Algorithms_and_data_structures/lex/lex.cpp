#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

typedef long long ll;

static constexpr ll P = 1000000009;
static constexpr ll P_pow = 992051;
static constexpr ll Q_pow = 994307;
static constexpr ll Q = 1000000007;
static constexpr ll MAX = 300005;
static bool match = false;

std::vector<ll> hash_P(MAX);
std::vector<ll> pow_P(MAX);
std::vector<ll> hash_Q(MAX);
std::vector<ll> pow_Q(MAX);

void calc_hash(std::string &s, std::vector<ll> &hash, std::vector<ll> &pow, ll base, ll p) {
    hash[0] = 0;
    pow[0] = 1;
    std::size_t n = s.size();
    for (int i = 0; i < n; i++) {
        hash[i + 1] = pow[i] * ((ll) s[i]) + hash[i];
        hash[i + 1] %= base;
        pow[i + 1] = pow[i] * p;
        pow[i + 1] %= base;
    }
}

char compare(int a, int b, int c, int d, std::string &s) {
    if (a > c) {
        match = true;
        return compare(c, d, a, b, s);
    }
    if (s[a] < s[c])
        return '<';
    if (s[a] > s[c])
        return '>';

    int l = 0;
    int r = std::min(b - a + 1, d - c + 1);

    while (r - l > 1) {
        int p = (l + r + 1) / 2;
        if ((pow_P[c - a] * (hash_P[a + p + 1] - hash_P[a] + P) % P) !=
                    ((hash_P[c + p + 1] - hash_P[c] + P) % P) ||
            (pow_Q[c - a] * (hash_Q[a + p + 1] - hash_Q[a] + Q) % Q) !=
                    (hash_Q[c + p + 1] - hash_Q[c] + Q) % Q)
            r = p;
        else
            l = p;
    }

    if (a + l == b && c + l == d)
        return '=';

    if (a + l == b && c + l < d)
        return '<';

    if (a + l < b && c + l == d)
        return '>';

    char first = s[a + l + 1], second = s[c + l + 1];

    if (first < second)
        return '<';
    if (first > second)
        return '>';

    /* Hash failure */
    assert(false);
}

int main() {
    std::ios::sync_with_stdio(false);
    int n, m;
    std::string s;
    std::cin >> n >> m >> s;

    calc_hash(s, hash_P, pow_P, P, P_pow);
    calc_hash(s, hash_Q, pow_Q, Q, Q_pow);

    while (m--) {
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        char r = compare(--a, --b, --c, --d, s);

        if (match && r != '=') {
            if (r == '<')
                r = '>';
            else
                r = '<';
        }

        std::cout << r << std::endl;
        match = false;
    }

    return 0;
}
