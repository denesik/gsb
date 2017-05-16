#ifndef _GSB_CONFIGURE_H_
#define _GSB_CONFIGURE_H_

// MSVC++ 4.x  _MSC_VER == 1000
// MSVC++ 5.0  _MSC_VER == 1100
// MSVC++ 6.0  _MSC_VER == 1200
// MSVC++ 7.0  _MSC_VER == 1300
// MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
// MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
// MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 14.1 _MSC_VER == 1910 (Visual Studio 2017)

#define GSB_CONF_PPCAT_NX(A, B) A ## B
#define GSB_CONF_PPCAT(A, B) GSB_CONF_PPCAT_NX(A, B)

#define GSB_CONF_STRINGIZE_NX(A) #A
#define GSB_CONF_STRINGIZE(A) GSB_CONF_STRINGIZE_NX(A)

#if (defined(_MSC_PLATFORM_TOOLSET) && (_MSC_PLATFORM_TOOLSET == 'v140'))
#define GSB_CONF_VER msvc15_x86
#endif
#if (defined(_MSC_PLATFORM_TOOLSET) && (_MSC_PLATFORM_TOOLSET == 'v141'))
#define GSB_CONF_VER msvc17_x86
#endif

#ifdef CORRADE_IS_DEBUG_BUILD
#define GSB_CONF_DEBUG -d
#else
#define GSB_CONF_DEBUG 
#endif

#define GSB_CONF_P1 ../3rdparty/
#define GSB_CONF_P2 /bin/magnum
#define GSB_CONF_P3 /importers

constexpr char *MAGNUM_PLUGINS_IMPORTER_DIR = GSB_CONF_STRINGIZE(GSB_CONF_PPCAT(GSB_CONF_P1, GSB_CONF_PPCAT(GSB_CONF_VER, GSB_CONF_PPCAT(GSB_CONF_P2, GSB_CONF_PPCAT(GSB_CONF_DEBUG, GSB_CONF_P3)))));

#undef  GSB_CONF_PPCAT_NX
#undef  GSB_CONF_PPCAT
#undef  GSB_CONF_STRINGIZE_NX
#undef  GSB_CONF_STRINGIZE
#undef  GSB_CONF_VER
#undef  GSB_CONF_DEBUG
#undef  GSB_CONF_P1
#undef  GSB_CONF_P2
#undef  GSB_CONF_P3

#endif
