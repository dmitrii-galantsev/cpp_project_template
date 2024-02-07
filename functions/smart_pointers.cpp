#include "smart_pointers.hpp"

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <list>
#include <memory>
#include <string_view>
#include <type_traits>
#include <unordered_set>
#include <utility>

// interface
class A {
public:
    A() = default;
    virtual int get_data() = 0;
    virtual void set_data(int) = 0;
};

// inherits A
class B : public A {
public:
    B() = default;  // data = 123
    B(const B&) = default;
    B(B&&) = default;
    B& operator=(const B&) = default;
    B& operator=(B&&) = default;
    ~B() = default;

    explicit B(int data) : data(data) {}  // data = this.data
    int get_data() override { return data; }
    void set_data(int data) override { this->data = data; }

private:
    int data{123};
};

// inherits A
class C : public A {
public:
    C() = default;
    int get_data() override { return 420; }
    // don't care about setting. this function will do nothing
    void set_data(int data) override {}
    std::string_view say_hi() { return "hello from C!"; }
};

int basic_traits() {
    B b{};
    C c{};

    std::cout << "b.get_data() = " << b.get_data() << "\n";

    std::cout << "is_same [int] vs [b.get_data()]? "
              << std::is_same_v<int, decltype(b.get_data())> << "\n";                      // 1
    std::cout << "is_base_of [A] [B]? " << std::is_base_of<A, B>()() << "\n";              // 1
    std::cout << "is_base_of [B] [A]? " << std::is_base_of<B, A>()() << "\n";              // 0
    std::cout << "is_convertible [A&] [B&]? " << std::is_convertible<A&, B&>()() << "\n";  // 0
    std::cout << "is_convertible [B&] [A&]? " << std::is_convertible<B&, A&>()() << "\n";  // 1
    std::cout << "is_convertible [C&] [A&]? " << std::is_convertible<C&, A&>()() << "\n";  // 1
    std::cout << "is_convertible [C&] [B&]? " << std::is_convertible<C&, B&>()() << "\n";  // 0

    // UPCASTING - limit scope of &c
    A& a_ptr = c;
    std::cout << "a_ptr->get_data() = " << a_ptr.get_data() << "\n";

    // DOWNCASTING - expand scope of a_ptr (DANGEROUS!)
    C& c_ptr = static_cast<C&>(a_ptr);  // potentially dangerous cast!
    std::cout << "c_ptr->get_data() = " << c_ptr.get_data() << "\n";
    std::cout << "c_ptr->say_hi() = " << c_ptr.say_hi() << "\n";

    // SIDECASTING - potentially change scope of &c (SUPER DANGEROUS)
    // C c2{};
    // B& b_ptr = dynamic_cast<B&>(c2);
    // std::cout << "b_ptr->get_data() = " << b_ptr.get_data() << "\n";  // fails..?
    // SEE? DANGEROUS!!! ^

    return 0;
}

class Holder {
public:
    Holder() = default;
    std::size_t get_size() { return objects.size(); }
    void print_data() {
        std::cout << "{\n";
        for (auto& a : objects) {
            std::cout << "  " << a->get_data() << ",\n";
        }
        std::cout << "}\n";
    }
    template <typename T>
    void append(std::shared_ptr<T> a) {
        static_assert(std::is_base_of_v<A, T>);
        objects.insert(a);
    }
    // convert unique_ptr to shared_ptr by consuming it
    template <typename T>
    void append(std::unique_ptr<T> a) {
        append(std::shared_ptr<T>(std::move(a)));
    }
    void incr_all() {
        for (auto& a : objects) {
            a->set_data(a->get_data() + 1);
        }
    }

private:
    std::unordered_set<std::shared_ptr<A>> objects;
};

int shared_pointers() {
    Holder holder;
    std::cout << "holder->get_size() = " << holder.get_size() << "\n";

    holder.append(std::make_shared<B>());
    holder.print_data();

    std::shared_ptr<B> b_ptr(new B(6));
    holder.append(b_ptr);
    holder.print_data();
    assert(holder.get_size() == 2);
    assert(b_ptr.use_count() == 2);

    // try to insert it again!
    holder.append(b_ptr);
    holder.append(b_ptr);
    holder.append(b_ptr);
    holder.append(b_ptr);
    holder.append(b_ptr);
    holder.append(b_ptr);
    holder.print_data();
    std::cout << "num of refs: " << b_ptr.use_count() << "\n";
    // once here, once inside holder
    assert(b_ptr.use_count() == 2);
    // this should leave us with only 2 elements
    assert(holder.get_size() == 2);

    // try to insert it again!!!!
    // move this time
    holder.append(std::move(b_ptr));
    holder.print_data();
    // this should leave us with only 2 elements.. still.
    assert(holder.get_size() == 2);
    // use count is reset because b_ptr has been moved
    assert(b_ptr.use_count() == 0);

    holder.incr_all();
    holder.print_data();

    // sizes or uses shouldn't have changed
    assert(holder.get_size() == 2);
    assert(b_ptr.use_count() == 0);

    // just to make sure different types work
    holder.append(std::make_shared<C>());
    holder.print_data();
    assert(holder.get_size() == 3);

    // and to make sure different objects work (even if underlying data is the same)
    holder.append(std::make_shared<C>());
    holder.print_data();
    assert(holder.get_size() == 4);

    // demonstrate using unique_ptr
    holder.append(std::make_unique<B>(1234567));
    holder.print_data();
    assert(holder.get_size() == 5);

    return 0;
}

// BASE CASE
template <typename T>
void print_types(T a) {
    //std::cout << boost::core::demangle(typeid(T).name()) << "\n";
}

// RECURSIVE CASE
template <typename T, typename... TArgs>
void print_types(T a, TArgs... args) {
    print_types(a);
    print_types(args...);
}

int print_types() {
    print_types('a', 5, 5.0F);
    return 0;
}

int smart_pointers() {
    int status = 0;
    // status += basic_traits();
    // status += shared_pointers();
    print_types();

    return status;
}
