//
// A simple example, which demonstrates the specifics of NaN and Inf
//

#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>

bool equal(double A, double B, double epsilon = 1e-10) {
	return
		std::isnan(A) && std::isnan(B) ||
		std::isinf(A) && std::isinf(B) && A == B ||
		std::abs(A - B) < epsilon;
}

bool less(double A, double B, double epsilon = 1e-10) {
	return !equal(A, B) && (A < B);
}

#define P(x) std::cout << #x " \033[90m--> \033[33m" << (x) << "\033[0m\n";

void h1(const char* text)
{
	size_t len = strlen(text) + 3;

	std::cout << "\n\033[36m# " << text << "\033[0m\n";

	for(size_t i = 0; i < len; ++i)
		std::cout << '-';

	std::cout << std::endl;
}

void h2(const char* text)
{
	std::cout << "\n\033[96m## " << text << "\033[0m\n";
}

int main()
{
	std::cout << std::boolalpha;

	h1("equality");
	P(NAN == NAN);
	P(NAN == INFINITY);
	P(NAN == -INFINITY);
	P(NAN == 5);
	P(INFINITY == INFINITY);
	P(-INFINITY == INFINITY);
	P(-INFINITY == -INFINITY);
	P(5 == -INFINITY);
	P(5 == INFINITY);

	h1("less than");
	P(NAN < NAN);
	P(NAN < INFINITY);
	P(NAN < -INFINITY);
	P(INFINITY < NAN);
	P(-INFINITY < NAN);
	P(NAN < 5);
	P(5 < NAN);
	P(INFINITY < INFINITY);
	P(-INFINITY < INFINITY);
	P(-INFINITY < -INFINITY);
	P(5 < INFINITY);
	P(5 < -INFINITY);
	P(INFINITY < 5);
	P(-INFINITY < 5);

	h1("equal()");

	h2("NAN");
	P(equal(NAN, NAN));
	P(equal(NAN, INFINITY));
	P(equal(INFINITY, NAN));

	P(equal(5, NAN));
	P(equal(NAN, 5));

	h2("INFINITY");
	P(equal(INFINITY, INFINITY));
	P(equal(INFINITY, -INFINITY));
	P(equal(-INFINITY, INFINITY));

	P(equal(5, INFINITY));
	P(equal(INFINITY, 5));
	P(equal(5, -INFINITY));
	P(equal(-INFINITY, 5));

	h2("other");
	P(equal(5,5));

	h1("less()");

	P(less(NAN, NAN));
	P(less(NAN, INFINITY));
	P(less(INFINITY, NAN));

	P(less(5, NAN));
	P(less(NAN, 5));

	P(less(INFINITY, INFINITY));
	P(less(INFINITY, -INFINITY));
	P(less(-INFINITY, INFINITY));

	P(less(5, INFINITY));
	P(less(INFINITY, 5));
	P(less(5, -INFINITY));
	P(less(-INFINITY, 5));

	P(less(5,5));

	h1("arithmetic operations");

	double max = std::numeric_limits<double>::max();
	double min = std::numeric_limits<double>::min();
	double lowest = std::numeric_limits<double>::lowest();
	double epsilon = std::numeric_limits<double>::epsilon();
	
	h2("values");
	P(max);
	P(min);
	P(lowest);

	h2("max");
	P(max+epsilon);
	P(max+1);
	P(max+max);

	h2("lowest");
	P(lowest-min);
	P(lowest-1);
	P(lowest+lowest);

	h2("min");
	P(min-epsilon);
	P(min-1);
	P(1+min);
	P(min+min);
	P(min-max);
	P(max+min);
	P(min-max-max);
	P(min+max);

	h2("NAN");
	P(NAN+1);
	P(NAN-1);

	h2("INFINITY");
	P(INFINITY + 1);
	P(INFINITY - 1);
	P(1 - INFINITY);
	P(1 + INFINITY);
	P(INFINITY + INFINITY);
	P(INFINITY - INFINITY);
}
