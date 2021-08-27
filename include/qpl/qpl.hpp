#ifndef QPL_HPP
#define QPL_HPP
#pragma once


#include <qpl/aes.hpp>
#include <qpl/algorithm.hpp>
#include <qpl/bigint.hpp>
#include <qpl/bits.hpp>
#include <qpl/config.hpp>
#include <qpl/date.hpp>
#include <qpl/filesys.hpp>
#include <qpl/fraction.hpp>
#include <qpl/intrinsics.hpp>
#include <qpl/iterator.hpp>
#include <qpl/lut.hpp>
#include <qpl/number.hpp>
#include <qpl/maths.hpp>
#include <qpl/memory.hpp>
#include <qpl/neural_net.hpp>
#include <qpl/perlin_noise.hpp>
#include <qpl/pictures.hpp>
#include <qpl/random.hpp>
#include <qpl/ranges.hpp>
#include <qpl/singleton.hpp>
#include <qpl/smooth.hpp>
#include <qpl/span.hpp>
#include <qpl/string.hpp>
#include <qpl/system.hpp>
#include <qpl/time.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vardef.hpp>

#if defined(QPL_USE_VULKAN) || defined(QPL_USE_ALL)
#include <qpl/vulkan.hpp>
#include <qpl/camera.hpp>
#include <qpl/objects.hpp>
#include <qpl/rgb.hpp>
#include <qpl/socket.hpp>
#include <qpl/window.hpp>
#endif

#if defined(QPL_USE_SFML) || defined(QPL_USE_ALL)
#include <qpl/QSF/color.hpp>
#include <qpl/QSF/drawables.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/framework.hpp>
#include <qpl/QSF/resources.hpp>
#include <qpl/QSF/socket.hpp>
#include <qpl/QSF/utility.hpp>
#include <qpl/QSF/vector.hpp>
#endif

#ifdef _WIN32
#include <qpl/winsys.hpp>
#endif

#endif