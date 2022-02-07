#ifndef TERRAIN_VERTEX_H
#define TERRAIN_VERTEX_H

#include "_header.h"
#include "I3dCoord.h"

class TerrainVertex : public I3dCoord
{
	friend class IcoSphere;
public:
	TerrainVertex(float x, float y, float z)
		: mX(x), mY(y), mZ(z)
	{
	}
	inline float X() const { return mX; }
	inline float Y() const { return mY; }
	inline float Z() const { return mZ; }
private:
	float mX, mY, mZ;
};

#endif