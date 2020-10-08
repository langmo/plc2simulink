#include <stdio.h>
#include <io.h>
#include <exception>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <TlHelp32.h>
#else
#error PLCSimAdvancedSFunction requires Microsoft Windows
#endif

#define S_FUNCTION_NAME SPSVirtualLab
#define S_FUNCTION_LEVEL 2

// Activate additional MEX functions which are used in this model.
#define MDL_START
#define MDL_SET_INPUT_PORT_DATA_TYPE
#define MDL_SET_OUTPUT_PORT_DATA_TYPE
#define MDL_SET_DEFAULT_PORT_DATA_TYPES

#include "simstruc.h"
#include "SimulationRuntimeApi.h"
#include "PLCConnection.h"
#include "helper_functions.h"

/*** Global Defines ***/




/* Simulink Block Parameters */
static const int SFCT_NUM_PARAMETERS = 6;
static const int ParameterIndex_InstanceName		= 0;
static const int ParameterIndex_InputPortCount	= 1;
static const int ParameterIndex_OutputPortCount	= 2;
static const int ParameterIndex_SampleTime = 3;
static const int ParameterIndex_ScaleFactor		= 4;
static const int ParameterIndex_Synchronization	= 5;


/* Reference time to be mapped to time 0.0 in Simulink */
static const SYSTEMTIME basePLCSimTime =
{
  2000,	// WORD wYear;
  1,	// WORD wMonth;
  0,	// WORD wDayOfWeek;
  1,	// WORD wDay;
  0,	// WORD wHour;
  0,	// WORD wMinute;
  0,	// WORD wSecond;
  0		// WORD wMilliseconds;
};

static void setPLC(SimStruct* S, PLCConnection* plc)
{
	void** PWork = ssGetPWork(S);
	PWork[0] = plc;
}
static PLCConnection* getPLC(SimStruct* S)
{
	void** PWork = ssGetPWork(S);
	if(PWork == NULL)
	{
		return NULL;
	}
	if(PWork[0] == NULL)
	{
		return NULL;
	}
	return (PLCConnection*)PWork[0];
}
/* Convert SYSTEMTIME to ULONGLONG (number of 100-nanosecond intervals since January 1, 1601 (UTC)) */
static ULONGLONG SystemTimeToULongLong(SYSTEMTIME s)
{
	FILETIME t;
	if (!SystemTimeToFileTime(&s,&t))
	{
        ::PrintText("Error: Could not convert SYSTEMTIME to FILETIME (error code %d).", GetLastError());
        return 0;
    }
	ULARGE_INTEGER i;
	i.LowPart = t.dwLowDateTime;
	i.HighPart = t.dwHighDateTime;
	return i.QuadPart;
}

/* Convert ULONGLONG (number of 100-nanosecond intervals since January 1, 1601 (UTC)) to SYSTEMTIME */
static SYSTEMTIME ULongLongToSystemTime(ULONGLONG u)
{
	ULARGE_INTEGER i;
	i.QuadPart = u;
	FILETIME t;
	t.dwLowDateTime = i.LowPart;
	t.dwHighDateTime = i.HighPart;
	SYSTEMTIME s;
	if (!FileTimeToSystemTime(&t,&s))
	{
        ::PrintText("Could not convert FILETIME to SYSTEMTIME (error code %d).", GetLastError());
		ZeroMemory(&s,sizeof(s));
        return s;
    }
	return s;
}

/* Convert Simulink time (time_T) to PLCSim time, using the PLCSim base time */
static SYSTEMTIME ConvertSimulinkTimeToPLCSimTime(time_T simulinkTime)
{
	ULONGLONG plcsimBlockTime = (ULONGLONG)(simulinkTime * 10000000);
	ULONGLONG basePlcsimTimeULL = SystemTimeToULongLong(basePLCSimTime);
	return ULongLongToSystemTime(plcsimBlockTime + basePlcsimTimeULL);
}

/* Convert PLCSim time to SYSTEMTIME Simulink time (time_T), using the PLCSim base time */
static time_T ConvertPLCSimTimeToSimulinkTime(SYSTEMTIME plcsimTime)
{
	ULONGLONG plcsimTimeULL = SystemTimeToULongLong(plcsimTime);
	ULONGLONG basePlcsimTimeULL = SystemTimeToULongLong(basePLCSimTime);
	ULONGLONG plcsimBlockTime = plcsimTimeULL - basePlcsimTimeULL;
	return ((time_T)plcsimBlockTime) / 10000000;
}

