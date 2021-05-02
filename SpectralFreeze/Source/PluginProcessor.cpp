/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpectralFreezeAudioProcessor::SpectralFreezeAudioProcessor()
: parameters(*this, nullptr, juce::Identifier("Spectral Freeze"),
           {
               std::make_unique<juce::AudioParameterBool>("freezeToggle",
                                                            "Freeze Toggle",
                                                            false)
           }),
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    freeze_toggle_parameter = parameters.getRawParameterValue("freezeToggle");
}

SpectralFreezeAudioProcessor::~SpectralFreezeAudioProcessor()
{
}

//==============================================================================
const juce::String SpectralFreezeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpectralFreezeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpectralFreezeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpectralFreezeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpectralFreezeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpectralFreezeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SpectralFreezeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpectralFreezeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SpectralFreezeAudioProcessor::getProgramName (int index)
{
    return {};
}

void SpectralFreezeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SpectralFreezeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    previous_freeze_toggle = (*freeze_toggle_parameter < 0.5f);
}

void SpectralFreezeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpectralFreezeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SpectralFreezeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int num_samples = buffer.getNumSamples();
    int num_channels = buffer.getNumChannels();
    
    bool current_freeze_toggle = !(*freeze_toggle_parameter < 0.5f);
    
    if (current_freeze_toggle != previous_freeze_toggle)
    {
        freezer.set_is_freeze_active(current_freeze_toggle);
        previous_freeze_toggle = current_freeze_toggle;
    }
    
    // process
    juce::AudioBuffer<float> output (1, num_samples);
    auto r = buffer.getReadPointer(0);
    auto w = output.getWritePointer(0);
    for (int n = 0; n < num_samples; n++)
    {
        freezer.push(r[n]);
        w[n] = freezer.read_sum();
        freezer.advance();
    }
    
    // copy output
    for (int channel = 0; channel < num_channels; channel++)
    {
        buffer.copyFrom(channel, 0, output, 0, 0, num_samples);
    }
}

//==============================================================================
bool SpectralFreezeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SpectralFreezeAudioProcessor::createEditor()
{
    return new SpectralFreezeAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void SpectralFreezeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SpectralFreezeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

boost::numeric::ublas::vector<float> SpectralFreezeAudioProcessor::get_magnitude()
{
    return freezer.get_magnitude();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralFreezeAudioProcessor();
}

