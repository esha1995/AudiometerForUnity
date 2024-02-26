#include "Audiometer.hpp"

void prepare(int sampleRate, int channels, int bufferSize)
{
    audiometer = new Audiometer(sampleRate, channels, bufferSize);
}

void read(float* data)
{
    audiometer->read(data);
}

void setFloat(float value, int index)
{
    switch (index) {
        case PARAM_FREQDEV : case PARAM_MODRATE : case PARAM_FREQUENCY : PARAM_SAMPLERATE :
            audiometer->SetFloat(value, index);
            break;
        default:
            break;
    }
}

void setBool(bool value, int index)
{
    switch (index) {
        case PARAM_ON:
            audiometer->SetOn(value);
            break;
        default:
            break;
    }
}




Audiometer::Audiometer(int sampleRate, int channels, int bufferSize)
{
    this->sampleRate = sampleRate;
    this->channels = channels;
    this->bufferSize = bufferSize;
    GenerateWaveTable();
}

void Audiometer::read(float* data)
{
    if(!on)
    {
        data = &std::vector<float>(0.0f, bufferSize)[0];
        return;
    }
    
    std::vector<float> modWave  = GetWaveBuffer(modFreq, &modPhase);
    std::vector<float> warbleWave = GetWarbleTone(modWave);
    
    for(int s = 0; s < bufferSize; ++s)
    {
        for(int ch = 0; ch < channels; ++ch, ++data)
        {
            // multiply the data signal with the warble wave for spatialization
            // The input audio clip in the audio source has to be a "dummy" signal of 1's containing spatializatio info
            *data *= warbleWave[s];
        }
    }
}

void Audiometer::GenerateWaveTable(int waveTableSize) {
    waveTable.resize(waveTableSize);
    for(int i = 0; i < waveTableSize; ++i)
    {
        waveTable[i] = sin(2.0f * M_PI * i / wavetableSize);
    }
}



Audiometer::~Audiometer() {
}

std::vector<float> Audiometer::GetWaveBuffer(float frequency, float* phase) {
    std::vector<float> buffer(bufferSize);
    for(int s = 0; s < bufferSize; ++s)
    {
        buffer[s] = GetWaveSample(frequency, phase);
    }
    return buffer;
}

float Audiometer::GetWaveSample(float frequency, float* phase){
    float phaseIncrement = frequency / sampleRate;
    int index0 = static_cast<int>(*phase * wavetableSize) % wavetableSize;
    int index1 = (index0 + 1) % wavetableSize;
    float frac = *phase * wavetableSize - index0;
    float sample = waveTable[index0] + frac * (waveTable[index1] - waveTable[index0]);
    
    // Update phase
    *phase += phaseIncrement;
    if (*phase >= 1.0f) {
        *phase -= 1.0f;
    }
    
    return sample;
}

std::vector<float> Audiometer::GetWarbleTone(std::vector<float> modWave) 
{

    if(modWave.size() != bufferSize)
        return std::vector<float>(0.0f, bufferSize);

    std::vector<float> buffer(bufferSize);
    float modIndex = (this->frequency * this->freqDeviation)/this->modFreq;
    
    for(int s = 0; s < bufferSize; ++s)
    {
        float frequency = this->frequency + (modIndex * modWave[s]);
        buffer[s] = GetWaveSample(frequency, &phase);
    }
    
    return buffer;
}


void Audiometer::SetFloat(float value, int index) { 
    switch (index) {
        case PARAM_FREQUENCY:
            this->frequency = value > 1.0f ? value : 1.0f;
            break;
        case PARAM_MODRATE:
            this->modFreq = value > 1.0f ? value : 1.0f;
            break;
        case PARAM_FREQDEV:
            this->freqDeviation = value;
            break;
        case PARAM_SAMPLERATE:
            this->sampleRate = static_cast<int>(std::floor(value));
        default:
            break;
    }
}

void Audiometer::SetFrequency(float freq) { 
    this->frequency = freq;
}





    





