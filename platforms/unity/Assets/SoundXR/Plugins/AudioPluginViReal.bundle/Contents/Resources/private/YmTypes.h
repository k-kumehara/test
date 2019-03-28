/*****************************************************************************************//**
 * @file			YmTypes.h
 * @brief
 * @attention
 *
 *                 (C) 2018 Yamaha Corporation
 *                  Confidential
 ********************************************************************************************/

#pragma once

#include <stdint.h>
#include <wchar.h>
#include "private/YmTarget.h"

#if defined(YM_TARGET_WWISE)
	#include <AK/SoundEngine/Common/IAkPlugin.h>
#elif defined(__ORBIS__)
	#include <scebase.h>
#elif defined(NN_NINTENDO_SDK)
	#include <sys/types.h>
#endif

#if defined(_WIN32)||defined(_WIN64)
#else
	#define wcsncpy_s(dst, size, src, cnt) wcsncpy(dst, src, cnt)
	#define _TRUNCATE ((size_t)-1)
#endif

//--- DEPRECATED MACRO
#ifdef _MSC_VER
	#define DEPRECATED(msg)		__declspec(deprecated("DEPRECATED! "##msg))
#elif defined(__GNUC__)
	#define DEPRECATED(msg)		__attribute__((deprecated("DEPRECATED! " msg)))
#else
	#define DEPRECATED(msg)
#endif

//--- DATA TYPE
#if defined(YM_TARGET_WWISE)
	typedef wchar_t				YmWChar;		///< Generic character string
	typedef AkUInt8				YmUInt8;		///< Unsigned 8-bit integer
	typedef AkUInt16			YmUInt16;		///< Unsigned 16-bit integer
	typedef AkUInt32			YmUInt32;		///< Unsigned 32-bit integer
	typedef AkUInt64			YmUInt64;		///< Unsigned 64-bit integer
	typedef AkInt8				YmInt8;			///< Signed 8-bit integer
	typedef AkInt16				YmInt16;		///< Signed 16-bit integer
	typedef AkInt32   			YmInt32;		///< Signed 32-bit integer
	typedef AkInt64   			YmInt64;		///< Signed 64-bit integer
	typedef AkOSChar			YmOSChar;		///< Generic character string
	typedef AkReal32			YmReal32;		///< 32-bit floating point
	typedef AkReal64			YmReal64;		///< 64-bit floating point
#elif defined(__ORBIS__)
	typedef char				YmChar;			///< Unsigned 8-bit integer
	typedef wchar_t				YmWChar;		///< Generic character string
	typedef SceByte8			YmUInt8;		///< Unsigned 8-bit integer
	typedef SceUInt16			YmUInt16;		///< Unsigned 16-bit integer
	typedef SceUInt32			YmUInt32;		///< Unsigned 32-bit integer
	typedef SceUInt64			YmUInt64;		///< Unsigned 64-bit integer
	typedef SceSByte8			YmInt8;			///< Signed 8-bit integer
	typedef SceInt16			YmInt16;		///< Signed 16-bit integer
	typedef SceInt32			YmInt32;		///< Signed 32-bit integer
	typedef SceInt64			YmInt64;		///< Signed 64-bit integer
	typedef SceFloat32			YmReal32;		///< 32-bit floating point
	typedef SceDouble64			YmReal64;		///< 64-bit floating point
#elif defined(NN_NINTENDO_SDK)
	typedef wchar_t				YmWChar;		///< Generic character string
	typedef uint8_t				YmUInt8;		///< Unsigned 8-bit integer
	typedef uint16_t			YmUInt16;		///< Unsigned 16-bit integer
	typedef uint32_t			YmUInt32;		///< Unsigned 32-bit integer
	typedef uint64_t			YmUInt64;		///< Unsigned 64-bit integer
	typedef int8_t				YmInt8;			///< Signed 8-bit integer
	typedef int16_t				YmInt16;		///< Signed 16-bit integer
	typedef int32_t   			YmInt32;		///< Signed 32-bit integer
	typedef int64_t   			YmInt64;		///< Signed 64-bit integer
	typedef char				YmOSChar;		///< Generic character string
	typedef float				YmReal32;		///< 32-bit floating point
	typedef double				YmReal64;		///< 64-bit floating point
#elif defined(_WIN32)||defined(_WIN64)
	typedef wchar_t				YmWChar;		///< Generic character string
	typedef unsigned char		YmUInt8;		///< Unsigned 8-bit integer
	typedef unsigned short		YmUInt16;		///< Unsigned 16-bit integer
	typedef unsigned long		YmUInt32;		///< Unsigned 32-bit integer
	typedef unsigned __int64	YmUInt64;		///< Unsigned 64-bit integer
	typedef char				YmInt8;			///< Signed 8-bit integer
	typedef short				YmInt16;		///< Signed 16-bit integer
	typedef long   				YmInt32;		///< Signed 32-bit integer
	typedef __int64				YmInt64;		///< Signed 64-bit integer
	typedef wchar_t				YmOSChar;		///< Generic character string
	typedef float				YmReal32;		///< 32-bit floating point
	typedef double				YmReal64;		///< 64-bit floating point
