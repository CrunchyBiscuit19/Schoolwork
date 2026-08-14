#include <cmath>
#include <functional>
#include <iostream>
#include <print>
#include <unordered_map>

int Sigma0(int n, std::function<int(int)>& f) {
    if (n < 0)
        return 0;
    int acc = 0;
    for (int i = 0; i <= n; i++) {
        acc += f(i);
    }
    return acc;
}

int Sigma1(int n, std::function<int(int)>& f) {
    if (n < 0)
        return 0;
    int acc = 0;
    for (int i = 1; i <= n; i++) {
        acc += f(i);
    }
    return acc;
}

int q5tests() {
    std::vector<int> n = {-3, -2, -1, 0, 1, 2};
    std::vector<std::function<int(int)>> f = {
        [](int i) -> int { return std::pow(i, 3); },
        [](int i) -> int { return 2 * i + 1; }, [](int i) -> int { return 1; },
        [](int i) -> int { return 5; }};
    std::vector<int> expected;
    expected.resize(n.size() * f.size());
    std::fill(expected.begin(), expected.end(), 0);
    expected[12] = 0;
    expected[13] = 1;
    expected[14] = 1;
    expected[15] = 5;
    expected[16] = 1;
    expected[17] = 4;
    expected[18] = 2;
    expected[19] = 10;
    expected[20] = 9;
    expected[21] = 9;
    expected[22] = 3;
    expected[23] = 15;
    for (int i = 0; i < expected.size(); i++) {
        int nIdx = i / f.size();
        int fIdx = i % f.size();
        int res = Sigma0(n[nIdx], f[fIdx]);
        if (res == expected[i]) {
            std::cout << std::format("Test {} passed\n", i);
        } else {
            std::cout << std::format("Test {} expected {}, not {}\n", i,
                                     expected[i], res);
        }
    }
}

int main() {
    q5tests();
}