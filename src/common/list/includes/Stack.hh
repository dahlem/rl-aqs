// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file Stack.hh
 * Specification of a simple stack
 */
#ifndef __DESCOMMONSTACK_HH__
#define __DESCOMMONSTACK_HH__


#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef __STDC_CONSTANT_MACROS
# define __STDC_CONSTANT_MACROS
#endif /* __STDC_CONSTANT_MACROS */

#include <boost/cstdint.hpp>

#include "StackException.hh"


namespace des
{
namespace common
{


/** @class Stack
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
 */
template <typename T>
class Stack
{
public:
    Stack();
    explicit Stack(int size);
    explicit Stack(const Stack &p_stack);

    ~Stack();

    void push(T elem);
    T top() throw (StackException);
    T pop() throw (StackException);
    boost::uint16_t size() const;
    bool empty();

private:

    Stack& operator=(const Stack&) {  }

    boost::uint16_t m_size;
    boost::uint16_t m_next;

    T *array;

};


template <typename T>
Stack<T>::Stack()
    : m_size(5), m_next(0)
{
    array = new T[m_size];
}

template <typename T>
Stack<T>::~Stack()
{
    if (array != NULL) {
        delete [] array;
    }
}

template <typename T>
Stack<T>::Stack(int size)
    : m_size(size), m_next(0)
{
    // adjust the size, if <= 0
    m_size = (m_size > 0) ? m_size : 5;
    array = new T[m_size];
}

template <typename T>
Stack<T>::Stack(const Stack<T> &p_stack)
    : m_size(p_stack.m_size), m_next(p_stack.m_next)
{
    array = new T[m_size];

    for (boost::uint16_t i = 0; i < m_size; ++i) {
        array[i] = p_stack.array[i];
    }
}

template <typename T>
void Stack<T>::push(T elem)
{
    if (m_next >= m_size) {
        // resize the array
        boost::uint16_t newSize = 2 * m_size;

        T *newArray = new T[newSize];

        // copy element-wise
        for (boost::uint16_t i = 0; i < m_size; ++i) {
            newArray[i] = array[i];
        }

        delete [] array;
        array = newArray;
        m_size = newSize;
    }

    array[m_next] = elem;
    m_next++;
}

template <typename T>
T Stack<T>::top() throw (StackException)
{
    if (m_next <= 0) {
        throw StackException(StackException::ERROR_EMPTY);
    } else {
        return array[m_next - 1];
    }
}

template <typename T>
T Stack<T>::pop() throw (StackException)
{
    if (m_next <= 0) {
        throw StackException(StackException::ERROR_EMPTY);
    } else {
        m_next--;
        return array[m_next];
    }
}

template <typename T>
boost::uint16_t Stack<T>::size() const
{
    return m_next;
}

template <typename T>
bool Stack<T>::empty()
{
    return (m_next > 0) ? false : true;
}


}
}



#endif
