#include <iostream>
#include <unordered_map>
#include <variant>
#include <functional>
#include <optional>

class Sigma {
public:
    Sigma(int start, int end, std::function<int(Sigma&)> func);

    int start;
    int end;
    int count{0};
    std::optional<int> result{std::nullopt};
    std::function<int(Sigma&)> func;

    int run();
};

Sigma::Sigma(int start, int end, std::function<int(Sigma&)> func)
    : start(start),
      end(end),
      func(std::move(func)) {}

int Sigma::run() {
    if (result.has_value()) return result.value();
    int acc = 0;
    for (; count <= end; count++) {
        acc += func(*this);
    }
    result = acc;
    return acc;
}




int sigma(int start, int end, int (*func)(int))
{
    int acc = 0;
    for (int i = start; i <= end; i++)
    {
        acc += func(i);
    }
    return acc;
}

int main()
{
    int m = 10;
    int acc = sigma(0, 2 * m, [](int i) -> int
                    { return i / 2; });
    std::cout << acc << std::endl;
}