#ifndef SMALL_LIST_H
#define SMALL_LIST_H

#include "_header.h"

template<class T>
class SmallList
{
public:
	class Iterator
	{
	public:
		Iterator(typename SmallList<T>::SmallListItem* item) : mThis(item) { };
		inline T operator*() { return mThis->mItem; }
		inline void operator++(int) { mThis = mThis->mNext; }
		inline bool operator !=(const Iterator& rhs) { return (rhs.mThis != mThis); }
	private:
		typename SmallList<T>::SmallListItem* mThis;
	};
	SmallList();
	~SmallList();
	bool ItemExists(const T& item) const;
	void PushToBack(const T& item);
	void PushToFront(const T& item);
	bool PopFromBack();
	bool PopFromFront();
	bool RemoveFirstOfItem(const T& item);
	void PrintList(std::ostream& out = cout) const;
	inline int Size() const { return mSize; }
	inline const T Front() const { return mFirst->mItem; }
	inline const T Back() const { return mLast->mItem; }
	inline Iterator BeginIterator() { return Iterator( mFirst ); }
	inline Iterator EndIterator() { return Iterator( NULL ); }
private:
	class SmallListItem
	{
	public:
		SmallListItem(const T& item) : mItem(item), mNext(NULL) { }
	private:
		const T mItem;
		SmallListItem* mNext;
		friend class SmallList<T>;
		friend class SmallList<T>::Iterator;
	};
	SmallListItem* mFirst;
	SmallListItem* mLast;
	int mSize;
};

template<class T>
SmallList<T>::SmallList()
	: mFirst(NULL), mLast(NULL), mSize(0)
{
}

template<class T>
SmallList<T>::~SmallList()
{
	SmallListItem* cur = mFirst;
	SmallListItem* next;
	while(cur != NULL) {
		next = cur->mNext;
		delete cur;
		cur = next;
	}
}

template<class T>
bool SmallList<T>::ItemExists(const T& item) const
{
	SmallListItem* cur = mFirst;
	while(cur != NULL) {
		if(cur->mItem == item) {
			return true;
		} else {
			cur = cur->mNext;
		}
	}
	return false;
}

template<class T>
void SmallList<T>::PushToBack(const T& item)
{
	if(mLast == NULL) { // && mFirst == NULL
		mFirst = mLast = new SmallListItem(item);
	} else {
		mLast->mNext = new SmallListItem(item);
		mLast = mLast->mNext;
	}
	++mSize;
}

template<class T>
void SmallList<T>::PushToFront(const T& item)
{
	if(mFirst == NULL) { // && mLast == NULL
		mFirst = mLast = new SmallListItem(item);
	} else {
		SmallListItem* firstPtr = mFirst;
		mFirst = new SmallListItem(item);
		mFirst->mNext = firstPtr;
	}
	++mSize;
}

template<class T>
bool SmallList<T>::PopFromBack()
{
	if(mLast != NULL) {
		if(mLast != mFirst) {
			SmallListItem* lastPtr = mFirst;
			while(lastPtr->mNext != mLast)
				lastPtr = lastPtr->mNext;
			delete mLast;
			mLast = lastPtr;
			lastPtr->mNext = NULL;
		} else {
			delete mLast;
			mLast = mFirst = NULL;
		}
		--mSize;
		return true;
	} else {
		return false;
	}
}

template<class T>
bool SmallList<T>::PopFromFront()
{
	if(mFirst != NULL) {
		if(mFirst != mLast) {
			SmallListItem* firstPtr = mFirst;
			mFirst = mFirst->mNext;
			delete firstPtr;
		} else {
			delete mFirst;
			mFirst = mLast = NULL;
		}
		--mSize;
		return true;
	} else {
		return false;
	}
}

template<class T>
bool SmallList<T>::RemoveFirstOfItem(const T& item)
{
	SmallListItem* cur = mFirst;
	SmallListItem* prev = NULL;
	while(cur != NULL) {
		if(cur->mItem == item) {
			if(prev != NULL) {
				prev->mNext = cur->mNext;
			} else {
				mFirst = cur->mNext;
			}
			if(cur == mLast) {
				mLast = prev;
			}
			delete cur;
			--mSize;
			return true;
		} else {
			prev = cur;
			cur = cur->mNext;
		}
	}
	return false;
}

template<class T>
void SmallList<T>::PrintList(std::ostream& out) const
{
	SmallListItem* cur = mFirst;
	while(cur != NULL) {
		out << cur->mItem << (cur != mLast ? ", " : "\n");
		cur = cur->mNext;
	}
}

#endif