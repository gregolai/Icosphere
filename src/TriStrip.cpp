#include "_include.h"

TriStrip::TriStrip(int num_indices)
{
	mNumIndices = num_indices;
	mIndices = new unsigned int[num_indices];
}

TriStrip::~TriStrip()
{
	delete[] mIndices;
}