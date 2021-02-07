/*
  ==============================================================================

    PhaseVocodeur.h
    Created: 9 Dec 2020 9:19:32pm
    Author:  Julian Vanasse
 
    Real-time phase vocodeur implementation using FFT and overlap-add (OLA). Can
    handle noncircular convolution (and the tail produced) with the ola_size
    parameter.
 
    frame_size
    hop_size
    n_fft
    ola_size

  ==============================================================================
*/

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <math.h>
#include <JuceHeader.h>
/* fft library */
#include "../kiss_fft130/kiss_fft.h"

#define DEFAULT_FRAME_SIZE 256
#define DEFAULT_HOP_SIZE 128

class PhaseVocodeur {
    
public:
    /* class constructors */
    PhaseVocodeur();
    PhaseVocodeur(int frame_size, int hop_size);
    PhaseVocodeur(int frame_size, int hop_size, int n_fft);
    PhaseVocodeur(int frame_size, int hop_size, int ola_size, int n_fft);
    
    /* class destructor */
    virtual ~PhaseVocodeur();
    
    /* low level operation */
    virtual void push(float input_sample);
    virtual void advance();
    float read_sum();
    
    /* spectral processing */
    virtual void spectral_routine(int b);   // apply windowing, FFT and IFFT to next buffer channel in push( float )
    virtual void spectral_processing();     // manipulating transformed data
    
    /* debugging help */
    void print();
    
//private: ... should be better interface?
protected:
    /* spectral containers */
    kiss_fft_cpx* fft_in;
    kiss_fft_cpx* fft_out;
    
// private:
    /* spectral dimension */
    int n_fft {2*DEFAULT_FRAME_SIZE};
    int num_bins {DEFAULT_FRAME_SIZE + 1};
    
    /* spectral plan */
    kiss_fft_cfg fft_forward;
    kiss_fft_cfg fft_inverse;
    
    /* time domain dimensions */
    int frame_size {DEFAULT_FRAME_SIZE};
    int hop_size {DEFAULT_HOP_SIZE};
    int ola_size {(DEFAULT_FRAME_SIZE * 2)};
    int num_ola_frames {ola_size / DEFAULT_HOP_SIZE};
    
    /* time domain containers */
    juce::AudioBuffer<float> ola_in;
    juce::AudioBuffer<float> ola_out;
    juce::AudioBuffer<float> window;
    
    /* read write positions */
    std::vector<int> rw;
    
    // ==============================================================================
    /* spectral processing */
    void apply_window(float const *r, float *w);
    
    /* spectral helpers */
    void clear_cpx();
    void copy_to_cpx(float *arr, kiss_fft_cpx* fft_in, int len);
    void copy_to_cpx(const float *r, kiss_fft_cpx* fft_in, int len);
    void copy_to_bfr(float *w, kiss_fft_cpx* fft_out, int len);
    
    /* coordinate conversion */
    void car2pol(kiss_fft_cpx* cpx_out, float *r, float *p, int len);
    void pol2car(kiss_fft_cpx* cpx_out, float *r, float *p, int len);
    
    // =========================================================================
    /* init subroutines */
    void init_fft();        // Initialize spectral processing
    void init_ola();        // Initialize time domain containers and rw
    void init_window();     // Initialize Hann window
    // =========================================================================
    /* init routine */
    void init();            // Calls all subroutines
    
    
    
};

static float mag(float a, float b)
{
    return sqrt(pow(a, 2) + pow(b, 2));
}

static float ang(float a, float b)
{
    return atan2(b, a);
}

static void print_int_vector(std::vector<int> arr)
{
    std::vector<int>::iterator it;
    printf("[");
    for (it = arr.begin(); it != arr.end(); it++)
    {
        printf("%d", *it);
        if (it != arr.end()-1)
            printf(", ");
    }
    printf("]");
}
