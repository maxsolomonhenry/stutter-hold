/*
  ==============================================================================

    Windows.cpp
    Created: 13 Mar 2021 1:53:00pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#include "Windows.h"

// Generate hann window of length len.
bst::vector<float> hann(int len)
{
    bst::vector<float> window (len);
    float N = static_cast<float>(len);
    for (int n = 0; n < len; n++)
    {
        float fn = static_cast<float>(n);
        window(n) = pow(sin(M_PI * fn / N), 2.0f);
    }
    return window;
}

// Generate hamming window of length len.
bst::vector<float> hamming(int len)
{
    bst::vector<float> window (len);
    float N = static_cast<float>(len-1);
    float a_0 = 0.54;
    float a_1 = 1.0 - a_0;
    for (int n = 0; n < N; n++)
    {
        float fn = static_cast<float>(n);
        window(n) = a_0 + a_1 * cos((2.0 * M_PI * fn / N) + M_PI);
    }
    window(len-1) = window(0);
    return window;
}
