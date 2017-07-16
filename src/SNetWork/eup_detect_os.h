#ifndef EUP_DETECT_OS_H
#define EUP_DETECT_OS_H

/*!
  \file eup_detect_os.h
  \brief Определение используемой операционнной системы
  \author Костин А. В. п.171
*/

#if defined(_WIN32)
#define EUP_WINDOWS_OS

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define EUP_MSC
#elif defined __CYGWIN__
#define EUP_CYGWIN
#elif defined __MINGW32__
#define EUP_WINDOWS_OS
#endif

#elif defined __QNX__
#define EUP_QNX_OS
#elif defined __linux__
#define EUP_LINUX_OS

#else
// Если ОС не определена,  предположительно это MAC
#define EUP_MAC_OS
#endif


#endif // EUP_DETECT_OS_H
