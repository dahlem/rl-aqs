// Copyright (C) 2007-2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <cstddef>

#include <iostream>
using std::getline;

#include <string>
using std::string;

#include "Seeds.hh"
using des::sampling::Seeds;


Seeds::Seeds() 
{}

Seeds::Seeds(const Seeds&)
{}

Seeds::~Seeds() 
{
    if (is != NULL) {
        if (is.is_open()) {
            is.close();
        }
    }
}

void Seeds::init(const char *p_file) throw (SamplingException)
{
    if (!is.is_open()) {
        is.open(p_file);

        if (!is.is_open()) {
            throw SamplingException(SamplingException::NO_SEEDS_FILE);
        }
    }
}

const unsigned long Seeds::getSeed() throw (SamplingException)
{
    string line;
    unsigned long seed = 0;
    
    if (is.is_open()) {
        if (!is.eof()) {
            getline(is, line);
            try {
                seed = lexical_cast<unsigned long>(line);
            } catch (bad_lexical_cast&) {
                throw SamplingException(SamplingException::NOT_A_NUMBER);
            }
        } else {
            throw SamplingException(SamplingException::NO_SEEDS);
        }
    } else {
        throw SamplingException(SamplingException::NOT_INITIALISED);
    }

    return seed;
}
