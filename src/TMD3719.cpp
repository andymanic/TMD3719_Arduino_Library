

#include "TMD3719.h"
#include "Arduino.h"

TMD3719::TMD3719()
{

}

bool TMD3719::begin(TwoWire& wirePort = Wire, TMD3719_Config& cfg)
{
	_i2cPort = &wirePort;
	uint8_t id = getID();
	if (id != 0x40)
	{
		return (false);
	}
	changeConfig(cfg);
	return (true);
}


bool TMD3719::dataAvailable(uint8_t dataType)
{
	uint8_t validByte = readRegister(TMD3719_REG_STATUS2);
	if (dataType == ALS_VALID)
	{
		if (validByte & ALS_VALID)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}
	else if (dataType == PROX_VALID)
	{
		if (validByte & PROX_VALID)
		{
			return(true);
		}
		else
		{
			return(false);
		}
	}
	else
	{
		Serial.println("Incorrect data type");
		return(false);
	}
}

void TMD3719::changeConfig(TMD3719_Config& cfg)
{
	writeRegister(TMD3719_REG_ENABLE, 0x00); // Disable before changing settings
	setDiodeSet(cfg.ALSDiodeSet);
	setGain(cfg.gain);
	setAGCEnabled(cfg.automatic_gain_control);
	setMaxAGCGain(cfg.agcGain);
	setProxGain(cfg.proximityGain);
	setProxFilter(cfg.proxFilterSetting);
	setISINKScaler(cfg.ISINKScaler);
	setISINKCurrent(cfg.ISINKCurrent);
	writeRegister(TMD3719_REG_ENABLE, 0b01011001); // enable everything
}

void TMD3719::calibrateProximityOffset()
{
	writeRegister(TMD3719_REG_CALIB, 0x01); // Start offset calibration
	delay(100);
	for (int i = 0; i < 300; i++)
	{
		uint8_t calibFinished = readRegister(TMD3719_REG_CALIBSTAT);
		if (calibFinished & 0x01)
		{
			writeRegister(TMD3719_REG_CALIBSTAT, 0x01);
		}
		delay(10);
	}
}


void TMD3719::setDiodeSet(uint8_t set)
{
	uint8_t existingValue = readRegister(TMD3719_REG_MEAS_MODE0);
	// check if VSYNC on, otherwise set 000011x1
	uint8_t setValue = set | TMD3719_MEAS_MODE0_EN_ALS_DIODE | TMD3719_MEAS_MODE0_EN_PROX_DIODE;
	uint8_t newValue = existingValue | (setValue & 0b00001111);
	writeRegister(TMD3719_REG_MEAS_MODE0, newValue);
}

void TMD3719::setIntegrationTime(uint8_t ATime)
{
	// set ASTEP Multiple (ASTEP x (ATime + 1)
	writeRegister(TMD3719_REG_ATIME, ATime);
}
void TMD3719::setIntegrationStep(uint16_t AStep)
{
	// 16 bit, 0x85 low byte, 0x86 high byte
	uint8_t highByte = (uint8_t)((AStep & 0xFF00) >> 8);
	uint8_t lowByte = (unit8_t)(AStep & 0x00FF);
	writeRegister(TMD3719_REG_ASTEP, lowByte);
	writeRegister(TMD3719_REG_ASTEP + 1, highByte);
}

// Gain Control
void TMD3719::setGain(uint8_t gain)
{
	if (gain > TMD3719_MOD_GAIN_4096X)
	{
		gain = TMD3719_MOD_GAIN_4096X;
	}
	uint8_t fullGain = gain << 4;
	fullGain |= gain;
	writeRegister(TMD3719_REG_MOD_GAIN_0_1, fullGain);
	writeRegister(TMD3719_REG_MOD_GAIN_2_3, fullGain);
	writeRegister(TMD3719_REG_MOD_GAIN_4_5, fullGain);
	writeRegister(TMD3719_REG_MOD_GAIN_6_7, fullGain);
}
void TMD3719::setAGCEnabled(bool agcEn)
{
	// AGC_ENABLE for all modulators, either 0x00 for off or 0xFF for all
	uint8_t AGC = 0x00;
	if (agcEn)
	{
		AGC = 0xFF;
	}
	writeRegister(TMD3719_REG_AGC_ENABLE, AGC);
}
void TMD3719::setMaxAGCGain(uint8_t gain)
{
	writeRegister(TMD3719_REG_AGC_GAIN_MAX, (gain & 0x07)); // masked off reserved bits
}
void TMD3719::setProxGain(uint8_t pGain)
{
	uint8_t pgain2 = pGain << 4;
	pgain2 |= pGain;
	writeRegister(TMD3719_REG_PCFG4, pgain2);
}

