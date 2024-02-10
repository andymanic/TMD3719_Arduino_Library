
#ifndef TMD3719_h
#define TMD3719_h

#include "Arduino.h"
#include "Wire.h"
class TMD3719 {
public:
	TMD3719();
	bool begin(TwoWire& wirePort = Wire, TMD3719_Config& cfg);
	bool dataAvailable(uint8_t dataType);  // 0x40 for ALS, 0x80 for proximity
	void changeConfig(TMD3719_Config& cfg); // as PON needs cycling during changes, wrapped method needed
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

private:
	TwoWire	*_i2cPort;
	int get16bValue(uint8_t reg);
	int get24bValue(uint8_t reg);
	uint8_t readRegister(uint8_t address);
	void writeRegister(uint8_t address, uint8_t data);

	void setDiodeSet(uint8_t set); 

	void setIntegrationTime(uint8_t ATime); // number of integration steps - 0 = ASTEP time, 255 = 256 x ASTEP time
	void setIntegrationStep(uint16_t AStep); // steps of 2.71us - 0 = 2.71us, 65535 = 178ms

	void setGain(uint8_t gain);				// light sensor gain
	void setAGCEnabled(bool agcEn);			// enable automatic gain control
	void setMaxAGCGain(uint8_t gain);			// automatic gain control setting
	void setProxGain(uint8_t pGain);

	void setProxFilterEnabled(bool proxEn);			// enable proximity filter
	void setProxFilter(uint8_t filter);				// proximity filter setting

