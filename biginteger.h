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

#include <cstdint>
#include <string>

namespace npasson {
	class BigInteger {
	private:
		typedef uint64_t element_t;
		typedef uint_fast64_t block_count_t;
		typedef uint_fast64_t bit_count_t;
		typedef uint_fast8_t sub_access_t;

		const bit_count_t _bit_amount;
		const block_count_t ELEMENT_COUNT;
		uint64_t *const _raw_data;

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
			BigInteger *_parent;
			sub_access_t _access_bit = 0;
			block_count_t _access_block = 0;
			element_t *const _orig_data_point;
			element_t *_bit_data;

			bool inaccessible_flag = false;

			block_count_t get_block_of_bit(bit_count_t num) {
				return ( _parent->ELEMENT_COUNT ) - ( num / 64 ) - 1;
			}

		public:
			explicit Bit(BigInteger *parent) :
				_parent(parent),
				_orig_data_point(parent->_raw_data),
				_bit_data(_orig_data_point) {}

			~Bit() = default;

			/**
			 * Chooses a bit from the right.
			 * @param bit The bit to be chosen, from the right.
			 * @return
			 */
			Bit &operator()(bit_count_t bit) {
				if (bit > _parent->_bit_amount) {
					this->inaccessible_flag = true;
					return *this;
				} else {
					this->inaccessible_flag = false;
					this->_access_block = get_block_of_bit(bit);
					this->_bit_data = _orig_data_point + _access_block;
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
			Bit &operator=(T bit_val) {
				if (inaccessible_flag) {
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

			bool get() {
				if (inaccessible_flag) {
					return false;
				} else {
					return (( *( this->_bit_data )) & (MOVE_LEFT(_access_bit))) != 0;
				}
			}
		};

		friend Bit;
		Bit _m_bit = Bit(this);

	public:
		BigInteger(bit_count_t size) // NOLINT
			: _bit_amount(size),
			  ELEMENT_COUNT((( size - 1 ) / 64 ) + 1),
			  _raw_data(new element_t[ELEMENT_COUNT]) {
			for (block_count_t i = 0; i < ELEMENT_COUNT; ++i) {
				this->_raw_data[i] = 0;
			}
		};

		~BigInteger() {
			delete[] _raw_data;
		}

		std::string str() {
			std::string str;
			bit_count_t i = ( ELEMENT_COUNT * 64 ) - 1;
			while (true) {
				str += std::to_string(_m_bit(i).get());
				if (--i == 0) break;
			}
			return str;
		}

		void test(bit_count_t bit) {
			_m_bit(bit) = 1;
			std::cout << this->str() << std::endl;
		}

	};
}

#endif
