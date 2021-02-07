/*
  ==============================================================================

    PhaseVocodeur.cpp
    Created: 9 Dec 2020 9:19:32pm
    Author:  Julian Vanasse

  ==============================================================================
*/

#include "PhaseVocodeur.h"

PhaseVocodeur::PhaseVocodeur()
{
    init();
}

PhaseVocodeur::PhaseVocodeur(int frame_size, int hop_size)
{
    this->frame_size = frame_size;
    this->hop_size = hop_size;
    this->ola_size = frame_size;
    this->n_fft = frame_size;
    this->num_bins = (frame_size / 2) + 1;
    
    init();
}

PhaseVocodeur::PhaseVocodeur(int frame_size, int hop_size, int n_fft)
{
    this->frame_size = frame_size;
    this->hop_size = hop_size;
    this->ola_size = n_fft;
    this->n_fft = n_fft;
    this->num_bins = (n_fft / 2) + 1;
    
    init();
}

PhaseVocodeur::PhaseVocodeur(int frame_size, int hop_size, int ola_size, int n_fft)
{
    this->frame_size = frame_size;
    this->hop_size = hop_size;
    this->ola_size = ola_size;
    this->n_fft = n_fft;
    this->num_bins = (n_fft / 2) + 1;
    
    init();
}

// ==============================================================================

PhaseVocodeur::~PhaseVocodeur()
{
    /* free spectral resources */
    kiss_fft_free(fft_forward);
    kiss_fft_free(fft_inverse);
    delete[] fft_in;
    delete[] fft_out;
}

// ==============================================================================

void PhaseVocodeur::push(float input_sample)
{
    /* write to buffers */
    auto ola_in_w = ola_in.getArrayOfWritePointers();
    for (int b = 0; b < num_ola_frames; b++)
    {
        ola_in_w[b][rw[b]] = input_sample;
        // IF rw[b] is at the end of a frame THEN process
        if (rw[b] == ola_size-1)
        {
            spectral_routine(b);
        }
    }
}

void PhaseVocodeur::advance()
{
    /* advances rw positions */
    for (int k = 0; k < rw.size(); k++)
    {
        rw[k] = (rw[k] + 1) % ola_size;
    }
}

float PhaseVocodeur::read_sum()
{
    /* sum to output */
    float s = 0.0f;
    auto r = ola_out.getArrayOfReadPointers();
    for (int b = 0; b < num_ola_frames; b++)
    {
        s += r[b][rw[b]];
    }
    return s;
}

// ==============================================================================


void PhaseVocodeur::spectral_routine(int b)
{
    // pointers
    auto ola_out_w = ola_out.getWritePointer(b);
    auto ola_out_r = ola_out.getReadPointer(b);
    // copy from ola_in
    ola_out.copyFrom(b, 0, ola_in, b, 0, frame_size);
    // apply window
    apply_window(ola_out_r, ola_out_w);
    // copy into spectral buffers
    clear_cpx();
    copy_to_cpx(ola_out_r, fft_in, frame_size);
    // transform
    kiss_fft(fft_forward, fft_in, fft_out);
    
    /* DO SOMETHING */
    spectral_processing();
    /* ------------ */
    
    /* BACK TO TIME-DOMAIN */
    kiss_fft(fft_inverse, fft_out, fft_in);
    // copy into ola_out
    copy_to_bfr(ola_out_w, fft_in, n_fft);
}

void PhaseVocodeur::spectral_processing()
{
    // bins: DC, 1, 2, ..., Nyquist
    int num_bins = (n_fft / 2) + 1;
    
    // transform to polar
    float magnitude[num_bins];
    float phase[num_bins];
    
    car2pol(fft_out, magnitude, phase, num_bins);

    
    /* DO SOMETHING */
    /* filter example */
    for (int k = 0; k < num_bins; k++)
    {
        magnitude[k] *= exp(-(float)(k+1) / 15.0f);
    }
    
    /* return to cartesian */
    pol2car(fft_out, magnitude, phase, num_bins);

    
//    std::ofstream out;
//    out.open("post_fft.csv", std::ios::app);
//    for (int n = 0; n < n_fft; n++)
//    {
//        std::string s = "+";
//        if (fft_out[n].i < 0)
//            s = "";
//        out << fft_out[n].r << s << fft_out[n].i << "i";
//        if (n == n_fft - 1)
//            out << "\n";
//        else
//            out << ", ";
//    }
}


// ==============================================================================

void PhaseVocodeur::print()
{
    printf("Phase Vocodeur:\n");
    // print parameters
    printf("\tframe_size:\n");
    printf("\t\t%d\n", frame_size);
    printf("\thop_size:\n");
    printf("\t\t%d\n", hop_size);
    printf("\tn_fft:\n");
    printf("\t\t%d\n", n_fft);
    // print ola container dimensions
    printf("\tola_in:\n");
    printf("\t\tsize = [%d, %d]\n", ola_in.getNumChannels(), ola_in.getNumSamples());
    printf("\tola_out:\n");
    printf("\t\tsize = [%d, %d]\n", ola_out.getNumChannels(), ola_out.getNumSamples());
    printf("\trw:\n");
    // print rw positions
    printf("\t\t");
    print_int_vector(rw);
    printf("\n");
    // print window dimensions
    printf("\twindow:\n");
    printf("\t\tsize = [%d, %d]\n", window.getNumChannels(), window.getNumSamples());
}

