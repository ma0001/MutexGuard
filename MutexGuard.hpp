
#include <mutex>

template<typename T, typename M = std::mutex>
class MutexGuard
{
public:
	//! Default constructor
	MutexGuard() = default;

	//! Copy constructor
	MutexGuard(const MutexGuard &ini) = delete;
	MutexGuard(const T &ini)
	{
		data_ = ini;
	}

	//! Move constructor
	MutexGuard(MutexGuard &&other) noexcept = default;

	//! Destructor
	virtual ~MutexGuard() noexcept = default;

	//! Copy assignment operator
	MutexGuard& operator=(const MutexGuard &other) = delete;

	//! Move assignment operator
	MutexGuard& operator=(MutexGuard &&other) noexcept = delete;


	class MutexGuard_ptr
	{
	public:
		//! Default constructor
		MutexGuard_ptr() = default;
		MutexGuard_ptr(MutexGuard *data) : ref_(data)
		{
			ref_->mutex_.lock();
			
		}

		//! Move constructor
		MutexGuard_ptr(MutexGuard_ptr &&other) noexcept
		{
			*this = std::move(other);
		}

		//! Destructor
		virtual ~MutexGuard_ptr() noexcept
		{
			if(ref_) {
				ref_->mutex_.unlock();
			}
		}

		//! Copy assignment operator
		MutexGuard_ptr& operator=(const MutexGuard_ptr &other) = delete;

		//! Move assignment operator
		MutexGuard_ptr& operator=(MutexGuard_ptr &&other) noexcept
		{
			if(this != &other){
				// moveの処理
				this->ref_ = other.ref_;
				other.ref_ = nullptr;
			}
			return *this;
		}

		T* operator->()  {
			return &ref_->data_;
		}

		T operator*()  {
			return ref_->data_;
		}
		
		//! Copy assignment operator
		MutexGuard_ptr& operator=(const T &rhs) noexcept
		{
			this->ref_->data = rhs;
			return *this;			
		}
		
	protected:
	private:
		MutexGuard	*ref_ = nullptr;
	};

	using pointer = MutexGuard_ptr;
	
    MutexGuard_ptr auto_lock()
	{
		MutexGuard_ptr	ptr(this);
		return ptr;
	}

protected:
private:
	friend MutexGuard_ptr;

	M mutex_;
	T data_;

};
