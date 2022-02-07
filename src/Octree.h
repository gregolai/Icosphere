#ifndef OCTREE_H
#define OCTREE_H

#include "_header.h"
#include "SmallList.h"

class Octree
{
public:
	Octree(int bucket_minimum, int bucket_capacity, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z);
	bool InsertItem(I3dCoord* item);
	void ItemsWithinRadius(float x, float y, float z, float radius, SmallList<I3dCoord*>& items);
	void Draw();
private:


	class Bucket {
		friend class Octree;
	public:
		Bucket(Bucket* parent, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z);
		void Merge();
		void Split();

		inline void InsertItem(I3dCoord* item) { mItems.PushToBack(item); }

		inline float MinX() const { return mMinX; }
		inline float MaxX() const { return mMaxX; }
		inline float MinY() const { return mMinY; }
		inline float MaxY() const { return mMaxY; }
		inline float MinZ() const { return mMinZ; }
		inline float MaxZ() const { return mMaxZ; }
		inline float MiddleX() const { return mMiddleX; }
		inline float MiddleY() const { return mMiddleY; }
		inline float MiddleZ() const { return mMiddleZ; }
		inline Bucket* FrontDownLeft() const { return mKids[0]; }
		inline Bucket* FrontUpLeft() const { return mKids[1]; }
		inline Bucket* FrontUpRight() const { return mKids[2]; }
		inline Bucket* FrontDownRight() const { return mKids[3]; }
		inline Bucket* BackDownLeft() const { return mKids[4]; }
		inline Bucket* BackUpLeft() const { return mKids[5]; }
		inline Bucket* BackUpRight() const { return mKids[6]; }
		inline Bucket* BackDownRight() const { return mKids[7]; }
		inline bool HasKids() const { return (FrontDownLeft() && true); }
		inline Bucket* Parent() const { return mParent; }
		inline int Count() { return CountRecursive(this); }
	private:
		void MergeRecursive(Bucket* cur);
		int CountRecursive(Bucket* cur);
		Bucket* mKids[8];
		Bucket* mParent;
		float mMinX;
		float mMaxX;
		float mMinY;
		float mMaxY;
		float mMinZ;
		float mMaxZ;
		float mMiddleX;
		float mMiddleY;
		float mMiddleZ;
		SmallList<I3dCoord*> mItems;
	};

private:
	Bucket* BucketAt(float x, float y, float z);
	void DrawBucketRecursive(Bucket* cur);

	bool IsWithinRadius(float x1, float y1, float z1, float x2, float y2, float z2, float radius);
	void ItemsWithinRadiusRecursive(float sphere_x, float sphere_y, float sphere_z, float cur_x, float cur_y, float cur_z,
		float radius, Bucket* cur_bucket, SimpleBinaryTree<Bucket*>& checked, SmallList<I3dCoord*>& items);

	int mBucketMinimum;				//	bucket minimum -> merge
	int mBucketCapacity;			//	bucket capacity -> split
	static float mDeltaAmount;		//	small increment used for edge adjacencies
	Bucket mRoot;
};


#endif