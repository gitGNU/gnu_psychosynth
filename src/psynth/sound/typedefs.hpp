/**
 *  Time-stamp:  <2010-11-02 12:38:49 raskolnikov>
 *
 *  @file        typedefs.hpp
 *  @author      Juan Pedro Bolivar Puente <raskolnikov@es.gnu.org>
 *  @date        Tue Oct 19 13:04:44 2010
 *
 *  Useful typedefs.
 *
 */

/*
 *  Copyright (C) 2010 Juan Pedro Bolivar Puente
 *
 *  This file is part of Psychosynh.
 *   
 *  Psychosynh is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Psychosynh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 *  Copyright 2005-2007 Adobe Systems Incorporated
 * 
 *  Use, modification and distribution are subject to the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt).
 */

#ifndef PSYNTH_SOUND_TYPEDEFS_H
#define PSYNTH_SOUND_TYPEDEFS_H

#include <boost/cstdint.hpp>
#include <memory>

#include <psynth/base/compat.hpp>
#include <psynth/sound/mono.hpp>
#include <psynth/sound/stereo.hpp>
#include <psynth/sound/quad.hpp>
#include <psynth/sound/surround.hpp>
#include <psynth/sound/device_n.hpp>

// CS = 'bgr' LAYOUT='bgr_layout_t'
#define PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS_INTERNAL(T,CS,LAYOUT)			\
    template <typename, typename>    struct frame;			\
    template <typename, typename>    struct planar_frame_reference;	\
    template <typename, typename>    struct planar_frame_iterator;	\
    template <typename>              class memory_based_step_iterator;	\
    template <typename>              class buffer_view;			\
    template <typename, bool, typename>     class buffer;		\
    typedef frame<bits##T, LAYOUT >         CS##T##_frame;		\
    typedef const frame<bits##T, LAYOUT >   CS##T##c_frame;		\
    typedef frame<bits##T, LAYOUT >&        CS##T##_ref;		\
    typedef const frame<bits##T, LAYOUT >&  CS##T##c_ref;		\
    typedef CS##T##_frame_t*                CS##T##_ptr;		\
    typedef CS##T##c_frame_t*               CS##T##c_ptr;		\
    typedef memory_based_step_iterator<CS##T##_ptr>			\
    CS##T##_step_ptr;							\
    typedef memory_based_step_iterator<CS##T##c_ptr>			\
    CS##T##c_step_ptr;							\
    typedef buffer_view<CS##T##_ptr>            CS##T##_view;		\
    typedef buffer_view<CS##T##c_ptr>           CS##T##c_view;		\
    typedef buffer_view<CS##T##_step_ptr>       CS##T##_step_view;	\
    typedef buffer_view<CS##T##c_step_ptr>      CS##T##c_step_view;	\
    typedef buffer<CS##T##_frame_t,false,std::allocator<unsigned char> > \
    CS##T##_buffer;

// CS = 'bgr' CS_FULL = 'rgb_t' LAYOUT='bgr_layout_t'
#define PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(T,CS,CS_FULL,LAYOUT)	\
    PSYNTH_DEFINE_BASE_TYPEDEFS_INTERNAL(T,CS,LAYOUT)			\
    typedef planar_frame_reference<bits##T&,CS_FULL >                   \
    CS##T##_planar_ref;							\
    typedef planar_frame_reference<const bits##T&,CS_FULL >             \
    CS##T##c_planar_ref;						\
    typedef planar_frame_iterator<bits##T*,CS_FULL >                    \
    CS##T##_planar_ptr;							\
    typedef planar_frame_iterator<const bits##T*,CS_FULL >              \
    CS##T##c_planar_ptr;						\
    typedef memory_based_step_iterator<CS##T##_planar_ptr_t>            \
    CS##T##_planar_step_ptr;						\
    typedef memory_based_step_iterator<CS##T##c_planar_ptr_t>           \
    CS##T##c_planar_step_ptr;						\
    typedef buffer_view<CS##T##_planar_ptr>				\
    CS##T##_planar_view;						\
    typedef buffer_view<CS##T##c_planar_ptr>				\
    CS##T##c_planar_view;						\
    typedef buffer_view<CS##T##_planar_step_ptr>			\
    CS##T##_planar_step_view;						\
    typedef buffer_view<CS##T##c_planar_step_ptr>			\
    CS##T##c_planar_step_view;					\
    typedef buffer<CS##T##_frame_t,true,std::allocator<unsigned char> >  \
    CS##T##_planar_buffer_t;    

