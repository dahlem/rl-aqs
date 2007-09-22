// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef COMPARE_HH
#define COMPARE_HH


namespace des
{
    namespace utils
    {

        
/**
 * Calculate the absolute value
 */
#define Abs(x)    ((x) < 0 ? -(x) : (x))

/**
 * Calculate the maximum value
 */
#define Max(a, b) ((a) > (b) ? (a) : (b))


        
/**
 * This class compares two <code>double</code> values and returns 0, if they are
 * equal, otherwise the difference.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Compare
{
private:
    Compare()
        {};
    ~Compare()
        {};

public:

    /**
     * @param double the left-hand value
     * @param double the right-hand value
     * @return the difference of the two double values
     */
    static double diff(double p_a, double p_b);
};

    }
}

#endif
