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

#define S_FUNCTION_NAME plc2simulink
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

/* Simulink Block Parameters */
enum BlockParameters
{
	BlockParameter_InstanceName,
	BlockParameter_SupplyVoltageExplicit,
	BlockParameter_InputPortCount,
	BlockParameter_OutputPortCount,
	BlockParameter_SampleTime,
	BlockParameter_ScaleFactor,

	BLOCK_PARAMETERS_LAST = BlockParameter_ScaleFactor
};

/**
** Saves a pointer to the PLC in the block working variables, such that it can be retrieved in subsequent simulation cycles.
**/
void SetBlockPLC(SimStruct* S, PLCConnection* plc)
{
	void** PWork = ssGetPWork(S);
	PWork[0] = plc;
}
/**
** Retrieves a previously, in the block's working variables, saved pointer to the PLC. Returns NULL if PLC was not set, yet.
**/
PLCConnection* GetBlockPLC(SimStruct* S)
{
	void** PWork = ssGetPWork(S);
	if (PWork == NULL)
	{
		return NULL;
	}
	if (PWork[0] == NULL)
	{
		return NULL;
	}
	return (PLCConnection*)PWork[0];
}

bool GetInputAsBool(SimStruct* S, int_T port)
{
	DTypeId   dataType = ssGetInputPortDataType(S, port);
	InputPtrsType dataPtr = ssGetInputPortSignalPtrs(S, port);
	switch (dataType)
	{
	case SS_DOUBLE:
		return *(((InputRealPtrsType)dataPtr)[0]) != 0;
		break;
	case SS_SINGLE:
		return *(((InputReal32PtrsType)dataPtr)[0]) != 0;
		break;
	case SS_INT8:
		return *(((InputInt8PtrsType)dataPtr)[0]) != 0;
		break;
	case SS_UINT8:
		return *(((InputUInt8PtrsType)dataPtr)[0]) != 0;
		break;
	case SS_INT16:
		return *(((InputInt16PtrsType)dataPtr)[0]) != 0;
		break;
	case SS_UINT16:
		return *(((InputUInt16PtrsType)dataPtr)[0]) != 0;
		break;
	case SS_INT32:
		return *(((InputInt32PtrsType)dataPtr)[0]) != 0;
		break;
	case SS_UINT32:
		return *(((InputUInt32PtrsType)dataPtr)[0]) != 0;
		break;
	case SS_BOOLEAN:
		return *(((InputBooleanPtrsType)dataPtr)[0]) != 0;
		break;
	default:
		throw ::CreateException("Input %d has invalid type %d.", port, dataType);
		break;
	}
}
const real_T GetInputAsReal(SimStruct* S, int_T port)
{
	DTypeId   dataType = ssGetInputPortDataType(S, port);
	InputPtrsType dataPtr = ssGetInputPortSignalPtrs(S, port);

	switch (dataType)
	{
	case SS_DOUBLE:
		return *(((InputRealPtrsType)dataPtr)[0]);
		break;
	case SS_SINGLE:
		return *(((InputReal32PtrsType)dataPtr)[0]);
		break;
	case SS_INT8:
		return *(((InputInt8PtrsType)dataPtr)[0]);
		break;
	case SS_UINT8:
		return *(((InputUInt8PtrsType)dataPtr)[0]);
		break;
	case SS_INT16:
		return *(((InputInt16PtrsType)dataPtr)[0]);
		break;
	case SS_UINT16:
		return *(((InputUInt16PtrsType)dataPtr)[0]);
		break;
	case SS_INT32:
		return *(((InputInt32PtrsType)dataPtr)[0]);
		break;
	case SS_UINT32:
		return *(((InputUInt32PtrsType)dataPtr)[0]);
		break;
	case SS_BOOLEAN:
		return (((InputBooleanPtrsType)dataPtr)[0]) ? 1.0 : 0.0;
		break;
	default:
		throw ::CreateException("Input %d has invalid type %d.", port, dataType);
		break;
	}
}
void SetOutputAsBool(SimStruct* S, int_T port, bool value)
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
void SetOutputVoltage(SimStruct* S, const int_T port, const double supplyVoltage, const bool value)
{
	const double voltage = value ? supplyVoltage : 0.0;

	DTypeId   dataType = ssGetOutputPortDataType(S, port);
	void* dataPtr = ssGetOutputPortSignal(S, port);
	switch (dataType)
	{
	case SS_DOUBLE:
		*((real_T*)dataPtr) = static_cast<real_T>(voltage);
		break;
	case SS_SINGLE:
		*((real32_T*)dataPtr) = static_cast<real32_T>(voltage);
		break;
	case SS_INT8:
		*((int8_T*)dataPtr) = static_cast<int8_T>(voltage + 0.5);
		break;
	case SS_UINT8:
		*((uint8_T*)dataPtr) = static_cast<uint8_T>(voltage + 0.5);
		break;
	case SS_INT16:
		*((int16_T*)dataPtr) = static_cast<int16_T>(voltage + 0.5);
		break;
	case SS_UINT16:
		*((uint16_T*)dataPtr) = static_cast<uint16_T>(voltage + 0.5);
		break;
	case SS_INT32:
		*((int32_T*)dataPtr) = static_cast<int32_T>(voltage + 0.5);
		break;
	case SS_UINT32:
		*((uint32_T*)dataPtr) = static_cast<uint32_T>(voltage + 0.5);
		break;
	case SS_BOOLEAN:
		*((boolean_T*)dataPtr) = static_cast<boolean_T>(value);
		break;
	default:
		throw ::CreateException("Output %d has invalid type %d.", port, dataType);
		break;
	}
}

