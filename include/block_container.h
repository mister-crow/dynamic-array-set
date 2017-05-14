#pragma once

// required by BlockAllocator
#include "block_allocator.h"
//
// required by std::memset
#include <cstring>


namespace {

template <class StoredItemType, unsigned MaxIdCount>
class BlockContainer {
public:
	explicit BlockContainer(unsigned max_cache_id) :
		m_allocator(max_cache_id) {
		std::memset(m_blocks, 0, sizeof(m_blocks));
	}

	BlockContainer(const BlockContainer &) = delete;
	BlockContainer & operator = (const BlockContainer &) = delete;

	const std::vector<StoredItemType> * get(unsigned id)  const {
		return m_blocks[id];
	}
	std::vector<StoredItemType> * get(unsigned id) {
		return m_blocks[id];
	}

	std::vector<StoredItemType> * add(unsigned id) {
		m_blocks[id] = m_allocator.get(id);
		return m_blocks[id];
	}
	std::vector<StoredItemType> * extract(unsigned id) {
		std::vector<StoredItemType> * array_ptr = m_blocks[id];
		m_blocks[id] = nullptr;
		return array_ptr;
	}
	void release(unsigned id) {
		m_allocator.free(id);
	}

	static unsigned get_size(unsigned id) {
		return BlockAllocator<StoredItemType, MaxIdCount>::get_size(id);
	}

private:
	BlockAllocator<StoredItemType, MaxIdCount> m_allocator;
	std::vector<StoredItemType> * m_blocks[MaxIdCount];
};

}
