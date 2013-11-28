// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#ifndef __LOGISTIC_HH__
#define __LOGISTIC_HH__

#include <cmath>

namespace des { namespace nnet {


/** @class Logistic
 * This class implements the logistic activation function.
 *
 * @author Dominik Dahlem
 */
class Logistic
{
public:
    inline
    static double activate(double x)
        {
            return 1 / (1 + exp(-x));
        }

    inline
    static double deriv(double x)
        {
            return activate(x) * (1 - activate(x));
        }

    inline
    static double deriv2nd(double x)
        {
            return 0.0;
        }


private:
    Logistic()
        {}

    Logistic(const Logistic&)
        {}

    ~Logistic()
        {}

};


}
}

#endif /* __LOGISTIC_HH__ */
