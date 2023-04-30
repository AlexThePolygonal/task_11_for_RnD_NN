#pragma once
#include <functional>
#include <map>

// checks if Args... is a parameter pack of ints
template <class ... Args>
inline constexpr bool are_ints_v = false;

template <class ... Others>
inline constexpr bool are_ints_v<int, Others...> = are_ints_v<Others...>;

template <>
inline constexpr bool are_ints_v<> = true;

static_assert(are_ints_v<int, char> == false);
static_assert(are_ints_v<int, int, int> == true);


// The cache class
template <class ... Ints>
class ReturnValueCache {
    static_assert(are_ints_v<Ints...>);

    // One concern: I did not give a way do set the max cache size.
    // Why? Because it is not required by the API
    // In a real application, it probably would be, but here it is not.

    // use std::map as it has better performance over medium-sized sets of integers
    // Reasons: if function needs hashing, when it will not be called a very large number of times
    // Through I need some examples to say more
    // If the required cache size is very large, std::unordered_map will be better
    std::map<std::tuple<Ints...>, int> cache; 

    // store the function pointer in a wrapper for generality
    std::function<int(Ints...)> func;    
public:

    ReturnValueCache(int(*f)(Ints ... )) : func(f) {};
    ReturnValueCache(std::function<int(Ints...)> f) : func(f) {};

    int operator()(Ints... ints) {
        auto arg = std::make_tuple(ints...);
        if (cache.count(arg)) {
            return cache[arg];
        } else {
            int res =  func(ints...);
            cache[arg] = res;
            return res;
        }
    }

    void drop() {
        cache.clear();
    }
};

