#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

// ί����ģ��
template <typename Signature>
class Delegate;

template <typename ReturnType, typename... Args>
class Delegate<ReturnType(Args...)> {
public:
    using FunctionType = std::function<ReturnType(Args...)>;

    // ��ӿɵ��ö���
    void operator+=(FunctionType func) {
        functions.push_back(func);
    }

    // ɾ���ɵ��ö���
    void operator-=(FunctionType func) {
        auto it = std::remove_if(functions.begin(), functions.end(),
            [&func](const FunctionType& f) { return f.target_type() == func.target_type(); });
        functions.erase(it, functions.end());
    }

    // �������пɵ��ö��󣬲��������һ���ķ���ֵ
    ReturnType operator()(Args... args) {
        ReturnType result{};
        for (auto& func : functions) {
            result = func(args...);
        }
        return result;
    }

    // �ṩ����������
    auto begin() { return functions.begin(); }
    auto end() { return functions.end(); }

private:
    std::vector<FunctionType> functions;
};

// �¼���ģ��
template <typename DelegateType>
class Event;

template <typename ReturnType, typename... Args>
class Event<Delegate<ReturnType(Args...)>> {
public:
    using EventHandler = Delegate<ReturnType(Args...)>;
    using FunctionType = typename EventHandler::FunctionType;

    Event(EventHandler& delegate) : delegate_(delegate) {}

    // �����¼�
    void on(FunctionType func) {
        delegate_ += func;
    }

    // ��Ӽ�����
    void operator+=(FunctionType func) {
        delegate_ += func;
    }

    // ɾ��������
    void operator-=(FunctionType func) {
        delegate_ -= func;
    }

private:
    EventHandler& delegate_;
};

// ����ʾ��
void example() {
    Delegate<void(int)> delegate;

    Event<Delegate<void(int)>> event(delegate);

    // ��Ӽ�����
    event.on([](int value) { std::cout << "Listener 1: " << value << std::endl; });
    event.on([](int value) { std::cout << "Listener 2: " << value << std::endl; });

    // �����¼�
    delegate(42);

    // ɾ��������
    auto func = [](int value) { std::cout << "Listener 2: " << value << std::endl; };
    event -= func;

    // �ٴδ����¼�
    delegate(84);
}

int main() {
    example();
    return 0;
}
