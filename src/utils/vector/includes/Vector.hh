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
     * @param double* the left vector
     * @param double* the right vector
     */
    static double dotproduct(int, double*, double*);

private:

    Vector()
        {}

    ~Vector()
        {}

};


    }
}


#endif
