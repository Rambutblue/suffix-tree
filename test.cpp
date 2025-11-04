#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>
using namespace std::literals;

class CDummy
{
public:
    CDummy(char c) : m_C(c)
    {
    }

    bool operator==(const CDummy &other) const = default;

    friend std::ostream &operator<<(std::ostream &os, const CDummy &x)
    {
        return os << '\'' << x.m_C << '\'';
    }

private:
    char m_C;
};

#define TEST_EXTRA_INTERFACE

#ifdef TEST_EXTRA_INTERFACE
template <typename T>
CSelfMatch(std::initializer_list<T> list) -> CSelfMatch<T>;

template <typename T>
CSelfMatch(const std::vector<T> &container) -> CSelfMatch<T>;

template <typename T>
CSelfMatch(const std::string &container) -> CSelfMatch<T>;
#endif

// Helper function to check if two position lists are equal, ignoring order of arrays
template <size_t N_>
bool positionMatch(const std::vector<std::array<size_t, N_>> &a, const std::vector<std::array<size_t, N_>> &b)
{
    if (a.size() != b.size())
        return false;

    std::vector<std::array<size_t, N_>> c = a, d = b;

    for (auto &x : c)
        std::sort(x.begin(), x.end());
    for (auto &x : d)
        std::sort(x.begin(), x.end());

    std::sort(c.begin(), c.end());
    std::sort(d.begin(), d.end());

    return c == d;
}

