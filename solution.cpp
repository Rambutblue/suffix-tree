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


class CDummy {
public:
    CDummy(char c) : m_C(c) {
    }

    bool operator==(const CDummy &other) const = default;

    friend std::ostream &operator<<(std::ostream &os, const CDummy &x) {
        return os << '\\'' << x.m_C << '\\'';
    }

private:
    char m_C;
};

struct Node;

class Link {
public:
    Link(size_t begin, const std::shared_ptr<size_t> &anEnd, bool isEnd, size_t beginIdx,
         const std::shared_ptr<Node> &nextNode) : begin(begin), end(anEnd), isEnd(isEnd),
                                                  beginIdx(beginIdx), nextNode(nextNode) {}

    size_t begin;
    std::shared_ptr<size_t> end;
    bool isEnd;
    size_t beginIdx;
    std::shared_ptr<Node> nextNode;
};

struct Node {
    std::unordered_map<size_t, std::shared_ptr<Link>> links;
    std::shared_ptr<Node> suffixLink;
    size_t totalLeafCount = 0; // Number of leaves in the subtree
};

template<typename T_>
class CSelfMatch {
private:
    std::vector<T_> m_Sequence;
    std::vector<size_t> m_SequenceInt;
    std::unordered_map<T_, size_t> m_Map;
    size_t m_NextID = 1;
    std::shared_ptr<size_t> m_End;
    std::shared_ptr<Node> m_Root;

    size_t m_ActiveNode = 0;
    size_t m_ActiveEdge = 0;
    size_t m_ActiveLength = 0;
    size_t m_RemainingSuffixes = 0;
    std::shared_ptr<Node> m_LastNewNode = nullptr;

    size_t mapElement(const T_ &element) {
        if (m_Map.find(element) == m_Map.end()) {
            m_Map[element] = m_NextID++;
        }
        return m_Map.at(element);
    }

    size_t edgeLen(const Link &link) const {
        return *link.end - link.begin + 1;
    }

    void walkDown(const std::shared_ptr<Node> &node, std::shared_ptr<Link> &link) {
        if (m_ActiveLength >= edgeLen(*link)) {
            m_ActiveEdge += edgeLen(*link);
            m_ActiveLength -= edgeLen(*link);
            m_ActiveNode = link->nextNode ? link->nextNode->totalLeafCount : 0;
            link = link->nextNode->links.at(m_SequenceInt[m_ActiveEdge]);
            walkDown(link->nextNode, link);
        }
    }

