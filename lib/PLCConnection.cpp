#include "PLCConnection.h"
#include <exception>
#include <string>
#include "helper_functions.h"

/**
** Callback function which is called by PLCSIM Advanced  when configuration changes
**/
static void ConfigurationChanged(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime, EInstanceConfigChanged in_InstanceConfigChanged, UINT32 in_Param1, UINT32 in_Param2, UINT32 in_Param3, UINT32 in_Param4)
{
}
PLCConnection::PLCConnection(const int inputPortCount, const int outputPortCount, const double scaleFactor, const char* instanceName, const bool supplyVoltageExplicit):
	InputPortCount(inputPortCount), OutputPortCount(outputPortCount), ScaleFactor(scaleFactor), SupplyVoltageExplicit(supplyVoltageExplicit),
	PLCSimInstance(NULL), RuntimeManager(NULL)
{
	size_t convertedChars;
	mbstowcs_s(&convertedChars, InstanceNameWS, maxInstanceNameLength + 1, instanceName, _TRUNCATE);
}
PLCConnection::~PLCConnection()
{
	uninitialize();
}
IInstance* PLCConnection::getInstance()
{
	return PLCSimInstance;
}

const bool PLCConnection::isSupplyVoltageExplicit() noexcept
{
	return SupplyVoltageExplicit;
}
const int PLCConnection::getInputPortCount() noexcept
{
	return InputPortCount;
}
const int PLCConnection::getOutputPortCount() noexcept
{
	return OutputPortCount;
}

void PLCConnection::uninitialize()
{
	/* Shut down PLCSIM Advanced interface, disconnect from runtime manager and return to the initial system state */
	if (PLCSimInstance != NULL)
	{
		::PrintText("Disconnecting from PLC...");
		ERuntimeErrorCode srec = DestroyInterface(PLCSimInstance);
		if (srec != SREC_OK)
			throw ::CreatePLCException(srec, "Could not destroy PLC interface.\n");
		PLCSimInstance = NULL;
		::PrintText("Disconnected from PLC.");
	}

	if (RuntimeManager != NULL)
	{
		::PrintText("Deactivating PLCSim Advanced API...");
		ERuntimeErrorCode srec = ShutdownAndFreeApi(RuntimeManager);
		if (srec != SREC_OK)
			throw ::CreatePLCException(srec, "Could not shutdown PLCSIM Advanced API.\n");
		RuntimeManager = NULL;
		::PrintText("PLCSim Advanced API deactivated.");
	}
}

const bool PLCConnection::isInitialized() noexcept
{
	return RuntimeManager != NULL && PLCSimInstance != NULL;
}

/* Main startup procedure for the PLCSIM Advanced S-Function */
void PLCConnection::initialize(bool createNewInstanceIfNotExist)
{
	uninitialize();

	if (RuntimeManager != NULL && PLCSimInstance != NULL)
		return;

	enum ERuntimeErrorCode srec = InitializeApi(&RuntimeManager);
	if (srec != SREC_OK)
	{
		uninitialize();
		throw ::CreatePLCException(srec, "Could not initialize PLCSIM Advanced API.");
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
				throw ::CreatePLCException(srec, "Could not create PLC instance %ls.", InstanceNameWS);
			}
			srec = PLCSimInstance->SetStoragePath(L"C:\\temp\\PLCConnection");
			if (srec != SREC_OK)
			{
				uninitialize();
				throw ::CreatePLCException(srec, "Could not set storage path of PLC instance %ls.", InstanceNameWS);
			}

			srec = PLCSimInstance->PowerOn();
			if (srec == SREC_WARNING_TRIAL_MODE_ACTIVE)
			{
				// do nothing.
			}
			else if (srec == SREC_STORAGE_PATH_ALREADY_IN_USE)
			{
				uninitialize();
				throw ::CreatePLCException(srec, "Could not set PLC %ls to ON state due to problem with storage path.", InstanceNameWS);
			}
			else if (srec != SREC_OK && srec != SREC_WARNING_ALREADY_EXISTS)
			{
				uninitialize();
				throw ::CreatePLCException(srec, "Could not set PLC %ls to ON state.", InstanceNameWS);
			}
		}
		else
		{
			throw ::CreatePLCException(srec, "PLC instance %ls does not exist.", InstanceNameWS);
		}
	}
	else if (srec != SREC_OK)
	{
		uninitialize();
		throw ::CreatePLCException(srec, "Could not connect to existing PLC instance %ls.", InstanceNameWS);
	}

	PLCSimInstance->RegisterOnConfigurationChangedEvent();
	PLCSimInstance->RegisterOnConfigurationChangedCallback(ConfigurationChanged);

	PLCSimInstance->SetOperatingMode(SROM_DEFAULT);


	srec = PLCSimInstance->SetScaleFactor(ScaleFactor);
	if (srec != SREC_OK)
	{
		uninitialize();
		throw ::CreatePLCException(srec, "Could not set time scaling factor of PLC %ls.", InstanceNameWS);
	}
}