/**
** Write block inputs to PLCSIM Advanced 
**/
const double ReadBlockInputs(SimStruct* S, PLCConnection* plcConnection)
{
	double supplyVoltage;
	if (plcConnection == NULL || !plcConnection->isInitialized())
		throw ::CreateException("PLC not initialized!");
	int_T startPort;
	if (plcConnection->isSupplyVoltageExplicit())
	{
		supplyVoltage = ::GetInputAsReal(S, 0);
		startPort = 1;
	}
	else
		startPort = 0;
	int_T numInputPorts = plcConnection->getInputPortCount();
	for (int_T i = 0; i < numInputPorts; i++)
	{
		bool input = ::GetInputAsBool(S, startPort+i);
		int_T byte = i / 8;
		int_T bit = i % 8;
		enum ERuntimeErrorCode srec = plcConnection->getInstance()->WriteBit(SRA_INPUT, (UINT32)byte, (UINT8)bit, input);
		if (srec != SREC_OK)
		{
			throw ::CreatePLCException(srec, "Could not set input %%I%d.%d od PLC.", byte, bit);
		}
	}
	return supplyVoltage;
}

/**
** Read block outputs from PLCSIM Advanced 
**/
void WriteBlockOutputs(SimStruct* S, PLCConnection* plcConnection, double supplyVoltage)
{
	if (plcConnection == NULL || !plcConnection->isInitialized())
		throw ::CreateException("PLC not initialized!");
	int_T numOutputPorts = plcConnection->getOutputPortCount();
	int_T startPort;
	if (plcConnection->isSupplyVoltageExplicit())
	{
		SetOutputVoltage(S, 0, supplyVoltage, false);
		startPort = 1;
	}
	else
		startPort = 0;
	for (int_T i = 0; i < numOutputPorts; i++)
	{
		int_T byte = i / 8;
		int_T bit = i % 8;

		bool output;
		enum ERuntimeErrorCode srec = plcConnection->getInstance()->ReadBit(SRA_OUTPUT, (UINT32)byte, (UINT8)bit, &output);
		if (srec != SREC_OK)
		{
			throw ::CreatePLCException(srec, "Could not read output %%Q%d.%d od PLC.", byte, bit);
		}
		SetOutputVoltage(S, startPort+i, supplyVoltage, output);
	}
}

/*** Callback functions for Simulink ***/