/* Invert the byte order of a byte array, non-destructive */
inline static void InvertByteOrder(BYTE inbytes[], BYTE outbytes[], int_T numbytes)
{
	for (int_T b = 0; b < numbytes; ++b)
	{
		outbytes[b] = inbytes[numbytes - 1 - b];
	}
}

/* Invert the byte order of a byte array, in place */
inline static void InvertByteOrder(BYTE inoutbytes[], int_T numbytes)
{
	for (int_T b1 = 0; b1 < (numbytes / 2); ++b1)
	{
		int_T b2 = numbytes - 1 - b1;
		inoutbytes[b1] ^= inoutbytes[b2];
		inoutbytes[b2] ^= inoutbytes[b1];
		inoutbytes[b1] ^= inoutbytes[b2];
	}
}


inline static boolean_T checkIODataType(DTypeId dataType)
{
	boolean_T isAcceptable =
		(dataType == SS_DOUBLE ||
			dataType == SS_SINGLE ||
			dataType == SS_INT8 ||
			dataType == SS_UINT8 ||
			dataType == SS_INT16 ||
			dataType == SS_UINT16 ||
			dataType == SS_INT32 ||
			dataType == SS_UINT32 ||
			dataType == SS_BOOLEAN);

	return isAcceptable;
}

/*** Behavior Functions ***/
template<typename T> bool convertInputToBool(InputPtrsType inputPtr)
{
	return ((T)inputPtr) != 0;
}
inline bool getInputAsBool(SimStruct* S, int_T port)
{
	DTypeId   dataType = ssGetInputPortDataType(S, port);
	InputPtrsType dataPtr = ssGetInputPortSignalPtrs(S, port);
	switch (dataType)
	{
	case SS_DOUBLE:
		return *((InputRealPtrsType)dataPtr[0]) != 0;
		break;
		case SS_SINGLE:
			return *((InputReal32PtrsType)dataPtr[0]) != 0;
			break;
		case SS_INT8:
			return *((InputInt8PtrsType)dataPtr[0]) != 0;
			break;
		case SS_UINT8:
			return *((InputUInt8PtrsType)dataPtr[0]) != 0;
			break;
		case SS_INT16:
			return *((InputInt16PtrsType)dataPtr[0]) != 0;
			break;
		case SS_UINT16:
			return *((InputUInt16PtrsType)dataPtr[0]) != 0;
			break;
		case SS_INT32:
			return *((InputInt32PtrsType)dataPtr[0]) != 0;
			break;
		case SS_UINT32:
			return *((InputUInt32PtrsType)dataPtr[0]) != 0;
			break;
		case SS_BOOLEAN:
			return *((InputBooleanPtrsType)dataPtr[0]) != 0;
			break;
		default:
			throw ::CreateException("Input %d has invalid type %d.", port, dataType);
			break;
	}
}
inline void setOutputAsBool(SimStruct* S, int_T port, bool value)
{
	DTypeId   dataType = ssGetOutputPortDataType(S, port);
	void* dataPtr = ssGetOutputPortSignal(S, port);
	switch (dataType)
	{
	case SS_DOUBLE:
		*((real_T*)dataPtr) = (real_T)value;
		break;
	case SS_SINGLE:
		*((real32_T*)dataPtr) = (real32_T)value;
		break;
	case SS_INT8:
		*((int8_T*)dataPtr) = (int8_T)value;
		break;
	case SS_UINT8:
		*((uint8_T*)dataPtr) = (uint8_T)value;
		break;
	case SS_INT16:
		*((int16_T*)dataPtr) = (int16_T)value;
		break;
	case SS_UINT16:
		*((uint16_T*)dataPtr) = (uint16_T)value;
		break;
	case SS_INT32:
		*((int32_T*)dataPtr) = (int32_T)value;
		break;
	case SS_UINT32:
		*((uint32_T*)dataPtr) = (uint32_T)value;
		break;
	case SS_BOOLEAN:
		*((boolean_T*)dataPtr) = (boolean_T)value;
		break;
	default:
		throw ::CreateException("Output %d has invalid type %d.", port, dataType);
		break;
	}
}

