#include <iostream>

#include "_biginteger_t.h"

int main() {
	npasson::BigInteger<120> a;
	npasson::BigInteger<120> b;

	b._set_bit(1,1);
	b._set_bit(0,1);

	a._set_bit(110, 1);
	a._set_bit(0, 1);

	std::cout << a.str() << std::endl;
	std::cout << b.str() << std::endl;

	b += a;

	std::cout << b.str() << std::endl;
}