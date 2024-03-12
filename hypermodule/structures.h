#pragma once
#pragma warning( disable : 4996 )

#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <chrono>

using namespace std;

typedef struct _UNICODE_STRING
{
	USHORT	Length;
	USHORT	MaximumLength;
	PWSTR	Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct PEB_LDR_DATA
{
	ULONG		Length;
	BOOLEAN		Initialized;
	PVOID		SsHandle;
	LIST_ENTRY  InLoadOrderModuleList;
	LIST_ENTRY  InMemoryOrderModuleList;
	LIST_ENTRY  InInitializationOrderModuleList;
	PVOID		EntryInProgress;
	BOOLEAN		ShutdownInProgress;
	HANDLE		ShutdownThreadId;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB
{
	BYTE       Reserved1[ 2 ];
	BYTE	   BeingDebugged;
	BYTE       Reserved2[ 1 ];
	PVOID      Reserved3[ 2 ];
	PPEB_LDR_DATA Ldr;
} PEB, * PPEB;

typedef struct _LDR_MODULE
{
	LIST_ENTRY		InLoadOrderModuleList;
	LIST_ENTRY		InMemoryOrderModuleList;
	LIST_ENTRY		InInitializationOrderModuleList;
	PVOID			BaseAddress;
	PVOID			EntryPoint;
	ULONG			SizeOfImage;
	UNICODE_STRING	FullDllName;
	UNICODE_STRING	BaseDllName;
	ULONG			Flags;
	SHORT			LoadCount;
	SHORT			TlsIndex;
	LIST_ENTRY		HashTableEntry;
	ULONG			TimeDateStamp;
} LDR_MODULE, * PLDR_MODULE;