#include "_include.h"

IcoSphere::IcoSphere(float radius, int max_lod)
	: mRadius(radius), mMaxLod(max_lod)
{
	std::map<unsigned int, unsigned int> index_map;
	CreateVertices(index_map);
	CreateIndices(index_map);
}

IcoSphere::~IcoSphere(){
	for (int face = 0; face<20; ++face) {
		delete mBigTriangles[face];
	}

	for (int v = 0; v<mNumVertices; ++v) {
		delete mVertices[v];
	}
	delete[] mVertices;
}

unsigned int IcoSphere::GetIndexOfShare(std::map<unsigned int, unsigned int>& index_map, int local_off,
										unsigned int src_face, unsigned int this_dir, unsigned int src_dir)
{
	int pow2maxlod = (int)pow(2.0, mMaxLod);
	unsigned int s = mBigTriangles[src_face]->IndexStart();

	if(src_dir == SRC_DOWN && this_dir == THIS_UP)  // like B -> A
		return index_map[ s + pow2maxlod-local_off ];
	
	if(src_dir == SRC_DOWN && this_dir == THIS_SIDE) //like T -> P
			return index_map[ s + local_off ];

	int i = 0;
	for(int col=0; col<=pow2maxlod; ++col) {
		for(int row=0; row<=(pow2maxlod-col); ++row) {

			// like Q -> A
			if(this_dir == THIS_SIDE && src_dir == SRC_UP && col == pow2maxlod-local_off && row == 0) 
				return index_map[ s + i ];

			//like T -> D
			if(this_dir == THIS_DOWN && src_dir == SRC_SIDE && row==local_off && row==(pow2maxlod-col))
				return index_map[ s + i ];

			// like N -> K
			if(this_dir == THIS_SIDE && src_dir == SRC_SIDE && row == local_off && row==(pow2maxlod-col)) 
				return index_map[ s + i ];

			// like Q -> M
			if(this_dir == THIS_UP && src_dir == SRC_SIDE && col == local_off && row==(pow2maxlod-col)) 
				return index_map[ s + i ];

			++i;
		}
	}

	return 0;
}

