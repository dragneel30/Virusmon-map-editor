

#include "../JuceLibraryCode/JuceHeader.h"
#include "PropertiesArea.h"

//==============================================================================
PropertiesArea::PropertiesArea()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

PropertiesArea::~PropertiesArea()
{
}

void PropertiesArea::paint (Graphics& g)
{


    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId)); 

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1); 

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("PropertiesArea", getLocalBounds(),
                Justification::centred, true);   
}

void PropertiesArea::resized()
{

}
