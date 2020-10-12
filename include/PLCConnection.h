#pragma once
#include "SimulationRuntimeApi.h"

class PLCConnection
{
public:
	static const int	maxInstanceNameLength = 255;
private:
	ISimulationRuntimeManager* RuntimeManager;
	IInstance* PLCSimInstance;
private:
	wchar_t		InstanceNameWS[maxInstanceNameLength + 1];
	int			InputPortCount;
	int			OutputPortCount;
	double		ScaleFactor;
	bool		SupplyVoltageExplicit;

public:
	PLCConnection(const int inputPortCount, const int outputPortCount, const double scaleFactor, const char* instanceName, const bool supplyVoltageExplicit);
	~PLCConnection();
	IInstance* getInstance();
	void uninitialize();
	void initialize(bool createNewInstanceIfNotExist);
	const bool isSupplyVoltageExplicit() noexcept;
	const bool isInitialized() noexcept;
	const int getInputPortCount() noexcept;
	const int getOutputPortCount() noexcept;
};