int main(void)
{
    CSelfMatch<char> x0("abababa");
    assert(x0.sequenceLen(1) == 7);
    assert(x0.sequenceLen(2) == 5);
    assert(positionMatch(x0.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 2}, {0, 4}, {2, 4} }));
    assert(x0.sequenceLen(3) == 3);
    assert(positionMatch(x0.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 2, 4} }));
    assert(x0.sequenceLen(4) == 1);
    assert(positionMatch(x0.findSequences<4>(), std::vector<std::array<size_t, 4>>{ {0, 2, 4, 6} }));
    assert(x0.sequenceLen(5) == 0);
    assert(positionMatch(x0.findSequences<5>(), std::vector<std::array<size_t, 5>>{}));

    CSelfMatch<char> x1("abcabc");
    assert(x1.sequenceLen(1) == 6);
    assert(x1.sequenceLen(2) == 3);
    assert(positionMatch(x1.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 3} }));
    assert(x1.sequenceLen(3) == 0);
    assert(positionMatch(x1.findSequences<3>(), std::vector<std::array<size_t, 3>>{}));

    CSelfMatch<char> x2("aaaaaaaa");
    assert(x2.sequenceLen(1) == 8);
    assert(x2.sequenceLen(2) == 7);
    assert(positionMatch(x2.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {3, 5}, {3, 6}, {4, 5}, {4, 6}, {5, 6} }));
    assert(x2.sequenceLen(7) == 2);
    assert(positionMatch(x2.findSequences<7>(), std::vector<std::array<size_t, 7>>{ {0, 1, 2, 3, 4, 5, 6} }));
    assert(x2.sequenceLen(8) == 1);
    assert(positionMatch(x2.findSequences<8>(), std::vector<std::array<size_t, 8>>{ {0, 1, 2, 3, 4, 5, 6, 7} }));
    assert(x2.sequenceLen(9) == 0);
    assert(positionMatch(x2.findSequences<9>(), std::vector<std::array<size_t, 9>>{}));

    CSelfMatch<char> x3("abcde");
    assert(x3.sequenceLen(1) == 5);
    assert(x3.sequenceLen(2) == 0);
    assert(positionMatch(x3.findSequences<2>(), std::vector<std::array<size_t, 2>>{}));

    CSelfMatch<char> x4("");
    assert(x4.sequenceLen(1) == 0);
    assert(x4.sequenceLen(2) == 0);
    assert(positionMatch(x4.findSequences<2>(), std::vector<std::array<size_t, 2>>{}));

    std::initializer_list<CDummy> init5{ 'a', 'b', 'c', 'X', 'a', 'b', 'c', 'Y', 'a', 'b', 'c', 'Z', 'a', 'b', 'c' };
    CSelfMatch<CDummy> x5(init5.begin(), init5.end());
    assert(x5.sequenceLen(1) == 15);
    assert(x5.sequenceLen(2) == 3);
    assert(positionMatch(x5.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 4}, {0, 8}, {0, 12}, {4, 8}, {4, 12}, {8, 12} }));
    assert(x5.sequenceLen(3) == 3);
    assert(positionMatch(x5.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 4, 8}, {0, 4, 12}, {0, 8, 12}, {4, 8, 12} }));
    assert(x5.sequenceLen(4) == 3);
    assert(positionMatch(x5.findSequences<4>(), std::vector<std::array<size_t, 4>>{ {0, 4, 8, 12} }));

    std::list<int> init6{ 10, 20, 30, 10, 20, 30, 40, 50, 60, 10, 20, 30, 70, 80 };
    CSelfMatch<int> x6(init6);
    assert(x6.sequenceLen(1) == 14);
    assert(x6.sequenceLen(2) == 3);
    assert(positionMatch(x6.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 3}, {0, 9}, {3, 9} }));
    assert(x6.sequenceLen(3) == 3);
    assert(positionMatch(x6.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 3, 9} }));
    assert(x6.sequenceLen(4) == 0);
    assert(positionMatch(x6.findSequences<4>(), std::vector<std::array<size_t, 4>>{}));

    std::map<int, std::string> init7{ {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"}, {6, "f"} };
    CSelfMatch<std::map<int, std::string>::const_iterator> x7(init7.begin(), init7.end());
    assert(x7.sequenceLen(1) == 6);
    assert(x7.sequenceLen(2) == 0);

    std::initializer_list<char> init8{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
    CSelfMatch<char> x8(init8);
    assert(x8.sequenceLen(1) == 30);
    assert(x8.sequenceLen(2) == 10);
    assert(positionMatch(x8.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 10}, {0, 20}, {10, 20} }));
    assert(x8.sequenceLen(3) == 10);
    assert(positionMatch(x8.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 10, 20} }));
    assert(x8.sequenceLen(4) == 0);

    CSelfMatch<std::string> x9({ "a", "b", "c", "d", "a", "b", "c", "d", "a", "b", "c", "d" });
    assert(x9.sequenceLen(1) == 12);
    assert(x9.sequenceLen(2) == 4);
    assert(positionMatch(x9.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 4}, {0, 8}, {4, 8} }));
    assert(x9.sequenceLen(3) == 4);
    assert(positionMatch(x9.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 4, 8} }));

    std::initializer_list<int> init10{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    CSelfMatch<int> x10(init10);
    assert(x10.sequenceLen(1) == 30);
    assert(x10.sequenceLen(2) == 10);
    assert(positionMatch(x10.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 10}, {0, 20}, {10, 20} }));
    assert(x10.sequenceLen(3) == 10);
    assert(positionMatch(x10.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 10, 20} }));
    assert(x10.sequenceLen(4) == 0);

    std::initializer_list<char> init11{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
    CSelfMatch<char> x11(init11);
    assert(x11.sequenceLen(1) == 26);
    assert(x11.sequenceLen(2) == 0);

    std::initializer_list<CDummy> init12{ 'a', 'b', 'c', 'a', 'b', 'c', 'a', 'b', 'c' };
    CSelfMatch<CDummy> x12(init12);
    assert(x12.sequenceLen(2) == 6);
    assert(positionMatch(x12.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 3}, {0, 6}, {3, 6} }));
    assert(x12.sequenceLen(3) == 3);
    assert(positionMatch(x12.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 3, 6} }));

    std::initializer_list<CDummy> init13{ 'a', 'b', 'c', 'd', 'X', 'a', 'b', 'c', 'd', 'Y', 'a', 'b', 'c', 'd' };
    CSelfMatch<CDummy> x13(init13.begin(), init13.end());
    assert(x13.sequenceLen(2) == 4);
    assert(positionMatch(x13.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 5}, {0, 10}, {5, 10} }));
    
    std::initializer_list<int> init14{ 1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2, 1, 0 };
    CSelfMatch<int> x14(init14.begin(), init14.end());
    assert(x14.sequenceLen(2) == 5);
    assert(positionMatch(x14.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {11, 14}, {7, 24} }));
    
    std::initializer_list<int> init15{ 1, 2, 1, 1, 2, 1, 0, 1, 2, 1, 0, 1, 2, 0 };
    CSelfMatch<int> x15(init15.begin(), init15.end());
    assert(x15.sequenceLen(3) == 2);
    assert(positionMatch(x15.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 3, 7} }));

#ifdef TEST_EXTRA_INTERFACE
    CSelfMatch x16(std::string("abababa"));
    assert(x16.sequenceLen(2) == 5);
    assert(positionMatch(x16.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 2}, {0, 4}, {2, 4} }));
    
    CSelfMatch x17(std::deque<int>{ 1, 2, 3, 1, 2, 3, 1, 2, 3 });
    assert(x17.sequenceLen(3) == 3);
    assert(positionMatch(x17.findSequences<3>(), std::vector<std::array<size_t, 3>>{ {0, 3, 6} }));

    CSelfMatch<char> x18("a");
    x18.push_back('b', 'a', 'b');
    assert(x18.sequenceLen(2) == 1);
    assert(positionMatch(x18.findSequences<2>(), std::vector<std::array<size_t, 2>>{ {0, 2} }));
#endif
    return 0;
}
