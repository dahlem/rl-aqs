// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file CI.hh
 * Header file with the declaration of confidence intervals with a specified
 * precision.
 */
#ifndef CI_HH
#define CI_HH


namespace des
{
    namespace statistics
    {


/**
 * This class calculates confidence intervals given a set of <code>double</code>
 * values.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class CI
{
public:
    /**
     * This method calculates the confidence interval with an adjusted relative
     * error given the data vector provided. It returns a boolean value indicating
     * whether the data is within a fixed confidence level of
     * \f$ 100(1 - \alpha) \f$ percent and a relative error of
     * \f$ \gamma (0 < \gamma < 1) \f$. The formula is given as
     * \f[
     * \delta(n, \alpha) = t_{n-1,1-\alpha/2}\sqrt{S^2(n)/n}
     * \f] evaluating to true, iff
     * \f[
     * \delta(n, \alpha) / \abs{\bar{X}(n)} \leq \gamma\prime
     * \f]
     *
     * @param double[] the data vector
     * @param int the size of the data vector
     * @param double the alpha level
     * @param double the error level
     * @return true, if the given data is within the confidence interval and within
     *         a given error. Otherwise, false.
     */
    static bool isConfidentWithPrecision(
        const double p_data[],
        const int p_size,
        double p_alpha,
        double p_error);

private:
    CI()
        {}

    ~CI()
        {}

};


    }
}




#endif