#elif defined(_MAC)||defined(__APPLE__)
	typedef wchar_t				YmWChar;		///< Generic character string
	typedef unsigned char		YmUInt8;		///< Unsigned 8-bit integer
	typedef unsigned short		YmUInt16;		///< Unsigned 16-bit integer
	typedef uint32_t			YmUInt32;		///< Unsigned 32-bit integer
	typedef uint64_t			YmUInt64;		///< Unsigned 64-bit integer
	typedef char				YmInt8;			///< Signed 8-bit integer
	typedef short				YmInt16;		///< Signed 16-bit integer
	typedef int32_t				YmInt32;		///< Signed 32-bit integer
	typedef int64_t				YmInt64;		///< Signed 64-bit integer
	typedef wchar_t				YmOSChar;		///< Generic character string
	typedef float				YmReal32;		///< 32-bit floating point
	typedef double				YmReal64;		///< 64-bit floating point
#else
	typedef wchar_t				YmWChar;		///< Generic character string
	typedef uint8_t				YmUInt8;		///< Unsigned 8-bit integer
	typedef uint16_t			YmUInt16;		///< Unsigned 16-bit integer
	typedef uint32_t			YmUInt32;		///< Unsigned 32-bit integer
	typedef uint64_t			YmUInt64;		///< Unsigned 64-bit integer
	typedef int8_t				YmInt8;			///< Signed 8-bit integer
	typedef int16_t				YmInt16;		///< Signed 16-bit integer
	typedef int32_t				YmInt32;		///< Signed 32-bit integer
	typedef int64_t				YmInt64;		///< Signed 64-bit integer
	typedef wchar_t				YmOSChar;		///< Generic character string
	typedef float				YmReal32;		///< 32-bit floating point
	typedef double				YmReal64;		///< 64-bit floating point
#endif

//--- Yamaha struct
struct YmVector3 {
public:
	YmReal32 x;		// [m]  right(+) <-> left(-)
	YmReal32 y;		// [m]  up(+) <-> down(-)
	YmReal32 z;		// [m]  front(+) <-> rear(-)

public:
	explicit YmVector3(YmReal32 a = 0.0f, YmReal32 b = 0.0f, YmReal32 c = 0.0f) : x(a), y(b), z(c) {};
	inline void Set(YmReal32 a, YmReal32 b, YmReal32 c)		{ x=a; y=b; z=c; }
	inline void Set(const YmVector3& v)						{ *this = v; }

	// direction
	inline static YmVector3 GetDirectionRight(void)			{ return YmVector3(1.0f, 0.0f, 0.0f); }
	inline static YmVector3 GetDirectionUp(void)			{ return YmVector3(0.0f, 1.0f, 0.0f); }
	inline static YmVector3 GetDirectionFront(void)			{ return YmVector3(0.0f, 0.0f, 1.0f); }

	// operators override
	inline YmVector3 operator+ (const YmVector3& v) const	{ return YmVector3(x+v.x, y+v.y, z+v.z); }
	inline YmVector3 operator- (const YmVector3& v) const	{ return YmVector3(x-v.x, y-v.y, z-v.z); }
	inline YmVector3 operator* (const YmVector3& v) const	{ return YmVector3(x*v.x, y*v.y, z*v.z); }
	inline YmVector3 operator/ (const YmVector3& v) const	{ return YmVector3(x/v.x, y/v.y, z/v.z); }
	inline YmVector3 operator^ (const YmVector3& v) const	{ return YmVector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline YmVector3 operator+= (const YmVector3& v)		{ x+=v.x; y+=v.y; z+=v.z; return *this; }
	inline YmVector3 operator-= (const YmVector3& v)		{ x-=v.x; y-=v.y; z-=v.z; return *this; }
	inline YmVector3 operator*= (const YmVector3& v)		{ x*=v.x; y*=v.y; z*=v.z; return *this; }
	inline YmVector3 operator/= (const YmVector3& v)		{ x/=v.x; y/=v.y; z/=v.z; return *this; }
	inline YmVector3 operator^= (const YmVector3& v)		{ x=y*v.z-z*v.y; y=z*v.x-x*v.z; z=x*v.y-y*v.x; return *this; }
	inline bool operator== (const YmVector3& v) const		{ return ((x==v.x) && (y==v.y) && (z==v.z)); }
	inline bool operator!= (const YmVector3& v) const		{ return ((x!=v.x) || (y!=v.y) || (z!=v.z)); }
	inline YmVector3 operator+ (YmReal32 a) const			{ return YmVector3(x+a, y+a, z+a); }
	inline YmVector3 operator- (YmReal32 a) const			{ return YmVector3(x-a, y-a, z-a); }
	inline YmVector3 operator* (YmReal32 a) const			{ return YmVector3(x*a, y*a, z*a); }
	inline YmVector3 operator/ (YmReal32 a) const			{ return YmVector3(x/a, y/a, z/a); }
	inline YmVector3 operator+= (YmReal32 a)				{ x+=a; y+=a; z+=a; return *this; }
	inline YmVector3 operator-= (YmReal32 a)				{ x-=a; y-=a; z-=a; return *this; }
	inline YmVector3 operator*= (YmReal32 a)				{ x*=a; y*=a; z*=a; return *this; }
	inline YmVector3 operator/= (YmReal32 a)				{ x/=a; y/=a; z/=a; return *this; }
	inline YmVector3 operator+ () const						{ return YmVector3(x, y, z); }
	inline YmVector3 operator- () const						{ return YmVector3(-x, -y, -z); }
	inline YmReal32& operator[] (int idx)					{ return (&x)[idx]; }
	inline YmReal32 operator[] (int idx) const				{ return (&x)[idx]; }
};

struct YmPolar3 {
public:
	YmReal32 azim;	// [rad]  -PI(rear) <-> -PI/2(right) <-> 0(front) <-> PI/2(left) <-> PI(rear)
	YmReal32 elev;	// [rad]  -PI/2(down) <-> PI/2(up)
	YmReal32 dist;	// [m]

public:
	explicit YmPolar3(YmReal32 a = 0.0f, YmReal32 b = 0.0f, YmReal32 c = 0.0f) : azim(a), elev(b), dist(c) {};
	inline void Set(YmReal32 a, YmReal32 b, YmReal32 c)		{ azim=a; elev=b; dist=c; }
	inline void Set(const YmPolar3& v)						{ *this = v; }

