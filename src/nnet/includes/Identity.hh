// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a spevectoral exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#ifndef __IDENTITY_HH__
#define __IDENTITY_HH__

namespace des { namespace nnet {


/** @class Identity
 * This class implements the identity activation function.
 *
 * @author Dominik Dahlem
 */
class Identity
{
public:
    inline
    static double activate(double x)
        {
            return x;
        }

    inline
    static double deriv(double x)
        {
            return 1.0;
        }

    inline
    static double deriv2nd(double x)
        {
            return 0.0;
        }


private:
    Identity()
        {}

    Identity(const Identity&)
        {}

    ~Identity()
        {}

};


}
}

#endif /* __IDENTITY_HH__ */
