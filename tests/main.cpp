
#include <cassert>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <complex>

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
	{
		MutexGuard<int>   a {0};
		MutexGuard<std::complex<float>, std::recursive_timed_mutex> c{{1.0f, 2.0f}};

		{
			auto p = a.auto_lock();
			(*p)++;
		}
		assert(*a.auto_lock() == 1);

		{
			auto p = c.try_auto_lock_for(std::chrono::seconds(1));
			if(p) {
				p->real(4.0);
				p->imag(1.0);
			}
		}
		auto d = *c.auto_lock();
		assert(d.real() == 4.0);
		assert(d.imag() == 1.0);
	}
	
	return 0;
}