void IcoSphere::CreateVertices(std::map<unsigned int, unsigned int>& index_map)
{

	unsigned int Shares[20] = {
		//				UNDEFINED		THIS_DOWN			THIS_UP			THIS_SIDE
		MAKE_UINT(0, 0, 0, 0, 0, 0),									// A = 0 []
		MAKE_UINT(0, 0, FACE_A, SRC_DOWN, 0, 0),						// B = 1 [UP, A DOWN]
		MAKE_UINT(0, 0, FACE_B, SRC_DOWN, 0, 0),						// C = 2 [UP, B DOWN]
		MAKE_UINT(0, 0, FACE_C, SRC_DOWN, 0, 0),						// D = 3 [UP, C DOWN]
		MAKE_UINT(0, 0, FACE_A, SRC_SIDE, 0, 0),						// E = 4 [UP, A SIDE]
		MAKE_UINT(0, 0, FACE_E, SRC_DOWN, FACE_C, SRC_SIDE),			// F = 5 [UP, E DOWN] [SIDE, C SIDE]
		MAKE_UINT(0, 0, FACE_F, SRC_DOWN, 0, 0),						// G = 6 [UP, F DOWN]
		MAKE_UINT(0, 0, FACE_G, SRC_DOWN, FACE_D, SRC_DOWN),			// H = 7 [UP, G DOWN] [SIDE, D DOWN]
		MAKE_UINT(0, 0, FACE_E, SRC_SIDE, 0, 0),						// I = 8 [UP, E SIDE]
		MAKE_UINT(0, 0, FACE_I, SRC_DOWN, FACE_G, SRC_SIDE),			// J = 9 [UP, I DOWN] [SIDE, G SIDE]
		MAKE_UINT(0, 0, FACE_J, SRC_DOWN, 0, 0),						// K = 10 [UP, J DOWN]
		MAKE_UINT(0, 0, FACE_K, SRC_DOWN, FACE_H, SRC_DOWN),			// L = 11 [UP, K DOWN] [SIDE, H DOWN]
		MAKE_UINT(0, 0, FACE_I, SRC_SIDE, 0, 0),						// M = 12 [UP, I SIDE]
		MAKE_UINT(0, 0, FACE_M, SRC_DOWN, FACE_K, SRC_SIDE),			// N = 13 [UP, M DOWN] [SIDE, K SIDE]
		MAKE_UINT(0, 0, FACE_N, SRC_DOWN, 0, 0),						// O = 14 [UP, N DOWN]
		MAKE_UINT(0, 0, FACE_O, SRC_DOWN, FACE_L, SRC_DOWN),			// P = 15 [UP, O DOWN] [SIDE, L DOWN]
		MAKE_UINT(0, 0, FACE_M, SRC_SIDE, FACE_A, SRC_UP),				// Q = 16 [UP, M SIDE] [SIDE, A UP]
		MAKE_UINT(0, 0, FACE_Q, SRC_DOWN, FACE_O, SRC_SIDE),			// R = 17 [UP, Q DOWN] [SIDE, O SIDE]
		MAKE_UINT(0, 0, FACE_R, SRC_DOWN, FACE_B, SRC_SIDE),			// S = 18 [UP, R DOWN] [SIDE, B SIDE]
		MAKE_UINT(FACE_D, SRC_SIDE, FACE_S, SRC_DOWN, FACE_P, SRC_DOWN)	// T = 19 [DOWN, D SIDE] [UP, S DOWN] [SIDE, P DOWN]
	};

	double BaseVertices[20][3][3] = {
		//		BASE (SIDE)					//UP						//DOWN
		-GOLDEN, 0, -1,				-1, GOLDEN, 0,					-GOLDEN, 0, 1,		//FACE A
		-GOLDEN, 0, 1,				-GOLDEN, 0, -1,					-1, -GOLDEN, 0,		//FACE B
		-1, -GOLDEN, 0,				-GOLDEN, 0, 1,					0, -1, GOLDEN,		//FACE C
		0, -1, GOLDEN,				-1, -GOLDEN, 0,					1, -GOLDEN, 0,		//FACE D
		-GOLDEN, 0, 1,				-1, GOLDEN, 0,					0, 1, GOLDEN,		//FACE E
		0, 1, GOLDEN,				-GOLDEN, 0, 1,					0, -1, GOLDEN,		//FACE F
		0, -1, GOLDEN,				0, 1, GOLDEN,					GOLDEN, 0, 1,		//FACE G
		GOLDEN, 0, 1,				0, -1, GOLDEN,					1, -GOLDEN, 0,		//FACE H
		0, 1, GOLDEN,				-1, GOLDEN, 0,					1, GOLDEN, 0,		//FACE I
		1, GOLDEN, 0,				0, 1, GOLDEN,					GOLDEN, 0, 1,		//FACE J
		GOLDEN, 0, 1,				1, GOLDEN, 0,					GOLDEN, 0, -1,		//FACE K
		GOLDEN, 0, -1,				GOLDEN, 0, 1,					1, -GOLDEN, 0,		//FACE L
		1, GOLDEN, 0,				-1, GOLDEN, 0,					0, 1, -GOLDEN,		//FACE M
		0, 1, -GOLDEN,				1, GOLDEN, 0,					GOLDEN, 0, -1,		//FACE N
		GOLDEN, 0, -1,				0, 1, -GOLDEN,					0, -1, -GOLDEN,		//FACE O
		0, -1, -GOLDEN,				GOLDEN, 0, -1,					1, -GOLDEN, 0,		//FACE P
		0, 1, -GOLDEN,				-1, GOLDEN, 0,					-GOLDEN, 0, -1,		//FACE Q
		-GOLDEN, 0, -1,				0, 1, -GOLDEN,					0, -1, -GOLDEN,		//FACE R
		0, -1, -GOLDEN,				-GOLDEN, 0, -1,					-1, -GOLDEN, 0,		//FACE S
		-1, -GOLDEN, 0,				0, -1, -GOLDEN,					1, -GOLDEN, 0		//FACE T
	};

	//scale base vertices by radius over golden ratio
	{
		for(int face=0; face<20; ++face) {					//for each big triangle face
			for(int vert=0; vert<3; ++vert) {				//for each vertex
				for(int axis=0; axis<3; ++axis) {					//for each axis
					BaseVertices[face][vert][axis] *= (mRadius / GOLDEN);
				}
			}
		}
	}

	std::deque<float> verts;
	
	int pow2maxlod = (int)pow(2.0, mMaxLod);			//a common variable used throughout
	
	int i = 0, v = 0;
	for(int face=0; face<20; ++face) {
		mBigTriangles[face] = new BigTriangle(mMaxLod, i);
		double* vpt = &(BaseVertices[face][0][0]);		//base vertex of this big triangle

		//define row and column increments
		Vector3 col_inc = ( Vector3(vpt[3]-vpt[0], vpt[4]-vpt[1], vpt[5]-vpt[2] ) ) / pow2maxlod;
		Vector3 row_inc = ( Vector3(vpt[6]-vpt[0], vpt[7]-vpt[1], vpt[8]-vpt[2] ) ) / pow2maxlod;
		
		
		for(int col=0; col<=pow2maxlod; ++col) {				//for each column

			for(int row=0; row<=(pow2maxlod-col); ++row) {		//for each row

				if( col == 0 && GET_SRC_DIR(Shares[face],THIS_DOWN) ) {
					//the only one that does this is FACE T
					unsigned int m;
					m = GetIndexOfShare(index_map, row, GET_SRC_FACE(Shares[face], THIS_DOWN), THIS_DOWN, GET_SRC_DIR(Shares[face], THIS_DOWN));
					index_map[i] = m;
				} else if( row==0 && GET_SRC_DIR(Shares[face],THIS_UP) ) {
					int m;
					m = GetIndexOfShare(index_map, col, GET_SRC_FACE(Shares[face], THIS_UP), THIS_UP, GET_SRC_DIR(Shares[face], THIS_UP));
					index_map[i] = m;
				} else if( row==(pow2maxlod-col) && GET_SRC_DIR(Shares[face],THIS_SIDE) ) {
					int m;
					m = GetIndexOfShare(index_map, row, GET_SRC_FACE(Shares[face], THIS_SIDE), THIS_SIDE, GET_SRC_DIR(Shares[face], THIS_SIDE));
					index_map[i] = m;
				} else {
					Vector3 base(vpt[0], vpt[1], vpt[2]);
					Vector3 newpt = base + ( (col_inc * col) + (row_inc * row) );
					float rho, phi, theta;
					Math::cartesianToSpherical(newpt.x, newpt.y, newpt.z, rho, phi, theta);
					Math::sphericalToCartesian(mRadius, phi, theta, newpt.x, newpt.y, newpt.z);
					verts.push_back( newpt.x );
					verts.push_back( newpt.y );
					verts.push_back( newpt.z );
					index_map[i] = v;
					++v;						//increment vertex number
				}
				++i;
			}
		}
	}

	mNumVertices = verts.size()/3;
	mVertices = new TerrainVertex*[ mNumVertices ];
	vertices = new float[mNumVertices*3];
	std::deque<float>::iterator it = verts.begin();
	for(int k=0; k<mNumVertices; ++k) {
		float x, y, z;
		x = *it;
		it++;
		y = *it; 
		it++;
		z = *it;
		it++;
		mVertices[k] = new TerrainVertex(x, y, z);
		vertices[k*3] = x;
		vertices[k*3+1] = y;
		vertices[k*3+2] = z;
	}
}

void IcoSphere::CreateIndices(std::map<unsigned int, unsigned int>& index_map)
{
	for(int face=0; face<20; ++face) {
		mBigTriangles[face]->CreateStrips(index_map);
	}
}

void IcoSphere::Draw()
{
	glPointSize(2.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1, 1, 1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	//glDrawArrays(GL_POINTS, 0, nVertices);

	TriStrip** strips;
	for(int face=0; face<20; ++face) {
		strips = mBigTriangles[face]->Strips();
		std::cout << "lod: " << mBigTriangles[face]->Lod() << std::endl;
		for(int s=0; s < mBigTriangles[face]->NumStrips(); ++s) {
			glDrawElements(GL_TRIANGLE_STRIP, strips[s]->IndexCount(), GL_UNSIGNED_INT, strips[s]->Indices() );
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);
}

void IcoSphere::incLod()
{
	for (int face = 0; face<20; ++face) {
		mBigTriangles[face]->incLod();
	}
}

void IcoSphere::decLod()
{
	for (int face = 0; face<20; ++face) {
		mBigTriangles[face]->decLod();
	}
}