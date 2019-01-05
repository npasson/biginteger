//
// Created by Nicholas on 02.01.2019.
//

#ifndef NPASSON_BIGINTEGER_TYPE
#define NPASSON_BIGINTEGER_TYPE

#include <cstdint>
#include <cstring>

#ifdef __JETBRAINS_IDE__ // because CLion is stupid with highlighting, ignore this part
#   define __has_attribute_unused true
#endif

#ifdef __cpp_if_constexpr
#	define if_constexpr if constexpr
#else
#	define if_constexpr if
#endif

#if defined(__GNUC__)
#   if __has_attribute(unused) || defined(__has_attribute_unused)
#       define NPASSON_EXTERNAL_USAGE __attribute__((unused))
#   else
#       pragma message ("__attribute__((unused)) is not defined, ignore possible 'unused' warnings from this library")
#       define NPASSON_EXTERNAL_USAGE
#   endif
#elif defined(__clang__) || defined(__has_attribute_unused)
#   if __has_attribute(unused)
#       define NPASSON_EXTERNAL_USAGE __attribute__((unused))
#   else
#       pragma message ("__attribute__((unused)) is not defined, ignore possible 'unused' warnings from this library")
#       define NPASSON_EXTERNAL_USAGE
#   endif
#endif

namespace npasson {
	template <uint_fast64_t _T_bit_amount>
	class NPASSON_EXTERNAL_USAGE BigInteger {
	private:
		typedef uint64_t element_t;
		typedef uint_fast64_t block_count_t;
		typedef uint_fast64_t bit_count_t;
		typedef uint_fast8_t sub_access_t;

		const block_count_t _block_count = ( _T_bit_amount / sizeof(element_t) * 8 ) + 1;
		element_t* const _raw_data = new element_t[_block_count];

	public:

		int _get_bit(bit_count_t bit);

		bool _set_bit(bit_count_t bit, int val);

		/* ****** THE BIG FIVE + DESTRUCTOR ****** */

		BigInteger();

		~BigInteger();

		BigInteger(const BigInteger& rhs);

		BigInteger(BigInteger&& rhs) noexcept;

		BigInteger& operator=(const BigInteger& rhs);

		BigInteger& operator=(BigInteger&& rhs) noexcept;

		/* ****** OTHER CONSTRUCTORS ****** */

		template <typename T> BigInteger(T, typename std::enable_if<std::is_unsigned<T>::value>::type* = 0);

		template <typename T> BigInteger(T, typename std::enable_if<std::is_signed<T>::value>::type* = 0);

		/* ****** MOVEMENT BETWEEN DIFFERENT SIZES ****** */

		template <uint_fast64_t _T_new_size>
		NPASSON_EXTERNAL_USAGE BigInteger<_T_new_size> resize();

		NPASSON_EXTERNAL_USAGE static void invert(BigInteger& b);

		NPASSON_EXTERNAL_USAGE BigInteger invert();

		/* OPERATORS! */

		template <uint_fast64_t _T_rhs_size>
		BigInteger<_T_bit_amount>& operator+=(BigInteger<_T_rhs_size>& rhs);

		template <uint_fast64_t _T_rhs_size>
		BigInteger<_T_bit_amount> operator+(BigInteger<_T_rhs_size>& rhs);

		template <uint_fast64_t _T_rhs_size>
		BigInteger<_T_bit_amount>& operator-=(BigInteger<_T_rhs_size>& rhs);

		template <uint_fast64_t _T_rhs_size>
		BigInteger<_T_bit_amount> operator-(BigInteger<_T_rhs_size>& rhs);

		/* ****** OUTPUT AND CASTS ******* */

		std::string str();
	};

	/* ****** THE BIG FIVE + DESTRUCTOR ****** */

	template <uint_fast64_t _T_bit_amount>
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount>::BigInteger() {
		memset(_raw_data, 0, static_cast<size_t>(sizeof(element_t) * _block_count));
	}

	template <uint_fast64_t _T_bit_amount>
	BigInteger<_T_bit_amount>::~BigInteger() {
		delete[]( _raw_data );
	}

	template <uint_fast64_t _T_bit_amount>
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount>::BigInteger(const BigInteger& rhs)
		:
		_block_count(rhs._block_count),
		_raw_data(new element_t[_block_count]) {
		std::memcpy(this->_raw_data, rhs._raw_data, static_cast<size_t>(sizeof(element_t) * _block_count));
	}

	template <uint_fast64_t _T_bit_amount>
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount>::BigInteger(BigInteger&& rhs) noexcept
		:
		_raw_data(rhs._raw_data) {}

	template <uint_fast64_t _T_bit_amount>
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator=(const BigInteger& rhs) {
		if (this != &rhs) {
			std::memcpy(this->_raw_data, rhs._raw_data, static_cast<size_t>(sizeof(element_t) * this->_block_count));
		}
		return *this;
	}

