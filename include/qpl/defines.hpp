#pragma once
#ifndef QPL_DEFINES_HPP
#define QPL_DEFINES_HPP

#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)
#define QPL_INTERN_VULKAN_USE
#endif
#if defined (QPL_USE_GLM) || defined(QPL_USE_ALL)
#define QPL_INTERN_GLM_USE
#endif
#if !defined (QPL_NO_SFML) || defined(QPL_USE_ALL)
#define QPL_INTERN_SFML_USE
#endif
#if (defined (QPL_USE_GLEW) && !defined (QPL_NO_SFML)) || defined(QPL_USE_ALL)
#define QPL_INTERN_GLEW_USE
#endif

#if (defined (QPLDLL_EXPORTS) && !defined (QPL_INTERN_GLEW_USE))
#define QPL_INTERN_STATIC_NO_GLEW
#endif

#endif