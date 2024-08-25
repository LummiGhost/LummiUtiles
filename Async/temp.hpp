#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

// 委托类模板
template <typename Signature>
class Delegate;

template <typename ReturnType, typename... Args>
class Delegate<ReturnType(Args...)> {
public:
    using FunctionType = std::function<ReturnType(Args...)>;

    // 添加可调用对象
    void operator+=(FunctionType func) {
        functions.push_back(func);
    }

    // 删除可调用对象
    void operator-=(FunctionType func) {
        auto it = std::remove_if(functions.begin(), functions.end(),
            [&func](const FunctionType& f) { return f.target_type() == func.target_type(); });
        functions.erase(it, functions.end());
    }

    // 调用所有可调用对象，并返回最后一个的返回值
    ReturnType operator()(Args... args) {
        ReturnType result{};
        for (auto& func : functions) {
            result = func(args...);
        }
        return result;
    }

    // 提供迭代器访问
    auto begin() { return functions.begin(); }
    auto end() { return functions.end(); }

private:
    std::vector<FunctionType> functions;
};

// 事件类模板
template <typename DelegateType>
class Event;

template <typename ReturnType, typename... Args>
class Event<Delegate<ReturnType(Args...)>> {
public:
    using EventHandler = Delegate<ReturnType(Args...)>;
    using FunctionType = typename EventHandler::FunctionType;

    Event(EventHandler& delegate) : delegate_(delegate) {}

    // 订阅事件
    void on(FunctionType func) {
        delegate_ += func;
    }

    // 添加监听器
    void operator+=(FunctionType func) {
        delegate_ += func;
    }

    // 删除监听器
    void operator-=(FunctionType func) {
        delegate_ -= func;
    }

private:
    EventHandler& delegate_;
};

// 测试示例
void example() {
    Delegate<void(int)> delegate;

    Event<Delegate<void(int)>> event(delegate);

    // 添加监听器
    event.on([](int value) { std::cout << "Listener 1: " << value << std::endl; });
    event.on([](int value) { std::cout << "Listener 2: " << value << std::endl; });

    // 触发事件
    delegate(42);

    // 删除监听器
    auto func = [](int value) { std::cout << "Listener 2: " << value << std::endl; };
    event -= func;

    // 再次触发事件
    delegate(84);
}

int main() {
    example();
    return 0;
}
