#include <Thing.h>
#include <BlinkPattern.h>
#include <Wire.h>
#include <DS18B20.h>

using namespace g3rb3n;

Thing thing;
BlinkPattern led(BUILTIN_LED);
DS18B20 ds1(D1);
DS18B20 ds2(D2);
DS18B20 ds3(D3);
DS18B20 ds4(D4);

BlinkPattern::Pattern<2> panic{{1,1},25};
BlinkPattern::Pattern<2> start{{1,9},25};
BlinkPattern::Pattern<2> normal{{1,39},25};

void setup() 
{
  Serial.begin(230400);
  Serial.println();
  Serial.println("Client:" + thing.clientId());

  led.setPattern(start);
  
  thing.onStateChange([](const String& msg){
    Serial.println(msg);
  });

  thing.addSensor(String("sensor/ds18b20/") + thing.clientId() + "/1", 5000, getValue1);
  thing.addSensor(String("sensor/ds18b20/") + thing.clientId() + "/2", 5000, getValue2);
  thing.addSensor(String("sensor/ds18b20/") + thing.clientId() + "/3", 5000, getValue3);
  thing.addSensor(String("sensor/ds18b20/") + thing.clientId() + "/4", 5000, getValue4);

  if (!ds1.begin()) 
  {
    Serial.println("Could not find DS18B20 sensor 1");
    led.setPattern(panic);
  }
  if (!ds2.begin()) 
  {
    Serial.println("Could not find DS18B20 sensor 2");
    led.setPattern(panic);
  }
  if (!ds3.begin()) 
  {
    Serial.println("Could not find DS18B20 sensor 3");
    led.setPattern(panic);
  }
  if (!ds4.begin()) 
  {
    Serial.println("Could not find DS18B20 sensor 4");
    led.setPattern(panic);
  }
  
  thing.begin();
}

void getValue1(Value& v)
{
  getValue(ds1, 1, v);
}

void getValue2(Value& v)
{
  getValue(ds2, 2, v);
}

void getValue3(Value& v)
{
  getValue(ds3, 3, v);
}

void getValue4(Value& v)
{
  getValue(ds4, 4, v);
}

void getValue(DS18B20& ds, uint8_t num, Value& value)
{
  Return<float> temperature = ds.temperature();
  if (!temperature.valid())
  {
    Serial.println(String("Error ") + num + " " + temperature.code());
    led.setPattern(panic);
    return;
  }
  Serial.println(String("Read ") + num + " " + (float)temperature);
  led.setPattern(normal);
  value = (float)temperature;
}


void loop()
{
  thing.handle();
  led.handle();
}
