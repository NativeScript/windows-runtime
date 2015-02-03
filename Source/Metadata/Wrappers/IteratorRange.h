#pragma once

namespace NativeScript {

template <typename T>
class IteratorRange final {
public:
    explicit IteratorRange(T beginIterator, T endIterator)
        : _begin(std::move(beginIterator))
        , _end(std::move(endIterator)) {
    }

    T begin() const {
        return _begin;
    }

    T end() const {
        return _end;
    }

    bool empty() const {
        return _begin == _end;
    }

private:
    T _begin;
    T _end;
};
}
