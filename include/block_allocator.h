#pragma once

// required by std::vector
#include <vector>

// required by std::memset
#include <cstring>

// required by assert
#include <cassert>


namespace {

template <class StoredItemType, unsigned MaxIdCount>
class BlockAllocator {
public:
	explicit BlockAllocator(unsigned max_cache_id) :
		m_max_cache_id(max_cache_id) {
		std::memset(m_blocks, 0, sizeof(m_blocks));
	}

	~BlockAllocator() {
		unsigned block_count =
			sizeof(m_blocks) / sizeof(std::vector<StoredItemType> *);
		for (unsigned i=0; i<block_count; i++) {
			if (m_blocks[i]) {
				delete  m_blocks[i];
			}
		}
	}

	BlockAllocator(const BlockAllocator&) = delete;
	BlockAllocator & operator = (const BlockAllocator &) = delete;

	std::vector<StoredItemType> * get(unsigned id) {
		unsigned size = get_size(id);
		if (!(m_blocks[id])) {
			m_blocks[id] = new std::vector<StoredItemType>(size, 0);
		}
		return m_blocks[id];
	}

	static unsigned get_size(unsigned id) {
		unsigned size = 0x1 << id;
		return size;
	}

	void free(unsigned id) {
		if (id > m_max_cache_id) {
			delete m_blocks[id];
			m_blocks[id] = nullptr;
		}
	}

private:

	unsigned m_max_cache_id;
	std::vector<StoredItemType> * m_blocks[MaxIdCount];
};

}
