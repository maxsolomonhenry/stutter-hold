/*
  ==============================================================================

    PhaseVocodeur3.h
    Created: 29 Mar 2021 9:42:27pm
    Author:  Julian Vanasse

    Phase Vocodeur (Vocoder) implementation using Boost for containers and
    kiss_fft for FFT.
 
    CURRENT ISSUES:
    *   frame_size, n_fft and hop_size must be powers of 2 to get perfect
        reconstruction. 
 
  ==============================================================================
*/

#pragma once

#include <complex>
#include <cmath>
#include <vector>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/assignment.hpp>

#include <kiss_fft/kiss_fft.h>
#include <kiss_fft/_kiss_fft_guts.h>
//#include "../kiss_fft/kiss_fft.h"
//#include "../kiss_fft/_kiss_fft_guts.h"


#include <JuceHeader.h>

#include "../VectorOperations2/VectorOperations2.h"
#include "../Windows/Windows.h"

namespace bst = boost::numeric::ublas;

class PhaseVocodeur3
{
  
public:
    PhaseVocodeur3();
    PhaseVocodeur3(int frame_size, int hop_size);
    PhaseVocodeur3(int frame_size, int hop_size, int n_fft);
    virtual ~PhaseVocodeur3();
    
    /* fifo operations */
    virtual void push(float input_sample);
    virtual void advance();
    float read_sum();
    
    /* spectral processing */
    virtual void spectral_processing(int fr);
    
    /* getters */
    int get_frame_size();
    int get_hop_size();
    int get_n_fft();
    int get_ola_size();
    int get_num_ola_frames();
    
    /* setters */
    void virtual set_frame_size(int frame_size);
    void virtual set_hop_size(int hop_size);
    void virtual set_n_fft(int n_fft);
    void virtual set_ola_size(int ola_size);
    
    
protected:
    /* dimensions */
    int frame_size          {256};
    int hop_size            {128};
    int n_fft               {1024};
    int ola_size            {1024};
    
    int num_ola_frames      {2};
    
    /* fft plans */
    kiss_fft_cfg fft_forward;
    kiss_fft_cfg fft_inverse;
    
    /* time domain containers */
    bst::vector<bst::vector<float> > ola_in, ola_out;
    bst::vector<float> window;
    
    /* read/write buffer positions */
    std::vector<int> rw;

    /* initialization */
    void init_fft();
    void init_ola();
    void init_window();
    
private:

    
};

