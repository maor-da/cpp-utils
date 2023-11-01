#pragma once

#include <Windows.h>
//
#include <SubAuth.h>

/**
* dt nt!_PEB32
   +0x000 InheritedAddressSpace : UChar
   +0x001 ReadImageFileExecOptions : UChar
   +0x002 BeingDebugged    : UChar
   +0x003 BitField         : UChar
   +0x003 ImageUsesLargePages : Pos 0, 1 Bit
   +0x003 IsProtectedProcess : Pos 1, 1 Bit
   +0x003 IsImageDynamicallyRelocated : Pos 2, 1 Bit
   +0x003 SkipPatchingUser32Forwarders : Pos 3, 1 Bit
   +0x003 IsPackagedProcess : Pos 4, 1 Bit
   +0x003 IsAppContainer   : Pos 5, 1 Bit
   +0x003 IsProtectedProcessLight : Pos 6, 1 Bit
   +0x003 IsLongPathAwareProcess : Pos 7, 1 Bit
   +0x004 Mutant           : Uint4B
   +0x008 ImageBaseAddress : Uint4B
   +0x00c Ldr              : Uint4B
   +0x010 ProcessParameters : Uint4B
   +0x014 SubSystemData    : Uint4B
   +0x018 ProcessHeap      : Uint4B
   +0x01c FastPebLock      : Uint4B
   +0x020 AtlThunkSListPtr : Uint4B
   +0x024 IFEOKey          : Uint4B
   +0x028 CrossProcessFlags : Uint4B
   +0x028 ProcessInJob     : Pos 0, 1 Bit
   +0x028 ProcessInitializing : Pos 1, 1 Bit
   +0x028 ProcessUsingVEH  : Pos 2, 1 Bit
   +0x028 ProcessUsingVCH  : Pos 3, 1 Bit
   +0x028 ProcessUsingFTH  : Pos 4, 1 Bit
   +0x028 ProcessPreviouslyThrottled : Pos 5, 1 Bit
   +0x028 ProcessCurrentlyThrottled : Pos 6, 1 Bit
   +0x028 ProcessImagesHotPatched : Pos 7, 1 Bit
   +0x028 ReservedBits0    : Pos 8, 24 Bits
   +0x02c KernelCallbackTable : Uint4B
   +0x02c UserSharedInfoPtr : Uint4B
   +0x030 SystemReserved   : Uint4B
   +0x034 AtlThunkSListPtr32 : Uint4B
   +0x038 ApiSetMap        : Uint4B
   +0x03c TlsExpansionCounter : Uint4B
   +0x040 TlsBitmap        : Uint4B
   +0x044 TlsBitmapBits    : [2] Uint4B
   +0x04c ReadOnlySharedMemoryBase : Uint4B
   +0x050 SharedData       : Uint4B
   +0x054 ReadOnlyStaticServerData : Uint4B
   +0x058 AnsiCodePageData : Uint4B
   +0x05c OemCodePageData  : Uint4B
   +0x060 UnicodeCaseTableData : Uint4B
   +0x064 NumberOfProcessors : Uint4B
   +0x068 NtGlobalFlag     : Uint4B
   +0x070 CriticalSectionTimeout : _LARGE_INTEGER
   +0x078 HeapSegmentReserve : Uint4B
   +0x07c HeapSegmentCommit : Uint4B
   +0x080 HeapDeCommitTotalFreeThreshold : Uint4B
   +0x084 HeapDeCommitFreeBlockThreshold : Uint4B
   +0x088 NumberOfHeaps    : Uint4B
   +0x08c MaximumNumberOfHeaps : Uint4B
   +0x090 ProcessHeaps     : Uint4B
   +0x094 GdiSharedHandleTable : Uint4B
   +0x098 ProcessStarterHelper : Uint4B
   +0x09c GdiDCAttributeList : Uint4B
   +0x0a0 LoaderLock       : Uint4B
   +0x0a4 OSMajorVersion   : Uint4B
   +0x0a8 OSMinorVersion   : Uint4B
   +0x0ac OSBuildNumber    : Uint2B
   +0x0ae OSCSDVersion     : Uint2B
   +0x0b0 OSPlatformId     : Uint4B
   +0x0b4 ImageSubsystem   : Uint4B
   +0x0b8 ImageSubsystemMajorVersion : Uint4B
   +0x0bc ImageSubsystemMinorVersion : Uint4B
   +0x0c0 ActiveProcessAffinityMask : Uint4B
   +0x0c4 GdiHandleBuffer  : [34] Uint4B
   +0x14c PostProcessInitRoutine : Uint4B
   +0x150 TlsExpansionBitmap : Uint4B
   +0x154 TlsExpansionBitmapBits : [32] Uint4B
   +0x1d4 SessionId        : Uint4B
   +0x1d8 AppCompatFlags   : _ULARGE_INTEGER
   +0x1e0 AppCompatFlagsUser : _ULARGE_INTEGER
   +0x1e8 pShimData        : Uint4B
   +0x1ec AppCompatInfo    : Uint4B
   +0x1f0 CSDVersion       : _STRING32
   +0x1f8 ActivationContextData : Uint4B
   +0x1fc ProcessAssemblyStorageMap : Uint4B
   +0x200 SystemDefaultActivationContextData : Uint4B
   +0x204 SystemAssemblyStorageMap : Uint4B
   +0x208 MinimumStackCommit : Uint4B
   +0x20c SparePointers    : [4] Uint4B
   +0x21c SpareUlongs      : [5] Uint4B
   +0x230 WerRegistrationData : Uint4B
   +0x234 WerShipAssertPtr : Uint4B
   +0x238 pUnused          : Uint4B
   +0x23c pImageHeaderHash : Uint4B
   +0x240 TracingFlags     : Uint4B
   +0x240 HeapTracingEnabled : Pos 0, 1 Bit
   +0x240 CritSecTracingEnabled : Pos 1, 1 Bit
   +0x240 LibLoaderTracingEnabled : Pos 2, 1 Bit
   +0x240 SpareTracingBits : Pos 3, 29 Bits
   +0x248 CsrServerReadOnlySharedMemoryBase : Uint8B
   +0x250 TppWorkerpListLock : Uint4B
   +0x254 TppWorkerpList   : LIST_ENTRY32
   +0x25c WaitOnAddressHashTable : [128] Uint4B
   +0x45c TelemetryCoverageHeader : Uint4B
   +0x460 CloudFileFlags   : Uint4B
   +0x464 CloudFileDiagFlags : Uint4B
   +0x468 PlaceholderCompatibilityMode : Char
   +0x469 PlaceholderCompatibilityModeReserved : [7] Char
   +0x470 LeapSecondData   : Uint4B
   +0x474 LeapSecondFlags  : Uint4B
   +0x474 SixtySecondEnabled : Pos 0, 1 Bit
   +0x474 Reserved         : Pos 1, 31 Bits
   +0x478 NtGlobalFlag2    : Uint4B
*/

