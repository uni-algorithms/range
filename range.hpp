#ifndef RANGE_RANGE_HPP
#define RANGE_RANGE_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

class range {
    int hd;
    int tl;

public:
    range(const int hd, const int tl) : hd(hd), tl(tl) {}

    int head() const {
        return hd;
    }

    int tail() const {
        return tl;
    }

    int value() const { return tl - hd; }

    auto contains(const int x) -> bool const {
        return hd <= x && x <= tl;
    }

    auto minus(const range &x) -> std::pair<range, range> const {
        auto rmvHd = contains(x.tail());
        auto rmvTl = contains(x.head());
        auto null = range(0, 0);

        if (rmvHd && rmvTl)
            return std::make_pair(range(hd, x.head()), range(x.tail(), tl));

        if (rmvHd)
            return std::make_pair(range(x.tail(), tl), null);

        if (rmvTl)
            return std::make_pair(range(hd, x.head()), null);

        return std::make_pair(range(hd, tl), null);
    }

    auto extend(const range &x) -> range const {
        return range(
                std::min(hd, x.head()),
                std::max(tl, x.tail())
        );
    }

    bool operator==(const range &rhs) const {
        return hd == rhs.head() &&
               tl == rhs.tail();
    }

    bool operator!=(const range &rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream &operator<<(std::ostream &os, const range &range1) {
        os << "[" << range1.head() << ", " << range1.tail() << "]";
        return os;
    }
};

class coverage {
    range covered;
    std::list<range> uncovered;

public:
    explicit coverage(const range &covered) : covered(covered) {}

    auto add(const range &x) -> void {
        if (covered.contains(x.head()) || covered.contains(x.tail())) {
            for (auto it = uncovered.begin(); it != uncovered.end(); ++it) {
                auto r = it->minus(x);

                if (r.first.value() != 0)
                    uncovered.insert(it, r.first);

                if (r.second.value() != 0)
                    uncovered.insert(it, r.second);

                uncovered.erase(it);
            }
        }

        if (x.tail() < covered.head()) {
            uncovered.push_front(range(x.tail(), covered.head()));
        }

        if (covered.tail() < x.head()) {
            uncovered.push_back(range(covered.tail(), x.head()));
        }

        covered = covered.extend(x);
    }

    auto addAll(const std::vector<range> &ranges) -> void {
        for (auto el : ranges) {
            this->add(el);
        }
    }

    auto max() -> range const {
        return *std::max_element(uncovered.begin(), uncovered.end(),
                                 [](const range &x, const range &y) {
                                     if (x.value() == y.value()) return x.head() > y.head();
                                     return x.value() < y.value();
                                 });
    }
};

auto uncovered(std::istream &input) -> const range {
    int n;
    int hd, tl;

    input >> n;
    input >> hd >> tl;

    coverage cov(range(hd, tl));

    for (auto i = 1; i < n; ++i) {
        input >> hd >> tl;
        cov.add(range(hd, tl));
    }

    return cov.max();
}

#endif //RANGE_RANGE_HPP
