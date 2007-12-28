/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) Juan Pedro Bolivar Puente 2007                          *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifndef VECTOR2D_H
#define VECTOR2D_H

/*
 * TODO: Fast initial implementation. Add more functionality.
 */
template<class T>
class Vector2D
{
    static const int NUM_D = 2;
public:
    T x;
    T y;

    Vector2D(T _x = 0, T _y = 0) :
	x(_x), y(_y)
	{}

    void set(T _x, T _y) {
	x = _x;
	y = _y;
    }
    
    T& operator[](size_t index) {
	return *(&x + index);
    }

    const T& operator[](size_t index) const {
	return *(&x + index);
    }
    
    T distance(const Vector2D& v) const {
	return sqrt(sqrDistance(v));
    }

    T sqrDistance(const Vector2D& v) const {
	return
	    (v.x-x) * (v.x-x) +
	    (v.y-y) * (v.y-y);
    }

    T length() const {
	return sqrt(x*x + y*y);
    }

    T sqrLength() const {
	return x*x + y*y;
    }

};

typedef Vector2D<float> Vector2f;
    
#endif
