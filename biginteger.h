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

#include <cstdint>
#include <string>

namespace npasson {
	template <uint_fast64_t BIT_AMOUNT>
	class BigInteger {
	private:
		const uint_fast64_t ELEMENT_COUNT = (BIT_AMOUNT%64) + 1;
		uint_fast64_t * const _m_array = new uint_fast64_t[ELEMENT_COUNT];

	public:
		BigInteger() {
			for (int i = 0; i < ELEMENT_COUNT; ++i) {
				this->_m_array[i] = 0;
			}
		};

		~BigInteger() {
			delete[] _m_array;
		}

	};
}

#endif
