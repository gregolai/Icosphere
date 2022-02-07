#ifndef HEADER_H
#define HEADER_H

#pragma comment(lib, "gl/glut32.lib")

//#define _CRT_SECURE_NO_WARNINGS

// WINDOWS
#include <windows.h>

// Standard C headers
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

//OPENGL stuff
#include "gl/opengl.h"
#include "gl/glut.h"



//FORWARD DECLARATIONS
class BigTriangle;
class I3dCoord;
class IcoSphere;
class Octree;
class TerrainVertex;
class TriStrip;

// TEMPLATE DECLARATIONS
template<class T> class SimpleBinaryTree;
template<class T> class SmallList;

// MATH DECLARATIONS
class Math;
class Matrix3;
class Matrix4;
class Quaternion;
class Vector2;
class Vector3;

#endif