#ifndef RANGE_RANGE_HPP
#define RANGE_RANGE_HPP

#include <algorithm>
#include <set>
#include <numeric>
#include <utility>
#include <fstream>
#include <iostream>

class range {
private:
    int hd;
    int tl;

public:
    range(const int hd, const int tl) : hd(hd), tl(tl) {}

    const int head() const {
        return hd;
    }

    const int tail() const {
        return tl;
    }

    const int length() const {
        return tl - hd;
    };

    const bool empty() const {
        return length() == 0;
    }

    const bool contains(const int x) const {
        return hd <= x && tl >= x;
    }

    bool operator==(const range &rhs) const {
        return hd == rhs.hd &&
               tl == rhs.tl;
    }

    bool operator!=(const range &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const range &rhs) const {
        return this->length() == rhs.length() ? hd > rhs.hd : this->length() < rhs.length();
    }

    bool operator>(const range &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const range &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const range &rhs) const {
        return !(*this < rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const range &range1) {
        os << "[ " << range1.hd << ", " << range1.tl << "] ";
        return os;
    }
};

class coverage {
private:
    range r;
    std::set<range> set;

public:
    explicit coverage(const range &cover) : r(cover), set({}) {}

    coverage(const range &cover, std::set<range> uncover) : r(cover), set(std::move(uncover)) {}

    const range &cover() const {
        return r;
    }

    const std::set<range> &uncover() const {
        return set;
    }

    const range max() const {
        return set.empty() || (*set.rbegin()).length() <= 1 ? range(0, 0) : *set.rbegin();
    }

    friend std::ostream &operator<<(std::ostream &os, const coverage &coverage1) {
        os << "r: " << coverage1.r << " set: ";
        for (auto el : coverage1.set) {
            os << el << " ";
        }
        return os;
    }
};

std::set<range> range_difference(const range &x, const range &y) {
    bool a = x.contains(y.head());
    bool b = x.contains(y.tail());

    if (x.empty() || x == y) {
        return {};
    }

    if (y.empty()) {
        return {{x.head(), x.tail()}};
    }

    // xh yh yt xt
    if (a && b) {
        return {{x.head(), y.head()},
                {y.tail(), x.tail()}};
    }

    // xh yh xt yt
    if (a) {
        return {{x.head(), y.head()}};
    }

    // yh xh yt xt
    if (b) {
        return {{y.tail(), x.tail()}};
    }

    // yh xh xt yt
    if (y.contains(x.head()) || y.contains(x.tail())) {
        return {};
    }

    // yh yt xh xt
    return {{x.head(), x.tail()}};
}

std::set<range> range_difference_not_empty(const range &x, const range &y) {
    std::set<range> tmp = range_difference(x, y);
    std::set<range> result;
    for (auto r : tmp) { if (!r.empty()) result.insert(r); }
    return result;
}

coverage range_union(const range &x, const range &y) {
    const range merge(
            std::min(x.head(), y.head()),
            std::max(x.tail(), y.tail())
    );

    if (y.empty()) {
        return {merge, range_difference_not_empty(merge, x)};
    }

    if (x.empty()) {
        return {merge, range_difference_not_empty(merge, y)};
    }

    std::set<range> diff = range_difference_not_empty(merge, x);
    std::set<range> result;

    for (auto el : diff) {
        std::cout << el << " ";
        std::set<range> ds = range_difference_not_empty(el, y);
        result.insert(ds.begin(), ds.end());
    }

    for (auto el : result) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    return {merge, result};
}

coverage coverage_union(const coverage &c1, const coverage &c2) {
    const range merge(
            std::min(c1.cover().head(), c2.cover().head()),
            std::max(c1.cover().tail(), c2.cover().tail())
    );

    std::set<range> result;
    std::set_union(c1.uncover().begin(), c1.uncover().end(),
                   c2.uncover().begin(), c2.uncover().end(),
                   std::inserter(result, result.begin()));

    return {merge, result};
}

coverage coverage_add(const coverage &c, const range &r) {
    coverage merge = coverage_union(c, range_union(c.cover(), r));

    std::set<range> result;

    if (c.cover().contains(r.head()) || c.cover().contains(r.tail()) ||
        (r.contains(c.cover().head()) && r.contains(c.cover().tail()))) {
        for (auto el : merge.uncover()) {
            std::set<range> ds = range_difference_not_empty(el, r);
            result.insert(ds.begin(), ds.end());
        }
    } else {
        result = merge.uncover();
    }

    coverage f(merge.cover(), result);

    std::cout << c << " + " << r << std::endl << "merge : " << merge << std::endl << f << std::endl << std::endl;

    return f;
}

coverage fromSet(const std::set<range> &ranges) {
    return std::accumulate(ranges.begin(), ranges.end(),
                           coverage(*ranges.begin()),
                           [](const coverage &c, const range &r) -> coverage { return coverage_add(c, r); });
}

range uncovered(std::istream &input) {
    int n;
    int hd, tl;
    std::set<range> ranges;

    input >> n;

    for (auto i = 0; i < n; ++i) {
        input >> hd >> tl;
        ranges.insert({hd, tl});
    }

    return fromSet(ranges).max();
}

#endif //RANGE_RANGE_HPP
