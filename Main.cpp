/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Editor.h"
#include "PropertiesWindow.h"
#include "Util.h"
//==============================================================================
class VirusmonmapeditorApplication  : public JUCEApplication
{
public:
    //==============================================================================
    VirusmonmapeditorApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
		editor = new Editor(getApplicationName(), Vector2i(800,800));
		//PropertiesWindow::instance = new PropertiesWindow("Properties", Vector2i(800, 200));
    }

    void shutdown() override
    {

        editor = nullptr; 
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
    }


    class MainWindow : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
			
			Vector2i size(800, 600);
			setSize(size.x, size.y);
			setUsingNativeTitleBar (0);
			setResizable(true,true);
            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }


    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };
private:
    ScopedPointer<Editor> editor;







	
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (VirusmonmapeditorApplication)
