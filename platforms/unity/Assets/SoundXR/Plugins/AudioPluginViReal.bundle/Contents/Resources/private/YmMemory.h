/*****************************************************************************************//**
 * @file			YmMemory.h
 * @brief
 * @attention
 *
 *                 (C) 2018 Yamaha Corporation
 *                  Confidential
 ********************************************************************************************/

#pragma once

#include <stdlib.h>
#include <string.h>

#ifdef YM_TARGET_WWISE
	#include <AK/SoundEngine/Common/IAkPlugin.h>
#endif

#ifndef YM_USE_CUSTOM_ALLOCATOR
	#define YM_USE_CUSTOM_ALLOCATOR 0
#endif

/***********************************************************************//**
 * custom allocator
 **************************************************************************/
#if YM_USE_CUSTOM_ALLOCATOR
	#ifdef YM_TARGET_WWISE
		#define YmMemAlloc			AK::IAkPluginMemAlloc
	#else
		#error Custom allocator is not implemented.
	#endif
#else
		#define YmMemAlloc			void
#endif

/***********************************************************************//**
 * delete
 **************************************************************************/
template <class T> inline void YM_DELETE(YmMemAlloc * in_pAllocator, T * in_pObject)
{
#if !defined(YM_TARGET_WWISE)
	(void)in_pAllocator;
#endif
	if (in_pObject)
	{
#if defined(YM_TARGET_WWISE)
		AK_PLUGIN_DELETE(in_pAllocator, in_pObject);
#else
		delete in_pObject;
#endif
	}
}

template <class T> inline void checked_delete(T*& p)
{
	// 不完全な型はコンパイル時エラー
	typedef char checked_type[sizeof(T)? 1 : -1];
	(void)sizeof(checked_type);

	if (p == nullptr)
		return;

	delete p;
	p = nullptr;
}

template <class T> inline void checked_array_delete(T*& p)
{
	// 不完全な型はコンパイル時エラー
	typedef char checked_type[sizeof(T)? 1 : -1];
	(void)sizeof(checked_type);

	if (p == nullptr)
		return;

	delete[] p;
	p = nullptr;
}

/***********************************************************************//**
 * new
 **************************************************************************/
#ifdef YM_TARGET_WWISE
	#define YM_NEW(_allocator,_what)	AK_PLUGIN_NEW(_allocator,_what)
#else
	#define YM_NEW(_allocator,_what)	new _what
#endif

/***********************************************************************//**
 * aligned free
 **************************************************************************/
inline void free_memory(YmMemAlloc * in_pAllocator, void* mem)
{
	if (mem == nullptr) return;
#if defined(YM_TARGET_WWISE)
	AK_PLUGIN_FREE(in_pAllocator, mem);
#else
	(void)in_pAllocator;
	#if defined (_WIN32)||defined(_WIN64)
	_aligned_free(mem);
	#else
	free(mem);
	#endif
#endif
	mem = nullptr;
}

inline void free_memory(void* mem)
{
	if (mem == nullptr) return;
#if defined (_WIN32)||defined(_WIN64)
	_aligned_free(mem);
#else
	free(mem);
#endif
	mem = nullptr;
}

/***********************************************************************//**
 * aligned malloc
 **************************************************************************/
// Visual Studio の警告レベルを /wall に指定すると以下の警告が出るが、無視する。
//   「warning C4571: 情報: catch(...) の意味が Visual C++ 7.1 から変更されています。構造化例外 (SEH) はキャッチされません」
// 
// ざっくり言うと、SEH は Microsoft 固有の例外処理であり、ANSI/ISO C++標準の例外処理(std::exception)の使用を勧める警告。
// ViReal にて実績のある現状維持とする。
inline void* alloc_memory(YmMemAlloc * in_pAllocator, size_t size, size_t align)
{
	void* ptr = nullptr;
	try
	{
#if defined(YM_TARGET_WWISE)
		(void)align;
		ptr = AK_PLUGIN_ALLOC(in_pAllocator, size);        // check 2017.01.17 align 指定できない？
#else
		(void)in_pAllocator;
	#if defined (_WIN32)||defined(_WIN64)
		ptr = _aligned_malloc(size, align);
	#else
		if (posix_memalign(&ptr, align, size) != 0) ptr = nullptr;
	#endif
#endif
		if (ptr != nullptr)
		{
			memset(ptr, 0, size);
		}
	}
	catch (...)
	{
		free_memory(in_pAllocator, ptr);
	}
	return ptr;
}

inline void* alloc_memory(size_t size, size_t align)
{
	void* ptr = nullptr;
	try
	{
#if defined (_WIN32)||defined(_WIN64)
		ptr = _aligned_malloc(size, align);
#else
		if (posix_memalign(&ptr, align, size) != 0) ptr = nullptr;
#endif
		if (ptr != nullptr)
		{
			memset(ptr, 0, size);
		}
	}
	catch (...)
	{
		free_memory(ptr);
	}
	return ptr;
}

/*********************************************************************************************
* EOF
*********************************************************************************************/
