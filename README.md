[![C/C++ CI](https://github.com/ma0001/MutexGuard/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ma0001/MutexGuard/actions/workflows/c-cpp.yml)
[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)

# MutexGuard

c++ mutex guarded variable similar to RUST's

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
You can also specify recursive_mutex as the second argument of MutexGuard
```c++
{
	MutexGuard<int, std::recursive_mutex> var {1};
	{
		auto p = var.auto_lock();
		*p += 1;
		{
			auto p2 = var.auto_lock();
			*p2 += 2;
		}
	}
	
	int res = *var.auto_lock();
	assert(res == 4);
}
```
By specifying timed_mutex or recursive_timed_mutex, you can use try_auto_lock_until() and try_auto_lock_for()
```c++
{
	MutexGuard<int, std::timed_mutex> var {0};
	{
		auto p = var.try_auto_lock_for(std::chrono::seconds(1));
		if(p) {		// check if timeout occurred
			(*p)++
		}
	}
	int res = *var.auto_lock();
	assert(res == 1);
}
```


## Licence

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)
