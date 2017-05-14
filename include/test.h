#pragma once

#include <chrono>


namespace {

unsigned number_of_tests = 0;
const unsigned & number_to_insert = number_of_tests;
const unsigned & number_to_find = number_of_tests;
const unsigned & number_to_erase = number_of_tests;

template <class SetType>
void test_insert(SetType & set) {
	for (unsigned i=0; i<number_to_insert; i++) {
		set.insert(i);
	}
}

template <class SetType>
void test_find(const SetType & set) {
	for (unsigned i=0; i<number_to_insert; i++) {
		set.find(i);
	}
}

template <class SetType>
void test_erase(SetType & set) {
	for (unsigned i=0; i<number_to_insert; i++) {
		set.remove(i);
	}
}

template <class SetType, class TestFunctionType>
unsigned long get_duration(SetType & set, TestFunctionType fn) {
	typedef std::chrono::system_clock ClockType;
	std::chrono::time_point<ClockType> tm = ClockType::now();
	auto before_insert = tm.time_since_epoch();

	fn(set);

	tm = ClockType::now();
	auto time_diff = tm.time_since_epoch() - before_insert;
	auto duration =
		std::chrono::duration_cast<std::chrono::milliseconds>(time_diff);
	return duration.count();
}

template <class SetType>
unsigned long get_insert_duration(SetType & set) {
	return get_duration(set, test_insert<SetType>);
}

template <class SetType>
unsigned long get_find_duration(SetType & set) {
	return get_duration(set, test_find<SetType>);
}

template <class SetType>
unsigned long get_erase_duration(SetType & set) {
	return get_duration(set, test_find<SetType>);
}


}
