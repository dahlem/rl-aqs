#ifndef COMPARE_HH
#define COMPARE_HH


#define Abs(x)    ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))


class Compare 
{
private:
    Compare() 
        {};
    ~Compare() 
        {};
    
public:
    static double diff(double p_a, double p_b);
};


#endif
