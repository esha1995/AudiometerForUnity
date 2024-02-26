// mylibrary.hpp
#ifndef mylibrary_hpp
#define mylibrary_hpp

#include <iostream>
#include <cmath>
#include <vector>

// parameters ->
enum
{
    PARAM_FREQUENCY,
    PARAM_MODRATE,
    PARAM_FREQDEV,
    PARAM_ON,
    PARAM_SAMPLERATE,
    NUM_PARAMS
};


class Audiometer {
    int channels = 2;
    int sampleRate = 44100;
    int bufferSize = 512;
    int wavetableSize = 1024;
    float phase = 0.0f;
    float modPhase = 0.0f;
    float frequency = 440.0f;
    float modFreq = 20.0f;
    float freqDeviation = 0.1f;
    bool on = false;
    
    std::vector<float> waveTable;
    void GenerateWaveTable(int waveTableSize = 1024);
    float GetWaveSample(float frequency, float* phase);
    std::vector<float> GetWaveBuffer(float frequeny, float* phase);
    std::vector<float> GetWarbleTone(std::vector<float> modWave);
    void SetFrequency(float freq);
public:
    void SetOn(bool on) {this->on = on; }
    Audiometer(int sampleRate, int channels, int bufferSize);
    ~Audiometer();
    void read(float* data);
    void SetFloat(float value, int index);
};


#ifdef __cplusplus
extern "C" {
#endif
    Audiometer* audiometer;
    void prepare(int sampleRate, int channels, int bufferSize);
    void read(float* data);
    void setFloat(float value, int index);
    void setBool(bool value, int index);
#ifdef __cplusplus
}
#endif

#endif /* mylibrary_hpp */