	// operator override
	inline YmPolar3 operator+ (const YmPolar3& v) const		{ return YmPolar3(azim+v.azim, elev+v.elev, dist+v.dist); }
	inline YmPolar3 operator- (const YmPolar3& v) const		{ return YmPolar3(azim-v.azim, elev-v.elev, dist-v.dist); }
	inline YmPolar3 operator* (const YmPolar3& v) const		{ return YmPolar3(azim*v.azim, elev*v.elev, dist*v.dist); }
	inline YmPolar3 operator/ (const YmPolar3& v) const		{ return YmPolar3(azim/v.azim, elev/v.elev, dist/v.dist); }
	inline YmPolar3 operator+= (const YmPolar3& v)			{ azim+=v.azim; elev+=v.elev; dist+=v.dist; return *this; }
	inline YmPolar3 operator-= (const YmPolar3& v)			{ azim-=v.azim; elev-=v.elev; dist-=v.dist; return *this; }
	inline YmPolar3 operator*= (const YmPolar3& v)			{ azim*=v.azim; elev*=v.elev; dist*=v.dist; return *this; }
	inline YmPolar3 operator/= (const YmPolar3& v)			{ azim/=v.azim; elev/=v.elev; dist/=v.dist; return *this; }
	inline bool operator== (const YmPolar3& v) const		{ return ((azim==v.azim) && (elev==v.elev) && (dist==v.dist)); }
	inline bool operator!= (const YmPolar3& v) const		{ return ((azim!=v.azim) || (elev!=v.elev) || (dist!=v.dist)); }
	inline YmPolar3 operator+ (YmReal32 a) const			{ return YmPolar3(azim+a, elev+a, dist+a); }
	inline YmPolar3 operator- (YmReal32 a) const			{ return YmPolar3(azim-a, elev-a, dist-a); }
	inline YmPolar3 operator* (YmReal32 a) const			{ return YmPolar3(azim*a, elev*a, dist*a); }
	inline YmPolar3 operator/ (YmReal32 a) const			{ return YmPolar3(azim/a, elev/a, dist/a); }
	inline YmPolar3 operator+= (YmReal32 a)					{ azim+=a; elev+=a; dist+=a; return *this; }
	inline YmPolar3 operator-= (YmReal32 a)					{ azim-=a; elev-=a; dist-=a; return *this; }
	inline YmPolar3 operator*= (YmReal32 a)					{ azim*=a; elev*=a; dist*=a; return *this; }
	inline YmPolar3 operator/= (YmReal32 a)					{ azim/=a; elev/=a; dist/=a; return *this; }
	inline YmPolar3 operator+ () const						{ return YmPolar3(azim, elev, dist); }
	inline YmPolar3 operator- () const						{ return YmPolar3(-azim, -elev, -dist); }
	inline YmReal32& operator[] (int idx)					{ return (&azim)[idx]; }
	inline YmReal32 operator[] (int idx) const				{ return (&azim)[idx]; }
};

//--- Deprecated type
DEPRECATED("Use 'YmVector3'") typedef YmVector3	YMH_Vector3;

/*********************************************************************************************
* EOF
*********************************************************************************************/