	void setISINKScaler(uint8_t scaler);
	void setISINKCurrent(uint8_t current);



#define TMD3719_ADDR							0x39
#define POLLING_DELAY							5		// millisecond delay between checking registers

#define TMD3719_REG_AILT						0x2E
#define TMD3719_REG_AIHT						0x31
#define TMD3719_REG_PILT0						0x34
#define TMD3719_REG_PIHT0						0x36
#define TMD3719_REG_PILT1						0x38
#define TMD3719_REG_PIHT1						0x3A
#define TMD3719_REG_PILTR						0x3C
#define TMD3719_REG_PIHTR						0x3E
#define TMD3719_REG_FD_CFG0						0x40
#define TMD3719_REG_FD_CFG1						0x41
#define TMD3719_REG_FD_CFG2						0x42
#define TMD3719_REG_FD_CFG3						0x43
#define TMD3719_REG_FD_CFG4						0x44
#define TMD3719_REG_FD_CFG5						0x45
#define TMD3719_REG_FD_CFG6						0x46
#define TMD3719_REG_FD_CFG7						0x47
#define TMD3719_REG_FD_CFG8						0x48
#define TMD3719_REG_SYNC_DELAY					0x4F
#define TMD3719_REG_VSYNC_CFG0					0x53
#define TMD3719_REG_VSYNC_CFG1					0x54
#define TMD3719_REG_VSYNC_CFG2					0x56
#define TMD3719_REG_VSYNC_CFG3					0x57
#define TMD3719_REG_CALIB						0x69
#define TMD3719_REG_CALIBCFG0					0x6A
#define TMD3719_REG_CALIBCFG1					0x6B
#define TMD3719_REG_CALIBCFG2					0x6C
#define TMD3719_REG_CALIBSTAT					0x6D
#define TMD3719_REG_ENABLE						0x80
#define TMD3719_REG_MEAS_MODE0					0x81
#define TMD3719_REG_MEAS_MODE1					0x82
#define TMD3719_REG_TRIGGER_MODE				0x83
#define TMD3719_REG_ATIME						0x84
#define TMD3719_REG_ASTEP						0x85
#define TMD3719_REG_PTIME						0x87
#define TMD3719_REG_WTIPE						0x88
#define TMD3719_REG_MOD_GAIN_0_1				0x89
#define TMD3719_REG_MOD_GAIN_2_3				0x8A
#define TMD3719_REG_MOD_GAIN_4_5				0x8B
#define TMD3719_REG_MOD_GAIN_6_7				0x8C
#define TMD3719_REG_AGC_ENABLE					0x8D
#define TMD3719_REG_AUXID						0x90
#define TMD3719_REG_REVID						0x91
#define TMD3719_REG_ID							0x92
#define TMD3719_REG_CFG0						0x93
#define TMD3719_REG_CFG3						0x96
#define TMD3719_REG_CFG4						0x97
#define TMD3719_REG_PERS						0x9A
#define TMD3719_REG_CFG8						0x9B
#define TMD3719_REG_CFG9						0x9C
#define TMD3719_REG_CFG10						0x9D
#define TMD3719_REG_CFG11						0x9E
#define TMD3719_REG_CFG12						0x9F
#define TMD3719_REG_AZ_CONFIG					0xA4
#define TMD3719_REG_CFG20						0xA7
#define TMD3719_REG_PCFG1						0xAA
#define TMD3719_REG_PCFG2						0xAB
#define TMD3719_REG_PCFG3						0xAC
#define TMD3719_REG_PCFG4						0xAD
#define TMD3719_REG_PCFG5						0xAE
#define TMD3719_REG_PCFG6						0xAF
#define TMD3719_REG_PCFG7						0xB0
#define TMD3719_REG_POFFSET0					0xB1
#define TMD3719_REG_PXAVG						0xB3
#define TMD3719_REG_PBSLN						0xB5
#define TMD3719_REG_STATUS						0xB7
#define TMD3719_REG_STATUS2						0xB8		// bit 7 proximity valid, bit 6 ALS valid
#define TMD3719_REG_STATUS3						0xB9		// ALS Channel saturation
#define TMD3719_REG_STATUS4						0xBA
#define TMD3719_REG_STATUS5						0xBB
#define TMD3719_REG_STATUS7						0xBD
#define TMD3719_REG_STATUS8						0xBE
#define TMD3719_REG_STATUS9						0xBF
#define TMD3719_REG_ASTATUS						0xC0
#define TMD3719_REG_ADATA0						0xC1		// CLEAR
#define TMD3719_REG_ADATA1						0xC4		// RED
#define TMD3719_REG_ADATA2						0xC7		// GREEN
#define TMD3719_REG_ADATA3						0xCA		// BLUE
#define TMD3719_REG_ADATA4						0xCD		// LEAKAGE
#define TMD3719_REG_ADATA5						0xD0		// WIDEBAND
#define TMD3719_REG_ADATA6						0xD3		// IR1
#define TMD3719_REG_ADATA7						0xD6		// IR2
#define TMD3719_REG_PSTATUS						0xD9
#define TMD3719_REG_PDATA0						0xDA
#define TMD3719_REG_PDATA1						0xDC
#define TMD3719_REG_PDATAR						0xDE
#define TMD3719_REG_GPIO						0xF2
#define TMD3719_REG_AGC_GAIN_MAX				0xF4
#define TMD3719_REG_INTENAB						0xF5
#define TMD3719_REG_CONTROL						0xF6
#define TMD3719_REG_POFFSET1					0xF7
#define TMD3719_REG_FIFO_MAP					0xF9
#define TMD3719_REG_FIFO_MAP2					0xFA
#define TMD3719_REG_FIFO_STATUS					0xFB
#define TMD3719_REG_FDATA						0xFC

// Enable sensors
#define TMD3719_ENABLE_FLICKER					0x40
#define TMD3719_ENABLE_ALS						0x10
#define TMD3719_ENABLE_PROX						0x08
#define TMD3719_ENABLE_POWER					0x01

// Measure mode setup
#define TMD3719_MEAS_MODE0_CL_APF_VSYNC			0x40
#define TMD3719_MEAS_MODE0_EN_PROX_DIODE		0x0C
#define TMD3719_MEAS_MODE0_ALS_SET_A_DIODE		0x00
#define TMD3719_MEAS_MODE0_ALS_SET_B_DIODE		0x02 
#define TMD3719_MEAS_MODE0_EN_ALS_DIODE			0x01

#define TMD3719_MOD_GAIN_2X						0x00
#define TMD3719_MOD_GAIN_4X						0x01
#define TMD3719_MOD_GAIN_8X						0x02
#define TMD3719_MOD_GAIN_16X					0x03
#define TMD3719_MOD_GAIN_32X					0x04
#define TMD3719_MOD_GAIN_64X					0x05
#define TMD3719_MOD_GAIN_128X					0x06
#define TMD3719_MOD_GAIN_256X					0x07
#define TMD3719_MOD_GAIN_512X					0x08
#define TMD3719_MOD_GAIN_1024X					0x09
#define TMD3719_MOD_GAIN_2048X					0x0A
#define TMD3719_MOD_GAIN_4096X					0x0B

// PROX_FILTER_DOWNSAMPLE added to all positive values
#define TMD3719_PROX_FILTER_1					0x00
#define TMD3719_PROX_FILTER_2					0x09
#define TMD3719_PROX_FILTER_3					0x0A
#define TMD3719_PROX_FILTER_4					0x0B
#define TMD3719_PROX_FILTER_5					0x0C
#define TMD3719_PROX_FILTER_6					0x0D
#define TMD3719_PROX_FILTER_7					0x0E
#define TMD3719_PROX_FILTER_8					0x0F

#define TMD3719_ISINK_SCALER_0p5				0x00
#define TMD3719_ISINK_SCALER_1					0x01
#define TMD3719_ISINK_SCALER_1p5				0x02
#define TMD3719_ISINK_SCALER_2					0x03
#define TMD3719_ISINK_CURRENT_0mA				0x00
#define TMD3719_ISINK_CURRENT_1mA				0x01
#define TMD3719_ISINK_CURRENT_2mA				0x02
#define TMD3719_ISINK_CURRENT_3mA				0x03
#define TMD3719_ISINK_CURRENT_4mA				0x04
#define TMD3719_ISINK_CURRENT_5mA				0x05
#define TMD3719_ISINK_CURRENT_6mA				0x06
#define TMD3719_ISINK_CURRENT_7mA				0x07

#define TMD3719_PROX_GAIN_1X					0x00
#define TMD3719_PROX_GAIN_2X					0x01
#define TMD3719_PROX_GAIN_4X					0x02
#define TMD3719_PROX_GAIN_8X					0x03

#define TMD3719_AUXID							0x90
#define TMD3719_REVID							0x91
#define TMD3719_ID								0x92

// Data Valid Bits
#define ALS_VALID								0x40
#define PROX_VALID								0x80


};

#endif