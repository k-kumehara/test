/*****************************************************************************************//**
 * @file			YmMath.h
 * @brief
 * @attention
 *
 *                 (C) 2018 Yamaha Corporation
 *                  Confidential
 ********************************************************************************************/

#pragma once

#include <math.h>
#include "private/YmTypes.h"

#define YMH_PI				3.14159265358979323846f
#define YMH_DEG2RAD			0.01745329251994329576f		// YMH_PI/180
#define YMH_RAD2DEG			57.2957795130823208767f		// 180/YMH_PI
#define YMH_SONIC			340.29f

namespace YmMath {

/***********************************************************************//**
 * @brief			最大、最小、リミット
 **************************************************************************/
template<typename T> inline T Min(const T& a, const T& b)
{
	return (a<b)? a : b;
}

template<typename T> inline T Max(const T& a, const T& b)
{
	return (a>b) ? a : b;
}

template<typename T> inline T Limit(const T& x, const T& minv, const T& maxv)
{
	return Min<T>(Max<T>(x, minv), maxv);
}

/***********************************************************************//**
 * @brief			dB <-> linear 変換
 **************************************************************************/
inline YmReal32 dBToLin(YmReal32 dB)
{
	return expf(0.115129255f*dB); // = 10^(volume/20)
}

inline YmReal32 LinTodB(YmReal32 lin)
{
	return 20.0f*log10f(lin);
}

/***********************************************************************//**
 * @brief			ベクトル内積
 **************************************************************************/
inline YmReal32 InnerProduct(const YmVector3& v1, const YmVector3& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

/***********************************************************************//**
 * @brief			ベクトル外積
 **************************************************************************/
inline YmVector3 CrossProduct(const YmVector3& v1, const YmVector3& v2)
{
	return v1 ^ v2;
}

/***********************************************************************//**
 * @brief			ベクトル自身の内積
 **************************************************************************/
inline YmReal32 Abs2(YmReal32 x, YmReal32 y, YmReal32 z)
{
	return x*x + y*y + z*z;
}

inline YmReal32 Abs2(const YmVector3& v)
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

/***********************************************************************//**
 * @brief			ベクトル絶対値
 **************************************************************************/
inline YmReal32 Abs(YmReal32 x, YmReal32 y, YmReal32 z)
{
	return sqrtf(x*x + y*y + z*z);
}

inline YmReal32 Abs(const YmVector3& v)
{
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

/***********************************************************************//**
 * @brief			直交座標 -> 極座標 変換
 **************************************************************************/
inline YmPolar3 RectToPolar(YmReal32 x, YmReal32 y, YmReal32 z)
{
	YmPolar3 dst;
	dst.dist = Abs(x,y,z);
	if (dst.dist<1.e-37f)
	{
		dst.azim = 0.0f;
		dst.elev = 0.0f;
	}
    else
    {
        dst.elev = asinf(y / dst.dist);
        if ((z<1.e-37f) && (z>-1.e-37f))
        {
            if ((x < 1.e-37f) && (x > -1.e-37f))
            {
                dst.azim = 0.0f;
            }
            else if (x<0)
            {
                dst.azim = 0.5f*YMH_PI;
            }
            else
            {
                dst.azim = 1.5f*YMH_PI;
            }
        }
        else if (z<0.0f)
        {
            dst.azim = YMH_PI - atanf(x / z);
        }
        else
        {
            dst.azim = -atanf(x / z);
        }
    }
	return dst;
}

inline YmPolar3 RectToPolar(YmVector3& src)
{
	return YmPolar3(RectToPolar(src.x, src.y, src.z));
}

/***********************************************************************//**
 * @brief			極座標 -> 直交座標 変換
 **************************************************************************/
inline YmVector3 PolarToRect(YmReal32 azim, YmReal32 elev, YmReal32 dist)
{
	const YmReal32 x = -dist*cosf(elev)*sinf(azim);
	const YmReal32 z =  dist*cosf(elev)*cosf(azim);
	const YmReal32 y =  dist*sinf(elev);
	return YmVector3(x, y, z);
}

inline YmVector3 PolarToRect(YmPolar3& src)
{
	return PolarToRect(src.azim, src.elev, src.dist);
}

} // namespace YmMath

/*********************************************************************************************
* EOF
*********************************************************************************************/
