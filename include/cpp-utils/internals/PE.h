#pragma once

#include <stdint.h>

#include "PEB.h"

enum class SYSTEM_TYPE {
	x64,
	x86
};
enum class PROCESS_TYPE : int {
	Native,
	WOW
};

template <PROCESS_TYPE T>
struct _PTR {
	using type = size_t;
};

template <>
struct _PTR<PROCESS_TYPE::WOW> {
	using type = DWORD;
};

template <PROCESS_TYPE T = PROCESS_TYPE::Native>
using ptr_t = _PTR<T>::type;

template <bool _Test, class _Ty = void, class _Ty2 = void>
struct enable_if_else {
	using type = _Ty2;	// type is _Ty for !_Test
};

template <class _Ty, class _Ty2>
struct enable_if_else<true, _Ty, _Ty2> {  // type is _Ty for _Test
	using type = _Ty;
};

template <bool _Test, class _Ty = void, class _Ty2 = void>
using enable_if_else_t = typename enable_if_else<_Test, _Ty, _Ty2>::type;

template <PROCESS_TYPE T = PROCESS_TYPE::Native>
class PE
{
public:
	PE() {}
	PE(size_t ptr);

	inline ptr_t<> get_end_address();

	PIMAGE_DOS_HEADER get_dos_header();

	PIMAGE_NT_HEADERS get_nt_header();

	PIMAGE_SECTION_HEADER get_section(ptr_t<> addr);

	size_t get_section_empty_chunk_size(PIMAGE_SECTION_HEADER section);

	ptr_t<> find_near_empty_chunk(ptr_t<> addr, size_t size);

	ptr_t<> find_empty_chunk(size_t size, uint32_t protection);

	ptr_t<> find_and_commit_empty_chunk(HANDLE process, size_t size, uint32_t protection);

	inline bool valid_address(ptr_t<> addr);

	PIMAGE_DATA_DIRECTORY get_directory(int directoryId);

	ptr_t<> operator()();

private:
	ptr_t<> m_Addr																					= 0;
	PIMAGE_DOS_HEADER m_DosHeader																	= nullptr;
	PIMAGE_SECTION_HEADER m_Sections																= nullptr;
	enable_if_else_t<T == PROCESS_TYPE::Native, PIMAGE_NT_HEADERS, PIMAGE_NT_HEADERS32> m_NtHeaders = nullptr;

	bool parse();
};

template <PROCESS_TYPE T>
PE<T>::PE(ptr_t<> ptr)
{
	m_Addr = ptr;

	parse();
}

template <PROCESS_TYPE T>
inline ptr_t<> PE<T>::get_end_address()
{
	return m_Addr + m_NtHeaders->OptionalHeader.SizeOfImage;
}

template <PROCESS_TYPE T>
PIMAGE_DOS_HEADER PE<T>::get_dos_header()
{
	return m_DosHeader;
}

template <PROCESS_TYPE T>
PIMAGE_NT_HEADERS PE<T>::get_nt_header()
{
	return m_NtHeaders;
}

template <PROCESS_TYPE T>
PIMAGE_SECTION_HEADER PE<T>::get_section(ptr_t<> addr)
{
	if (!valid_address(addr)) {
		return nullptr;
	}

	auto alignment	   = m_NtHeaders->OptionalHeader.SectionAlignment;
	auto alignmentMask = ptr_t<>(-1) - (ptr_t<>(-1) % alignment);

	for (int i = 0; i < m_NtHeaders->FileHeader.NumberOfSections; i++) {
		auto section		 = (m_Sections + i);
		ptr_t<> sectionStart = m_Addr + section->VirtualAddress;
		ptr_t<> sectionEnd	 = m_Addr + section->VirtualAddress + section->Misc.VirtualSize;
		sectionEnd			 = (sectionEnd & alignmentMask) + alignment;

		if (addr >= sectionStart && addr < sectionEnd) {
			return section;
		}
	}

	return nullptr;
}

template <PROCESS_TYPE T>
size_t PE<T>::get_section_empty_chunk_size(PIMAGE_SECTION_HEADER section)
{
	auto alignment = m_NtHeaders->OptionalHeader.SectionAlignment;

	return alignment - (section->Misc.VirtualSize % alignment);
}

template <PROCESS_TYPE T>
ptr_t<> PE<T>::find_near_empty_chunk(ptr_t<> addr, size_t size)
{
	do {
		if (!addr && !valid_address(addr)) {
			break;
		}

		auto curSection = get_section(addr);
		if (!curSection) {
			// The addr is not in a section!
			break;
		}

		if (size > get_section_empty_chunk_size(curSection)) {
			// Not sufficient size
			break;
		}

		return m_Addr + curSection->VirtualAddress + curSection->Misc.VirtualSize;

	} while (false);

	return NULL;
}

template <PROCESS_TYPE T>
ptr_t<> PE<T>::find_empty_chunk(size_t size, uint32_t protection)
{
	// IMAGE_SCN_MEM_WRITE
	do {
		for (int i = 0; i < m_NtHeaders->FileHeader.NumberOfSections; i++) {
			auto section = (m_Sections + i);
			if (protection && !(section->Characteristics & protection)) {
				continue;
			}
			if (size <= get_section_empty_chunk_size(section)) {
				// TODO: check the chunk is empty e.g. xor with 0
				return m_Addr + section->VirtualAddress + section->Misc.VirtualSize;
			}
		}

		ptr_t<> headerPageSpace	 = ptr_t<>(m_Sections + m_NtHeaders->FileHeader.NumberOfSections);
		auto headerPageSpaceSize = m_NtHeaders->OptionalHeader.SectionAlignment -
								   (ptr_t<>)headerPageSpace % m_NtHeaders->OptionalHeader.SectionAlignment;

		if (headerPageSpaceSize >= size) {
			// TODO: check the chunk is empty e.g. xor with 0
			return headerPageSpace;
		}
		// Find another places for sufficient size

	} while (false);

	return NULL;
}

template <PROCESS_TYPE T>
inline bool PE<T>::valid_address(ptr_t<> addr)
{
	return addr >= m_Addr && addr < get_end_address();
}

template <PROCESS_TYPE T>
PIMAGE_DATA_DIRECTORY PE<T>::get_directory(int directoryId)
{
	// IMAGE_DIRECTORY_ENTRY_IMPORT
	return &m_NtHeaders->OptionalHeader.DataDirectory[directoryId];
}

template <PROCESS_TYPE T>
ptr_t<> PE<T>::operator()()
{
	return m_Addr;
}