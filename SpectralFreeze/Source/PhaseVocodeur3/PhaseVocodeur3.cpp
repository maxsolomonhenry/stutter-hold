/*
  ==============================================================================

    PhaseVocodeur3.cpp
    Created: 29 Mar 2021 9:42:27pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#include "PhaseVocodeur3.h"

PhaseVocodeur3::PhaseVocodeur3()
{
    init_ola();
    init_window();
    init_fft();
}

PhaseVocodeur3::PhaseVocodeur3(int frame_size, int hop_size)
{
    this->frame_size    = frame_size;
    this->hop_size      = hop_size;
    this->n_fft         = 2*frame_size;
    this->ola_size      = 2*frame_size;
    
    init_ola();
    init_window();
    init_fft();
}

PhaseVocodeur3::PhaseVocodeur3(int frame_size, int hop_size, int n_fft)
{
    this->frame_size    = frame_size;
    this->hop_size      = hop_size;
    this->n_fft         = n_fft;
    this->ola_size      = n_fft;
    
    init_ola();
    init_window();
    init_fft();
}

PhaseVocodeur3::~PhaseVocodeur3()
{
    
}

//============ FIFO Buffer Methods =============================================

void PhaseVocodeur3::push(float input_sample)
{
    /* write input to overlapping buffers */
    for (int fr = 0; fr < num_ola_frames; fr++)
    {
        ola_in(fr)(rw[fr]) = input_sample;
        // if full, transform
        if (rw[fr] == ola_size - 1)
        {
            spectral_processing(fr);
        }
    }
}

void PhaseVocodeur3::advance()
{
    for (int fr = 0; fr < rw.size(); fr++)
    {
        rw[fr] = (rw[fr] + 1) % ola_size;
    }
}

float PhaseVocodeur3::read_sum()
{
    /* overlap-add */
    float s = 0.0f;
    for (int fr = 0; fr < num_ola_frames; fr++)
    {
        s += ola_out(fr)(rw[fr]);
    }
    s /= 0.5f * (static_cast<float>(frame_size) / static_cast<float>(hop_size));
    return s;
}

//============ Spectral Processing Methods ======================================

void PhaseVocodeur3::spectral_processing(int fr)
{
    // copy current frame
    ola_out(fr) = ola_in(fr);
    // apply window
    ola_out(fr) = bst::element_prod(ola_out(fr), window);
    // fft
    bst::vector<std::complex<float> > spectrum = jv_bst::fft(ola_out(fr), fft_forward);
    
    // ifft
    spectrum = jv_bst::fft(spectrum, fft_inverse);
    // store
    ola_out(fr) = jv_bst::real(spectrum); 
}

//============ Getters ============================================================

int PhaseVocodeur3::get_frame_size()
{
    return frame_size;
}
int PhaseVocodeur3::get_hop_size()
{
    return hop_size;
}
int PhaseVocodeur3::get_n_fft()
{
    return n_fft;
}
int PhaseVocodeur3::get_ola_size()
{
    return ola_size;
}
int PhaseVocodeur3::get_num_ola_frames()
{
    return num_ola_frames;
}

//============ Setters ============================================================

void PhaseVocodeur3::set_frame_size(int frame_size)
{
    this->frame_size = frame_size;
    init_window();
}
void PhaseVocodeur3::set_hop_size(int hop_size)
{
    this->hop_size = hop_size;
    init_ola();
}
void PhaseVocodeur3::set_n_fft(int n_fft)
{
    /* set fft length */
    // set n_fft
    this->n_fft = n_fft;
    // also set ola_size
    this->ola_size = n_fft;
    init_fft();
    init_ola();
}
void PhaseVocodeur3::set_ola_size(int ola_size)
{
    this->ola_size = ola_size;
    init_ola();
}

//============ Initialization =====================================================

void PhaseVocodeur3::init_fft()
{
    fft_forward = kiss_fft_alloc(n_fft, 0, 0, 0);
    fft_inverse = kiss_fft_alloc(n_fft, 1, 0, 0);
}

void PhaseVocodeur3::init_ola()
{
    /* Initialize Overlap-Add (OLA) containers and rw positions */
    
    // assign number of frames based on overlap
    num_ola_frames = ceil(static_cast<float>(ola_size) / static_cast<float>(hop_size));
    
    // allocate ola in/out frames
    ola_in  = bst::vector<bst::vector<float> > (num_ola_frames, bst::vector<float>(ola_size, 0.0));
    ola_out = bst::vector<bst::vector<float> > (num_ola_frames, bst::vector<float>(ola_size, 0.0));
    
    
    // initialize rw positions
    int pos = 0;
    rw.clear();
    for (int i = 0; i < num_ola_frames; i++)
    {
        rw.push_back(pos);
        pos = (pos + hop_size) % ola_size;
    }
    
}

void PhaseVocodeur3::init_window()
{
    window = hann(frame_size);
    window = jv_bst::zp(window, ola_size - frame_size);
}


