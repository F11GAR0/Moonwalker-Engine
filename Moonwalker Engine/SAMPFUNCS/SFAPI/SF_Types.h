#pragma once

#ifndef __SF_TYPES_H__
#define __SF_TYPES_H__

typedef signed char							int8_t;
typedef unsigned char						uint8_t;
typedef signed short						int16_t;
typedef unsigned short						uint16_t;
typedef signed int							int32_t;
typedef unsigned int						uint32_t;
typedef signed long long					int64_t;
typedef unsigned long long					uint64_t;
typedef const signed char					cchar;
typedef const unsigned char					cuchar;
typedef const signed short					csort;
typedef const unsigned short				cushort;
typedef const signed int					cint;
typedef const unsigned int					cuint;
typedef const signed long long				clong;
typedef const unsigned long long			culong;

template <typename T>
struct rect
{
	T										_left;
	T										_top;
	T										_right;
	T										_bottom;

	rect(T left, T top, T right, T bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {}
	rect(T v) : _left(v), _top(v), _right(v), _bottom(v) {}
	rect(void) : rect(0, 0, 0, 0) {}
};
typedef rect<float>							frect;

#endif
