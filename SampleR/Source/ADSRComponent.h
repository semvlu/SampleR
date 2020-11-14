/*
  ==============================================================================

    ADSRComponent.h
    Created: 20 Jun 2020 2:52:28pm
    Author:  SEM

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ADSRComponent    : public Component
{
public:
    ADSRComponent(SampleRAudioProcessor& p);
    ~ADSRComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    ImageComponent mImageComponent;

    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mAttackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mDecayAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mSustainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mReleaseAttachment;
    SampleRAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
