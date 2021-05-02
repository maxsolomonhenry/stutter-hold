/*
  ==============================================================================

    JVFreezer.h
    Created: 6 Apr 2021 5:00:57pm
    Author:  Julian Vanasse

        Spectral Freeze processor.
 
        When is_freeze_active is set to true, then oscillates last filled
        buffer using cumulative phase advance.
 
  ==============================================================================
*/

#pragma once

#include <complex>

#include <boost/numeric/ublas/vector.hpp>

#include "../PhaseVocodeur3/PhaseVocodeur3.h"
#include "../VectorOperations2/VectorOperations2.h"
#include "../Windows/Windows.h"

namespace bst = boost::numeric::ublas;

class JVFreezer: public PhaseVocodeur3
{
    
public:
    
    JVFreezer();
    
    void spectral_processing(int fr) override;
    
    void set_is_freeze_active(bool is_freeze_active);
    
    bst::vector<float> get_magnitude();
private:
    
    int num_freq_bins {513};
    
    bst::vector<std::complex<float> > current_frozen_spectrum;
    bst::vector<std::complex<float> > last_frozen_spectrum;
    bst::vector<float> cumulative_phase;
    bst::vector<float> phase_advance;
    
    bool is_freeze_active {false};
    
    void init_phase_advance();
};
