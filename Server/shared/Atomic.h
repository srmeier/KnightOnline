#pragma once

template <typename T>
class Atomic
{
public:
	Atomic() {}
	Atomic(T value) : m_atomic(value) {}

	operator T() { return (T) m_atomic; }

	INLINE Atomic<T>& operator++() { increment(); return *this; }
	INLINE Atomic<T>& operator--() { decrement(); return *this; }

	// NOTE: The following operators should only be used if it can't be helped.
#define ATOMIC_COMPARISON(op) \
	template <typename T2> \
	INLINE friend bool operator op (Atomic<T>& lhs, const T2 rhs) { return lhs.m_atomic op rhs; }

	// Hide signed/unsigned comparison warning.
	// It's only valid on VS2010, when the type is always long (so, signed).
#pragma warning(push)
#pragma warning(disable: 4018)

	ATOMIC_COMPARISON(>)
		ATOMIC_COMPARISON(>=)
		ATOMIC_COMPARISON(<)
		ATOMIC_COMPARISON(<=)
		ATOMIC_COMPARISON(==)
		ATOMIC_COMPARISON(!=)

#pragma warning(pop)
#undef ATOMIC_COMPARISON

		template <typename T2> INLINE Atomic<T>& operator=(const T2& rhs) { m_atomic = rhs; return *this; }
	template <typename T2> INLINE Atomic<T>& operator+=(const T2 rhs) { m_atomic += rhs; return *this; }
	template <typename T2> INLINE Atomic<T>& operator-=(const T2 rhs) { m_atomic -= rhs; return *this; }

	INLINE T increment() { return ++m_atomic; }
	INLINE T decrement() { return --m_atomic; }

	INLINE bool compare_exchange(T & expected, T desired) { return m_atomic.compare_exchange_strong(expected, desired); }

protected:
	std::atomic<T>	m_atomic;

private:
	Atomic(const Atomic & other); /* disable copy constructor */
};
