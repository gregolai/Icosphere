#include "_include.h"

float Octree::mDeltaAmount = 1.0e-30f;

Octree::Octree(int bucket_minimum, int bucket_capacity, float min_x, float max_x, float min_y, float max_y,
				  float min_z, float max_z)
	:	mBucketMinimum(bucket_minimum), mBucketCapacity(bucket_capacity),
		mRoot(NULL, min_x, max_x, min_y, max_y, min_z, max_z)
{
}


Octree::Bucket* Octree::BucketAt(float x, float y, float z)
{
	Bucket* cur = &mRoot;
	while( cur && cur->HasKids() ) {
		if( x < cur->MiddleX() ) {				//if left
			if( y < cur->MiddleY() ) {			//if lower left
				if( z < cur->MiddleZ() ) {		//if front lower left
					cur = cur->FrontDownLeft();
				} else {						// else back lower left
					cur = cur->BackDownLeft();
				}
			} else {							// else upper left
				if( z < cur->MiddleZ() ) {		//if front upper left
					cur = cur->FrontUpLeft();
				} else {						// else back upper left
					cur = cur->BackUpLeft();
				}
			}
		} else {								// else right
			if( y < cur->MiddleY() ) {			//if lower right
				if( z < cur->MiddleZ() ) {		//if front lower right
					cur = cur->FrontDownRight();
				} else {						// else back lower right
					cur = cur->BackDownRight();
				}
			} else {							// else upper right
				if( z < cur->MiddleZ() ) {		//if front upper right
					cur = cur->FrontUpRight();
				} else {						// else back upper right
					cur = cur->BackUpRight();
				}
			}
		}
	}
	return cur;
}


bool Octree::InsertItem(I3dCoord* item)
{
	Bucket* b = BucketAt( item->X(), item->Y(), item->Z() );
	if( !b ) {
		return false;
	}

	b->InsertItem(item);
	if( b->Count() > mBucketCapacity ) {
		b->Split();
	}

	return true;
}

void Octree::ItemsWithinRadius(float x, float y, float z, float radius, SmallList<I3dCoord*>& items)
{
	SimpleBinaryTree<Bucket*> checked(false);
	Bucket* b = BucketAt(x, y, z);
	if(b) {
		ItemsWithinRadiusRecursive(x, y, z, x, y, z, radius, b, checked, items);
	}
}

void Octree::Draw()
{
	glLineWidth(1.0f);
	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	DrawBucketRecursive(&mRoot);
}


void Octree::DrawBucketRecursive(Bucket* cur)
{
	if( cur->HasKids() ) {
		DrawBucketRecursive(	cur->FrontDownLeft()	);
		DrawBucketRecursive(	cur->FrontUpLeft()		);
		DrawBucketRecursive(	cur->FrontUpRight()		);
		DrawBucketRecursive(	cur->FrontDownRight()	);
		DrawBucketRecursive(	cur->BackDownLeft()		);
		DrawBucketRecursive(	cur->BackUpLeft()		);
		DrawBucketRecursive(	cur->BackUpRight()		);
		DrawBucketRecursive(	cur->BackDownRight()	);
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		//front face
		glVertex3f( cur->MinX(), cur->MinY(), cur->MinZ() );
		glVertex3f( cur->MinX(), cur->MaxY(), cur->MinZ() );
		glVertex3f( cur->MaxX(), cur->MaxY(), cur->MinZ() );
		glVertex3f( cur->MaxX(), cur->MinY(), cur->MinZ() );

		//back face
		glVertex3f( cur->MinX(), cur->MinY(), cur->MaxZ() );
		glVertex3f( cur->MinX(), cur->MaxY(), cur->MaxZ() );
		glVertex3f( cur->MaxX(), cur->MaxY(), cur->MaxZ() );
		glVertex3f( cur->MaxX(), cur->MinY(), cur->MaxZ() );

		//left face
		glVertex3f( cur->MinX(), cur->MinY(), cur->MaxZ() );
		glVertex3f( cur->MinX(), cur->MaxY(), cur->MaxZ() );
		glVertex3f( cur->MinX(), cur->MaxY(), cur->MinZ() );
		glVertex3f( cur->MinX(), cur->MinY(), cur->MinZ() );

		//right face
		glVertex3f( cur->MaxX(), cur->MinY(), cur->MinZ() );
		glVertex3f( cur->MaxX(), cur->MaxY(), cur->MinZ() );
		glVertex3f( cur->MaxX(), cur->MaxY(), cur->MaxZ() );
		glVertex3f( cur->MaxX(), cur->MinY(), cur->MaxZ() );

		//down face
		glVertex3f( cur->MinX(), cur->MinY(), cur->MinZ() );
		glVertex3f( cur->MinX(), cur->MinY(), cur->MaxZ() );
		glVertex3f( cur->MaxX(), cur->MinY(), cur->MaxZ() );
		glVertex3f( cur->MaxX(), cur->MinY(), cur->MinZ() );

		//up face
		glVertex3f( cur->MinX(), cur->MaxY(), cur->MinZ() );
		glVertex3f( cur->MinX(), cur->MaxY(), cur->MaxZ() );
		glVertex3f( cur->MaxX(), cur->MaxY(), cur->MaxZ() );
		glVertex3f( cur->MaxX(), cur->MaxY(), cur->MinZ() );
	glEnd();

	glColor3f(1.0f, 0.5f, 0.5f);
	SmallList<I3dCoord*>::Iterator it = cur->mItems.BeginIterator();
	glBegin(GL_POINTS);
		for(; it != cur->mItems.EndIterator(); it++) {
			glVertex3f( (*it)->X(), (*it)->Y(), (*it)->Z() );
		}
	glEnd();
}

inline bool Octree::IsWithinRadius(float x1, float y1, float z1, float x2, float y2, float z2, float radius)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float dz = z2 - z1;
	return dx*dx + dy*dy + dz*dz <= radius*radius;
}

