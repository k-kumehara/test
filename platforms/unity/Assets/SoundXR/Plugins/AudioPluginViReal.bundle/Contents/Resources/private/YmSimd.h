/*****************************************************************************************//**
 * @file			YmSimd.h
 * @brief
 * @attention
 *
 *                 (C) 2018 Yamaha Corporation
 *                  Confidential
 ********************************************************************************************/

#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <wchar.h>
#include <math.h>
#include "private/YmTypes.h"

#if defined(YM_TARGET_WWISE)
	#include <AK/SoundEngine/Common/AkSimd.h>
#endif
#if defined(_M_ARM)||defined(_M_ARM64)||defined(__ARM_NEON)
#include <arm_neon.h>
#elif defined(_WIN32)||defined(_WIN64)||defined(__linux__)||defined(_MAC)
	#include <pmmintrin.h>
#endif
#define NUM_SIMD				4				///< SIMD命令で並列実行される数

//--- SIMD DATA TYPE
#if defined(YM_TARGET_WWISE)
	typedef AKSIMD_V4F32		YmV4F32;		///< Vector of 4 32-bit floats
	typedef AKSIMD_V4I32		YmV4I32;		///< Vector of 4 32-bit signed integers
#elif defined(__ORBIS__)
	typedef __m128				YmV4F32;		///< Vector of 4 32-bit floats
	typedef __m128i				YmV4I32;		///< Vector of 4 32-bit signed integers
#elif defined(NN_NINTENDO_SDK)
	typedef float32x4_t			YmV4F32;		///< Vector of 4 32-bit floats
	typedef int32x4_t			YmV4I32;		///< Vector of 4 32-bit signed integers
#elif defined(_M_ARM)||defined(_M_ARM64)||defined(__ARM_NEON)
	typedef float32x4_t			YmV4F32;		///< Vector of 4 32-bit floats
	typedef int32x4_t			YmV4I32;		///< Vector of 4 32-bit signed integers
#elif defined(_WIN32)||defined(_WIN64)||defined(__linux__)||defined(_MAC)
	typedef __m128				YmV4F32;		///< Vector of 4 32-bit floats
	typedef __m128i				YmV4I32;		///< Vector of 4 32-bit signed integers
#endif

//--- INTRINSIC
#if defined(YM_TARGET_WWISE)
	#define YMSIMD_ADD_V4F32( a, b )					AKSIMD_ADD_V4F32( a, b )
	#define YMSIMD_MUL_SS_V4F32( a, b )					AKSIMD_MUL_SS_V4F32( a, b )
	#define YMSIMD_STORE_V4F32( __addr__, __vec__ )		AKSIMD_STORE_V4F32( __addr__, __vec__ )
	#define YMSIMD_STOREU_V4F32( __addr__, __vec__ )	AKSIMD_STOREU_V4F32( __addr__, __vec__ )
	#define YMSIMD_SET_V4F32( __scalar__ )				AKSIMD_SET_V4F32( __scalar__ )
	#define YMSIMD_MADD_V4F32( __a__, __b__, __c__ )	AKSIMD_MADD_V4F32( __a__, __b__, __c__ )
	#define YMSIMD_MUL_V4F32( a, b )					AKSIMD_MUL_V4F32( a, b )
	#define YMSIMD_LOAD_V4F32( __addr__ )				AKSIMD_LOAD_V4F32( __addr__ )
	#define YMSIMD_LOADU_V4F32( __addr__ )				AKSIMD_LOADU_V4F32( __addr__ )
	#define YMSIMD_LOAD1_V4F32( __scalar__ )			AKSIMD_LOAD1_V4F32( __scalar__ )
	#define YMSIMD_COMPLEXMUL( a, b )					AKSIMD_COMPLEXMUL( a, b )
	#define YMSIMD_SET1_EPI32_V4I32( a )				AKSIMD_SET_V4I32( a )
	#if !defined(NN_NINTENDO_SDK)
		#define YMSIMD_SET_EPI32_V4I32( a, b, c, d )	AKSIMD_SETV_V4I32( a, b, c, d )
	#endif