#define PSYNTH_DEFINE_BASE_TYPEDEFS(T,CS)        \
    PSYNTH_DEFINE_BASE_TYPEDEFS_INTERNAL(T,CS,CS##_layout)

#define PSYNTH_DEFINE_ALL_TYPEDEFS(T,CS)         \
    PSYNTH_DEFINE_ALL_TYPEDEFS_INTERNAL(T,CS,CS##_t,CS##_layout)

namespace psynth
{
namespace sound
{

// forward declarations
template <typename B, typename Mn, typename Mx> struct scoped_sample_value;
struct float_zero;
struct float_one;
typedef scoped_sample_value<float,float_zero,float_one> bits32f;
typedef uint8_t  bits8;
typedef uint16_t bits16;
typedef uint32_t bits32;
typedef int8_t   bits8s;
typedef int16_t  bits16s;
typedef int32_t  bits32s;

PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8,  mono)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8s, mono)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16, mono)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16s,mono)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32 ,mono)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32s,mono)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32f,mono)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8,  stereo)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8s, stereo)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16, stereo)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16s,stereo)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32 ,stereo)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32s,stereo)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32f,stereo)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8,  quad)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8s, quad)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16, quad)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16s,quad)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32, quad)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32s,quad)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32f,quad)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8,  surround)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(8s, surround)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16, surround)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(16s,surround)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32 ,surround)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32s,surround)
PSYNTH_SOUND_DEFINE_BASE_TYPEDEFS(32f,surround)

PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(8,  stereo)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(8s, stereo)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(16, stereo)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(16s,stereo)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32 ,stereo)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32s,stereo)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32f,stereo)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(8,  quad)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(8s, quad)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(16, quad)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(16s,quad)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32 ,quad)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32s,quad)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32f,quad)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(8,  surround)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(8s, surround)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(16, surround)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(16s,surround)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32 ,surround)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32s,surround)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS(32f,surround)


template <int N> struct devicen_space;
template <int N> struct devicen_layout;
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8,  dev2n, devicen_space<2>, devicen_layout<2>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8s, dev2n, devicen_space<2>, devicen_layout<2>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16, dev2n, devicen_space<2>, devicen_layout<2>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16s,dev2n, devicen_space<2>, devicen_layout<2>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32 ,dev2n, devicen_space<2>, devicen_layout<2>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32s,dev2n, devicen_space<2>, devicen_layout<2>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32f,dev2n, devicen_space<2>, devicen_layout<2>)

PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8,  dev3n, devicen_space<3>, devicen_layout<3>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8s, dev3n, devicen_space<3>, devicen_layout<3>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16, dev3n, devicen_space<3>, devicen_layout<3>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16s,dev3n, devicen_space<3>, devicen_layout<3>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32 ,dev3n, devicen_space<3>, devicen_layout<3>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32s,dev3n, devicen_space<3>, devicen_layout<3>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32f,dev3n, devicen_space<3>, devicen_layout<3>)

PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8,  dev4n, devicen_space<4>, devicen_layout<4>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8s, dev4n, devicen_space<4>, devicen_layout<4>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16, dev4n, devicen_space<4>, devicen_layout<4>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16s,dev4n, devicen_space<4>, devicen_layout<4>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32 ,dev4n, devicen_space<4>, devicen_layout<4>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32s,dev4n, devicen_space<4>, devicen_layout<4>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32f,dev4n, devicen_space<4>, devicen_layout<4>)

PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8,  dev5n, devicen_space<5>, devicen_layout<5>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(8s, dev5n, devicen_space<5>, devicen_layout<5>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16, dev5n, devicen_space<5>, devicen_layout<5>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(16s,dev5n, devicen_space<5>, devicen_layout<5>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32 ,dev5n, devicen_space<5>, devicen_layout<5>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32s,dev5n, devicen_space<5>, devicen_layout<5>)
PSYNTH_SOUND_DEFINE_ALL_TYPEDEFS_INTERNAL(32f,dev5n, devicen_space<5>, devicen_layout<5>)

} /* namespace sound */
} /* namespace psynth */

#endif /* PSYNTH_SOUND_TYPEDEFS */