// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#ifndef __HTANGENT_HH__
#define __HTANGENT_HH__

#include <cmath>

namespace des { namespace nnet {


/** @class HTangent
 * This class implements the hyperbolic tangent activation function.
 *
 * @author Dominik Dahlem
 */
class HTangent
{
public:
    inline
    static double activate(double x)
        {
            return tanh(0.5 * x);
        }

    inline
    static double deriv(double x)
        {
            double temp = (1 / cosh(x));

            return temp * temp;
        }

    inline
    static double deriv2nd(double x)
        {
            double temp = (1 / cosh(x));

            return -2 * temp * temp * tanh(x);
        }


private:
    HTangent()
        {}

    HTangent(const HTangent&)
        {}

    ~HTangent()
        {}

};


}
}

#endif /* __HTANGENT_HH__ */