/* Write block inputs to PLCSIM Advanced */
static void ReadInputs(SimStruct* S, IInstance* instance)
{
	if (instance == 0)
		return;
	int_T numInputPorts = ssGetNumInputPorts(S);
	for (int_T i = 0; i < numInputPorts; i++)
	{
		bool input = getInputAsBool(S, i);
		int_T byte = i/8;
		int_T bit = i % 8;
		enum ERuntimeErrorCode srec = instance->WriteBit(SRA_INPUT, (UINT32)byte, (UINT8)bit, input);
		if (srec != SREC_OK)
		{
			throw ::CreatePLCException(srec, "Could not set input %%I%d.%d od PLC.", byte, bit);
		}
	}
}

/* Read block outputs from PLCSIM Advanced */
static void WriteOutputs(SimStruct *S, IInstance* instance)
{
	int_T numOutputPorts = ssGetNumOutputPorts(S);

	for (int_T i = 0; i < numOutputPorts; i++)
	{
		//bool *output = (bool*)ssGetOutputPortSignal(S,i);
		int_T byte = i/8;
		int_T bit = i % 8;
		if (instance == 0)
			setOutputAsBool(S, i, false);
		else
		{
			bool output;
			enum ERuntimeErrorCode srec = instance->ReadBit(SRA_OUTPUT, (UINT32)byte, (UINT8)bit, &output);
			if (srec != SREC_OK)
			{
				throw ::CreatePLCException(srec, "Could not read output %%Q%d.%d od PLC.", byte, bit);
			}
			setOutputAsBool(S, i, output);
		}
	}
}

/* Execute a simulation step of the PLC emulation, using time synchronization with the Simulink time. */
static void ExecuteSynchronizedSimulationStep(SimStruct *S)
{
	PLCConnection* plc = getPLC(S);
	if(plc == NULL)
	{
		return;
		ssSetErrorStatus(S,"PLC connection not initialized!");
	}
	time_T currentSimulinkTime = ssGetT(S);
	time_T currentPLCSimTime = ConvertPLCSimTimeToSimulinkTime(plc->GetInstance()->GetSystemTime());

	// Read the current inputs from Simulink if the PLCSim time is at or beyond the Simulink time. 
	if (currentPLCSimTime >= currentSimulinkTime)
	{
		ReadInputs(S, plc->GetInstance());
	}

	// Execute PLCSim if the PLCSim time is before or at the Simulink time. The PLCSim inputs have already been
	// updated in this same simulation step or in a previous simulation step.
	if (currentPLCSimTime <= currentSimulinkTime)
	{
#if ExecuteRunNextCycleLoop
		// Execute PLC cycles until the current simulation time is reached.
		do
		{
			// First, write the outputs from the previous cycle. These values will be the result of the 
			// current simulation step, if this is the last iteration of the while loop.
			WriteOutputs(S, plc->GetInstance());
			
			// Execute the next PLC cycle.
			enum ERuntimeErrorCode srec = plc->GetInstance()->RunNextCycle();
			if (srec != SREC_OK)
			{
				throw ::CreatePLCException(srec, "Could not run next PLC cycle.");
				return;
			}

			try
			{
				plc->WaitForEndOfCycle();
			}
			catch (std::exception e)
			{
				const char* message = e.what();
				ssSetErrorStatus(S, message);
				return;
	}

			// Get the new PLCSim time
			currentPLCSimTime = ConvertPLCSimTimeToSimulinkTime(plc->GetInstance()->GetSystemTime());

		} while (currentPLCSimTime <= currentSimulinkTime);
#endif
#if UseTimespanMode
		// This code needs to be reviewed. StartProcessing executes beyond the current simulation time from Simulink.

		INT64 stepDuration_ns = (currentSimulinkTime - currentPLCSimTime) * 1000000000;

		// Execute the next PLC cycles.
		enum ERuntimeErrorCode srec = plc->GetInstance()->StartProcessing(stepDuration_ns);
		if (srec != SREC_OK)
		{
			PrintFormattedPLCSimError(srec, "StartProcessing");
			return;
		}

		try
		{
			plc->WaitForEndOfCycle();
		}
		catch (std::exception e)
		{
			const char* message = e.what();
			ssSetErrorStatus(S, message);
			return;
		}

		// Now write the outputs from the last cycle. These values will be the result of the 
		// current simulation step. This is actually incorrect, because the PLCSim time is beyond the Simulink time.
		WriteOutputs(S, plc->GetInstance());
#endif

		// Now read the current inputs from Simulink, because the PLCSim time is beyond the Simulink time.
		// These inputs will be used in the following simulation step.
		ReadInputs(S, plc->GetInstance());
	}
}