    void buildSuffixTree() {
        if (m_SequenceInt.empty()) return;

        m_Root = std::make_shared<Node>();
        m_End = std::make_shared<size_t>(0);

        m_ActiveNode = 0;
        m_ActiveEdge = 0;
        m_ActiveLength = 0;
        m_RemainingSuffixes = 0;
        m_LastNewNode = nullptr;

        std::vector<std::shared_ptr<Node>> nodes = {m_Root};
        auto getNode = [&](size_t idx) -> std::shared_ptr<Node> {
            return nodes[idx];
        };
        auto addNode = [&](const std::shared_ptr<Node> &node) -> size_t {
            nodes.push_back(node);
            return nodes.size() - 1;
        };

        for (size_t i = 0; i < m_SequenceInt.size(); ++i) {
            (*m_End)++;
            m_RemainingSuffixes++;
            m_LastNewNode = nullptr;

            while (m_RemainingSuffixes > 0) {
                if (m_ActiveLength == 0) {
                    m_ActiveEdge = i;
                }

                auto activeNode = getNode(m_ActiveNode);
                auto it = activeNode->links.find(m_SequenceInt[m_ActiveEdge]);
                std::shared_ptr<Link> link = (it != activeNode->links.end()) ? it->second : nullptr;

                if (!link) {
                    size_t beginIdx = i - m_RemainingSuffixes + 1;
                    activeNode->links[m_SequenceInt[m_ActiveEdge]] =
                            std::make_shared<Link>(i, m_End, true, beginIdx, nullptr);

                    if (m_LastNewNode) {
                        m_LastNewNode->suffixLink = activeNode;
                        m_LastNewNode = nullptr;
                    }
                } else {
                    if (m_ActiveLength >= edgeLen(*link)) {
                        m_ActiveEdge += edgeLen(*link);
                        m_ActiveLength -= edgeLen(*link);
                        m_ActiveNode = link->nextNode ? (link->nextNode->totalLeafCount) : 0;
                        activeNode = getNode(m_ActiveNode);
                        it = activeNode->links.find(m_SequenceInt[m_ActiveEdge]);
                        link = (it != activeNode->links.end()) ? it->second : nullptr;
                        continue;
                    }

                    size_t charIndexInEdge = link->begin + m_ActiveLength;
                    if (charIndexInEdge < m_SequenceInt.size() && m_SequenceInt[i] == m_SequenceInt[charIndexInEdge]) {
                        m_ActiveLength++;
                        if (m_LastNewNode) {
                            m_LastNewNode->suffixLink = activeNode;
                            m_LastNewNode = nullptr;
                        }
                        break;
                    } else {
                        size_t newEnd = charIndexInEdge - 1;
                        auto splitNode = std::make_shared<Node>();
                        addNode(splitNode);

                        auto splitLink = std::make_shared<Link>(link->begin, std::make_shared<size_t>(newEnd), false,
                                                               link->beginIdx, splitNode);

                        link->begin = charIndexInEdge;

                        size_t beginIdx = i - m_RemainingSuffixes + 1;
                        splitNode->links[m_SequenceInt[i]] =
                                std::make_shared<Link>(i, m_End, true, beginIdx, nullptr);

                        splitNode->links[m_SequenceInt[link->begin]] = link;

                        activeNode->links[m_SequenceInt[m_ActiveEdge]] = splitLink;

                        if (m_LastNewNode) {
                            m_LastNewNode->suffixLink = splitNode;
                        }
                        m_LastNewNode = splitNode;
                    }
                }
                m_RemainingSuffixes--;
                if (m_ActiveNode == 0) {
                    m_ActiveEdge = i - m_RemainingSuffixes + 1;
                    if (m_ActiveLength > 0) m_ActiveLength--;
                } else {
                    m_ActiveNode = getNode(m_ActiveNode)->suffixLink ?
                                   getNode(m_ActiveNode)->suffixLink->totalLeafCount : 0;
                }
            }
        }
        computeLeafCounts(m_Root);
        fixLeafEnds(m_Root);
    }

    size_t computeLeafCounts(const std::shared_ptr<Node> &node) {
        size_t count = 0;
        for (const auto &pair : node->links) {
            const auto &link = pair.second;
            if (link->isEnd) {
                count++;
            } else {
                count += computeLeafCounts(link->nextNode);
            }
        }
        node->totalLeafCount = count;
        return count;
    }

    void fixLeafEnds(const std::shared_ptr<Node> &node) {
        for (const auto &pair : node->links) {
            const auto &link = pair.second;
            if (link->isEnd && link->end.get() != m_End.get()) {
                link->end = m_End;
            } else if (link->nextNode) {
                fixLeafEnds(link->nextNode);
            }
        }
    }

    struct SearchResult {
        size_t longestLen = 0;
        std::vector<size_t> beginIndexes;
    };

