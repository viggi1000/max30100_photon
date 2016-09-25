/*
Port of the Oxullo Interscans library for Particle Photon/Electron.
Work by Vignesh Ravichandran (hello@rvignesh.xyz).

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
#include "MAX30100/MAX30100_PulseOximeter.h"
#include "application.h"
#define REPORTING_PERIOD_MS     1000
#define slaveAddress 0x5A


// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;
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
    // Initialize the PulseOximeter instance and register a beat-detected callback
    pox.begin();
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
pox.update();
BPM=pox.getHeartRate();
sp=pox.getSpO2();
Serial.print("Heart rate:");
Serial.print(BPM);
Serial.print("bpm / SpO2:");
Serial.print(sp);
Serial.println("% ");
    
if (millis() - tsLastReport1 > 20000) {
Particle.publish("BPM", String(BPM));
Particle.publish("spo2", String(sp));
Particle.publish("temp", String(t));
tsLastReport1 = millis();
}

}
