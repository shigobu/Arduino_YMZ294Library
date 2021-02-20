#include <arduino.h>
#include "YMZ294.h"

YMZ294::YMZ294() 
      : TP{
          15289,14431,13621,12856,12135,11454,10811,10204,9631,9091,8581,8099,7645,7215,6810,6428,6067,5727,
          5405,5102,4816,4545,4290,4050,3822,3608,3405,3214,3034,2863,2703,2551,2408,2273,2145,2025,1911,1804,
          1703,1607,1517,1432,1351,1276,1204,1136,1073,1012,956,902,851,804,758,716,676,638,602,568,536,506,478,
          451,426,402,379,358,338,319,301,284,268,253,239,225,213,201,190,179,169,159,150,142,134,127,119,113,106,
          100,95,89,84,80,75,71,67,63,60,56,53,50,47,45,42,40,38,36,34,32,30,28,27,25,24,22,21,20,19,18,17,16,15,14,13,13,12,11,11,10
        }{}

YMZ294::YMZ294(int wr, int cs, int ad, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) 
       : YMZ294()
{
  WR = wr; CS = cs; AD = ad; D0 = d0; D1 = d1; D2 = d2; D3 = d3; D4 = d4; D5 = d5; D6 = d6; D7 = d7;
  isCsAvailable = true;
}
YMZ294::YMZ294(int wr, int ad, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) 
       : YMZ294()
{
  WR = wr; AD = ad; D0 = d0; D1 = d1; D2 = d2; D3 = d3; D4 = d4; D5 = d5; D6 = d6; D7 = d7;
  isCsAvailable = false;
}

void YMZ294::begin(){
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(AD, OUTPUT);
  pinMode(WR, OUTPUT);
  if (isCsAvailable) {
    pinMode(CS, OUTPUT);
    digitalWrite(CS, HIGH);
  }
  
  setVolume(Channel::ChA, 0x0f);
  setVolume(Channel::ChB, 0x0f);
  setVolume(Channel::ChC, 0x0f);
  setMixer(Channel::Noise1, MixerOnOff::Off);
  setMixer(Channel::Noise2, MixerOnOff::Off);
  setMixer(Channel::Noise3, MixerOnOff::Off);
}

void YMZ294::noteOn(Channel ch, uint8_t noteNum){
  setFrequency(ch, TP[noteNum]);
  setMixer(ch, MixerOnOff::On);
}

void YMZ294::noteOff(Channel ch){
  setMixer(ch, MixerOnOff::Off);
}

void YMZ294::selectAddressData(AddressData ad){
  digitalWrite(AD, ad);
}

void YMZ294::setDataBus(uint8_t data){
  if (isCsAvailable) {
    digitalWrite(CS, LOW);
  }
  digitalWrite(WR, LOW);
  digitalWrite(D7, data & 0b10000000);
  digitalWrite(D6, data & 0b01000000);
  digitalWrite(D5, data & 0b00100000);
  digitalWrite(D4, data & 0b00010000);
  digitalWrite(D3, data & 0b00001000);
  digitalWrite(D2, data & 0b00000100);
  digitalWrite(D1, data & 0b00000010);
  digitalWrite(D0, data & 0b00000001);
  digitalWrite(WR, HIGH);
  if (isCsAvailable) {
    digitalWrite(CS, HIGH);
  }
}

void YMZ294::setData(uint8_t data){
  selectAddressData(AddressData::Data);
  setDataBus(data);
}

void YMZ294::setAddress(Addresses data){
  selectAddressData(AddressData::Address);
  setDataBus(data);
}

void YMZ294::setFrequency(Channel ch, uint16_t TP){
  Addresses MSBaddr;
  Addresses LSBaddr;
  switch (ch)
  {
  case Channel::ChA:
    LSBaddr = Addresses::ChA1;
    MSBaddr = Addresses::ChA2;
    break;
  case Channel::ChB:
    LSBaddr = Addresses::ChB1;
    MSBaddr = Addresses::ChB2;
    break;
  case Channel::ChC:
    LSBaddr = Addresses::ChC1;
    MSBaddr = Addresses::ChC2;
    break;
  default:
    return;
  }
  setAddress(LSBaddr);
  setData(TP & 0x00ff);
  setAddress(MSBaddr);
  setData((TP & 0xff00) >> 8);
}

void YMZ294::setVolume(Channel ch, uint8_t volume){
  Addresses addr;
  switch (ch)
  {
  case Channel::ChA:
    addr = Addresses::VolumeA;
    break;
  case Channel::ChB:
    addr = Addresses::VolumeB;
    break;
  case Channel::ChC:
    addr = Addresses::VolumeC;
    break;
  default:
    return;
  }

  setAddress(addr);
  setData(volume & 0x0f);
}

void YMZ294::setMixer(Channel ch, MixerOnOff mixer){
  uint8_t mixerValue = 0;
  setAddress(Addresses::Mixer);
  switch (ch)
  {
  case Channel::ChA:
    mixerValue = 0b1;
    break;
  case Channel::ChB:
    mixerValue = 0b10;
    break;
  case Channel::ChC:
    mixerValue = 0b100;
    break;
  case Channel::Noise1:
    mixerValue = 0b1000;
    break;
  case Channel::Noise2:
    mixerValue = 0b10000;
  case Channel::Noise3:
    mixerValue = 0b100000;
    break;
default:
    return;
  }

  if (mixer == MixerOnOff::On){
    currentMixerVal &= ~mixerValue;
  }
  else{
    currentMixerVal |= mixerValue;
  }
  
  setData(currentMixerVal);

}

void YMZ294::setEnvelopeFrequency(uint16_t freq) {
  setAddress(Addresses::EnvelopeFreqLSB);
  setData(freq & 0x00ff);
  setAddress(Addresses::EnvelopeFreqMSB);
  setData(freq >> 8);
}

void YMZ294::setEnvelopeShape(EnvelopeShapes shape){
  setAddress(Addresses::EnvelopeShape);
  setData(shape);
  setAddress(Addresses::VolumeA);
  setData(0xff);
}
