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

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;
int BPM,sp;
float t;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup()
{
    Serial.begin(115200);

    Serial.println("Initializing MAX30100");

    // Initialize the PulseOximeter instance
    pox.begin();

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the
    //   available options.
    // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
	BPM=pox.getHeartRate();
	sp=pox.getSpO2();
	t=pox.getTemperature();
    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print("bpm / SpO2:");
        Serial.print(sp);
        Serial.print("% / temp:");
        Serial.print(t);
        Serial.println("C");
		Particle.publish("BPM", String(BPM));
		Particle.publish("spo2", String(sp));
		Particle.publish("temp", String(t));
        tsLastReport = millis();
    }
}
