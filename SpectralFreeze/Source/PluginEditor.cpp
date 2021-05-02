/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SpectralFreezeAudioProcessorEditor::SpectralFreezeAudioProcessorEditor (SpectralFreezeAudioProcessor& p, juce::AudioProcessorValueTreeState& s)
    : AudioProcessorEditor (&p), audioProcessor (p), state(s)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    header_label.setText("Spectral Freeze", juce::dontSendNotification);
    header_label.setJustificationType(juce::Justification::centredTop);
    header_label.setFont(juce::Font(24.0f, juce::Font::FontStyleFlags::bold));
    addAndMakeVisible(header_label);
    
    freeze_toggle_button_label.setText("activate", juce::dontSendNotification);
    freeze_toggle_button_label.attachToComponent(&freeze_toggle_button, true);
    addAndMakeVisible(freeze_toggle_button_label);
    
    addAndMakeVisible(freeze_toggle_button);
    freeze_toggle_attachment.reset( new juce::AudioProcessorValueTreeState::ButtonAttachment (state, "freezeToggle", freeze_toggle_button));
    
    addAndMakeVisible(bar_graph);
    
    startTimer(100);
    
    setSize (400, 300);
}

SpectralFreezeAudioProcessorEditor::~SpectralFreezeAudioProcessorEditor()
{
}

//==============================================================================
void SpectralFreezeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
}

void SpectralFreezeAudioProcessorEditor::resized()
{
    header_label.setBounds(0, 0, getWidth(), getHeight());
//    freeze_toggle_button.setBounds(freeze_toggle_button_label.getWidth() + 10, header_label.getY() + header_label.getHeight() + 10, 100, freeze_toggle_button_label.getHeight());
    freeze_toggle_button.setBounds(100, 100, 100, 100);
    bar_graph.setBounds(10, 200, getWidth()-20, 100);
}

void SpectralFreezeAudioProcessorEditor::timerCallback()
{
    boost::numeric::ublas::vector<float> magnitude_bst = audioProcessor.get_magnitude();
    juce::Array<float> magnitude;
    magnitude.resize(magnitude_bst.size()/2 + 1);
    
    std::copy(magnitude_bst.begin(), magnitude_bst.begin() + magnitude.size(), magnitude.begin());
    
    bar_graph.set_data(magnitude);
    bar_graph.repaint();
}