/*** Callback functions for Simulink ***/

void mdlCheckParameters(SimStruct *S)
{
	::PrintText("mdlCheckParameters");
	// InstanceName
	const mxArray *instanceNameParam = ssGetSFcnParam(S,ParameterIndex_InstanceName);
	DTypeId instanceNameType = ssGetDTypeIdFromMxArray(instanceNameParam);
	size_t instanceNameLength = mxGetNumberOfElements(instanceNameParam);
	if (instanceNameType != INVALID_DTYPE_ID || instanceNameLength > PLCConnection::maxInstanceNameLength)
	{
		ssSetErrorStatus(S,"Invalid parameter value for InstanceName");
		return;
	}

	// InputPortCount
	const mxArray *inputPortCountParam = ssGetSFcnParam(S,ParameterIndex_InputPortCount);
	DTypeId inputPortCountType = ssGetDTypeIdFromMxArray(inputPortCountParam);
	size_t inputPortCountLength = mxGetNumberOfElements(inputPortCountParam);
	if (inputPortCountType != SS_DOUBLE || inputPortCountLength != 1)
	{
		ssSetErrorStatus(S,"Invalid parameter value for InputPortCount");
		return;
	}
	int inputPorts = (int)*(real_T*)mxGetData(inputPortCountParam);
	if(inputPorts <0)
	{
		ssSetErrorStatus(S,"Number of input ports must be non-negative.");
		return;
	}
	
	// OutputPortCount
	const mxArray *outputPortCountParam = ssGetSFcnParam(S,ParameterIndex_OutputPortCount);
	DTypeId outputPortCountType = ssGetDTypeIdFromMxArray(outputPortCountParam);
	size_t outputPortCountLength = mxGetNumberOfElements(outputPortCountParam);
	if (outputPortCountType != SS_DOUBLE || outputPortCountLength != 1)
	{
		ssSetErrorStatus(S,"Invalid parameter value for OutputPortCount");
		return;
	}
	int outputPorts = (int)*(real_T*)mxGetData(outputPortCountParam);
	if(outputPorts <0)
	{
		ssSetErrorStatus(S,"Number of output ports must be non-negative.");
		return;
	}

	// ScaleFactor
	const mxArray *scaleFactorParam = ssGetSFcnParam(S,ParameterIndex_ScaleFactor);
	DTypeId scaleFactorType = ssGetDTypeIdFromMxArray(scaleFactorParam);
	size_t scaleFactorLength = mxGetNumberOfElements(scaleFactorParam);
	if (scaleFactorType != SS_DOUBLE || scaleFactorLength != 1)
	{
		ssSetErrorStatus(S,"Invalid parameter value for ScaleFactor");
		return;
	}
	
	// Synchronization
	const mxArray *synchronizationParam = ssGetSFcnParam(S,ParameterIndex_Synchronization);
	DTypeId synchronizationType = ssGetDTypeIdFromMxArray(synchronizationParam);
	size_t synchronizationLength = mxGetNumberOfElements(synchronizationParam);
	if (synchronizationType != SS_DOUBLE || synchronizationLength != 1)
	{
		ssSetErrorStatus(S,"Invalid parameter value for Synchronization");
		return;
	}
}

void mdlInitializeSizes(SimStruct *S)
{
	::PrintText("mdlInitializeSizes");
	ssSetNumPWork(S, 1);
	ssSetOptions(S,SS_OPTION_CALL_TERMINATE_ON_EXIT);

	ssSetNumSFcnParams(S, SFCT_NUM_PARAMETERS);  /* Number of expected parameters */

    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S))
	{
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != 0) 
			return;
    }
	else 
		return; /* Parameter mismatch reported by the Simulink engine */

	const mxArray *inputPortCountParam = ssGetSFcnParam(S,ParameterIndex_InputPortCount);
	int_T numInputPorts = (int)*(real_T*)mxGetData(inputPortCountParam);
	const mxArray *outputPortCountParam = ssGetSFcnParam(S,ParameterIndex_OutputPortCount);
	int_T numOutputPorts = (int)*(real_T*)mxGetData(outputPortCountParam);

	if (!ssSetNumInputPorts(S, numInputPorts)) 
		return;
	for (int_T i = 0; i < numInputPorts; ++i)
	{
		ssSetInputPortWidth(S, i, 1);
		ssSetInputPortDirectFeedThrough(S, i, 1);
		ssSetInputPortDataType(S, i, DYNAMICALLY_TYPED);
	}

    if (!ssSetNumOutputPorts(S, numOutputPorts)) 
		return;
	for (int_T i = 0; i < numOutputPorts; ++i)
	{
		ssSetOutputPortWidth(S, i, 1);
		ssSetOutputPortDataType(S, i, DYNAMICALLY_TYPED);
	}
    ssSetNumSampleTimes(S,1);
}

