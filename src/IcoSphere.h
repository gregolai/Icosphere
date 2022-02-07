#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include "_header.h"

#define FACE_A	0
#define FACE_B	1
#define FACE_C	2
#define FACE_D	3
#define FACE_E	4
#define FACE_F	5
#define FACE_G	6
#define FACE_H	7
#define FACE_I	8
#define FACE_J	9
#define FACE_K	10
#define FACE_L	11
#define FACE_M	12
#define FACE_N	13
#define FACE_O	14
#define FACE_P	15
#define FACE_Q	16
#define FACE_R	17
#define FACE_S	18
#define FACE_T	19

#define GOLDEN 1.61803398874989484820458683436563811772030917980576286213544862270526046281890

#define THIS_SIDE	0
#define THIS_UP		8
#define	THIS_DOWN	16

#define SRC_SIDE	0x01
#define SRC_UP		0x02
#define SRC_DOWN	0x04

#define GET_SRC_DIR(uint, thisdir)	( ((uint) >> (thisdir)) & 0x07 )
#define GET_SRC_FACE(uint, thisdir)	( (((uint) >> (thisdir)) >> 3) & 0x1F )

#define MAKE_UINT(thisdownface, downfacedir, thisupface, upfacedir, thissideface, sidefacedir) \
	(((thisdownface) << 19) | ((downfacedir) << 16) | ((thisupface) << 11) | \
		((upfacedir) << 8) | ((thissideface) << 3) | (sidefacedir))

class IcoSphere
{
	unsigned int GetIndexOfShare(std::map<unsigned int, unsigned int>& ind_map, int local_off,
		unsigned int src_face, unsigned int this_dir, unsigned int src_dir);
public:
	IcoSphere(float radius, int max_lod);
	~IcoSphere();
	void Draw();
	void incLod();
	void decLod();
	inline float Radius() const { return mRadius; }
	inline TerrainVertex** Vertices() const { return mVertices; }
	inline int NumVertices() const { return mNumVertices; }
private:
	void CreateVertices(std::map<unsigned int, unsigned int>& index_map);
	void CreateIndices(std::map<unsigned int, unsigned int>& index_map);

	float mRadius;
	TerrainVertex** mVertices;
	float* vertices;
	int mNumVertices;
	int mMaxLod;
	BigTriangle* mBigTriangles[20];
	GLuint mVertexBuffer;
};

#endif