/**
* dt nt!_PEB
   +0x000 InheritedAddressSpace : UChar
   +0x001 ReadImageFileExecOptions : UChar
   +0x002 BeingDebugged    : UChar
   +0x003 BitField         : UChar
   +0x003 ImageUsesLargePages : Pos 0, 1 Bit
   +0x003 IsProtectedProcess : Pos 1, 1 Bit
   +0x003 IsImageDynamicallyRelocated : Pos 2, 1 Bit
   +0x003 SkipPatchingUser32Forwarders : Pos 3, 1 Bit
   +0x003 IsPackagedProcess : Pos 4, 1 Bit
   +0x003 IsAppContainer   : Pos 5, 1 Bit
   +0x003 IsProtectedProcessLight : Pos 6, 1 Bit
   +0x003 IsLongPathAwareProcess : Pos 7, 1 Bit
   +0x004 Padding0         : [4] UChar
   +0x008 Mutant           : Ptr64 Void
   +0x010 ImageBaseAddress : Ptr64 Void
   +0x018 Ldr              : Ptr64 _PEB_LDR_DATA
   +0x020 ProcessParameters : Ptr64 _RTL_USER_PROCESS_PARAMETERS
   +0x028 SubSystemData    : Ptr64 Void
   +0x030 ProcessHeap      : Ptr64 Void
   +0x038 FastPebLock      : Ptr64 _RTL_CRITICAL_SECTION
   +0x040 AtlThunkSListPtr : Ptr64 _SLIST_HEADER
   +0x048 IFEOKey          : Ptr64 Void
   +0x050 CrossProcessFlags : Uint4B
   +0x050 ProcessInJob     : Pos 0, 1 Bit
   +0x050 ProcessInitializing : Pos 1, 1 Bit
   +0x050 ProcessUsingVEH  : Pos 2, 1 Bit
   +0x050 ProcessUsingVCH  : Pos 3, 1 Bit
   +0x050 ProcessUsingFTH  : Pos 4, 1 Bit
   +0x050 ProcessPreviouslyThrottled : Pos 5, 1 Bit
   +0x050 ProcessCurrentlyThrottled : Pos 6, 1 Bit
   +0x050 ProcessImagesHotPatched : Pos 7, 1 Bit
   +0x050 ReservedBits0    : Pos 8, 24 Bits
   +0x054 Padding1         : [4] UChar
   +0x058 KernelCallbackTable : Ptr64 Void
   +0x058 UserSharedInfoPtr : Ptr64 Void
   +0x060 SystemReserved   : Uint4B
   +0x064 AtlThunkSListPtr32 : Uint4B
   +0x068 ApiSetMap        : Ptr64 Void
   +0x070 TlsExpansionCounter : Uint4B
   +0x074 Padding2         : [4] UChar
   +0x078 TlsBitmap        : Ptr64 Void
   +0x080 TlsBitmapBits    : [2] Uint4B
   +0x088 ReadOnlySharedMemoryBase : Ptr64 Void
   +0x090 SharedData       : Ptr64 Void
   +0x098 ReadOnlyStaticServerData : Ptr64 Ptr64 Void
   +0x0a0 AnsiCodePageData : Ptr64 Void
   +0x0a8 OemCodePageData  : Ptr64 Void
   +0x0b0 UnicodeCaseTableData : Ptr64 Void
   +0x0b8 NumberOfProcessors : Uint4B
   +0x0bc NtGlobalFlag     : Uint4B
   +0x0c0 CriticalSectionTimeout : _LARGE_INTEGER
   +0x0c8 HeapSegmentReserve : Uint8B
   +0x0d0 HeapSegmentCommit : Uint8B
   +0x0d8 HeapDeCommitTotalFreeThreshold : Uint8B
   +0x0e0 HeapDeCommitFreeBlockThreshold : Uint8B
   +0x0e8 NumberOfHeaps    : Uint4B
   +0x0ec MaximumNumberOfHeaps : Uint4B
   +0x0f0 ProcessHeaps     : Ptr64 Ptr64 Void
   +0x0f8 GdiSharedHandleTable : Ptr64 Void
   +0x100 ProcessStarterHelper : Ptr64 Void
   +0x108 GdiDCAttributeList : Uint4B
   +0x10c Padding3         : [4] UChar
   +0x110 LoaderLock       : Ptr64 _RTL_CRITICAL_SECTION
   +0x118 OSMajorVersion   : Uint4B
   +0x11c OSMinorVersion   : Uint4B
   +0x120 OSBuildNumber    : Uint2B
   +0x122 OSCSDVersion     : Uint2B
   +0x124 OSPlatformId     : Uint4B
   +0x128 ImageSubsystem   : Uint4B
   +0x12c ImageSubsystemMajorVersion : Uint4B
   +0x130 ImageSubsystemMinorVersion : Uint4B
   +0x134 Padding4         : [4] UChar
   +0x138 ActiveProcessAffinityMask : Uint8B
   +0x140 GdiHandleBuffer  : [60] Uint4B
   +0x230 PostProcessInitRoutine : Ptr64     void
   +0x238 TlsExpansionBitmap : Ptr64 Void
   +0x240 TlsExpansionBitmapBits : [32] Uint4B
   +0x2c0 SessionId        : Uint4B
   +0x2c4 Padding5         : [4] UChar
   +0x2c8 AppCompatFlags   : _ULARGE_INTEGER
   +0x2d0 AppCompatFlagsUser : _ULARGE_INTEGER
   +0x2d8 pShimData        : Ptr64 Void
   +0x2e0 AppCompatInfo    : Ptr64 Void
   +0x2e8 CSDVersion       : _UNICODE_STRING
   +0x2f8 ActivationContextData : Ptr64 _ACTIVATION_CONTEXT_DATA
   +0x300 ProcessAssemblyStorageMap : Ptr64 _ASSEMBLY_STORAGE_MAP
   +0x308 SystemDefaultActivationContextData : Ptr64 _ACTIVATION_CONTEXT_DATA
   +0x310 SystemAssemblyStorageMap : Ptr64 _ASSEMBLY_STORAGE_MAP
   +0x318 MinimumStackCommit : Uint8B
   +0x320 SparePointers    : [4] Ptr64 Void
   +0x340 SpareUlongs      : [5] Uint4B
   +0x358 WerRegistrationData : Ptr64 Void
   +0x360 WerShipAssertPtr : Ptr64 Void
   +0x368 pUnused          : Ptr64 Void
   +0x370 pImageHeaderHash : Ptr64 Void
   +0x378 TracingFlags     : Uint4B
   +0x378 HeapTracingEnabled : Pos 0, 1 Bit
   +0x378 CritSecTracingEnabled : Pos 1, 1 Bit
   +0x378 LibLoaderTracingEnabled : Pos 2, 1 Bit
   +0x378 SpareTracingBits : Pos 3, 29 Bits
   +0x37c Padding6         : [4] UChar
   +0x380 CsrServerReadOnlySharedMemoryBase : Uint8B
   +0x388 TppWorkerpListLock : Uint8B
   +0x390 TppWorkerpList   : _LIST_ENTRY
   +0x3a0 WaitOnAddressHashTable : [128] Ptr64 Void
   +0x7a0 TelemetryCoverageHeader : Ptr64 Void
   +0x7a8 CloudFileFlags   : Uint4B
   +0x7ac CloudFileDiagFlags : Uint4B
   +0x7b0 PlaceholderCompatibilityMode : Char
   +0x7b1 PlaceholderCompatibilityModeReserved : [7] Char
   +0x7b8 LeapSecondData   : Ptr64 _LEAP_SECOND_DATA
   +0x7c0 LeapSecondFlags  : Uint4B
   +0x7c0 SixtySecondEnabled : Pos 0, 1 Bit
   +0x7c0 Reserved         : Pos 1, 31 Bits
   +0x7c4 NtGlobalFlag2    : Uint4B
*/

