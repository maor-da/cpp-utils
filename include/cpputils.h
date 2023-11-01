#pragma once

#if defined(CPP_UTILS_API_EXPORT)
#define CPP_UTILS_API __declspec(dllexport)
#else
#define CPP_UTILS_API __declspec(dllimport)
#endif

#include <cpp-utils/Crypt.h>
#include <cpp-utils/Logger.h>
#include <cpp-utils/TypeHelpers.h>
#include <cpp-utils/Process.h>
#ifdef _WIN32
#include <cpp-utils/Winutil.h>
#endif
