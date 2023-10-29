#include <cpp-utils/internals/PE.h>

template <>
bool PE<PROCESS_TYPE::Native>::parse()
{
	do {
		m_DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(m_Addr);
		if (m_DosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
			break;
		}

		m_NtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(m_Addr + m_DosHeader->e_lfanew);

		if (m_NtHeaders->Signature != IMAGE_NT_SIGNATURE) {
			break;
		}

		m_Sections = IMAGE_FIRST_SECTION(m_NtHeaders);

		return true;
	} while (false);

	return false;
}

template <>
bool PE<PROCESS_TYPE::WOW>::parse()
{
	do {
		m_DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(m_Addr);
		if (m_DosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
			break;
		}

		m_NtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS32>(m_Addr + m_DosHeader->e_lfanew);

		if (m_NtHeaders->Signature != IMAGE_NT_SIGNATURE) {
			break;
		}

		m_Sections = IMAGE_FIRST_SECTION(m_NtHeaders);

		return true;
	} while (false);

	return false;
}