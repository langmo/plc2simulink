#pragma once
#include "SimulationRuntimeApi.h"

class PLCConnection
{
public:
	enum SynchronizationMethod
	{
		SyncWithSemaphore,
		SyncWithFlag,
		SyncWithEvent,
		NoSync
	};
	static const SynchronizationMethod syncMethod = NoSync;
	static const int	maxInstanceNameLength = 255;
private:
	ISimulationRuntimeManager* RuntimeManager = 0;
	IInstance* PLCSimInstance = 0;

	union
	{
		HANDLE Semaphore = 0;
		bool waiting;
	};

	wchar_t		InstanceNameWS[maxInstanceNameLength + 1];
	int			InputPortCount = -1;
	int			OutputPortCount = -1;
	double		ScaleFactor = 1;
	bool		TimeSynchronization = false;

public:
	PLCConnection(int inputPortCount, int outputPortCount, double scaleFactor, bool synchronization, char* instanceName);
	~PLCConnection();
	bool IsTimeSynchronization();
	IInstance* GetInstance();
	SynchronizationMethod GetSynchronizationMethod();
	void uninitialize();

	/* Main startup procedure for the PLCSIM Advanced S-Function */
	void initialize(bool createNewInstanceIfNotExist);
	void WaitForEndOfCycle();
};

