#ifndef ENTRY_HH
#define ENTRY_HH


namespace des 
{
    namespace common
    {
        

/**
 * This structure specifies an (event) entry for the priority queue.
 * An event is specified by the arrival time, the destination, the origin, and
 * a type of the event.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
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

    }
}

        
#endif