void Octree::ItemsWithinRadiusRecursive(float sphere_x, float sphere_y, float sphere_z, float cur_x, float cur_y, float cur_z,
	float radius, Bucket* cur_bucket, SimpleBinaryTree<Bucket*>& checked, SmallList<I3dCoord*>& items)
{
	if( !checked.Insert(cur_bucket) ) {
		return;
	}

	//initial test point. if not first bucket, should be point along an edge
	if( IsWithinRadius(sphere_x, sphere_y, sphere_z, cur_x, cur_y, cur_z, radius) ) {
		SmallList<I3dCoord*>::Iterator it = cur_bucket->mItems.BeginIterator();
		SmallList<I3dCoord*>::Iterator end = cur_bucket->mItems.EndIterator();
		I3dCoord* item;
		for(; it != end; it++) {
			item = *it;
			if( IsWithinRadius( sphere_x, sphere_y, sphere_z, item->X(), item->Y(), item->Z(), radius) ) {
				items.PushToBack( item );
			}
		}

		Bucket* b = NULL;
		float tx = cur_bucket->MinX() - mDeltaAmount;		//test left neighbor
		float ty = cur_y;
		float tz = cur_z;
		b = BucketAt(tx, ty, tz);
		if(b) {
			ItemsWithinRadiusRecursive(sphere_x, sphere_y, sphere_z, tx, ty, tz, radius, b, checked, items);
		}
		
		tx = cur_bucket->MaxX();							//test right neighbor
		b = BucketAt(tx, ty, tz);
		if(b) {
			ItemsWithinRadiusRecursive(sphere_x, sphere_y, sphere_z, tx, ty, tz, radius, b, checked, items);
		}

		tx = cur_x;
		ty = cur_bucket->MinY() - mDeltaAmount;				//test down neighbor
		b = BucketAt(tx, ty, tz);
		if(b) {
			ItemsWithinRadiusRecursive(sphere_x, sphere_y, sphere_z, tx, ty, tz, radius, b, checked, items);
		}

		ty = cur_bucket->MaxY();							//test up neighbor
		b = BucketAt(tx, ty, tz);
		if(b) {
			ItemsWithinRadiusRecursive(sphere_x, sphere_y, sphere_z, tx, ty, tz, radius, b, checked, items);
		}

		ty = cur_y;
		tz = cur_bucket->MinZ() - mDeltaAmount;				//test front neighbor
		b = BucketAt(tx, ty, tz);
		if(b) {
			ItemsWithinRadiusRecursive(sphere_x, sphere_y, sphere_z, tx, ty, tz, radius, b, checked, items);
		}

		tz = cur_bucket->MaxZ();							//test back neighbor
		b = BucketAt(tx, ty, tz);
		if(b) {
			ItemsWithinRadiusRecursive(sphere_x, sphere_y, sphere_z, tx, ty, tz, radius, b, checked, items);
		}
	}
}

