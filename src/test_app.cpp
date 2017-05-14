#include <iostream>
#include <set>

#include "dynamic_set.h"

#include "test.h"


int main() {
	std::cout << "Enter the number of tests for each operation" << std::endl;

	std::cin >> number_of_tests;

	typedef DynamicSet<int> DynamicSetType;
	DynamicSetType dn_set;
	typedef std::set<int> RBTreeSetType;
	RBTreeSetType rb_set;

	std::cout << "Number of iterations " << number_of_tests << std::endl;
	std::cout << std::endl;

	std::cout << "RB Tree insert duration " << get_insert_duration(rb_set)
		<< std::endl;

	std::cout << "Dynamic Array insert duration " << get_insert_duration(dn_set)
		<< std::endl;

	std::cout << std::endl;

	std::cout << "RB Tree find duration " << get_find_duration(rb_set)
		<< std::endl;

	std::cout << "Dynamic Array find duration " << get_find_duration(dn_set)
		<< std::endl;

	std::cout << std::endl;

	std::cout << "RB Tree erase duration " << get_erase_duration(rb_set)
		<< std::endl;

	std::cout << "Dynamic Array erase duration " << get_erase_duration(dn_set)
		<< std::endl;

	return 0;
}
