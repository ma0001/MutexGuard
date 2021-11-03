
#include <cassert>
#include <thread>
#include <iostream>

#include "../MutexGuard.hpp"

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
		}});
	std::thread th1([&]{
		for(int i = 0; i < 100000; i++) {
			auto p = var.auto_lock();
			p->a--;
			p->b++;
		}});

	th0.join();
	th1.join();

	Hoge res = *var.auto_lock();
	std::cout << "a:" << res.a << " b:" << res.b << std::endl;
	assert(res.a == 1 && res.b == 2);
	return 0;
}

