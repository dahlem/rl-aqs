// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Vector.hh
 * Header file with the declaration of utility methods for vector operations
 */
#ifndef VECTOR_HH
#define VECTOR_HH


#include <boost/shared_array.hpp>


typedef boost::shared_array <double> DoubleSA;


namespace des
{
    namespace utils
    {



/**
 * This class provides vector utilities, such as the dot product
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Vector
{
public:
    /** @fn double dotproduct(int, double*, double*)
     * This function calculates the dotproduct given two vectors.
     *
     * @param int the length of the vectors
     * @param DoubleSA the left vector
     * @param DoubleSA the right vector
     */
    static double dotproduct(int, DoubleSA, DoubleSA);


    /** @ double mult(int, DoubleSA, double)
     * Multiply a scalar component-wise.
     *
     * @param int the length of the vectors
     * @param DoubleSA the left vector
     * @param double the scalar to multiply the vector with
     */
    static void mult(int, DoubleSA, double);

private:

    Vector()
        {}

    ~Vector()
        {}

};


    }
}


#endif
