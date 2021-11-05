
#include <cassert>
#include <thread>
#include <iostream>
#include <unistd.h>

#include "../MutexGuard.hpp"

int main()
{
	struct Hoge {
		int a;
		long b;
	};

	{
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
	}
	{
		MutexGuard<Hoge> var {{0, 0}};
		auto p = var.try_auto_lock();
		assert(p);
		auto p2 = var.try_auto_lock();
		assert(!p2);

	}
	{
		MutexGuard<int> var {1};
		{
			auto p = var.try_auto_lock();
			*p += 1;
		}
		assert(*var.auto_lock() == 2);
	}
	{
		MutexGuard<int, std::recursive_mutex> var {1};
		{
			auto p = var.auto_lock();
			assert(p);
			*p += 1;

			auto p2 = var.auto_lock();
			assert(p2);
			*p2 += 2;
		}
		
		int res = *var.auto_lock();
		assert(res == 4);
	}
	{
		MutexGuard<int, std::timed_mutex> var {0};

		{
			auto p = var.try_auto_lock_for(std::chrono::seconds(1));
			assert(p);
			(*p)++;

			std::thread th1([&]{
				auto p2 = var.try_auto_lock_for(std::chrono::seconds(1));
				assert(!p2);
			});

			th1.join();
		}
		
		int res = *var.auto_lock();
		assert(res == 1);

	}
	{
		MutexGuard<int, std::timed_mutex> var {0};

		{
			
			auto now = std::chrono::system_clock::now();
			auto p = var.try_auto_lock_until(now + std::chrono::seconds(1));
			assert(p);
			(*p)++;

			std::thread th1([&]{
				auto p2 = var.try_auto_lock_until(now + std::chrono::seconds(1));
				assert(!p2);
			});

			th1.join();
		}
		
		int res = *var.auto_lock();
		assert(res == 1);

	}
	
	return 0;
}

