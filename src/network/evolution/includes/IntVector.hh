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

/** @file IntVector.hh
 * Wrapper of the std::vector<int> to be able to use it with the boost::dynamic_properties
 *
 * @author Dominik Dahlem
 */
#ifndef __DES_NETWORK_INTVECTOR_HH__
#define __DES_NETWORK_INTVECTOR_HH__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>


namespace des { namespace network {

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;


class IntVector : public std::vector<int>
{
public:
    IntVector() { }

    friend std::ostream &operator<<(std::ostream &stream, IntVector &p_intVector)
        {
            for (boost::uint16_t i = 0; i < p_intVector.size(); ++i) {
                if (i < (p_intVector.size() - 1)) {
                    stream << p_intVector[i] << ",";
                } else {
                    stream << p_intVector[i];
                }
            }

            return stream;
        }

    friend std::istream &operator>>(std::istream &stream, IntVector &p_intVector)
        {
            boost::char_separator<char> sep(",");
            std::string str;
            getline(stream, str);

            tokenizer tokens(str, sep);

            for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
                try
                {
                    int a = boost::lexical_cast<int>(*tok_iter);
                    p_intVector.push_back(a);
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


#endif /* __DES_NETWORK_INTVECTOR_HH__ */