Octree::Bucket::Bucket(Bucket* parent, float min_x, float max_x, float min_y, float max_y, float min_z, float max_z)
	: mParent(parent), mMinX(min_x), mMaxX(max_x), mMinY(min_y), mMaxY(max_y), mMinZ(min_z), mMaxZ(max_z),
	mMiddleX(min_x + (max_x - min_x) * 0.5f),
	mMiddleY(min_y + (max_y - min_y) * 0.5f),
	mMiddleZ(min_z + (max_z - min_z) * 0.5f)
{
	memset(reinterpret_cast<void*>(mKids), NULL, sizeof(mKids));
}

void Octree::Bucket::Merge(){
	MergeRecursive(this);
}

void Octree::Bucket::Split()
{
	if (HasKids())
		return;
	float split_x = MiddleX();
	float split_y = MiddleY();
	float split_z = MiddleZ();

	mKids[0] = new Bucket(this, mMinX, split_x, mMinY, split_y, mMinZ, split_z);		//front lower left
	mKids[1] = new Bucket(this, mMinX, split_x, split_y, mMaxY, mMinZ, split_z);		//front upper left
	mKids[2] = new Bucket(this, split_x, mMaxX, split_y, mMaxY, mMinZ, split_z);		//front upper right
	mKids[3] = new Bucket(this, split_x, mMaxX, mMinY, split_y, mMinZ, split_z);		//front lower right
	mKids[4] = new Bucket(this, mMinX, split_x, mMinY, split_y, split_z, mMaxZ);		//back lower left
	mKids[5] = new Bucket(this, mMinX, split_x, split_y, mMaxY, split_z, mMaxZ);		//back upper left
	mKids[6] = new Bucket(this, split_x, mMaxX, split_y, mMaxY, split_z, mMaxZ);		//back upper right
	mKids[7] = new Bucket(this, split_x, mMaxX, mMinY, split_y, split_z, mMaxZ);		//back lower right

	I3dCoord* item;
	while (mItems.Size() != 0) {
		item = mItems.Front();
		if (item->X() < split_x) {					//if left
			if (item->Y() < split_y) {				//if lower left
				if (item->Z() < split_z) {			//if front lower left
					FrontDownLeft()->InsertItem(item);
				}
				else {							//else back lower left
					BackDownLeft()->InsertItem(item);
				}
			}
			else {								//else upper left
				if (item->Z() < split_z) {			//if front upper left
					FrontUpLeft()->InsertItem(item);
				}
				else {							//else back upper left
					BackUpLeft()->InsertItem(item);
				}
			}
		}
		else {									//else right
			if (item->Y() < split_y) {				//if lower right
				if (item->Z() < split_z) {			//if front lower right
					FrontDownRight()->InsertItem(item);
				}
				else {							//else back lower right
					BackDownRight()->InsertItem(item);
				}
			}
			else {								//else upper right
				if (item->Z() < split_z) {			//if front upper right
					FrontUpRight()->InsertItem(item);
				}
				else {							//else back upper right
					BackUpRight()->InsertItem(item);
				}
			}
		}
		mItems.PopFromFront();
	}
}

void Octree::Bucket::MergeRecursive(Bucket* cur)
{
	if (cur->HasKids()) {

		MergeRecursive(FrontDownLeft());
		MergeRecursive(FrontUpLeft());
		MergeRecursive(FrontUpRight());
		MergeRecursive(FrontDownRight());
		MergeRecursive(BackDownLeft());
		MergeRecursive(BackUpLeft());
		MergeRecursive(BackUpRight());
		MergeRecursive(BackDownRight());
		delete FrontDownLeft();
		delete FrontUpLeft();
		delete FrontUpRight();
		delete FrontDownRight();
		delete BackDownLeft();
		delete BackUpLeft();
		delete BackUpRight();
		delete BackDownRight();
		memset(reinterpret_cast<void*>(mKids), NULL, sizeof(mKids));
	}
	else {
		while (cur->mItems.Size() != 0) {
			InsertItem(cur->mItems.Front());
			cur->mItems.PopFromFront();
		}
	}
}

int Octree::Bucket::CountRecursive(Bucket* cur) {
	if (cur->HasKids()) {
		return	CountRecursive(cur->FrontDownLeft()) +
			CountRecursive(cur->FrontUpLeft()) +
			CountRecursive(cur->FrontUpRight()) +
			CountRecursive(cur->FrontDownRight()) +
			CountRecursive(cur->BackDownLeft()) +
			CountRecursive(cur->BackUpLeft()) +
			CountRecursive(cur->BackUpRight()) +
			CountRecursive(cur->BackDownRight());
	}
	else {
		return cur->mItems.Size();
	}
}