void TMD3719::setProxFilter(uint8_t filter)
{
	uint8_t existingValue = readRegister(TMD3719_REG_PCFG1);
	uint8_t newValue = existingValue | (filter & 0x0F);
	writeRegister(TMD3719_REG_PCFG1, newValue);
}

void TMD3719::setISINKScaler(uint8_t scaler)
{
	uint8_t existingValue = readRegister(TMD3719_REG_PCFG2);
	uint8_t setValue = scaler << 6;
	setValue |= scaler << 4;
	uint8_t newValue = existingValue | (setValue & 0xF0);
	writeRegister(TMD3719_REG_PCFG2, newValue);
}
void TMD3719::setISINKCurrent(uint8_t current)
{
	uint8_t existingValue = readRegister(TMD3719_REG_PCFG2);
	uint8_t newValue = existingValue | (current & 0x07);
	writeRegister(TMD3719_REG_PCFG2, newValue);
	uint8_t setValue = current << 4;
	setValue |= current;
	writeRegister(TMD3719_REG_PCFG3, setValue);
}

// Identification
uint8_t TMD3719::getAUXID()
{
	return readRegister(TMD3719_REG_AUXID)
}
uint8_t TMD3719::getREVID()
{
	return readRegister(TMD3719_REG_REVID)
}
uint8_t TMD3719::getID()
{
	return readRegister(TMD3719_REG_ID)
}

// ALS Results
ALSResults TMD3719::getALSResults()
{
	uint8_t ATIME = readRegister(TMD3719_REG_ATIME);
	uint16_t ASTEP = get16bValue(TMD3719_REG_ASTEP);
	int sampleTime = ATIME * (ASTEP + 1) * 2.71;
	sampleTime /= 1000;
	uint8_t astatus = readRegister(TMD3719_REG_ASTATUS); // read ASTATUS reg to start fresh sampling
	delay(sampleTime); // delay by sample time
	while (dataAvailable(ALS_VALID) == false) { delay(POLLING_DELAY); } // wait for AVALID to go high
	ALSResults als;
	als.Clear = get24bValue(TMD3719_REG_ADATA0);
	als.Red = get24bValue(TMD3719_REG_ADATA1);
	als.Green = get24bValue(TMD3719_REG_ADATA2);
	als.Blue = get24bValue(TMD3719_REG_ADATA3);
	als.Leakage = get24bValue(TMD3719_REG_ADATA4);
	als.Wideband = get24bValue(TMD3719_REG_ADATA5);
	als.IR1 = get24bValue(TMD3719_REG_ADATA6);
	als.IR2 = get24bValue(TMD3719_REG_ADATA7);
	return(als);
}

// Proximity Results
ProximityResults TMD3719::getProximityResults()
{
	// read PDATA to trigger sample taking (pstatus??)
	uint8_t pstatus = readRegister(TMD3719_REG_PSTATUS); // read PSTATUS reg to start fresh sampling
	
	while (dataAvailable(PROX_VALID) == false) { delay(POLLING_DELAY); } // wait for PVALID to go high
	ProximityResults prox;
	prox.ProximityZero = get16bValue(TMD3719_REG_PDATA0);
	prox.ProximityOne = get16bValue(TMD3719_REG_PDATA1);
	prox.ProximityRatio = get16bValue(TMD3719_REG_PDATAR);
	uint8_t pcfg6 = readRegister(TMD3719_REG_PCFG6);
	prox.EvalChannel = pcfg6 >> 7; // return just the eval channel
	return(prox);
}


// Private methods
int TMD3719::get16bValue(uint8_t reg)
{
	int fullByte = readRegister(reg + 1) << 8;
	fullByte |= readRegister(reg);
	return(fullByte);
}
int TMD3719::get24bValue(uint8_t reg)
{
	int fullByte = readRegister(reg + 2) << 16;
	fullByte |= readRegister(reg + 1) << 8;
	fullByte |= readRegister(reg);
	return(fullByte);
}

uint8_t TMD3719::readRegister(uint8_t address)
{
	_i2cPort->beginTransmission(TMD3719_ADDR);
	_i2cPort->write(addr);
	_i2cPort->endTransmission();

	_i2cPort->requestFrom(TMD3719_ADDR, 1);
	if (_i2cPort->available()) {
		return (_i2cPort->read());
	}
	else {
		Serial.println("I2C Error - Unable to read data");
		return (0xFF);
	}
}
void TMD3719::writeRegister(uint8_t address, uint8_t data)
{
	_i2cPort->beginTransmission(TMD3719_ADDR);
	_i2cPort->write(address);
	_i2cPort->write(data);
	_i2cPort->endTransmission();
}