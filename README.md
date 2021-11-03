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

## Licence

[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
