/*==================================================================================*\
 *
 *   BigInteger Type
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

#ifndef NPASSON_BIGINTEGER
#define NPASSON_BIGINTEGER

#define MOVE_LEFT(a) (static_cast<element_t>(1) << (a))
#define CASE(a, b) ((this->_m_bit(i).get() == (a)) && (rhs._m_bit(i).get() == (b)))

#include <cstdint>
#include <string>
#include <bitset>

namespace npasson {
	class BigInteger {
	private:
		typedef uint64_t element_t;
		typedef uint_fast64_t block_count_t;
		typedef uint_fast64_t bit_count_t;
		typedef uint_fast8_t sub_access_t;

		const bit_count_t _C_bit_amount;
		const block_count_t _C_element_count;
		uint64_t* const _raw_data;

		/**
		 *  A bit class used to modify single bits.
		 *
		 *  To be used as a singleton.
		 *
		 *  Bits are counted from the right, the lowest bit is bit 0.
		 *
		 *  Blocks are counted from the left, due to varying amounts.
		 */
		class Bit {
		private:
			BigInteger* _parent;
			sub_access_t _access_bit = 0;
			block_count_t _access_block = 0;
			element_t* _bit_data;
			bool _inaccessible_flag = false;

			element_t* orig_data_point() const;

			block_count_t get_block_of_bit(bit_count_t) const;

		public:
			explicit Bit(BigInteger* parent)
				:
				_parent(parent),
				_bit_data(orig_data_point()) {}

			~Bit() = default;

			Bit& operator()(bit_count_t);

			template <typename T>
			Bit& operator=(T);

			bool get();
		};

		friend Bit;
		Bit _m_bit = Bit(this);

	public:
		BigInteger() = delete;

		BigInteger(bit_count_t size); // NOLINT
		~BigInteger();

		std::string str();

		void test(bit_count_t bit);
		void set(bit_count_t bit, int set);

		BigInteger operator+=(BigInteger& rhs);
	};

	/* ************************************************************************************************************ */

	/**
	 * Returns the block index (0-indexed) in which the bit `num` is located.
	 * @param num
	 * @return
	 */
	BigInteger::block_count_t BigInteger::Bit::get_block_of_bit(BigInteger::bit_count_t num) const {
		return ( _parent->_C_element_count ) - ( num / 64 ) - 1;
	}

	/**
	 * Chooses a bit from the right.
	 * @param bit The bit to be chosen, from the right.
	 * @return
	 */
	BigInteger::Bit& BigInteger::Bit::operator()(BigInteger::bit_count_t bit) {
		if (bit > _parent->_C_bit_amount) {
			this->_inaccessible_flag = true;
			return *this;
		} else {
			this->_inaccessible_flag = false;
			this->_access_block = get_block_of_bit(bit);
			this->_bit_data = orig_data_point() + _access_block;
			this->_access_bit = static_cast<sub_access_t>(bit % 64);
			return *this;
		}
	}

	/**
	 * Sets the bit that was previously chosen by operator() to either 0 or 1.
	 *
	 * All values != 0 count as 1.
	 *
	 * @param bit_val 0 or 1, or any other integer
	 * @return The bit itself.
	 */
	template <typename T>
	BigInteger::Bit& BigInteger::Bit::operator=(T bit_val) {
		if (_inaccessible_flag) {
			return *this;
		}

		if (bit_val == 0) {
			/*
			 * Step 1: Set the nth bit from the left to 1
			 *                          MOVE_LEFT(_access_bit)
			 *
			 * Step 2: invert it, so that e.g. 001000 becomes 110111
			 *                        ~(                      )
			 *
			 * Step 3: AND the values, essentially setting the one bit that's 0 after inverting to 0 in the byte
			 *                   &= (                          )
			 */
			*( this->_bit_data ) &= ( ~(MOVE_LEFT(_access_bit)));
		} else {
			/*
			 * Step 1: Set the nth bit from the left to 1
			 *                       MOVE_LEFT(_access_bit)
			 *
			 * Step 2: OR the values, essentially setting the one bit that's 1 to 1 in the byte
			 *                   |= (                      )
			 */
			*( this->_bit_data ) |= (MOVE_LEFT(_access_bit));
		}

		return *this;
	}

	bool BigInteger::Bit::get() {
		if (_inaccessible_flag) {
			return false;
		} else {
			return (( *( this->_bit_data )) & (MOVE_LEFT(_access_bit))) != 0;
		}
	}

	BigInteger::element_t* BigInteger::Bit::orig_data_point() const {
		return _parent->_raw_data;
	}

	/* ************************************************************************************************************ */

	/**
	 * Constructs a BigInteger of the given size.
	 * @param size The amount of bits the
	 */
	BigInteger::BigInteger(BigInteger::bit_count_t size)  // NOLINT
		: _C_bit_amount(size),
		  _C_element_count((( size - 1 ) / 64 ) + 1),
		  _raw_data(new element_t[_C_element_count]) {
		for (block_count_t i = 0; i < _C_element_count; ++i) {
			this->_raw_data[i] = 0;
		}
	}

	BigInteger::~BigInteger() {
		delete[] _raw_data;
	}

	std::string BigInteger::str() {
		std::string str;
		/*
		bit_count_t i = ( _C_element_count * 64 ) - 1;
		while (true) {
			str += std::to_string(_m_bit(i).get());
			if (--i == 0) break;
		}
		return str;
		 */

		for (block_count_t i = 0; i < _C_element_count; ++i) {
			str += std::bitset<64>(_raw_data[i]).to_string();
		}
		return str;
	}

	void BigInteger::test(BigInteger::bit_count_t bit) {
		_m_bit(bit) = 1;
		printf("%s\n", this->str().c_str());
	}

	void BigInteger::set(BigInteger::bit_count_t bit, int set) {
		this->_m_bit(bit) = set;
	}

	BigInteger BigInteger::operator+=(BigInteger& rhs) {
		if (this->_C_bit_amount == rhs._C_bit_amount) {
			for (bit_count_t i = 0; i < _C_bit_amount; ++i) {

				static bool carry = false;
				int amount = static_cast<uint_fast8_t>(carry)
				             + this->_m_bit(i).get()
				             + rhs._m_bit(i).get();

				//printf("Amount at location %llu is %d\n", i, amount);

				switch (amount) {
					case 3:
						this->_m_bit(i) = 1;
						carry = true;
						break;
					case 2:
						this->_m_bit(i) = 0;
						carry = true;
						break;
					case 1:
						this->_m_bit(i) = 1;
						carry = false;
						break;
					case 0:
						break;
					default:
						break;
				}

				//printf("Set the bit at location %llu to %d, carry to %d\n", i, this->_m_bit(i).get(), carry);
			}
			return *this;
		} else if (this->_C_bit_amount > rhs._C_bit_amount) {
			return *this;
		} else {
			return *this;
		}
	}



}

#endif
