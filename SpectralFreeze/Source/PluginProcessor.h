/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <boost/numeric/ublas/vector.hpp>

#include <JuceHeader.h>

#include "JVFreezer/JVFreezer.h"

//==============================================================================
/**
*/
class SpectralFreezeAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SpectralFreezeAudioProcessor();
    ~SpectralFreezeAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    boost::numeric::ublas::vector<float> get_magnitude();
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    
    bool previous_freeze_toggle {true};
    std::atomic<float*> freeze_toggle_parameter;
    
    JVFreezer freezer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralFreezeAudioProcessor)
};
