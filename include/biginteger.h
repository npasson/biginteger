/*==================================================================================*\
 *
 *   BigInteger Type v0.0.1 DEVELOPMENT VERSION
 *   Copyright (C) 2018  Nicholas Passon
 *   Documentation: http://www.npasson.com/
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published
 *   by the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
\*==================================================================================*/

#ifndef NPASSON_BIGINTEGER_TYPE
#define NPASSON_BIGINTEGER_TYPE

#pragma message(\
"BIGINTEGER v0.0.1 - This is a version of the BigInteger library IN DEVELOPMENT. \
Use with caution, and keep in mind this library is distributed WITHOUT ANY WARRANTY."\
)

#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>

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

#define NPASSON_BIGINTEGER_SIZE(a) template <uint_fast64_t a>

#define NPASSON_BIGINTEGER_CTORDECL(...) \
template <typename T> \
BigInteger(T val, typename std::enable_if<__VA_ARGS__>::type* = 0)

#define NPASSON_BIGINTEGER_CTORDEF(...) \
template <typename T> \
NPASSON_EXTERNAL_USAGE \
BigInteger<_T_bit_amount>::BigInteger( \
        T val, \
        typename std::enable_if<__VA_ARGS__>::type* \
)

namespace npasson {
	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	class NPASSON_EXTERNAL_USAGE BigInteger {
	private:
		typedef uint64_t      element_t;
		typedef uint_fast64_t block_count_t;
		typedef uint_fast64_t bit_count_t;
		typedef uint_fast8_t  sub_access_t;

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

		NPASSON_BIGINTEGER_CTORDECL(std::is_unsigned<T>::value&& std::is_integral<T>::value); // NOLINT

		NPASSON_BIGINTEGER_CTORDECL(std::is_signed<T>::value&& std::is_integral<T>::value); // NOLINT

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

		BigInteger<_T_bit_amount>& operator++();

		const BigInteger<_T_bit_amount> operator++(int);

		/* ****** OUTPUT AND CASTS ******* */

		char* str();
	};

	/* ****** THE BIG FIVE + DESTRUCTOR ****** */

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount>::BigInteger() {
		memset(_raw_data, 0, static_cast<size_t>(sizeof(element_t) * _block_count));
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	BigInteger<_T_bit_amount>::~BigInteger() {
		delete[]( _raw_data );
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount>::BigInteger(const BigInteger& rhs)
			:
			_block_count(rhs._block_count),
			_raw_data(new element_t[_block_count]) {
		std::memcpy(this->_raw_data, rhs._raw_data, static_cast<size_t>(sizeof(element_t) * _block_count));
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount>::BigInteger(BigInteger&& rhs) noexcept
			:
			_raw_data(rhs._raw_data) {}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator=(const BigInteger& rhs) {
		if (this != &rhs) {
			std::memcpy(this->_raw_data, rhs._raw_data, static_cast<size_t>(sizeof(element_t) * this->_block_count));
		}
		return *this;
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator=(BigInteger&& rhs) noexcept {
		std::memmove(this->_raw_data, rhs._raw_data, static_cast<size_t>(sizeof(element_t) * this->_block_count));
		return *this;
	}


	/* ****** OTHER CONSTRUCTORS ****** */

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_BIGINTEGER_CTORDEF(std::is_unsigned<T>::value&& std::is_integral<T>::value)
			: BigInteger() {
		bit_count_t      val_size = sizeof(T) * 8;
		for (bit_count_t i        = 0; i < val_size; ++i) {
			int bit = ( val & ( ( (bit_count_t) 1 ) << i ) ) != 0;
			this->_set_bit(i, bit);
		}
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_BIGINTEGER_CTORDEF(std::is_signed<T>::value&& std::is_integral<T>::value)
			: BigInteger<_T_bit_amount>(static_cast<typename std::make_unsigned<T>::type>(val)) {}

	/* ****** BIT SET AND GET ****** */

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	int BigInteger<_T_bit_amount>::_get_bit(BigInteger::bit_count_t bit) {
		if (bit >= _T_bit_amount) {
			return 0;
		} else {
			block_count_t block = ( _block_count - ( bit / 64 ) ) - 1;
			auto          shift = static_cast<sub_access_t>(bit % 64);

			return ( ( _raw_data[block] & ( 1ULL << shift ) ) != 0 ) ? 1 : 0;
		}
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	bool BigInteger<_T_bit_amount>::_set_bit(BigInteger::bit_count_t bit, int val) {
		if (bit >= _T_bit_amount) {
			return false;
		} else {
			block_count_t block = ( _block_count - ( bit / 64 ) ) - 1;
			auto          shift = static_cast<sub_access_t>(bit % 64);

			if (val != 0) {
				_raw_data[block] |= ( 1ULL << shift );
			} else {
				_raw_data[block] &= ( ~( 1ULL << shift ) );
			}
			return true;
		}
	}

	/* ****** MOVEMENT BETWEEN DIFFERENT SIZES ****** */

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	template <uint_fast64_t _T_new_size>
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_new_size> BigInteger<_T_bit_amount>::resize() {
		// TODO
		return BigInteger<_T_new_size>();
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_EXTERNAL_USAGE
	char* BigInteger<_T_bit_amount>::str() {
		std::string ret {};
		bit_count_t i = _T_bit_amount;

		char* const ret_c = new char[_T_bit_amount + 1];
		ret_c[_T_bit_amount] = '\0';

		while (i-- > 0) {
			ret_c[_T_bit_amount - ( i + 1 )] = static_cast<char>('0' + _get_bit(i));
		}

		return ret_c;
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_EXTERNAL_USAGE
	void BigInteger<_T_bit_amount>::invert(BigInteger& b) {
		for (bit_count_t i = 0; i < _T_bit_amount; ++i) {
			b._set_bit(i, !( b._get_bit(i) ));
		}
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	NPASSON_EXTERNAL_USAGE
	BigInteger<_T_bit_amount> BigInteger<_T_bit_amount>::invert() {
		BigInteger<_T_bit_amount> temp = *this;
		invert(temp);
		return temp;
	}


	/* ****** OPERATORS ****** */

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator+=(BigInteger<_T_rhs_size>& rhs) {
		int              carry = 0;
		for (bit_count_t i     = 0; i < _T_bit_amount; ++i) {
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

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount> BigInteger<_T_bit_amount>::operator+(BigInteger<_T_rhs_size>& rhs) {
		BigInteger<_T_bit_amount> temp = *this;
		return temp += rhs;
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator-=(BigInteger<_T_rhs_size>& rhs) {
		int              carry = 0;
		for (bit_count_t i     = 0; i < _T_bit_amount; ++i) {
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

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	template <uint_fast64_t _T_rhs_size>
	BigInteger<_T_bit_amount> BigInteger<_T_bit_amount>::operator-(BigInteger<_T_rhs_size>& rhs) {
		BigInteger<_T_bit_amount> temp = *this;
		return temp -= rhs;
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	BigInteger<_T_bit_amount>& BigInteger<_T_bit_amount>::operator++() {
		BigInteger<1> rhs(1u);
		return ( *this += rhs );
	}

	NPASSON_BIGINTEGER_SIZE(_T_bit_amount)
	const BigInteger<_T_bit_amount> BigInteger<_T_bit_amount>::operator++(int) {
		BigInteger result(*this);
		++( *this );
		return result;
	}
}

#undef if_constexpr
#undef NPASSON_BIGINTEGER_SIZE
#undef NPASSON_EXTERNAL_USAGE

#endif //NPASSON_BIGINTEGER_TYPE
