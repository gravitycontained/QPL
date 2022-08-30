/*
QPL (Quick Production Library)  - Copyright (c) Daniel Rabl (danielrabl1999@gmail.com). 
This is a C++20 personal utility library with many purposes to help developement.
It uses alot of features from the great SFML library (https://www.sfml-dev.org/) and builds upon that.
Please note that since this a one-man project there can be mistakes in the implementations.
Features like qpl::aes, qpl::huffman_coding and qpl::floating_point<mantissa_bits, exponent_bits>
were implemented for my personal usage. They might have mistakes and might be slow:
I highly recommend installing the feature from a greater source, especially if deployed in a serious environment.


This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

- The origin of this software must not be misrepresented; you must not claim
   that you wrote the original software. If you use this software in a product,
   an acknowledgment in the product documentation would be appreciated but is
   not required.

- Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

- This notice may not be removed or altered from any source distribution.
*/

#ifndef QPL_HPP
#define QPL_HPP
#pragma once

#include <qpl/defines.hpp>

#include <qpl/algorithm.hpp>
#include <qpl/bits.hpp>
#include <qpl/compression.hpp>
#include <qpl/camera.hpp>
#include <qpl/config.hpp>
#include <qpl/codec.hpp>
#include <qpl/color.hpp>
#include <qpl/date.hpp>
#include <qpl/encryption.hpp>
#include <qpl/exception.hpp>
#include <qpl/filesys.hpp>
#include <qpl/fraction.hpp>
#include <qpl/intrinsics.hpp>
#include <qpl/iterator.hpp>
#include <qpl/lut.hpp>
#include <qpl/number.hpp>
#include <qpl/maths.hpp>
#include <qpl/memory.hpp>
#include <qpl/obfuscate.hpp>
#include <qpl/neural_net.hpp>
#include <qpl/perlin_noise.hpp>
#include <qpl/pictures.hpp>
#include <qpl/random.hpp>
#include <qpl/smooth.hpp>
#include <qpl/string.hpp>
#include <qpl/system.hpp>
#include <qpl/time.hpp>
#include <qpl/type_traits.hpp>
#include <qpl/vardef.hpp>
#include <qpl/vector.hpp>

#if defined(QPL_INTERN_VULKAN_USE)
#include <qpl/VK/vulkan.hpp>
#include <qpl/VK/objects.hpp>
#include <qpl/VK/window.hpp>
#endif

#if defined(QPL_INTERN_SFML_USE)
#include <qpl/QSF/drawables.hpp>
#include <qpl/QSF/event_info.hpp>
#include <qpl/QSF/framework.hpp>
#include <qpl/QSF/resources.hpp>
#include <qpl/QSF/socket.hpp>
#include <qpl/QSF/utility.hpp>
#include <qpl/QSF/view.hpp>

#include <qpl/QGL/attributes.hpp>
#include <qpl/QGL/glew.hpp>
#include <qpl/QGL/shader.hpp>
#include <qpl/QGL/shapes.hpp>
#include <qpl/QGL/vertex.hpp>
#endif

#ifdef _WIN32
#include <qpl/winsys.hpp>
#include <qpl/winsock.hpp>
#endif

#endif