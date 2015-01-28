#pragma once

#include <type_traits>
#include <typeindex>
#include <vector>
#include <map>
#include <stack>
#include <WTF/ThreadSpecific.h>

namespace NativeScript {
class ReleasePoolBase {
public:
    typedef std::map<std::type_index, std::unique_ptr<ReleasePoolBase>> Item;

    virtual void drain() = 0;

    virtual ~ReleasePoolBase() = default;

protected:
    friend class ReleasePoolHolder;

    ReleasePoolBase() = default;

    static std::stack<Item>& releasePools() {
        static auto pools = new WTF::ThreadSpecific<std::stack<Item>>();
        return **pools;
    }
};

template <typename T>
class ReleasePool : ReleasePoolBase {
    static_assert(std::is_destructible<T>::value, "Type must be destructible");
    static_assert(!std::is_pointer<T>::value, "Type must not be a pointer");

public:
    static void releaseSoon(T&& item) {
        ASSERT(!releasePools().empty());

        ReleasePool<T>* pool = nullptr;

        Item& poolsMap = releasePools().top();
        auto& iter = poolsMap.find(typeid(T));
        if (iter != poolsMap.end()) {
            pool = static_cast<ReleasePool<T>*>(iter->second.get());
        } else {
            pool = new ReleasePool<T>();
            poolsMap.emplace(typeid(T), std::unique_ptr<ReleasePoolBase>(pool));
        }

        pool->_items.push_back(std::move(item));
    }

    virtual void drain() override {
        _items.clear();
    }

private:
    ReleasePool() = default;

    std::vector<T> _items;
};

class ReleasePoolHolder {
public:
    ReleasePoolHolder() {
        ReleasePoolBase::releasePools().emplace();
    }

    void drain() {
        ASSERT(!ReleasePoolBase::releasePools().empty());
        auto& poolsMap = ReleasePoolBase::releasePools().top();
        for (auto& pair : poolsMap) {
            pair.second->drain();
        }
    }

    ~ReleasePoolHolder() {
        ASSERT(!ReleasePoolBase::releasePools().empty());
        ReleasePoolBase::releasePools().pop();
    }
};
}