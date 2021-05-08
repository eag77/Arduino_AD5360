#include <math.h>
#include <SPI.h>

#define LED 13
#define CS 8

float deg = 0.0;
long msn = 0;
long ms = 0;
int16_t c = 0;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(CS, OUTPUT);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE2);
  ms = millis();
  msn = ms+1;
}

void loop()
{
  ms = millis();
  if(msn <= ms)
  {
    digitalWrite(LED, HIGH);

    AD5360ch(0, c);
    AD5360ch(1, c+0x2000);
    AD5360ch(2, c+0x4000);
    AD5360ch(3, c+0x8000);
    AD5360ch(4, c+0xA000);
    AD5360ch(5, c+0xC000);
    AD5360ch(6, c+0xE000);
    AD5360ch(7, c<<1);
    AD5360ch(8, c<<2);
    AD5360ch(9, c<<3);
    AD5360ch(10, c<<4);

    AD5360ch(11, SinD(deg));
    AD5360ch(12, SinD(deg+90));
    AD5360ch(13, SinD(deg+120));
    AD5360ch(14, SinD(deg+240));
    AD5360ch(15, SinD(deg+270));
    c+=100;
    deg+=1.0;
    if(deg>=360.0)
      deg = 0.0;
    msn = ms+1;
    digitalWrite(LED, LOW);
  }
}

void AD5360write(uint8_t CMD, uint8_t ADDR, uint16_t DATA)
{
  digitalWrite(CS, LOW);
  SPI.transfer((CMD<<6)|(ADDR&0x3F));
  SPI.transfer(DATA>>8);
  SPI.transfer(DATA&0xFF);
  digitalWrite(CS, HIGH);
}

void AD5360ch(uint8_t CH, uint16_t D)
{
  AD5360write(0x03, CH<8?CH|0x08:(CH&0x07)|0x10, (uint16_t)D);
}

int16_t SinD(float d)
{
  return 32767*(sin((d/360)*(2*M_PI))+1.0);
}
