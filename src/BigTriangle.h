#ifndef BIG_TRIANGLE_H
#define BIG_TRIANGLE_H

#include "_header.h"

class BigTriangle {
public:
	BigTriangle(int max_lod, unsigned int index_start);
	~BigTriangle();

	inline int NumStrips() const { return mNumStrips[mLod]; }
	//inline void setLod(int lod) { mLod = (lod > mMaxLod ? mMaxLod : (lod < 0 ? lod = 0 : lod) ); }
	inline void incLod() { mLod = (mLod + 1 > mMaxLod ? mMaxLod : mLod + 1); }
	inline void decLod() { mLod = (mLod - 1 < 0 ? 0 : mLod - 1); }
	inline TriStrip** Strips() { return mTriStrips[mLod]; }
	inline int Lod() const { return mLod; }

	void CreateStrips(std::map<unsigned int, unsigned int>& index_map);
	inline unsigned int IndexStart() const { return mIndexStart; }
private:
	int mLod;
	int mMaxLod;
	TriStrip*** mTriStrips; //for each LOD, for each tri strip
	int* mNumStrips;
	unsigned int mIndexStart;
};

#endif