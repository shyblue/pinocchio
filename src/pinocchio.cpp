#include "pinocchio.h"
#include <string>


int main(void)
{
	std::string error;

	auto conf = ucl::Ucl::parse("pinocchio.conf",error);
	return 0;
}
