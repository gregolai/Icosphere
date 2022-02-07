#ifndef I3DCOORD_H
#define I3DCOORD_H

class I3dCoord
{
public:
	virtual float X() const = 0;
	virtual float Y() const = 0;
	virtual float Z() const = 0;
};

#endif