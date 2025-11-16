#pragma once
#include <queue>

template <typename T>
class Queue {
private:
    std::queue<T> q;

public:
    void push(const T& v) { q.push(v); }
    void pop() { if (!q.empty()) q.pop(); }
    T front() const { return q.front(); }
    bool empty() const { return q.empty(); }
    size_t size() const { return q.size(); }
    void clear() { while (!q.empty()) q.pop(); }
};