// ==============================================================================
/* spectral processing */
void PhaseVocodeur::apply_window(const float *r, float *w)
{
    /* apply window to a buffer */
    auto win = window.getReadPointer(0);
    for (int n = 0; n < frame_size; n++)
    {
        w[n] = r[n] * win[n];
    }
}

/* spectral helpers */
void PhaseVocodeur::clear_cpx()
{
    /* clear complex kiss_fft buffers */
    for (int n = 0; n < n_fft; n++)
    {
        fft_in[n].r = 0.0f; fft_in[n].i = 0.0f;
        fft_out[n].r = 0.0f; fft_out[n].i = 0.0f;
    }
}

void PhaseVocodeur::copy_to_cpx(float *arr, kiss_fft_cpx *cpx_in, int len)
{
    const float *r = const_cast<float *>(arr);
    copy_to_cpx(r, cpx_in, len);
}

void PhaseVocodeur::copy_to_cpx(const float *r, kiss_fft_cpx *cpx_in, int len)
{
    /* copy into real part of fft_in */
    for (int n = 0; n < len; n++)
    {
        cpx_in[n].r = r[n];
    }

}

void PhaseVocodeur::copy_to_bfr(float *w, kiss_fft_cpx *cpx_in, int len)
{
    /* copy real cpx_in to buffer and scale */
    float fN = static_cast<float>(n_fft);
    for (int n = 0; n < len; n++)
    {
        w[n] = cpx_in[n].r / fN;
    }
    
}

/* coordinate conversion */
void PhaseVocodeur::car2pol(kiss_fft_cpx *cpx_out, float *r, float *p, int len)
{
    for (int k = 0; k < len; k++)
    {
        r[k] = mag(cpx_out[k].r, cpx_out[k].i);
        p[k] = ang(cpx_out[k].r, cpx_out[k].i);
    }
}

void PhaseVocodeur::pol2car(kiss_fft_cpx *cpx_out, float *r, float *p, int len)
{
    for (int k = 0; k < len; k++)
    {
        cpx_out[k].r = cos(p[k])*r[k]*2.0f;
        cpx_out[k].i = sin(p[k])*r[k]*2.0f;
    }
    // negative frequencies
    int m = num_bins-2;
    for (int k = num_bins; k < n_fft; k++)
    {
        cpx_out[k].r = cpx_out[m].r;
        cpx_out[k].i = -cpx_out[m].i;
        m--;
    }
}

// ==============================================================================

void PhaseVocodeur::init_fft()
{
    /* Initialize spectral containers and routines */
    
    // allocate space for input and output
    fft_in = new kiss_fft_cpx[n_fft];
    fft_out = new kiss_fft_cpx[n_fft];
    
    // initialize data storage
    memset(fft_in, 0, n_fft * sizeof(kiss_fft_cpx));
    memset(fft_out, 0, n_fft * sizeof(kiss_fft_cpx));
    
    // initialize plans
    fft_forward = kiss_fft_alloc(n_fft, 0, 0, 0);
    fft_inverse = kiss_fft_alloc(n_fft, 1, 0, 0);
}

void PhaseVocodeur::init_ola()
{
    /* Initialize Overlap-Add (OLA) containers and rw positions */
    
    // assign number of frames based on overlap
    num_ola_frames = ceil(static_cast<float>(ola_size) / static_cast<float>(hop_size));
    
    // allocate memory
    ola_in = juce::AudioBuffer<float> (num_ola_frames, ola_size);
    ola_out = juce::AudioBuffer<float> (num_ola_frames, ola_size);
    
    // clear
    ola_in.clear(); ola_out.clear();
    
    // initialize rw positions
    int pos = 0;
    rw.clear();
    for (int i = 0; i < num_ola_frames; i++)
    {
        rw.push_back(pos);
        pos += (hop_size % ola_size);
    }
}

void PhaseVocodeur::init_window()
{
    /* Initialize Hann window buffer. */
    // Hann window is computed using the 'periodic' method.
    
    // allocate memory and clear
    window = juce::AudioBuffer<float>(1, frame_size);
    window.clear();
    // fill using Hann function
    auto w = window.getWritePointer(0);
    float N = static_cast<float>(frame_size);
    for (int n = 0; n < frame_size; n++)
    {
        float fn = static_cast<float>(n);
        w[n] = pow(sin(M_PI * fn / N), 2.0f);
    }
}

// =========================================================================

void PhaseVocodeur::init()
{
    /* Call all init subroutines */
    init_ola();
    init_window();
    init_fft();
}
