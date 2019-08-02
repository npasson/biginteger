## BigInteger
An arbitrary size integer class. Arbitrary size in this context means "as much as you can allocate at once".
### THIS IS A DEVELOPMENT VERSION. THERE IS NO GUARANTEE FOR FEATURES, AND SYNTAX MAY CHANGE ARBITRARILY.

## Code Example

The BigInteger type has to have its size specified at compile time, but can be resized later:

```
#include <iostream>
#include "biginteger/include/biginteger.h"

int main() {
	npasson::BigInteger<120 /* bit */> a = 123456789ull;
	npasson::BigInteger<120 /* bit */> b = 123456789ull;

	std::cout << a.str() << std::endl;
	std::cout << b.str() << std::endl;

	b += a;

	std::cout << b.str() << std::endl;
	return 0;
}
```
Since this is a dev version, the code example does not portray the full capabilities of the end result and syntax may change.

## Motivation
Big numbers. Who needs 64 bit integers when you can have 64,000,000 bit integers?

## Usage

**1\.**
 `cd` into your project folder, and run
```
git clone github.com/npasson/biginteger
```
This will download the code.

**2\.**
Add these two lines at the top of your program:
```
#include "biginteger/include/biginteger.h"
using npasson::BigInteger;
```
In case you're using two BigInteger implementations, you can leave out the second line and access the type via the `npasson` namespace (just replace the affected `BigInteger`s by `npasson::BigInteger`).
## Reference

Positive range:
from `0`
to `<arbitrary>`

A documentation is not yet available, but a temporary feature tracker can be found at <http://www.npasson.com/biginteger>.

## License

**BigInteger**  
**Copyright (C) 2018  Nicholas Passon**  

This program is free software: you can redistribute it and/or modify  it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details. 

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