    SearchResult findLongestRepeating(const std::shared_ptr<Node> &node, size_t currentLen, size_t n) const {
        SearchResult result;

        if (node->totalLeafCount >= n) {
            result.longestLen = currentLen;
        }

        for (const auto &pair : node->links) {
            const auto &link = pair.second;
            size_t linkLen = *link->end - link->begin + 1;

            if (link->nextNode && link->nextNode->totalLeafCount >= n) {
                size_t pathLen = currentLen + linkLen;
                SearchResult subResult = findLongestRepeating(link->nextNode, pathLen, n);

                if (subResult.longestLen > result.longestLen) {
                    result = std::move(subResult);
                }
            }

            if (link->isEnd && currentLen > result.longestLen && node->totalLeafCount >= n) {
                // Check edge case where the longest match ends exactly at the parent node (currentLen)
                // and this is the deepest match found so far that satisfies the count.
                if (currentLen > result.longestLen) {
                    result.longestLen = currentLen;
                }
            }
        }

        if (result.longestLen == currentLen && node->totalLeafCount >= n) {
            findAllBeginIndexes(node, result.beginIndexes);
        }

        return result;
    }

    void findAllBeginIndexes(const std::shared_ptr<Node> &node, std::vector<size_t> &result) const {
        for (const auto &pair : node->links) {
            const auto &link = pair.second;
            if (link->isEnd) {
                result.push_back(link->beginIdx);
            } else if (link->nextNode) {
                findAllBeginIndexes(link->nextNode, result);
            }
        }
    }

    template<size_t N_>
    void generateCombinations(const std::vector<size_t> &arr, std::vector<std::array<size_t, N_>> &result,
                              std::array<size_t, N_> &combination, size_t start, size_t k) const {
        if (k == 0) {
            result.push_back(combination);
            return;
        }

        for (size_t i = start; i < arr.size(); ++i) {
            combination[N_ - k] = arr[i];
            generateCombinations(arr, result, combination, i + 1, k - 1);
        }
    }


public:
    CSelfMatch(std::initializer_list<T_> list) : CSelfMatch(list.begin(), list.end()) {}

    template<typename Iterator>
    CSelfMatch(Iterator begin, Iterator end) {
        m_Sequence.reserve(std::distance(begin, end));
        for (auto it = begin; it != end; ++it) {
            m_Sequence.push_back(*it);
            m_SequenceInt.push_back(mapElement(*it));
        }
        m_SequenceInt.push_back(0);
        buildSuffixTree();
    }

    template<typename Container>
    CSelfMatch(const Container &container) : CSelfMatch(container.begin(), container.end()) {}


    size_t sequenceLen(size_t n) const {
        if (n == 0) throw std::invalid_argument("n must be greater than 0");
        if (m_Sequence.empty()) return 0;

        SearchResult result = findLongestRepeating(m_Root, 0, n);

        return result.longestLen;
    }

    template<size_t N_>
    std::vector<std::array<size_t, N_>> findSequences() const {
        if (N_ == 0) throw std::invalid_argument("N_ must be greater than 0");
        if (m_Sequence.empty()) return {};

        SearchResult searchResult = findLongestRepeating(m_Root, 0, N_);

        if (searchResult.longestLen == 0 || searchResult.beginIndexes.size() < N_) {
            return {};
        }

        std::sort(searchResult.beginIndexes.begin(), searchResult.beginIndexes.end());
        searchResult.beginIndexes.erase(
                std::unique(searchResult.beginIndexes.begin(), searchResult.beginIndexes.end()),
                searchResult.beginIndexes.end());

        std::vector<std::array<size_t, N_>> combinations;
        std::array<size_t, N_> combination;
        generateCombinations<N_>(searchResult.beginIndexes, combinations, combination, 0, N_);

        return combinations;
    }

    template<typename... Args>
    void push_back(Args &&... elements) {
        for (auto &&element : {std::forward<Args>(elements)...}) {
            m_Sequence.push_back(element);
            m_SequenceInt.push_back(mapElement(element));
        }
        
        m_SequenceInt.push_back(0);
        buildSuffixTree();
    }
};

template<typename ContainerType>
CSelfMatch(ContainerType container) -> CSelfMatch<typename ContainerType::value_type>;

template<typename T>
CSelfMatch(std::initializer_list<T> list) -> CSelfMatch<T>;
