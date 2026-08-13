#include <iostream>
#include <unordered_map>

int f(int i) {
    static std::unordered_map<int, int> records;
    if (i <= 0) return 0;
    if (!records.contains(i - 1)) {
        records[i - 1] = f(i - 1);
    }
    return records[i - 1] + i;
}

int main() {
    int n = 300;
    int acc = 0;
    for (int i = 0; i <= n; i++) {
        acc += f(i);
    }
    std::cout << acc << std::endl;
}