#include "Compare.hh"


double Compare::diff(double p_a, double p_b)
{
	double c = Abs(p_a);
	double d = Abs(p_b);

	d = Max(c, d);

	return d == 0.0 ? 0.0 : Abs(p_a - p_b) / d;
}
