#ifndef ENTRY_HH
#define ENTRY_HH

struct entry_t
{
    double arrival;
    int destination;
    int origin;
    int type;

    entry_t(double a, int d, int o, int t)
        {
            arrival = a;
            destination = d;
            origin = o;
            type = t;
        }
    
};

#endif
