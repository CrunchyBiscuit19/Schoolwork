#include <cmath>
#include <functional>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <variant>

class Sigma {
   public:
    Sigma(int start, int end, std::function<int(Sigma&)> func);

    int start;
    int end;
    int count{start};
    std::optional<int> result{std::nullopt};
    std::function<int(Sigma&)> func;

    int run();
};

Sigma::Sigma(int start, int end, std::function<int(Sigma&)> func)
    : start(start), end(end), func(std::move(func)) {}

int Sigma::run() {
    if (result.has_value())
        return result.value();
    int acc = 0;
    for (; count <= end; count++) {
        acc += func(*this);
        //std::cout << acc << std::endl;
    }
    result = acc;
    return acc;
}

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);

    /*Sigma s(1, n, [](Sigma& s1) -> int {
        return Sigma(1, s1.count, [](Sigma& s2) -> int { return s2.count; })
            .run();
    });*/
    /*Sigma s(1, n, [&](Sigma& s1) -> int {
        return Sigma(1, static_cast<int>(std::pow(s1.count, 2)),
                     [&](Sigma& s2) -> int {
                         return Sigma(
                                    1, s1.count + s2.count,
                                    [&](Sigma& s3) -> int { return s3.count; })
                             .run();
                     })
            .run();
    });*/
    Sigma s(0, n, [&](Sigma& s1) -> int {
        //return 1;
        return Sigma(0, s1.count, [&](Sigma& s2) -> int { return 1; }).run();
        /*return Sigma(1, s1.count,
                     [&](Sigma& s2) -> int {
                         return Sigma(1, s2.count,
                                      [&](Sigma& s2) -> int { return 1; })
                             .run();
                     })
            .run();*/
    });

    int result = s.run();

    std::cout << result << std::endl;
}