#ifndef TRI_STRIP_H
#define TRI_STRIP_H

#include "_header.h"

class TriStrip {
public:
	TriStrip(int num_indices);
	~TriStrip();

	inline void setIndex(int offset, unsigned int value) { mIndices[offset] = value; }
	inline unsigned int* Indices() const { return mIndices; }
	inline int IndexCount() const { return mNumIndices; }
private:
	unsigned int* mIndices;
	int mNumIndices;
};

#endif