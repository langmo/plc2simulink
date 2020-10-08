#include "PLCConnection.h"
#include <exception>
#include <string>
#include "helper_functions.h"



//TODO: find out how to pass user data (pointer to PLCConnection) to this callback function and make it work again.	
/* Callback function for end-of-cycle event from PLCSIM Advanced */
static void EndOfCycle(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime, INT64 CycleTime_ns, UINT32 CycleCount)
{
	/*if(syncMethod == SyncWithSemaphore)
		ReleaseSemaphore(Semaphore,1,0);
	else if(syncMethod == SyncWithFlag)
		waiting = false;*/
}

/* Callback function for PLCSIM Advanced */
static void ConfigurationChanged(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime, EInstanceConfigChanged in_InstanceConfigChanged, UINT32 in_Param1, UINT32 in_Param2, UINT32 in_Param3, UINT32 in_Param4)
{
}

PLCConnection::PLCConnection(int inputPortCount, int outputPortCount, double scaleFactor, bool synchronization, char* instanceName) :
	InputPortCount(inputPortCount), OutputPortCount(outputPortCount), ScaleFactor(scaleFactor), TimeSynchronization(synchronization)
{
	size_t convertedChars;
	mbstowcs_s(&convertedChars, InstanceNameWS, maxInstanceNameLength + 1, instanceName, _TRUNCATE);
}
PLCConnection::~PLCConnection()
{
	uninitialize();
}
bool PLCConnection::IsTimeSynchronization()
{
	return TimeSynchronization;
}
PLCConnection::SynchronizationMethod PLCConnection::GetSynchronizationMethod()
{
	return syncMethod;
}
IInstance* PLCConnection::GetInstance()
{
	return PLCSimInstance;
}



void PLCConnection::uninitialize()
{
	/* Shut down PLCSIM Advanced interface, disconnect from runtime manager and return to the initial system state */
	if (PLCSimInstance != 0)
	{
		::PrintText("Disconnecting from PLC...");
		ERuntimeErrorCode srec = DestroyInterface(PLCSimInstance);
		if (srec == SREC_OK)
			PLCSimInstance = 0;
		else
			throw ::CreatePLCException(srec, "Could not destroy instance interface.\n");
	}

	if (RuntimeManager != 0)
	{
		::PrintText("Deactivating API...");
		ERuntimeErrorCode srec = ShutdownAndFreeApi(RuntimeManager);
		if (srec == SREC_OK)
			RuntimeManager = 0;
		else
			throw CreatePLCException(srec, "Could not shutdown PLCSIM Advanced API.\n");
	}
}

/* Main startup procedure for the PLCSIM Advanced S-Function */
void PLCConnection::initialize(bool createNewInstanceIfNotExist)
{
	uninitialize();

	if (RuntimeManager != 0 && PLCSimInstance != 0)
		return;

	enum ERuntimeErrorCode srec = InitializeApi(&RuntimeManager);
	if (srec != SREC_OK)
	{
		uninitialize();
		throw CreatePLCException(srec, "Could not initialize PLCSIM Advanced API.");
	}

	if (syncMethod == SyncWithSemaphore)
	{
		Semaphore = CreateSemaphore(0, 0, 1, 0);
		if (!Semaphore)
		{
			uninitialize();
			throw CreatePLCException(srec, "Could not initialize synchronization.");
		}
	}

	srec = RuntimeManager->CreateInterface(InstanceNameWS, &PLCSimInstance);
	if (srec == SREC_DOES_NOT_EXIST)
	{
		if (createNewInstanceIfNotExist)
		{
			srec = RuntimeManager->RegisterInstance(InstanceNameWS, &PLCSimInstance);
			if (srec != SREC_OK)
			{
				uninitialize();
				throw CreatePLCException(srec, "Could not create PLC instance %ls.", InstanceNameWS);
			}
			srec = PLCSimInstance->SetStoragePath(L"C:\\temp\\PLCConnection");
			if (srec != SREC_OK)
			{
				uninitialize();
				throw CreatePLCException(srec, "Could not set storage path of PLC instance %ls.", InstanceNameWS);
			}

			srec = PLCSimInstance->PowerOn();
			if (srec == SREC_WARNING_TRIAL_MODE_ACTIVE)
			{
				// do nothing.
			}
			else if (srec == SREC_STORAGE_PATH_ALREADY_IN_USE)
			{
				uninitialize();
				throw CreatePLCException(srec, "Could not set PLC %ls to ON state due to problem with storage path.", InstanceNameWS);
			}
			else if (srec != SREC_OK && srec != SREC_WARNING_ALREADY_EXISTS)
			{
				uninitialize();
				throw CreatePLCException(srec, "Could not set PLC %ls to ON state.", InstanceNameWS);
			}
		}
		else
		{
			throw CreatePLCException(srec, "PLC instance %ls does not exist.", InstanceNameWS);
		}
	}
	else if (srec != SREC_OK)
	{
		uninitialize();
		throw CreatePLCException(srec, "Could not connect to existing PLC instance %ls.", InstanceNameWS);
	}
	
	if (syncMethod == SyncWithSemaphore || syncMethod == SyncWithFlag)
		PLCSimInstance->RegisterOnEndOfCycleCallback(EndOfCycle);
	else if (syncMethod == SyncWithEvent)
		PLCSimInstance->RegisterOnEndOfCycleEvent();

	PLCSimInstance->RegisterOnConfigurationChangedEvent();
	PLCSimInstance->RegisterOnConfigurationChangedCallback(ConfigurationChanged);

	if (syncMethod == NoSync)
		PLCSimInstance->SetOperatingMode(SROM_DEFAULT);
	else
		PLCSimInstance->SetOperatingMode(SROM_SINGLE_STEP);


	srec = PLCSimInstance->SetScaleFactor(ScaleFactor);
	if (srec != SREC_OK)
	{
		uninitialize();
		throw CreatePLCException(srec, "Could not set time scaling factor of PLC %ls.", InstanceNameWS);
	}

	return;
}
/* Wait for the end of the PLC cycle. */
void PLCConnection::WaitForEndOfCycle()
{
	if (syncMethod == SyncWithSemaphore)
	{
		WaitForSingleObject(Semaphore, 10 * 1000);
	}
	else if (syncMethod == SyncWithFlag)
	{
		waiting = true;
		while (waiting)
			Sleep(10);
	}
	else if (syncMethod == SyncWithEvent)
	{
		// Wait up to 10 seconds for the end of the cycle.
		if (!PLCSimInstance->WaitForOnEndOfCycleEvent(10 * 1000))
		{
			throw ::CreateException("Timeout ocurred while waiting for end of cycle from PLCSIM Advanced.");
		}
	}
}