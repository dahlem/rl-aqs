// Copyright (C) 2010 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

/** @file IntSet.hh
 * Wrapper of the std::set<int> to be able to use it with the boost::dynamic_properties
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_NETWORK_INTSET_HH__
#define __DES_NETWORK_INTSET_HH__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <set>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>


namespace des { namespace network {

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;


class IntSet : public std::set<int>
{
public:
    IntSet() { }

    friend std::ostream &operator<<(std::ostream &stream, IntSet &p_intSet)
        {
            std::set<int>::iterator secondLast = p_intSet.end();
            if (!p_intSet.empty()) {
                secondLast--;
            }
            
            for (std::set<int>::iterator iter = p_intSet.begin(); iter != p_intSet.end(); ++iter) {
                if (iter != secondLast) {
                    stream << *iter << ",";
                } else {
                    stream << *iter;
                }
            }

            return stream;
        }

    friend std::istream &operator>>(std::istream &stream, IntSet &p_intSet)
        {
            boost::char_separator<char> sep(",");
            std::string str;
            getline(stream, str);

            tokenizer tokens(str, sep);

            for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
                try
                {
                    int a = boost::lexical_cast<int>(*tok_iter);
                    p_intSet.insert(a);
                } catch (const boost::bad_lexical_cast& blc) {
                    std::cerr << "ERROR: input format invalid" << std::endl;
                    throw blc;
                }
            }

            return stream;
        }
};

}
}


#endif /* __DES_NETWORK_INTSET_HH__ */
