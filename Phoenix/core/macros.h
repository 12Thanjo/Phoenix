#pragma once


// using class member functions in a function param
#define BIND_FUNCTION(fn) [this](auto&&... args) -> decltype(auto){ return this->fn(std::forward<decltype(args)>(args)...); }

/*
creates a getter / setter for a value
adapted from: https://stackoverflow.com/a/51616035

T:    type
name: name of the variable
val:  value to attach to

example: 
	foo.name() += 2;

*/
#define GET_SET(T, name, val) inline auto name() const& -> const T&  { return val; }\
							  inline auto name() &      ->       T&  { return val; }\
							  inline auto name() &&     ->       T&& { return std::move(val); }