#elif defined(_M_ARM)||defined(_M_ARM64)||(__ARM_NEON)
	#define YMSIMD_ADD_V4F32( a, b )					(vaddq_f32( ( a ), ( b ) ))
	#define YMSIMD_MUL_SS_V4F32( a, b )					(vmulq_f32( a, b ))
	#define YMSIMD_STORE_V4F32( __addr__, __vec__ )		(vst1q_f32( (YmReal32*)(__addr__), (__vec__) ))
	#define YMSIMD_STOREU_V4F32( __addr__, __vec__ )	(vst1q_f32( (YmReal32*)(__addr__), (__vec__) ))
	#define YMSIMD_SET_V4F32( __scalar__ )				(vdupq_n_f32( (__scalar__) ))
	#define YMSIMD_MADD_V4F32( __a__, __b__, __c__ )	(vmlaq_f32( (__c__), (__a__), (__b__)  ))
	#define YMSIMD_MUL_V4F32( __a__, __b__ )			(vmulq_f32( ( __a__ ), ( __b__ ) ))
	#define YMSIMD_LOAD_V4F32( __addr__ )				(vld1q_f32(__addr__))
	#define YMSIMD_LOADU_V4F32( __addr__ )				(vld1q_f32( (YmReal32*)(__addr__) ))
	#define YMSIMD_LOAD1_V4F32( __scalar__ )			(vld1q_dup_f32( &(__scalar__) ))
	#define YMSIMD_SET1_EPI32_V4I32( a )				(vdupq_n_s32( a ))
	#define YMSIMD_SET_EPI32_V4I32(a, b, c, d)			__extension__ ({ \
		int32_t __attribute__((aligned(16))) data[4] = { (d), (c), (b), (a) }; \
		int32x4_t __ret; \
		__ret = (int32x4_t) (vld1q_s32(data)); \
		__ret; \
	})
	static inline YmV4F32 YMSIMD_COMPLEXMUL(const YmV4F32 vCIn1, const YmV4F32 vCIn2)
	{
		constexpr YmV4F32 vSign = { 1.f, -1.f, 1.f, -1.f };
		// SHUFFLE(2, 2, 0, 0)_V4F32
		float32x2_t xx = vdup_lane_f32(vget_low_f32(vCIn1), 0);
		float32x2_t cc = vdup_lane_f32(vget_high_f32(vCIn1), 0);
		YmV4F32 v1 = vcombine_f32(xx, cc);

		// SHUFFLE(3, 3, 1, 1)_V4F32
		float32x2_t yy = vdup_lane_f32(vget_low_f32(vCIn1), 1);
		float32x2_t dd = vdup_lane_f32(vget_high_f32(vCIn1), 1);
		YmV4F32 v2 = vcombine_f32(yy, dd);

		v2 = YMSIMD_MUL_V4F32(v2, vSign);
		v2 = YMSIMD_MUL_V4F32(v2, vCIn2);
		v2 = vrev64q_f32(v2);
		return YMSIMD_ADD_V4F32(v2, YMSIMD_MUL_V4F32(v1, vCIn2));
	}
#elif defined(_WIN32)||defined(_WIN64)||defined(__linux__)||defined(_MAC)
	#define YMSIMD_ADD_V4F32( a, b )					_mm_add_ps( a, b )
	#define YMSIMD_MUL_SS_V4F32( a, b )					_mm_mul_ss( a, b )
	#define YMSIMD_STORE_V4F32( __addr__, __vec__ )		_mm_store_ps( (YmReal32*)(__addr__), (__vec__) )
	#define YMSIMD_STOREU_V4F32( __addr__, __vec__ )	_mm_storeu_ps( (YmReal32*)(__addr__), (__vec__) )
	#define YMSIMD_SET_V4F32( __scalar__ )				_mm_set_ps1( (__scalar__) )
	#define YMSIMD_MADD_V4F32( __a__, __b__, __c__ )	_mm_add_ps( _mm_mul_ps( (__a__), (__b__) ), (__c__) )
	#define YMSIMD_MUL_V4F32( a, b )					_mm_mul_ps( a, b )
	#define YMSIMD_LOAD_V4F32( __addr__ )				_mm_load_ps( (YmReal32*)(__addr__) )
	#define YMSIMD_LOADU_V4F32( __addr__ )				_mm_loadu_ps( (YmReal32*)(__addr__) )
	#define YMSIMD_LOAD1_V4F32( __scalar__ )			_mm_load1_ps( &(__scalar__) )
	#define YMSIMD_SET1_EPI32_V4I32( a )				_mm_set1_epi32( a )
	#define YMSIMD_SET_EPI32_V4I32( a, b, c, d )		_mm_set_epi32( a, b, c, d )
	static inline YmV4F32 YMSIMD_COMPLEXMUL(const YmV4F32 vCIn1, const YmV4F32 vCIn2)
	{
		YmV4F32 t1, t2, t3;
		t1 = vCIn1;
		t2 = vCIn2;
		t3 = _mm_moveldup_ps(t2);
		t3 = _mm_mul_ps(t1, t3);
		t2 = _mm_movehdup_ps(t2);
		t1 = _mm_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 3, 0, 1));
		t2 = _mm_mul_ps(t1, t2);
		return _mm_addsub_ps(t3, t2);
	}
#endif

//--- INTRINSIC (not supported by Wwise)
#if defined(_M_ARM)||defined(_M_ARM64)||defined(__ARM_NEON)
	#define YMSIMD_XOR_V4I32( a, b )					(veorq_s32( ( a ), ( b ) ))
#else
	#define YMSIMD_XOR_V4I32( a, b )					_mm_xor_si128( a, b )
#endif

#if defined(YM_TARGET_WWISE) && defined(NN_NINTENDO_SDK)
	#define YMSIMD_SET_EPI32_V4I32(a, b, c, d)			__extension__ ({ \
		int32_t __attribute__((aligned(16))) data[4] = { (d), (c), (b), (a) }; \
		int32x4_t __ret; \
		__ret = (int32x4_t) (vld1q_s32(data)); \
		__ret; \
	})
#endif

//--- MACRO
#if defined(YM_TARGET_WWISE)
	#define YM_ALIGN_SIMD( __Declaration__ )			AK_ALIGN_SIMD(__Declaration__)
#elif defined (_WIN32)||defined(_WIN64)
	#define YM_ALIGN_SIMD( __Declaration__ )			__declspec(align(16)) __Declaration__ ///< Platform-specific alignment requirement for SIMD data
#else
	#define YM_ALIGN_SIMD( __Declaration__ )			__attribute__ ((aligned (16))) __Declaration__ ///< Platform-specific alignment requirement for SIMD data
#endif

/*********************************************************************************************
* EOF
*********************************************************************************************/
