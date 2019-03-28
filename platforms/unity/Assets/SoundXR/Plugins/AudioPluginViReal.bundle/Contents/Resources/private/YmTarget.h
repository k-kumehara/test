/*****************************************************************************************//**
 * @file			YmTarget.h
 * @brief			ターゲットごとの設定を行う
 * @attention		
 *
 *                 (C) 2018 Yamaha Corporation
 *                  Confidential
 ********************************************************************************************/
/*****************************************************************************************//**
 * ビルド構成ごとに以下のプリプロセッサ定義を追加してください。
 *
 * [target] 一つ選択
 *		YM_TARGET_WWISE
 *		YM_TARGET_UNITY
 *		YM_TARGET_VST3
 *		YM_TARGET_SDK
 *
 ********************************************************************************************/

#pragma once

/***************************************************************************
 * ターゲットごとの設定 		             [0 or 1]
 **************************************************************************/
#if defined(YM_TARGET_WWISE)
	// base（どちらかは１でないと動かない。現在は片方のみ１である必要がある）
	#define	YM_USE_FREQ_DOMAIN				1	// 周波数軸畳込処理		[0:OFF,1:ON]
	#define	YM_USE_TIME_DOMAIN				0	// 時間軸畳込処理		[0:OFF,1:ON]
	// option（全部０でも動く）
	#define	YM_USE_CUSTOM_ALLOCATOR			1	// メモリアロケータ		[0:標準,1:カスタム]
	#define YM_USE_SIMD						1	// SIMD命令				[0:OFF,1:ON]
	#define YM_USE_DISTANCE_DECAY			0	// 距離減衰機能			[0:OFF,1:ON]
//	#define YM_USE_SOUND_SIZE				0	// 音源サイズ設定機能		[0:OFF,1:ON]
	#define YM_USE_TIMBRE_CORRECTION		0	// 音質補正機能			[0:OFF,1:ON]
	#define YM_USE_HRTF_PACK				0	// 外部HRTF読込機能		[0:OFF,1:ON]
	#define YM_USE_HRTF_SELECTOR			0	// HRTF切替機能			[0:OFF,1:ON]
#if defined YM_USE_AUTH // 従来のプロジェクト設定がそのまま活きるよう、一時的な措置
	#undef  YM_USE_AUTH
	#define YM_USE_AUTH						1	// 認証機能				[0:OFF,1:ON]
#else
	#define YM_USE_AUTH						0	// 認証機能				[0:OFF,1:ON]
#endif

#elif defined(YM_TARGET_UNITY)
	// base
	#define	YM_USE_FREQ_DOMAIN				1	// 周波数軸畳込処理		[0:OFF,1:ON]
	#define	YM_USE_TIME_DOMAIN				0	// 時間軸畳込処理		[0:OFF,1:ON]
	// option
	#define	YM_USE_CUSTOM_ALLOCATOR			0	// メモリアロケータ		[0:標準,1:カスタム]
	#define YM_USE_SIMD						1	// SIMD命令				[0:OFF,1:ON]
	#define YM_USE_DISTANCE_DECAY			0	// 距離減衰機能			[0:OFF,1:ON]
//	#define YM_USE_SOUND_SIZE				1	// 音源サイズ設定機能		[0:OFF,1:ON]
	#define YM_USE_TIMBRE_CORRECTION		0	// 音質補正機能			[0:OFF,1:ON]
	#define YM_USE_HRTF_PACK				0	// 外部HRTF読込機能		[0:OFF,1:ON]
	#define YM_USE_HRTF_SELECTOR			0	// HRTF切替機能			[0:OFF,1:ON]
	#define YM_USE_AUTH						0	// 認証機能				[0:OFF,1:ON]

#elif defined(YM_TARGET_VST3)
	// base
	#define	YM_USE_FREQ_DOMAIN				0	// 周波数軸畳込処理		[0:OFF,1:ON]
	#define	YM_USE_TIME_DOMAIN				1	// 時間軸畳込処理		[0:OFF,1:ON]
	// option
	#define	YM_USE_CUSTOM_ALLOCATOR			0	// メモリアロケータ		[0:標準,1:カスタム]
	#define YM_USE_SIMD						1	// SIMD命令				[0:OFF,1:ON]
	#define YM_USE_DISTANCE_DECAY			1	// 距離減衰機能			[0:OFF,1:ON]
//	#define YM_USE_SOUND_SIZE				1	// 音源サイズ設定機能		[0:OFF,1:ON]
	#define YM_USE_TIMBRE_CORRECTION		1	// 音質補正機能			[0:OFF,1:ON]
	#define YM_USE_HRTF_PACK				1	// 外部HRTF読込機能		[0:OFF,1:ON]
	#define YM_USE_HRTF_SELECTOR			1	// HRTF切替機能			[0:OFF,1:ON]
	#define YM_USE_AUTH						1	// 認証機能				[0:OFF,1:ON]

