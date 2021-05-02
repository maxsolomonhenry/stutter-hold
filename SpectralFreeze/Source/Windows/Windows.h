/*
  ==============================================================================

    Windows.h
    Created: 12 Mar 2021 3:33:36pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/assignment.hpp>

namespace bst = boost::numeric::ublas;

// Generate hann window of length len.
bst::vector<float> hann(int len);
// Generate hamming window of length len.
bst::vector<float> hamming(int len);
