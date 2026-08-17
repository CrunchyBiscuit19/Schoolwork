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

void q5tests() {
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

int q7(int N, int x) {
    if (N < 0)
        return 0;
    int numerator = 1;
    int divisor = 1;
    for (int i = 1; i <= N; i++) {
        numerator *= (x + i);
        divisor *= i;
    }
    return numerator / divisor;
}

void q7tests() {
    for (int i = 0; i <= 5; i++) {
        for (int j = 0; j <= 5; j++) {
            std::cout << std::format("N = {}, x = {} | {}\n", i, j, q7(i, j));
        }
    }
}

int q8(int N, int bound, int layer) {
    if (N < 0)
        return 0;
    if (N == 0)
        return 1;
    if (layer == 0)
        return 1;
    int acc = 0;
    for (int i = 0; i <= bound; i++) {
        acc += q8(N, i + 1, layer - 1);
    }
    return acc;
}

void q8tests() {
    for (int n = 0; n <= 10; n++) {
        std::cout << std::format("N = {} | {}\n", n, q8(n, 0, n));
    }
}

int q9(int N) {
    if (N < 0)
        return 0;
    if (N <= 1)
        return 1;
    return q9(N - 2) + q9(N - 1);
}

void q9tests() {
    for (int n = 0; n <= 10; n++) {
        std::cout << std::format("N = {} | {}\n", n, q9(n));
    }
}

int q10(int N) {
    if (N < 0)
        return 0;
    if (N == 0)
        return 1;
    int acc = 1;
    for (int i = N + 1; i >= 1; i--) {
        acc *= i;
    }
    return acc;
}

void q10tests() {
    for (int n = 0; n <= 10; n++) {
        std::cout << std::format("N = {} | {}\n", n, q10(n));
    }
}

int q11(int N, int layer = 0) {
    if (N < 0)
        return 0;
    if (N == 0)
        return 1;
    if (N < layer)
        return 1;

    int sum = 0;
    for (int i = 0; i <= layer; i++) {
        sum += q11(N, layer + 1);
    }
    return sum;
}

void q11tests() {
    for (int n = 0; n <= 10; n++) {
        std::cout << std::format("N = {} | {}\n", n, q11(n, 0));
    }
}

int main() {
    //q5tests();
    //q7tests();
    q8tests();
    //q9tests();
    //q10tests();
    //q11tests();
}