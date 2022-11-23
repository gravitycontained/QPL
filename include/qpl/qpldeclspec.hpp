#ifndef QPL_DECLSPEC_HPP
#define QPL_DECLSPEC_HPP
#pragma once

#ifndef QPLDLL
#ifdef QPLDLL_EXPORTS
#define QPLDLL __declspec(dllexport)
#else
#define QPLDLL __declspec(dllimport)
#endif
#endif


#endif