void mdlInitializeSampleTimes(SimStruct *S)
{
	::PrintText("mdlInitializeSampleTimes");
	
	const mxArray* sampleTimeParam = ssGetSFcnParam(S, ParameterIndex_SampleTime);
	real_T sampleTime = *(real_T*)mxGetData(sampleTimeParam);
	ssSetSampleTime(S, 0, sampleTime);
    ssSetOffsetTime(S, 0, 0.0);
}

void mdlStart(SimStruct *S)
{
	::PrintText("mdlStart");
	char_T instanceName[PLCConnection::maxInstanceNameLength + 1];
	const mxArray *instanceNameParam = ssGetSFcnParam(S,ParameterIndex_InstanceName);
	mxGetString(instanceNameParam, instanceName, PLCConnection::maxInstanceNameLength+1);
	const mxArray *inputPortCountParam = ssGetSFcnParam(S,ParameterIndex_InputPortCount);
	int inputPortCount = (int)*(real_T*)mxGetData(inputPortCountParam);
	const mxArray *outputPortCountParam = ssGetSFcnParam(S,ParameterIndex_OutputPortCount);
	int outputPortCount = (int)*(real_T*)mxGetData(outputPortCountParam);
	const mxArray *scaleFactorParam = ssGetSFcnParam(S,ParameterIndex_ScaleFactor);
	double scaleFactor = *(real_T*)mxGetData(scaleFactorParam);
	const mxArray *synchronizationParam = ssGetSFcnParam(S,ParameterIndex_Synchronization);
	bool synchronization = (*(real_T*)mxGetData(synchronizationParam) != 0);
	
	PLCConnection* plc = new PLCConnection(inputPortCount, outputPortCount, scaleFactor, synchronization, instanceName);
	::PrintText("Initializing PLC connection...");
	try
	{
		plc->initialize(false);
	}
	catch (std::exception e)
	{
		delete(plc);
		plc = NULL;
		const char* message = e.what();
		ssSetErrorStatus(S, message);
		return;
	}
	::PrintText("PLC connection initialized!");
	::PrintText("Synchronizing clocks...");
	// Set time in PLCSim from simulation start time of Simulink
	time_T simulinkStartTime = ssGetTStart(S);
	plc->GetInstance()->SetSystemTime(ConvertSimulinkTimeToPLCSimTime(simulinkStartTime));
	
	setPLC(S, plc);
	::PrintText("Clocks synchronized!");

	::PrintText("Starting PLC program...");
	enum ERuntimeErrorCode srec = plc->GetInstance()->Run();
	if (srec != SREC_OK)
	{
		ssSetErrorStatus(S, "Could not start PLC program!");
		return;
	}
	::PrintText("PLC program started...");
}

static void mdlOutputs(SimStruct *S, int_T tid)
{
	PLCConnection* plc = getPLC(S);
	if(plc == NULL)
	{
		ssSetErrorStatus(S,"PLC connection not initialized!");
		return;
	}
	/*if(plc->GetSynchronizationMethod() != PLCConnection::NoSync &&  plc->IsTimeSynchronization())
	{
		try
		{
			ExecuteSynchronizedSimulationStep(S);
		}
		catch (std::exception e)
		{
			const char* message = e.what();
			ssSetErrorStatus(S, message);
			return;
		}
	}
	else*/
	{
		// Time synchronization is disabled. Run exactly one PLC cycle for every simulation step.
		// The time in Simulink and in PLCSIM Advanced will deviate arbitrarily.

		// First, write the outputs from the previous cycle. These values will be the result of the 
		// current simulation step.
		try
		{
			WriteOutputs(S, plc->GetInstance());
		}
		catch (std::exception e)
		{
			const char* message = e.what();
			ssSetErrorStatus(S, message);
			return;
		}
		
		// Now read the current inputs from Simulink, to be used for the next PLC cycle.
		try
		{
			ReadInputs(S, plc->GetInstance());
		}
		catch (std::exception e)
		{
			const char* message = e.what();
			ssSetErrorStatus(S, message);
			return;
		}

		if (plc->GetSynchronizationMethod() != PLCConnection::NoSync)
		{
			// Execute the next PLC cycle.
			enum ERuntimeErrorCode srec = plc->GetInstance()->RunNextCycle();
			if (srec != SREC_OK)
			{
				ssSetErrorStatus(S, ::FormatPLCError(srec, "Could not run next cycle of PLC.").c_str());
				return;
			}

			try
			{
				plc->WaitForEndOfCycle();
			}
			catch (std::exception e)
			{
				const char* message = e.what();
				ssSetErrorStatus(S, message);
				return;
			}
		}
	}
}

