#ifndef YMZ294_H
#define YMZ294_H

enum AddressData{
  Address = 0,
  Data = 1
};

enum Addresses{
  ChA1,
  ChA2,
  ChB1,
  ChB2,
  ChC1,
  ChC2,
  Noise,
  Mixer,
  VolumeA,
  VolumeB,
  VolumeC,
  EnvelopeFreqLSB,
  EnvelopeFreqMSB,
  EnvelopeShape
};

enum Channel{
  ChA,
  ChB,
  ChC,
  Noise1,
  Noise2,
  Noise3,
};

enum MixerOnOff{
  On,
  Off,
};

enum EnvelopeShapes{
  sh1 = 0b1000,
  sh2,
  sh3,
  sh4,
  sh5,
  sh6,
  sh7,
  sh8,
};

class YMZ294{
  private:
    YMZ294();
  public:
    YMZ294(int wr, int cs, int ad, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7);
    YMZ294(int wr, int ad, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7);

    void begin();
    void noteOn(Channel ch, uint8_t noteNum);
    void noteOff(Channel ch);
    void selectAddressData(AddressData ad);
    void setDataBus(uint8_t data);
    void setData(uint8_t data);
    void setAddress(Addresses data);
    void setFrequency(Channel ch, uint16_t TP);
    void setVolume(Channel ch, uint8_t volume);
    void setMixer(Channel ch, MixerOnOff mixer);
    void setEnvelopeFrequency(uint16_t freq) ;
    void setEnvelopeShape(EnvelopeShapes shape);

  private:
    int WR;
    int CS;
    int AD;
    int D0;
    int D1;
    int D2;
    int D3;
    int D4;
    int D5;
    int D6;
    int D7;

    bool isCsAvailable;

    uint8_t currentMixerVal = 0xFF;

    const int16_t TP[128];
};

#endif
