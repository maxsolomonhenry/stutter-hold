#include "BarGraph.h"

BarGraph::BarGraph()
{
    data.clear();
}

BarGraph::BarGraph(Array<float> new_data)
{
    // store
    data = new_data;
    
    // calc width
    rect_width = getWidth() / (float) data.size();
    
    // update
    update_rects();
}

BarGraph::BarGraph(Array<float> new_data, float new_rect_width)
{
    data = new_data;
    
    rect_width = new_rect_width;
    
    update_rects();
}

BarGraph::~BarGraph()
{
    
}



// ===========================================================
void BarGraph::paint(Graphics &g)
{
    g.setColour(rect_colour);
    for (Rectangle<float> & r : rects)
    {
        g.drawRect(r);
    }
}

void BarGraph::resized()
{
    
}


void BarGraph::set_data(Array<float> new_data)
{
    data.clear();
    data = new_data;
    
    // calc width
    rect_width = getWidth() / (float) data.size();
    
    update_rects();
}


void BarGraph::set_colour(Colour new_colour)
{
    rect_colour = new_colour;
}



// ==========================================================
void BarGraph::update_rects()
{
    rects.clear();
    
    int i = 0;
    for (float & value : data)
    {
        float temp = abs(value * rect_height_scale) + 1.0f;
        
        // generate rectangle
        Rectangle<float> rect = Rectangle<float>((i) * rect_width, getHeight() - temp, rect_width, temp);
        
        // store
        rects.add(rect);
        
        // increment 2ndary
        i++;
    }
}
