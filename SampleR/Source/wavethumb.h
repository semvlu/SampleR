#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class wavethumb    : public Component,
                     public FileDragAndDropTarget
{
public:
    wavethumb(SampleRAudioProcessor& p);
    ~wavethumb();

    void paint (Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& files, int x, int y) override;

private:
    std::vector<float> mAudioPoints;
    bool mShouldBePainting {false};
    String mFileName{""};
    SampleRAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (wavethumb)
};
