#pragma once

// required by std::move, std::sort
#include <algorithm>

// required by BlockContainer
#include "block_container.h"


namespace {

template <class StoredItemType, unsigned MaxIdCount=32>
class DynamicSet {
public:
	void print_inner_structure() {
		std::cout << "print_inner_structure" << std::endl;
		for (unsigned i=0;i<=m_top_used_id; i++) {
			const auto block_ptr = m_block_container.get(i);
			if (block_ptr) {
				std::cout << "i=" << i << "| ";
				for (unsigned j=0; j<block_ptr->size(); j++) {
					std::cout << (*block_ptr)[j] << ", ";
				}
				std::cout << std::endl;
			}
			else {
				std::cout << "i=" << i << "|" << std::endl;
			}
		}
		std::cout << std::endl;
	}
	explicit DynamicSet(unsigned max_cache_id = 16) :
		m_first_unused_id(0),
		m_top_used_id(0),
		m_size(0),
		m_block_container(max_cache_id) {
	}

	DynamicSet(const DynamicSet &) = delete;
	DynamicSet & operator = (const DynamicSet &) = delete;

	const StoredItemType * find(const StoredItemType & item) const {
		for (unsigned i=0; i<=m_top_used_id; i++) {
			const std::vector<StoredItemType> * vector_ptr =
				m_block_container.get(i);
			if (vector_ptr) {
				//bool is_found = std::binary_search(
				//		vector_ptr->begin(), vector_ptr->end(), item);
				const StoredItemType * found_item_ptr =
					binary_search(*vector_ptr, item);
				if (found_item_ptr) {
					return found_item_ptr;
				}
			}
		}
		return nullptr;
	}

	void insert(const StoredItemType & item) {
		if (m_first_unused_id) {
			insert_in_new_bigger_array(item);
		}
		else {
			std::vector<StoredItemType> * vector_ptr = m_block_container.add(0);
			(*vector_ptr)[0] = item;
			for (unsigned i=0; i<MaxIdCount; i++) {
				if (!m_block_container.get(i)) {
					m_first_unused_id = i;
					break;
				}
			}
		}
		m_size ++;
	}

	bool erase(const StoredItemType & item) {
		StoredItemType * found_item_ptr = nullptr;
		std::vector<StoredItemType> * found_item_vector_ptr = nullptr;
		for (unsigned i=0; i<=m_top_used_id; i++) {
			found_item_vector_ptr = m_block_container.get(i);
			if (!found_item_vector_ptr) {
				continue;
			}
			found_item_ptr = binary_search(*found_item_vector_ptr, item);
			if (found_item_ptr) {
				if (i==0) {
					m_block_container.extract(0);
					m_block_container.release(0);
					m_first_unused_id = 0;
					return true;
				}
				break;
			}
		}
		if (!found_item_ptr) {
			return false;
		}
		for (unsigned i=0; i<MaxIdCount;i++) {
			auto bigger_ptr = m_block_container.extract(i);
			if (!bigger_ptr) {
				continue;
			}
			m_first_unused_id = i;
			if (m_top_used_id == i) {
				m_top_used_id--;
			}
			auto input_iterator = bigger_ptr->begin();
			// move the first element in place of the element that is going to
			// be removed
			*found_item_ptr = std::move(*input_iterator);
			input_iterator++;
			if (bigger_ptr == found_item_vector_ptr) {
				// if found_item_ptr is inside the same vector then we should
				// sort all elements of the vector excluding the first one
				std::sort(input_iterator, bigger_ptr->end());
			}
			else {
				// if we changed an item to higher vector then we should sort it
				std::sort(found_item_vector_ptr->begin(),
						found_item_vector_ptr->end());
			}
			// starting from the second element move them to new lesser arrays
			unsigned lowest_used_id = i;
			for (unsigned i=0; i<lowest_used_id; i++) {
				auto lesser_ptr = m_block_container.add(i);
				std::move(
					input_iterator,
					input_iterator + lesser_ptr->size(),
					lesser_ptr->begin());
				input_iterator += lesser_ptr->size();
			}
			if (input_iterator != bigger_ptr->end()) {
				assert(0);
			}
			m_block_container.release(i);
			return true;
		}
		return false;
	}

private:

	void insert_in_new_bigger_array(const StoredItemType & item) {
		auto bigger_ptr = m_block_container.add(m_first_unused_id);
		(*bigger_ptr)[0] = item;
		typename std::vector<StoredItemType>::iterator output_iterator =
			bigger_ptr->begin() + 1;
		for (unsigned i=0; i<m_first_unused_id;i++) {
			auto smaller_ptr = m_block_container.extract(i);
			output_iterator = std::move(smaller_ptr->begin(),
										smaller_ptr->end(),
										output_iterator);
			m_block_container.release(i);
		}
		if (output_iterator != bigger_ptr->end()) {
			assert(0);
		}
		std::sort(bigger_ptr->begin(), bigger_ptr->end());
		if (m_first_unused_id > m_top_used_id) {
			m_top_used_id = m_first_unused_id;
		}
		m_first_unused_id = 0;
	}

	static const StoredItemType * binary_search(
			const std::vector<StoredItemType>& sorted_vec,
			const StoredItemType & key) {
		return binary_search(
				const_cast<std::vector<StoredItemType> &>(sorted_vec), key);
	}

	static StoredItemType * binary_search(
			std::vector<StoredItemType>& sorted_vec,
			const StoredItemType & key) {
		size_t mid, left = 0 ;
		size_t right = sorted_vec.size();
		while (left < right) {
				mid = left + (right - left)/2;
			if (key > sorted_vec[mid]){
				left = mid+1;
			}
			else if (key < sorted_vec[mid]){
				right = mid;
			}
			else {
				return &(sorted_vec[mid]);
			}
		}
		return nullptr;
	}


	unsigned m_first_unused_id;
	unsigned m_top_used_id;
	size_t m_size;

	BlockContainer<StoredItemType, MaxIdCount> m_block_container;
};

}
