#pragma once

namespace NativeScript {

template <typename T>
class IteratorRange final {
public:
    explicit IteratorRange(T beginIterator, T endIterator)
        : _beginIterator(std::move(beginIterator))
          , _endIterator(std::move(endIterator)) {
    }

    T begin() const {
        return _beginIterator;
    }

    T end() const {
        return _endIterator;
    }

private:
    T _beginIterator;
    T _endIterator;
};

}
