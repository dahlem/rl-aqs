// Copyright (C) 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
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

    /** @fn double ssd(int p_n, double p_xbar, double p_sd, double p_x)
     * Calculate the running sum of squared deviations. In order to calculate
     * the sample variance divide the SSD by the number of data points, i.e.,
     * \f$ \sigma^2 = ssd / p_n \f$. And consequently the sample standard deviation
     * is the square root of the variance.
     *
     * @param int the running number of data points
     * @param double the running sample mean
     * @param double the running sum of squared deviations
     * @param double the new data point
     * @return double the new sample mean
     */
    static double ssd(int p_n, double p_xbar, double p_sd, double p_x);

private:
    Stats()
        {}

    ~Stats()
        {}

};


    }
}




#endif
