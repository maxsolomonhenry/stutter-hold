/*
  ==============================================================================

    JVFreezer.cpp
    Created: 6 Apr 2021 5:00:57pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#include "JVFreezer.h"

JVFreezer::JVFreezer()
: PhaseVocodeur3(1024, 256, 1024)
{
    num_freq_bins = n_fft/2 + 1;
    
    last_frozen_spectrum = bst::vector<std::complex<float> > (n_fft, 0.0f);
    current_frozen_spectrum = last_frozen_spectrum;
    
    cumulative_phase = bst::vector<float> (num_freq_bins, 0.0f);
    phase_advance = cumulative_phase;
    init_phase_advance();
}

void JVFreezer::spectral_processing(int fr)
{
    // copy current frame
    ola_out(fr) = ola_in(fr);
    // apply window
    ola_out(fr) = bst::element_prod(ola_out(fr), window);
    
    // transform
    bst::vector<std::complex<float> > spectrum = jv_bst::fft(ola_out(fr), fft_forward);
    
    if (!is_freeze_active)
    {
        last_frozen_spectrum = current_frozen_spectrum;
        current_frozen_spectrum = spectrum;
        
        cumulative_phase = jv_bst::angle(current_frozen_spectrum);
        cumulative_phase.resize(num_freq_bins, true);
    }
    
    if (is_freeze_active)
    {
        // truncate
        bst::vector<std::complex<float> > Xcurrent (num_freq_bins, 0.0f);
        bst::vector<std::complex<float> > Xlast (num_freq_bins, 0.0f);
        
        std::copy(current_frozen_spectrum.begin(), current_frozen_spectrum.begin() + num_freq_bins, Xcurrent.begin());
        std::copy(last_frozen_spectrum.begin(), last_frozen_spectrum.begin()+num_freq_bins, Xlast.begin());
        
        // current magnitude
        bst::vector<float> mX = jv_bst::abs(Xcurrent);
        
        // phase advance
        bst::vector<float> current_phase = jv_bst::angle(Xcurrent);
        bst::vector<float> last_phase = jv_bst::angle(Xlast);
        bst::vector<float> dp = current_phase - last_phase - phase_advance;
        dp = jv_bst::wrap_to_pi(dp);
        
        // output half-spectrum
        bst::vector<std::complex<float> > Y = jv_bst::pol2cart(mX, cumulative_phase);
        
        // form whole spectrum
        bst::vector<std::complex<float> > Y_whole (n_fft, 0.0f);
        std::copy(Y.begin(), Y.begin()+num_freq_bins, Y_whole.begin());
        
            // hermitian symmetry
        int k = num_freq_bins-2;
        for (int n = num_freq_bins; n < n_fft; n++)
        {
            Y_whole(n) = std::conj(Y(k));
            
            k--;
        }
        
        // inverse transform
        bst::vector<float> y = jv_bst::real(jv_bst::fft(Y_whole, fft_inverse));
        
        // advance cumulative phase
        bst::vector<float> temp = cumulative_phase + phase_advance + dp;
        cumulative_phase = jv_bst::wrap_to_pi(temp);
        
        // store
        ola_out(fr) = bst::element_prod(y, window) * (4.0/3.0);
    }
    
}

void JVFreezer::set_is_freeze_active(bool is_freeze_active)
{
    this->is_freeze_active = is_freeze_active;
}

bst::vector<float> JVFreezer::get_magnitude()
{
    return jv_bst::abs(current_frozen_spectrum);
}

void JVFreezer::init_phase_advance()
{
    float phi = 0.0f;
    for (int n = 1; n < phase_advance.size(); n++)
    {
        phi = M_PI * 2.0f * static_cast<float>(hop_size);
        phi = phi / (static_cast<float>(n_fft) / static_cast<float>(n));
        phase_advance(n) = phi;
    }
}