typedef void* PPS_POST_PROCESS_INIT_ROUTINE;

// typedef struct _LSA_UNICODE_STRING {
//	USHORT Length;
//	USHORT MaximumLength;
//	PWSTR Buffer;
//} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

// typedef struct _STRING {
//	USHORT Length;
//	USHORT MaximumLength;
//	PCHAR Buffer;
//} STRING, *PSTRING, ANSI_STRING, *PANSI_STRING;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE Reserved1[16];
	PVOID Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

// PEB defined by rewolf
// http://blog.rewolf.pl/blog/?p=573
typedef struct _PEB_LDR_DATA {
	ULONG Length;
	BOOL Initialized;
	LPVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	LPVOID DllBase;
	LPVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_CRITICAL_SECTION RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;

// typedef struct _RTL_CRITICAL_SECTION_DEBUG {
//	WORD Type;
//	WORD CreatorBackTraceIndex;
//	PRTL_CRITICAL_SECTION CriticalSection;
//	LIST_ENTRY ProcessLocksList;
//	ULONG EntryCount;
//	ULONG ContentionCount;
//	ULONG Flags;
//	WORD CreatorBackTraceIndexHigh;
//	WORD SpareUSHORT;
// } RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;

// struct _RTL_CRITICAL_SECTION {
//	PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
//	LONG LockCount;
//	LONG RecursionCount;
//	PVOID OwningThread;
//	PVOID LockSemaphore;
//	ULONG SpinCount;
// };

typedef struct _PEB {
	BYTE InheritedAddressSpace;
	BYTE ReadImageFileExecOptions;
	BYTE BeingDebugged;
	union {
		BYTE _SYSTEM_DEPENDENT_01;
		struct {
			BYTE ImageUsesLargePages : 1;
			BYTE IsProtectedProcess : 1;
			BYTE IsImageDynamicallyRelocated : 1;
			BYTE SkipPatchingUser32Forwarders : 1;
			BYTE IsPackagedProcess : 1;
			BYTE IsAppContainer : 1;
			BYTE IsProtectedProcessLight : 1;
			BYTE IsLongPathAwareProcess : 1;
		} BitField;
	};

	LPVOID Mutant;
	LPVOID ImageBaseAddress;

	PPEB_LDR_DATA Ldr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	LPVOID SubSystemData;
	LPVOID ProcessHeap;
	LPVOID FastPebLock;
	union {
		LPVOID _SYSTEM_DEPENDENT_02;
		PSLIST_HEADER AtlThunkSListPtr;
	};
	union {
		LPVOID _SYSTEM_DEPENDENT_03;
		LPVOID IFEOKey;
	};
	union {
		DWORD _SYSTEM_DEPENDENT_04;
		struct {
			DWORD ProcessInJob : 1;
			DWORD ProcessInitializing : 1;
			DWORD ProcessUsingVEH : 1;
			DWORD ProcessUsingVCH : 1;
			DWORD ProcessUsingFTH : 1;
			DWORD ProcessPreviouslyThrottled : 1;
			DWORD ProcessCurrentlyThrottled : 1;
			DWORD ProcessImagesHotPatched : 1;
			DWORD ReservedBits0 : 24;
		} CrossProcessFlags;
	};
	union {
		LPVOID KernelCallbackTable;
		LPVOID UserSharedInfoPtr;
	};
	DWORD SystemReserved;
	union {
		DWORD _SYSTEM_DEPENDENT_05;
		DWORD AtlThunkSListPtr32;
	};
	union {
		LPVOID _SYSTEM_DEPENDENT_06;
		LPVOID ApiSetMap;
	};

	LPVOID TlsExpansionCounter;
	LPVOID TlsBitmap;
	DWORD TlsBitmapBits[2];
	LPVOID ReadOnlySharedMemoryBase;
	union {
		LPVOID _SYSTEM_DEPENDENT_07;
		LPVOID SharedData;
	};

	LPVOID ReadOnlyStaticServerData;
	LPVOID AnsiCodePageData;
	LPVOID OemCodePageData;
	LPVOID UnicodeCaseTableData;
	DWORD NumberOfProcessors;
	union {
		DWORD NtGlobalFlag;
		LPVOID dummy02;
	};
	LARGE_INTEGER CriticalSectionTimeout;
	LPVOID HeapSegmentReserve;
	LPVOID HeapSegmentCommit;
	LPVOID HeapDeCommitTotalFreeThreshold;
	LPVOID HeapDeCommitFreeBlockThreshold;
	DWORD NumberOfHeaps;
	DWORD MaximumNumberOfHeaps;
	LPVOID ProcessHeaps;
	LPVOID GdiSharedHandleTable;
	LPVOID ProcessStarterHelper;
	LPVOID GdiDCAttributeList;
	PRTL_CRITICAL_SECTION LoaderLock;
	DWORD OSMajorVersion;
	DWORD OSMinorVersion;
	WORD OSBuildNumber;
	WORD OSCSDVersion;
	DWORD OSPlatformId;
	DWORD ImageSubsystem;
	DWORD ImageSubsystemMajorVersion;
	LPVOID ImageSubsystemMinorVersion;
	union {
		LPVOID ImageProcessAffinityMask;
		LPVOID ActiveProcessAffinityMask;
	};
#ifdef _WIN64
	LPVOID GdiHandleBuffer[64];
#else
	LPVOID GdiHandleBuffer[32];
#endif
	LPVOID PostProcessInitRoutine;
	LPVOID TlsExpansionBitmap;
	DWORD TlsExpansionBitmapBits[32];
	LPVOID SessionId;
	ULARGE_INTEGER AppCompatFlags;
	ULARGE_INTEGER AppCompatFlagsUser;
	LPVOID pShimData;
	LPVOID AppCompatInfo;
	UNICODE_STRING CSDVersion;
	LPVOID ActivationContextData;
	LPVOID ProcessAssemblyStorageMap;
	LPVOID SystemDefaultActivationContextData;
	LPVOID SystemAssemblyStorageMap;
	LPVOID MinimumStackCommit;
} PEB, *PPEB;

#ifndef _KERNEL_MODE
typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
#endif

typedef struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME* PRTL_ACTIVATION_CONTEXT_STACK_FRAME;
typedef struct _ACTIVATION_CONTEXT* PACTIVATION_CONTEXT;
typedef struct _TEB_ACTIVE_FRAME* PTEB_ACTIVE_FRAME;
typedef struct _TEB_ACTIVE_FRAME_CONTEXT* PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME {
	PRTL_ACTIVATION_CONTEXT_STACK_FRAME Previous;
	PACTIVATION_CONTEXT* ActivationContext;
	ULONG Flags;
} RTL_ACTIVATION_CONTEXT_STACK_FRAME, *PRTL_ACTIVATION_CONTEXT_STACK_FRAME;

typedef struct _ACTIVATION_CONTEXT_STACK {
	PRTL_ACTIVATION_CONTEXT_STACK_FRAME ActiveFrame;
	LIST_ENTRY FrameListCache;
	ULONG Flags;
	ULONG NextCookieSequenceNumber;
	ULONG StackId;
} ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;

#ifndef GDI_BATCH_BUFFER_SIZE
#define GDI_BATCH_BUFFER_SIZE 310
#endif	// !GDI_BATCH_BUFFER_SIZE

typedef struct _GDI_TEB_BATCH {
	ULONG Offset;
	ULONG_PTR HDC;
	ULONG Buffer[GDI_BATCH_BUFFER_SIZE];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _TEB_ACTIVE_FRAME_CONTEXT {
	ULONG Flags;
	PSTR FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME {
	ULONG Flags;
	struct _TEB_ACTIVE_FRAME* Previous;
	PTEB_ACTIVE_FRAME_CONTEXT Context;
} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

#if !defined(_MSC_VER)
typedef struct _PROCESSOR_NUMBER {
	USHORT Group;
	UCHAR Number;
	UCHAR Reserved;
} PROCESSOR_NUMBER, *PPROCESSOR_NUMBER;
#endif

typedef struct _TEB {
	NT_TIB NtTib;

	PVOID EnvironmentPointer;
	CLIENT_ID ClientId;
	PVOID ActiveRpcHandle;
	PVOID ThreadLocalStoragePointer;
	PPEB ProcessEnvironmentBlock;

	ULONG LastErrorValue;
	ULONG CountOfOwnedCriticalSections;
	PVOID CsrClientThread;
	PVOID Win32ThreadInfo;
	ULONG User32Reserved[26];
	ULONG UserReserved[5];
	PVOID WOW32Reserved;
	LCID CurrentLocale;
	ULONG FpSoftwareStatusRegister;
	PVOID SystemReserved1[54];
	NTSTATUS ExceptionCode;
	PVOID ActivationContextStackPointer;
#ifdef _M_X64
	UCHAR SpareBytes[24];
#else
	UCHAR SpareBytes[36];
#endif
	ULONG TxFsContext;

	GDI_TEB_BATCH GdiTebBatch;
	CLIENT_ID RealClientId;
	HANDLE GdiCachedProcessHandle;
	ULONG GdiClientPID;
	ULONG GdiClientTID;
	PVOID GdiThreadLocalInfo;
	ULONG_PTR Win32ClientInfo[62];
	PVOID glDispatchTable[233];
	ULONG_PTR glReserved1[29];
	PVOID glReserved2;
	PVOID glSectionInfo;
	PVOID glSection;
	PVOID glTable;
	PVOID glCurrentRC;
	PVOID glContext;

	NTSTATUS LastStatusValue;
	UNICODE_STRING StaticUnicodeString;
	WCHAR StaticUnicodeBuffer[261];

	PVOID DeallocationStack;
	PVOID TlsSlots[64];
	LIST_ENTRY TlsLinks;

	PVOID Vdm;
	PVOID ReservedForNtRpc;
	PVOID DbgSsReserved[2];

	ULONG HardErrorMode;
#ifdef _M_X64
	PVOID Instrumentation[11];
#else
	PVOID Instrumentation[9];
#endif
	GUID ActivityId;

	PVOID SubProcessTag;
	PVOID EtwLocalData;
	PVOID EtwTraceData;
	PVOID WinSockData;
	ULONG GdiBatchCount;

	union {
		PROCESSOR_NUMBER CurrentIdealProcessor;
		ULONG IdealProcessorValue;
		struct {
			UCHAR ReservedPad0;
			UCHAR ReservedPad1;
			UCHAR ReservedPad2;
			UCHAR IdealProcessor;
		} Processor;
	};

	ULONG GuaranteedStackBytes;
	PVOID ReservedForPerf;
	PVOID ReservedForOle;
	ULONG WaitingOnLoaderLock;
	PVOID SavedPriorityState;
	ULONG_PTR SoftPatchPtr1;
	PVOID ThreadPoolData;
	PVOID* TlsExpansionSlots;
#ifdef _M_X64
	PVOID DeallocationBStore;
	PVOID BStoreLimit;
#endif
	ULONG MuiGeneration;
	ULONG IsImpersonating;
	PVOID NlsCache;
	PVOID pShimData;
	ULONG HeapVirtualAffinity;
	HANDLE CurrentTransactionHandle;
	PTEB_ACTIVE_FRAME ActiveFrame;
	PVOID FlsData;

	PVOID PreferredLanguages;
	PVOID UserPrefLanguages;
	PVOID MergedPrefLanguages;
	ULONG MuiImpersonation;

	union {
		USHORT CrossTebFlags;
		USHORT SpareCrossTebBits : 16;
	} CrossTeb;
	union {
		USHORT SameTebFlags;
		struct {
			USHORT SafeThunkCall : 1;
			USHORT InDebugPrint : 1;
			USHORT HasFiberData : 1;
			USHORT SkipThreadAttach : 1;
			USHORT WerInShipAssertCode : 1;
			USHORT RanProcessInit : 1;
			USHORT ClonedThread : 1;
			USHORT SuppressDebugMsg : 1;
			USHORT DisableUserStackWalk : 1;
			USHORT RtlExceptionAttached : 1;
			USHORT InitialThread : 1;
			USHORT SessionAware : 1;
			USHORT SpareSameTebBits : 4;
		} flags;
	};

	PVOID TxnScopeEnterCallback;
	PVOID TxnScopeExitCallback;
	PVOID TxnScopeContext;
	ULONG LockCount;
	ULONG SpareUlong0;
	PVOID ResourceRetValue;
	PVOID ReservedForWdf;
} TEB, *PTEB;