void mdlCheckParameters(SimStruct *S)
{
	int_T numParams = ssGetSFcnParamsCount(S);
	if(numParams != BLOCK_PARAMETERS_LAST + 1)
	{
		std::string message = ::FormatText("Invalid number of Block parameters. Expected %d, found %d.", BLOCK_PARAMETERS_LAST + 1, numParams);
		ssSetErrorStatus(S, message.c_str());
		return;
	}

	// InstanceName
	const mxArray *instanceNameParam = ssGetSFcnParam(S,BlockParameter_InstanceName);
	DTypeId instanceNameType = ssGetDTypeIdFromMxArray(instanceNameParam);
	size_t instanceNameLength = mxGetNumberOfElements(instanceNameParam);
	if (instanceNameType != INVALID_DTYPE_ID || instanceNameLength > PLCConnection::maxInstanceNameLength)
	{
		ssSetErrorStatus(S,"Invalid parameter value for InstanceName");
		return;
	}

	// InputPortCount
	const mxArray *inputPortCountParam = ssGetSFcnParam(S,BlockParameter_InputPortCount);
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
	const mxArray *outputPortCountParam = ssGetSFcnParam(S,BlockParameter_OutputPortCount);
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
	const mxArray *scaleFactorParam = ssGetSFcnParam(S,BlockParameter_ScaleFactor);
	DTypeId scaleFactorType = ssGetDTypeIdFromMxArray(scaleFactorParam);
	size_t scaleFactorLength = mxGetNumberOfElements(scaleFactorParam);
	if (scaleFactorType != SS_DOUBLE || scaleFactorLength != 1)
	{
		ssSetErrorStatus(S,"Invalid parameter value for ScaleFactor");
		return;
	}

	// ScaleFactor
	const mxArray* supplyVoltageExplicitParam = ssGetSFcnParam(S, BlockParameter_SupplyVoltageExplicit);
	DTypeId supplyVoltageExplicitType = ssGetDTypeIdFromMxArray(supplyVoltageExplicitParam);
	size_t supplyVoltageExplicitLength = mxGetNumberOfElements(supplyVoltageExplicitParam);
	if (supplyVoltageExplicitType != SS_DOUBLE || supplyVoltageExplicitLength != 1)
	{
		ssSetErrorStatus(S, "Invalid parameter value for SupplyVoltageExplicit");
		return;
	}
}

