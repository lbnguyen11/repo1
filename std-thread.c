#include <cstdint>
#include <cstdio>
#include <iostream>
#include <thread>
#include <functional>

using namespace std;

struct Data
{
    int i = 7;
};

// Accepts a reference
void foo(Data& data)
{
    printf("data.i = %i\n", data.i);
}

int main()
{
    printf("`std::thread` test\n");

    int x = 5;

    Data data;

    //auto a = __is_invocable_impl<__invoke_result<void (&)(Data), Data>, void>::value;
    //auto a = __is_invocable_impl<__invoke_result<void (&)(Data&), Data>, void>::value;
    auto a = __is_invocable_impl<__invoke_result<void (&)(Data&), std::reference_wrapper<Data>>, void>::value;

    auto b = is_member_object_pointer<
                                      remove_reference<void (&)(Data)>::type
                                                                            >::value;
    auto c = is_member_function_pointer<
                                      remove_reference<void (&)(Data)>::type
                                                                            >::value;

    using d = __invoke_result<void (&)(Data), Data>::type;

    cout << typeid(a).name() << endl;
    if (typeid(a) == typeid(bool)) cout << "a bool\n";
    cout << a << endl;

    cout << typeid(b).name() << endl;
    if (typeid(b) == typeid(bool)) cout << "b bool\n";
    cout << b << endl;

    cout << typeid(c).name() << endl;
    if (typeid(c) == typeid(bool)) cout << "c bool\n";
    cout << c << endl;

    cout << typeid(d).name() << endl;
    if (typeid(d) == typeid(void)) cout << "d void\n";
    //cout << sizeof(d) << endl;










    static_assert(1==1, "aaaa");
    // static_assert( __is_invocable<typename decay<void (&)(Data&)>::type,
    //                   typename decay<Data&>::type>::value,
    //   "lgb1hc std::thread arguments must be invocable after conversion to rvalues"
    //   );
    // static_assert( __is_invocable<typename decay<void (&)(Data&)>::type,
    //                   typename decay<std::reference_wrapper<Data>>::type>::value,
    //   "lgb1hc std::thread arguments must be invocable after conversion to rvalues"
    //   );

    //std::thread t1 = std::thread(foo, data);  // <========= here
    //t1.join();

    return 0;
}