	template <uint_fast64_t _T_bit_amount>
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator=(BigInteger&& rhs) noexcept {
	    std::memmove(this->_raw_data, rhs._raw_data, static_cast<size_t>(sizeof(element_t) * this->_block_count));
        return *this;
	}


	/* ****** OTHER CONSTRUCTORS ****** */

	template<uint_fast64_t _T_bit_amount>
	template<typename T>
	BigInteger<_T_bit_amount>::BigInteger(T, typename std::enable_if<std::is_unsigned<T>::value>::type*) {
		std::cout << "unsigned" << std::endl;
		// TODO
	}

	template<uint_fast64_t _T_bit_amount>
	template<typename T>
	BigInteger<_T_bit_amount>::BigInteger(T, typename std::enable_if<std::is_signed<T>::value>::type*) {
		std::cout << "signed" << std::endl;
		// TODO
	}


	/* ****** BIT SET AND GET ****** */

	template <uint_fast64_t _T_bit_amount>
	int BigInteger<_T_bit_amount>::_get_bit(BigInteger::bit_count_t bit) {
		if (bit >= _T_bit_amount) {
			return 0;
		} else {
			block_count_t block = ( _block_count - ( bit / 64 )) - 1;
			auto shift = static_cast<sub_access_t>(bit % 64);

			return (( _raw_data[block] & ( 1ULL << shift )) != 0 ) ? 1 : 0;
		}
	}

	template <uint_fast64_t _T_bit_amount>
	bool BigInteger<_T_bit_amount>::_set_bit(BigInteger::bit_count_t bit, int val) {
		if (bit >= _T_bit_amount) {
			return false;
		} else {
			block_count_t block = ( _block_count - ( bit / 64 )) - 1;
			auto shift = static_cast<sub_access_t>(bit % 64);

			if (val != 0) {
				_raw_data[block] |= ( 1ULL << shift );
			} else {
				_raw_data[block] &= ( ~( 1ULL << shift ));
			}
			return true;
		}
	}

	/* ****** MOVEMENT BETWEEN DIFFERENT SIZES ****** */

	template <uint_fast64_t _T_bit_amount>
	template <uint_fast64_t _T_new_size>
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_new_size> BigInteger<_T_bit_amount>::resize() {
		// TODO
		return BigInteger<_T_new_size>();
	}

	template <uint_fast64_t _T_bit_amount>
	NPASSON_EXTERNAL_USAGE
	std::string BigInteger<_T_bit_amount>::str() {
		std::string ret{};
		for (bit_count_t i = _T_bit_amount; i > 0; --i) {
			ret += std::to_string(static_cast<unsigned int>(+_get_bit(i - 1)));
		}
		return ret;
	}

	template <uint_fast64_t _T_bit_amount>
	NPASSON_EXTERNAL_USAGE
	void BigInteger<_T_bit_amount>::invert(BigInteger& b) {
		for (bit_count_t i = 0; i < _T_bit_amount; ++i) {
			b._set_bit(i, !( b._get_bit(i)));
		}
	}

	template <uint_fast64_t _T_bit_amount>
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount> BigInteger<_T_bit_amount>::invert() {
		BigInteger<_T_bit_amount> temp = *this;
		invert(temp);
		return temp;
	}


	/* ****** OPERATORS ****** */

	template <uint_fast64_t _T_bit_amount>
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator+=(BigInteger<_T_rhs_size>& rhs) {
		int carry = 0;
		for (bit_count_t i = 0; i < _T_bit_amount; ++i) {
			static int amount = 0;
			amount = this->_get_bit(i) + rhs._get_bit(i) + carry;

			switch (amount) {
				case 3:
					this->_set_bit(i, 1);
					carry = 1;
					break;
				case 2:
					this->_set_bit(i, 0);
					carry = 1;
					break;
				case 1:
					this->_set_bit(i, 1);
					carry = 0;
					break;
				case 0:
				default:
					break;
			}
		}
		return *this;
	}

	template <uint_fast64_t _T_bit_amount>
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount> BigInteger<_T_bit_amount>::operator+(BigInteger<_T_rhs_size>& rhs) {
		BigInteger<_T_bit_amount> temp = *this;
		return temp += rhs;
	}

	template <uint_fast64_t _T_bit_amount>
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator-=(BigInteger<_T_rhs_size>& rhs) {
		int carry = 0;
		for (bit_count_t i = 0; i < _T_bit_amount; ++i) {
			static int amount = 0;
			amount = this->_get_bit(i) - ( rhs._get_bit(i) + carry );

			switch (amount) {
				case 1:
					carry = 0;
					break;
				case 0:
					this->_set_bit(i, 0);
					carry = 0;
					break;
				case -1:
					carry = 1;
					break;
				default:
					break;
			}
		}
		return *this;
	}

	template <uint_fast64_t _T_bit_amount>
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount> BigInteger<_T_bit_amount>::operator-(BigInteger<_T_rhs_size>& rhs) {
		BigInteger<_T_bit_amount> temp = *this;
		return temp -= rhs;
	}
}

#undef if_constexpr

#endif //NPASSON_BIGINTEGER_TYPE
