#include <iostream>
#include "range.hpp"

auto main() -> int {
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");
    assert(input);
    assert(output);

    auto r = uncovered(input);

    if (r.value() == 0)
        output << 0;
    else
        output << r.head() << " " << r.tail();

    input.close();
    output.close();
    return 0;
}