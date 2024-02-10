/*
This is a library written for the TMD3719 colour and proximity sensor, communicating
via I2C and focusing on the ALS (colour) and proximity readings. 

Written by Andrew McDonald

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  ---FUNCTIONS AND DATATYPES---
  bool begin(TwoWire& wirePort = Wire, TMD3719_Config& cfg);
	bool dataAvailable(uint8_t dataType);
	void changeConfig(TMD3719_Config& cfg); 
	void calibrateProximityOffset();

	uint8_t getAUXID();
	uint8_t getREVID();
	uint8_t getID();

	ALSResults getALSResults();

	ProximityResults getProximityResults();

	// All config options - incomplete
	struct TMD3719_Config
	{
		uint8_t ALSDiodeSet = TMD3719_MEAS_MODE0_ALS_SET_A_DIODE;
		uint8_t gain = TMD3719_MOD_GAIN_64X;
		bool automatic_gain_control = false;
		uint8_t agcGain = 0x06;
		uint8_t proximityGain = TMD3719_PROX_GAIN_4X;
		uint8_t proxFilterSetting = TMD3719_PROX_FILTER_1;
		uint8_t ISINKScaler = TMD3719_ISINK_SCALER_0p5;
		uint8_t ISINKCurrent = TMD3719_ISINK_CURRENT_0mA;
	};

	struct ALSResults
	{
		int Clear;
		int Red;
		int Green;
		int Blue;
		int Leakage;
		int Wideband;
		int IR1;
		int IR2;
	};
	struct ProximityResults
	{
		int ProximityZero;
		int ProximityOne;
		int ProximityRatio;
		int EvalChannel;
	};
	*/

	#include "TMD3719.h"
	#include <Wire.h>

	TMD3719 sensor;

	void setup() {
		Wire.begin();
		Serial.begin(115200);
		sensor.begin();
	}

	void loop() {
		ProximityResults prox = sensor.getProximityResults();
		if (prox.EvalChannel == 0)
		{
		Serial.print("ProximityOne: ");
		Serial.print(prox.ProximityOne);
		
		}
		else
		{
			Serial.print("ProximityZero: ");
			Serial.print(prox.ProximityZero);
		}
		Serial.print(", ProximityRatio: ");
		Serial.print(prox.ProximityRatio);
		Serial.println();

		ALSResults als = sensor.getALSResults();
		Serial.print("Clear: ");
		Serial.print(als.Clear);
		Serial.print(", Red: ");
		Serial.print(als.Red);
		Serial.print(", Green: ");
		Serial.print(als.Green);
		Serial.print(", Blue: ");
		Serial.print(als.Blue);
		Serial.print(", Leakage: ");
		Serial.print(als.Leakage);
		Serial.print(", Wideband: ");
		Serial.print(als.Wideband);
		Serial.print(", IR1: ");
		Serial.print(als.IR1);
		Serial.print(", IR2: ");
		Serial.print(als.IR2);
		Serial.println();
	}