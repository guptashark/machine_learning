#include <iostream>

#include "tests.h"

int main(int argc, char *argv[]) {

	(void)argc;
       	(void)argv;

	test_stat_util();
	test_linear_model();

	std::cout << "Machine Learning" << std::endl;
	return 0;
}
