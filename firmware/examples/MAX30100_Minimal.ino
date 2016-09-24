/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//#include <Wire.h>
#include "ThingSpeak.h"
#include "Adafruit_MLX90614.h"
#include "MAX30100_PulseOximeter.h"
#include "application.h"
#define REPORTING_PERIOD_MS     1000
#define slaveAddress 0x5A
unsigned int myChannelNumber = 154087; // replace with your ChannelID
const char * myWriteAPIKey = "1VQ79H4AGL68UM5E"; // replace with your WriteAPIKey
TCPClient client;

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int BPM,sp;
float t;
uint32_t tsLastReport = 0;
uint32_t tsLastReport1 = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{

    Serial.begin(115200);
    ThingSpeak.begin(client);
    mlx.begin();
    // Initialize the PulseOximeter instance and register a beat-detected callback
    pox.begin();
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
     pox.update();
     BPM=pox.getHeartRate();
     sp=pox.getSpO2();
     t=mlx.readObjectTempC();

Serial.print("Heart rate:");
Serial.print(BPM);
Serial.print("bpm / SpO2:");
Serial.print(sp);
Serial.print("% ");
Serial.print("Temp:");
Serial.println(t);
if (millis() - tsLastReport1 > 20000) {
  ThingSpeak.setField(1,BPM);
  ThingSpeak.setField(2,sp);
  ThingSpeak.setField(3,t);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Particle.publish("BPM", String(BPM));
Particle.publish("spo2", String(sp));
Particle.publish("temp", String(t));
tsLastReport1 = millis();
}
//ThingSpeak.setField(1,BPM);
//ThingSpeak.setField(2,sp);
//ThingSpeak.setField(3,t);
//ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

}