void mdlInitializeSizes(SimStruct *S)
{
	ssSetNumPWork(S, 1);
	ssSetOptions(S, SS_OPTION_CALL_TERMINATE_ON_EXIT);
	ssSetNumSFcnParams(S, BLOCK_PARAMETERS_LAST+1);  /* Number of expected parameters */

    mdlCheckParameters(S);
    if (ssGetErrorStatus(S) != 0) 
		return;

	const mxArray* inputPortCountParam = ssGetSFcnParam(S,BlockParameter_InputPortCount);
	int_T numInputPorts = (int)*(real_T*)mxGetData(inputPortCountParam);
	const mxArray* outputPortCountParam = ssGetSFcnParam(S,BlockParameter_OutputPortCount);
	int_T numOutputPorts = (int)*(real_T*)mxGetData(outputPortCountParam);
	const mxArray* supplyVoltageExplicitParam = ssGetSFcnParam(S, BlockParameter_SupplyVoltageExplicit);
	bool supplyVoltageExplicit = *(real_T*)mxGetData(supplyVoltageExplicitParam) != 0;
	if (supplyVoltageExplicit)
	{
		numInputPorts+=1;
		numOutputPorts += 1;
	}

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

void mdlInitializeSampleTimes(SimStruct* S)
{
	const mxArray* sampleTimeParam = ssGetSFcnParam(S, BlockParameter_SampleTime);
	real_T sampleTime = *(real_T*)mxGetData(sampleTimeParam);
	ssSetSampleTime(S, 0, sampleTime);
    ssSetOffsetTime(S, 0, 0.0);
}

void mdlStart(SimStruct* S)
{
	mdlCheckParameters(S);

	// Read parameters.
	char_T instanceName[PLCConnection::maxInstanceNameLength + 1];
	const mxArray* instanceNameParam = ssGetSFcnParam(S,BlockParameter_InstanceName);
	mxGetString(instanceNameParam, instanceName, PLCConnection::maxInstanceNameLength+1);
	const mxArray* inputPortCountParam = ssGetSFcnParam(S,BlockParameter_InputPortCount);
	int inputPortCount = (int)*(real_T*)mxGetData(inputPortCountParam);
	const mxArray* outputPortCountParam = ssGetSFcnParam(S,BlockParameter_OutputPortCount);
	int outputPortCount = (int)*(real_T*)mxGetData(outputPortCountParam);
	const mxArray* scaleFactorParam = ssGetSFcnParam(S,BlockParameter_ScaleFactor);
	double scaleFactor = *(real_T*)mxGetData(scaleFactorParam);
	const mxArray* supplyVoltageExplicitParam = ssGetSFcnParam(S, BlockParameter_SupplyVoltageExplicit);
	bool supplyVoltageExplicit = *(real_T*)mxGetData(supplyVoltageExplicitParam) != 0;
	
	// Initialize PLC
	::PrintText("Initializing PLC connection...");
	PLCConnection* plc = new PLCConnection(inputPortCount, outputPortCount, scaleFactor, instanceName, supplyVoltageExplicit);
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
	::SetBlockPLC(S, plc);
	::PrintText("PLC connection initialized!");

	// Set time in PLCSim from simulation start time of Simulink
	::PrintText("Synchronizing clocks...");
	time_T simulinkStartTime = ssGetTStart(S);
	enum ERuntimeErrorCode srec = plc->getInstance()->SetSystemTime(::ConvertSimulinkTimeToPLCSimTime(simulinkStartTime));
	if (srec != SREC_OK)
	{
		// we do not have to uninitialize PLC, since this is done in mdlTerminate which is called immediately after setting the error status.
		ssSetErrorStatus(S, "Could not synchronize clocks between PLC and Simulink!");
		return;
	}
	::PrintText("Clocks synchronized!");

	// Start PLC
	::PrintText("Starting PLC program...");
	srec = plc->getInstance()->Run();
	if (srec != SREC_OK)
	{
		// we do not have to uninitialize PLC, since this is done in mdlTerminate which is called immediately after setting the error status.
		ssSetErrorStatus(S, "Could not start PLC program!");
		return;
	}
	::PrintText("PLC program started...");
}

void mdlOutputs(SimStruct *S, int_T tid)
{
	PLCConnection* plc = GetBlockPLC(S);
	if(plc == NULL)
	{
		ssSetErrorStatus(S,"PLC connection not initialized!");
		return;
	}

	// Read block inputs from Simulink and send them to the inputs of the PLC.
	double supplyVoltage;
	try
	{
		supplyVoltage = ::ReadBlockInputs(S, plc);
	}
	catch (std::exception e)
	{
		const char* message = e.what();
		ssSetErrorStatus(S, message);
		return;
	}

	// Now, read PLC outputs and set them as block outputs
	try
	{
		::WriteBlockOutputs(S, plc, supplyVoltage);
	}
	catch (std::exception e)
	{
		const char* message = e.what();
		ssSetErrorStatus(S, message);
		return;
	}
}

void mdlTerminate(SimStruct *S)
{
	PLCConnection* plc = ::GetBlockPLC(S);
	if(plc == NULL)
	{
		// We don't have anything to do, PLC was never initialized!
		return;
	}
	
	::PrintText("Stopping PLC...");
	plc->getInstance()->SetOperatingMode(SROM_DEFAULT);
	enum ERuntimeErrorCode srec = plc->getInstance()->Stop();
	if (srec == SREC_OK)
	{
		::PrintText("Stopped PLC.");
	}
	else
	{
		std::string message = FormatPLCError(srec, "Could not stop PLC.");
		ssSetErrorStatus(S, message.c_str());
	}
	
	delete(plc);
	SetBlockPLC(S, NULL);
	::PrintText("Disconnected from PLC.");
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
void mdlSetInputPortDataType(SimStruct* S,
	int       port,
	DTypeId   dataType)
{
	int_T numInputPorts = ssGetNumInputPorts(S);

	if (port < numInputPorts) 
	{
		if (IsCompatibleIODataType(dataType))
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
void mdlSetOutputPortDataType(SimStruct* S,
	int       port,
	DTypeId   dataType)
{
	int_T numOutputPorts = ssGetNumOutputPorts(S);

	if (port < numOutputPorts)
	{
		if (IsCompatibleIODataType(dataType))
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
void mdlSetDefaultPortDataTypes(SimStruct* S)
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