static void mdlTerminate(SimStruct *S)
{
	::PrintText("mdlTerminate");
	PLCConnection* plc = getPLC(S);
	if(plc == NULL)
	{
		// OK, we don't have anything to do!
		return;
	}
	
	::PrintText("Stopping PLC...");
	plc->GetInstance()->SetOperatingMode(SROM_DEFAULT);
	enum ERuntimeErrorCode srec = plc->GetInstance()->Stop();
	
	::PrintText("Disconnecting from PLC...");
	delete(plc);
	setPLC(S, NULL);
}

/* Function: mdlSetInputPortDataType ==========================================
 *    This routine is called with the candidate data type for a dynamically
 *    typed port.  If the proposed data type is acceptable, the routine should
 *    go ahead and set the actual port data type using ssSetInputPortDataType.
 *    If the data type is unacceptable an error should generated via
 *    ssSetErrorStatus.  Note that any other dynamically typed input or
 *    output ports whose data types are implicitly defined by virtue of knowing
 *    the data type of the given port can also have their data types set via
 *    calls to ssSetInputPortDataType or ssSetOutputPortDataType.
 */
static void mdlSetInputPortDataType(SimStruct* S,
	int       port,
	DTypeId   dataType)
{
	int_T numInputPorts = ssGetNumInputPorts(S);

	if (port < numInputPorts) 
	{
		if (checkIODataType(dataType))
		{
			ssSetInputPortDataType(S, port, dataType);
		}
		else 
		{
			ssSetErrorStatus(S, "Invalid input data type.");
		}
	}
	else 
	{
		/*
		 * Should not end up here.  Simulink will only call this function
		 * for existing input ports whose data types are unknown.
		 */
		ssSetErrorStatus(S, "Invalid call to mdlSetInputPortDataType");
	}
} 


/* Function: mdlSetOutputPortDataType =========================================
 *    This routine is called with the candidate data type for a dynamically
 *    typed port.  If the proposed data type is acceptable, the routine should
 *    go ahead and set the actual port data type using ssSetOutputPortDataType.
 *    If the data tyoe is unacceptable an error should generated via
 *    ssSetErrorStatus.  Note that any other dynamically typed input or
 *    output ports whose data types are implicitly defined by virtue of knowing
 *    the data type of the given port can also have their data types set via
 *    calls to ssSetInputPortDataType or ssSetOutputPortDataType.
 */
static void mdlSetOutputPortDataType(SimStruct* S,
	int       port,
	DTypeId   dataType)
{
	int_T numOutputPorts = ssGetNumOutputPorts(S);

	if (port < numOutputPorts)
	{
		if (checkIODataType(dataType))
		{
			ssSetOutputPortDataType(S, port, dataType);
		}
		else
		{
			ssSetErrorStatus(S, "Invalid output data type.");
		}
	}
	else
	{
		/*
		 * Should not end up here.  Simulink will only call this function
		 * for existing input ports whose data types are unknown.
		 */
		ssSetErrorStatus(S, "Invalid call to mdlSetOutputPortDataType");
	}

}

/* Function: mdlSetDefaultPortDataTypes ========================================
 *    This routine is called when Simulink is not able to find data type
 *    candidates for dynamically typed ports. This function must set the data
 *    type of all dynamically typed ports.
 */
static void mdlSetDefaultPortDataTypes(SimStruct* S)
{
	int_T numInputPorts = ssGetNumInputPorts(S);
	int_T numOutputPorts = ssGetNumOutputPorts(S);
	for (int_T i = 0; i < numInputPorts; i++)
	{
		ssSetInputPortDataType(S, i, SS_DOUBLE);
	}
	for (int_T i = 0; i < numOutputPorts; i++)
	{
		ssSetOutputPortDataType(S, i, SS_DOUBLE);
	}
}

#include "simulink.c"
