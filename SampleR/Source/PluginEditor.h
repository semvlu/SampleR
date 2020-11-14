
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "wavethumb.h"
#include "ADSRComponent.h"

class SampleRAudioProcessorEditor  : public AudioProcessorEditor,
                                     public Timer
{
public:
    SampleRAudioProcessorEditor (SampleRAudioProcessor&);
    ~SampleRAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
private:
    wavethumb mWavethumb;
    ADSRComponent mADSR;
    SampleRAudioProcessor& processor;

};
