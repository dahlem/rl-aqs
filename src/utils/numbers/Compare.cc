// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include "Compare.hh"
using des::utils::Compare;



double Compare::diff(double p_a, double p_b)
{
	double c = Abs(p_a);
	double d = Abs(p_b);

	d = Max(c, d);

	return d == 0.0 ? 0.0 : Abs(p_a - p_b) / d;
}