#elif defined(YM_TARGET_SDK)
	// base
	#define	YM_USE_FREQ_DOMAIN				1	// 周波数軸畳込処理		[0:OFF,1:ON]
	#define	YM_USE_TIME_DOMAIN				0	// 時間軸畳込処理		[0:OFF,1:ON]
	// option
	#define	YM_USE_CUSTOM_ALLOCATOR			0	// メモリアロケータ		[0:標準,1:カスタム]
	#define YM_USE_SIMD						1	// SIMD命令				[0:OFF,1:ON]
	#define YM_USE_DISTANCE_DECAY			1	// 距離減衰機能			[0:OFF,1:ON]
//	#define YM_USE_SOUND_SIZE				1	// 音源サイズ設定機能		[0:OFF,1:ON]
	#define YM_USE_TIMBRE_CORRECTION		1	// 音質補正機能			[0:OFF,1:ON]
	#define YM_USE_HRTF_PACK				0	// 外部HRTF読込機能		[0:OFF,1:ON]
	#define YM_USE_HRTF_SELECTOR			0	// HRTF切替機能			[0:OFF,1:ON]
	#define YM_USE_AUTH						1	// 認証機能				[0:OFF,1:ON]

#elif defined(YM_TARGET_TEST_FREQ)
	// base
	#define	YM_USE_FREQ_DOMAIN				1	// 周波数軸畳込処理		[0:OFF,1:ON]
	#define	YM_USE_TIME_DOMAIN				0	// 時間軸畳込処理		[0:OFF,1:ON]
	// option
	#define	YM_USE_CUSTOM_ALLOCATOR			0	// メモリアロケータ		[0:標準,1:カスタム]
	#define YM_USE_SIMD						1	// SIMD命令				[0:OFF,1:ON]
	#define YM_USE_DISTANCE_DECAY			1	// 距離減衰機能			[0:OFF,1:ON]
//	#define YM_USE_SOUND_SIZE				1	// 音源サイズ設定機能		[0:OFF,1:ON]
	#define YM_USE_TIMBRE_CORRECTION		1	// 音質補正機能			[0:OFF,1:ON]
	#define YM_USE_HRTF_PACK				1	// 外部HRTF読込機能		[0:OFF,1:ON]
	#define YM_USE_HRTF_SELECTOR			1	// HRTF切替機能			[0:OFF,1:ON]
	#define YM_USE_AUTH						1	// 認証機能				[0:OFF,1:ON]

#elif defined(YM_TARGET_TEST_TIME)
	// base
	#define	YM_USE_FREQ_DOMAIN				0	// 周波数軸畳込処理		[0:OFF,1:ON]
	#define	YM_USE_TIME_DOMAIN				1	// 時間軸畳込処理		[0:OFF,1:ON]
	// option
	#define	YM_USE_CUSTOM_ALLOCATOR			0	// メモリアロケータ		[0:標準,1:カスタム]
	#define YM_USE_SIMD						1	// SIMD命令				[0:OFF,1:ON]
	#define YM_USE_DISTANCE_DECAY			1	// 距離減衰機能			[0:OFF,1:ON]
//	#define YM_USE_SOUND_SIZE				1	// 音源サイズ設定機能		[0:OFF,1:ON]
	#define YM_USE_TIMBRE_CORRECTION		1	// 音質補正機能			[0:OFF,1:ON]
	#define YM_USE_HRTF_PACK				1	// 外部HRTF読込機能		[0:OFF,1:ON]
	#define YM_USE_HRTF_SELECTOR			1	// HRTF切替機能			[0:OFF,1:ON]
	#define YM_USE_AUTH						1	// 認証機能				[0:OFF,1:ON]

#else
	#define YM_TARGET_GENERIC
	// base
	#define	YM_USE_FREQ_DOMAIN				0	// 周波数軸畳込処理		[0:OFF,1:ON]
	#define	YM_USE_TIME_DOMAIN				1	// 時間軸畳込処理		[0:OFF,1:ON]
	// option
	#define	YM_USE_CUSTOM_ALLOCATOR			0	// メモリアロケータ		[0:標準,1:カスタム]
	#define YM_USE_SIMD						1	// SIMD命令				[0:OFF,1:ON]
	#define YM_USE_DISTANCE_DECAY			1	// 距離減衰機能			[0:OFF,1:ON]
//	#define YM_USE_SOUND_SIZE				1	// 音源サイズ設定機能		[0:OFF,1:ON]
	#define YM_USE_TIMBRE_CORRECTION		1	// 音質補正機能			[0:OFF,1:ON]
	#define YM_USE_HRTF_PACK				1	// 外部HRTF読込機能		[0:OFF,1:ON]
	#define YM_USE_HRTF_SELECTOR			1	// HRTF切替機能			[0:OFF,1:ON]
	#define YM_USE_AUTH						0	// 認証機能				[0:OFF,1:ON]
#endif

// YMH3DHPDLL が定義された場合以外は YMH3DHP_STATIC を定義する
// (基本的には static lib 利用を想定)
#ifdef YMH3DHPDLL
	#ifdef YMH3DHP_STATIC
	#undef YMH3DHP_STATIC
	#endif
#else
	#ifndef YMH3DHP_STATIC
	#define YMH3DHP_STATIC
	#endif
#endif

/*********************************************************************************************
* EOF
*********************************************************************************************/
