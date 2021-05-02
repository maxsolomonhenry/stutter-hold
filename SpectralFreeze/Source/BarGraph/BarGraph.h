
# pragma once

#include <JuceHeader.h>

using namespace juce;

class BarGraph : public Component
{
public:
    
    BarGraph();
    BarGraph(Array<float> new_data);
    BarGraph(Array<float> new_data, float new_rect_width);
    ~BarGraph();
    
    
    void paint (Graphics &g) override;
    void resized() override;
    
    void set_data(Array<float> new_data);
    void set_colour(Colour new_colour);
    
private:
    
    void init_data();
    Array<float> data;
    
    Array<Rectangle<float>> rects;
    float rect_width = {10.0f};
    float rect_height_scale = {50.0f};
    Colour rect_colour {Colours::crimson};
    void update_rects();
};
