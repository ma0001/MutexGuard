[![C/C++ CI](https://github.com/ma0001/MutexGuard/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ma0001/MutexGuard/actions/workflows/c-cpp.yml)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

# MutexGuard

c++ mutex template similar to RUST's

# About

This is a c++ template class that hides data inside a mutex, like Rust's Mutex. To access the data, use auto_lock() to get a pointer to the data with an exclusive lock and access it. The exclusion lock is automatically released when the pointer is destructed.

# Requirement

c++11

# Usage

## Example

```c++
#include "MutexGuard.hpp"

int main()
{
	struct Hoge {
		int a;
		long b;
	};
	
	MutexGuard<Hoge>	var {{1, 2}};
	std::thread th0([&]{
		for(int i = 0; i < 100000; i++) {
			auto p = var.auto_lock();
			p->a++;
			p->b--;
		}
	});
	std::thread th1([&]{
		for(int i = 0; i < 100000; i++) {
			auto p = var.auto_lock();
			p->a--;
			p->b++;
		}
	});

	th0.join();
	th1.join();

	Hoge res = *var.auto_lock();
	std::cout << "a:" << res.a << " b:" << res.b << std::endl;
	assert(res.a == 1 && res.b == 2);
	return 0;
}
```
You can also specify recursive_mutex as the second argument of template
```c++
	MutexGuard<Hoge, std::recursive_mutex> var {{0, 0}};
	{
		auto p = var.auto_lock();
		assert(p);
		auto p2 = var.auto_lock();
		assert(p2);
		p->a = 1;
		p2->b = 2;
	}
	
	Hoge res = *var.auto_lock();
	assert(res.a == 1 && res.b == 2);
```

## Licence

[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
