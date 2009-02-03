// Copyright (C) 2008, 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

/** @file Stats.hh
 * Header file with the declaration of standard statistics functions.
 */
#ifndef __STATS_HH__
#define __STATS_HH__


namespace des
{
    namespace statistics
    {


/**
 * This class calculates sample mean and sum of squared deviation
 * using Welford's one-pass algorithm.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
class Stats
{
public:
    /** @fn double mean(int p_n, double p_xbar, double p_x)
     * Calculate the sample mean in a one-pass recursive manner using
     * Welford's algorithm.
     *
     * @param int the running number of data points
     * @param double the running sample mean
     * @param double the new data point
     * @return double the new sample mean
     */
    static double mean(int p_n, double p_xbar, double p_x);

    /** @fn double variance(double p_xbar_old, double p_xbar_new, double p_var, double p_x)
     * Calculate the running variance. In order to calculate
     * the sample variance divide the variance by the number of data points minus one, i.e.,
     * \f$ sample \sigma^2 = \sigma^2 / (p_n - 1) \f$. And consequently the sample standard deviation
     * is the square root of the variance.
     *
     * @param double the old running sample mean
     * @param double the new running sample mean
     * @param double the running variance
     * @param double the new data point
     * @return double the new variance
     */
    static double variance(double p_xbar_old, double p_xbar_new, double p_var, double p_x);


    /** @fn bool isAbove(const double, const double, const int, const double)
     * Calculate whether a datum is within a given half-interval from the mean. The
     * method uses the t-distribution to account for broader tails when the sample size
     * is small.
     *
     * @param const double the new datum
     * @param const double the mean
     * @param const double the sample variance
     * @param const int the size of the sample
     * @param const double the alpha value
     */
    static bool isAbove(const double, const double, const double, const int, const double);
    static bool isBelow(const double, const double, const double, const int, const double);


private:
    Stats()
        {}

    ~Stats()
        {}

};


    }
}




#endif
