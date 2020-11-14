#include "PluginProcessor.h"
#include "PluginEditor.h"

SampleRAudioProcessor::SampleRAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), mAPVTS(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    mFormatManager.registerBasicFormats();
    mAPVTS.state.addListener(this);
    
    for (int i = 0; i <  mNumVoices; i++)
    {
        mSampler.addVoice(new SamplerVoice());
    }
}

SampleRAudioProcessor::~SampleRAudioProcessor()
{
    
    mFormatReader = nullptr;
}

const String SampleRAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SampleRAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SampleRAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SampleRAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SampleRAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SampleRAudioProcessor::getNumPrograms()
{
    return 1;
}

int SampleRAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SampleRAudioProcessor::setCurrentProgram (int index)
{
}

const String SampleRAudioProcessor::getProgramName (int index)
{
    return {};
}

void SampleRAudioProcessor::changeProgramName (int index, const String& newName)
{
}


void SampleRAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate (sampleRate);
    updateADSR();
    
}

void SampleRAudioProcessor::releaseResources()
{
   
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SampleRAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SampleRAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    if (mShouldUpdate)
    {
        updateADSR();

    }
    MidiMessage m;
    MidiBuffer::Iterator it {midiMessages};
    int sample;
    
    while (it.getNextEvent(m, sample))
    {
        if (m.isNoteOn())
            mIsNotePlayed = true;
        else if (m.isNoteOff())
            mIsNotePlayed = false;
    }
    mSampleCount = mIsNotePlayed ? mSampleCount += buffer.getNumSamples() : 0;
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

    }
}

bool SampleRAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* SampleRAudioProcessor::createEditor()
{
    return new SampleRAudioProcessorEditor (*this);
}

void SampleRAudioProcessor::getStateInformation (MemoryBlock& destData)
{
   
}

void SampleRAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

void SampleRAudioProcessor::loadFile()
{
    mSampler.clearSounds();
    FileChooser chooser  { "Please load a file" };
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor (file);
    }
    
    BigInteger range;
    range.setRange (0, 128, true);
    
    mSampler.addSound (new SamplerSound ("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
}

void SampleRAudioProcessor::loadFile(const String& path)
{
    mSampler.clearSounds();
    auto file = (path);
    mFormatReader = mFormatManager.createReaderFor (file);
    
    auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
    mWaveForm.setSize(1, sampleLength);
    mFormatReader->read(&mWaveForm, 0, sampleLength, 0, true, false);
    
    auto buffer = mWaveForm.getReadPointer(0);
    
    BigInteger range;
    range.setRange (0, 128, true);
    
    mSampler.addSound (new SamplerSound ("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0));
    updateADSR();
}
void SampleRAudioProcessor::updateADSR()
{
    mADSRParams.attack = mAPVTS.getRawParameterValue("ATTACK")->load();
    mADSRParams.decay = mAPVTS.getRawParameterValue("DECAY")->load();
    mADSRParams.sustain = mAPVTS.getRawParameterValue("SUSTAIN")->load();
    mADSRParams.release = mAPVTS.getRawParameterValue("RELEASE")->load();

   for (int i = 0; i < mSampler.getNumSounds(); ++i)
   {
       if (auto sound = dynamic_cast<SamplerSound*>(mSampler.getSound(i).get()))
       {
           sound->setEnvelopeParameters(mADSRParams);
       }
   }
}
AudioProcessorValueTreeState::ParameterLayout SampleRAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;
   
    parameters.push_back(std::make_unique<AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("DECAY", "Decay", 0.0f, 3.0f, 0.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    parameters.push_back(std::make_unique<AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 2.0f));


    
    return {parameters.begin(), parameters.end()};
}
void SampleRAudioProcessor::valueTreePropertyChanged(ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    mShouldUpdate = true;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SampleRAudioProcessor();
}
