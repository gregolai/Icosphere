#include "_include.h"

BigTriangle::BigTriangle(int max_lod, unsigned int index_start)
	: mLod(max_lod), mMaxLod(max_lod), mIndexStart(index_start)
{
	mNumStrips = new int[max_lod + 1];					//alloc space for strip counts
	mTriStrips = new TriStrip**[max_lod + 1];				//alloc space for tri strips for each lod
	for (int lod = 0; lod <= max_lod; ++lod) {					//for each lod
		mNumStrips[lod] = pow(2.0L, lod);						//set num strips for this lod
		mTriStrips[lod] = new TriStrip*[mNumStrips[lod]];		//alloc space for tri strip array in this lod
		for (int s = 0; s<mNumStrips[lod]; ++s) {					//for each strip in this lod
			mTriStrips[lod][s] = new TriStrip(2 * (mNumStrips[lod] - s) + 1);		//alloc tri strip for each place in array
		}
	}
}

BigTriangle::~BigTriangle()
{
	for (int lod = 0; lod <= mMaxLod; ++lod) {
		for (int s = 0; s<mNumStrips[lod]; ++s) {
			delete mTriStrips[lod][s];
		}
		delete[] mTriStrips[lod];
	}
	delete[] mTriStrips;
	delete[] mNumStrips;
}


void BigTriangle::CreateStrips(std::map<unsigned int, unsigned int>& index_map)
{
	int offset;
	TriStrip* stripPtr;


	for (int lod = mMaxLod; lod >= 0; --lod) {
		if (lod == mMaxLod) {
			int p0, p1;
			p0 = 0;
			p1 = pow(2.0L, lod) + 1;
			for (int s = 0; s<mNumStrips[lod]; ++s) {		//for each strip within each face						
				offset = 0;
				stripPtr = mTriStrips[lod][s];
				while (offset < stripPtr->IndexCount() - 1) {
					stripPtr->setIndex(offset++, index_map[mIndexStart + (p0++)]);
					stripPtr->setIndex(offset++, index_map[mIndexStart + (p1++)]);
				}
				stripPtr->setIndex(offset++, index_map[mIndexStart + (p0++)]);
			}
		}
		else {
			unsigned int* p0;
			unsigned int* p1;
			int sp = 0;
			for (int s = 0; s<mNumStrips[lod]; ++s) {
				p0 = mTriStrips[lod + 1][sp]->Indices();
				if (s == mNumStrips[lod] - 1) {
					p1 = mTriStrips[lod + 1][sp + 1]->Indices();
					++p1;
				}
				else {
					p1 = mTriStrips[lod + 1][sp + 2]->Indices();
				}
				offset = 0;
				stripPtr = mTriStrips[lod][s];
				while (offset < stripPtr->IndexCount() - 1) {
					stripPtr->setIndex(offset++, *p0);
					p0 += 4;
					stripPtr->setIndex(offset++, *p1);
					p1 += 4;
				}
				stripPtr->setIndex(offset++, *p0);
				sp += 2;
			}
		}
	}
}