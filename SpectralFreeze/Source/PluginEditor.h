/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <boost/numeric/ublas/vector.hpp>

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "BarGraph/BarGraph.h"

//==============================================================================
/**
*/
class SpectralFreezeAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    SpectralFreezeAudioProcessorEditor (SpectralFreezeAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~SpectralFreezeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback();

private:
    juce::AudioProcessorValueTreeState& state;
    
    juce::Label header_label;
    
    juce::ToggleButton freeze_toggle_button;
    juce::Label freeze_toggle_button_label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> freeze_toggle_attachment;
    
    BarGraph bar_graph;
    
    SpectralFreezeAudioProcessor& audioProcessor;

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralFreezeAudioProcessorEditor)
};
