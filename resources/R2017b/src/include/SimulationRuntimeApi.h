/* *************************************** */

#ifndef SimulationRuntimeApi_H
#define SimulationRuntimeApi_H

#include <Windows.h>
#include <Objidl.h>
#include <Shlwapi.h>
class IBaseInterface;

class IInstance;

class IRemoteRuntimeManager;

class ISimulationRuntimeManager;


#pragma comment(lib, "Shlwapi.lib")

#define DAPI_DLL_INTERFACE_VERSION 1
#define DAPI_DLL_PATH_REGISTRY_KEY_NAME L"SOFTWARE\\Wow6432Node\\Siemens\\Shared Tools\\PLCSIMADV_SimRT\\"
#define DAPI_DLL_PATH_REGISTRY_VALUE_NAME L"Path"
#define DAPI_DLL_PATH_VERSION_SUBDIRECTORY L"API\\1.0"
#define DAPI_DLL_NAME_X86 L"Siemens.Simatic.Simulation.Runtime.Api.x86.dll"
#define DAPI_DLL_NAME_X64 L"Siemens.Simatic.Simulation.Runtime.Api.x64.dll"
#define DAPI_ENTRY_INITIALIZE "RuntimeApiEntry_Initialize"
#define DAPI_ENTRY_DESTROY_INTERFACE "RuntimeApiEntry_DestroyInterface"
#define DINSTANCE_NAME_MAX_LENGTH 64
#define DSTORAGE_PATH_MAX_LENGTH 130
#define DTAG_NAME_MAX_LENGTH 300
#define DTAG_ARRAY_DIMENSION 6
#define DCONTROLLER_NAME_MAX_LENGTH 128
#define DCONTROLLER_SHORT_DESIGNATION_MAX_LENGTH 32
/// <summary>
/// This enumeration contains all error codes used by the SimulationRuntime API. Most API functions will return one of these error codes. If
/// these functions succeed their return value will always be SREC_OK. Errors are represended by negative values, warnings by positive values.
/// </summary>
enum ERuntimeErrorCode
{
    SREC_OK = 0,
    SREC_INVALID_ERROR_CODE = -1,
    SREC_NOT_IMPLEMENTED = -2,
    SREC_INDEX_OUT_OF_RANGE = -3,
    SREC_DOES_NOT_EXIST = -4,
    SREC_ALREADY_EXISTS = -5,
    SREC_UNKNOWN_MESSAGE_TYPE = -6,
    SREC_INVALID_MESSAGE_ID = -7,
    SREC_WRONG_ARGUMENT = -8,
    SREC_WRONG_PIPE = -9,
    SREC_CONNECTION_ERROR = -10,
    SREC_TIMEOUT = -11,
    SREC_MESSAGE_CORRUPT = -12,
    SREC_WRONG_VERSION = -13,
    SREC_INSTANCE_NOT_RUNNING = -14,
    SREC_INTERFACE_REMOVED = -15,
    SREC_SHARED_MEMORY_NOT_INITIALIZED = -16,
    SREC_API_NOT_INITIALIZED = -17,
    SREC_WARNING_ALREADY_EXISTS = 18,
    SREC_NOT_SUPPORTED = -19,
    SREC_WARNING_INVALID_CALL = 20,
    SREC_ERROR_LOADING_DLL = -21,
    SREC_SIGNAL_NAME_DOES_NOT_EXIST = -22,
    SREC_SIGNAL_TYPE_MISMATCH = -23,
    SREC_SIGNAL_CONFIGURATION_ERROR = -24,
    SREC_NO_SIGNAL_CONFIGURATION_LOADED = -25,
    SREC_CONFIGURED_CONNECTION_NOT_FOUND = -26,
    SREC_CONFIGURED_DEVICE_NOT_FOUND = -27,
    SREC_INVALID_CONFIGURATION = -28,
    SREC_TYPE_MISMATCH = -29,
    SREC_LICENSE_NOT_FOUND = -30,
    SREC_NO_LICENSE_AVAILABLE = -31,
    SREC_WRONG_COMMUNICATION_INTERFACE = -32,
    SREC_LIMIT_REACHED = -33,
    SREC_NO_STORAGE_PATH_SET = -34,
    SREC_STORAGE_PATH_ALREADY_IN_USE = -35,
    SREC_MESSAGE_INCOMPLETE = -36,
    SREC_ARCHIVE_STORAGE_FAILED = -37,
    SREC_RETRIEVE_STORAGE_FAILURE = -38,
    SREC_INVALID_OPERATING_STATE = -39,
    SREC_INVALID_ARCHIVE_PATH = -40,
    SREC_DELETE_EXISTING_STORAGE_FAILED = -41,
    SREC_CREATE_DIRECTORIES_FAILED = -42,
    SREC_NOT_ENOUGH_MEMORY = -43,
    SREC_WARNING_TRIAL_MODE_ACTIVE = 44,
    SREC_NOT_RUNNING = -45,
    SREC_NOT_EMPTY = -46,
    SREC_NOT_UP_TO_DATE = -47,
    SREC_COMMUNICATION_INTERFACE_NOT_AVAILABLE = -48,
    SREC_WARNING_NOT_COMPLETE = 49,
    SREC_VIRTUAL_SWITCH_MISCONFIGURED = -50,
    SREC_RUNTIME_NOT_AVAILABLE = -51,
    SREC_IS_EMPTY = -52
};

enum EArea
{
    SRA_INVALID_AREA = 0,
    SRA_INPUT = 1,
    SRA_MARKER = 2,
    SRA_OUTPUT = 3,
    SRA_COUNTER = 4,
    SRA_TIMER = 5,
    SRA_DATABLOCK = 6,
    SRA_ENUMERATION_SIZE = 7
};

enum EOperatingState
{
    SROS_INVALID_OPERATING_STATE = 0,
    SROS_OFF = 1,
    SROS_BOOTING = 2,
    SROS_STOP = 3,
    SROS_STARTUP = 4,
    SROS_RUN = 5,
    SROS_FREEZE = 6,
    SROS_SHUTTING_DOWN = 7,
    SROS_ENUMERATION_SIZE = 8
};

enum ECPUType
{
    SRCT_1500_Unspecified = 0x000005DC, /// <summary> MLFB: 6ES7 SIM-01500-VPLC </summary>
    SRCT_1511 = 0x000005E7, /// <summary> MLFB: 6ES7 511-1AK00-0AB0 </summary>
    SRCT_1511v2 = 0x010005E7, /// <summary> MLFB: 6ES7 511-1AK01-0AB0 </summary>
    SRCT_1513 = 0x000005E9, /// <summary> MLFB: 6ES7 513-1AL00-0AB0 </summary>
    SRCT_1513v2 = 0x010005E9, /// <summary> MLFB: 6ES7 513-1AL01-0AB0 </summary>
    SRCT_1515 = 0x000005EB, /// <summary> MLFB: 6ES7 515-2AM00-0AB0 </summary>
    SRCT_1515v2 = 0x010005EB, /// <summary> MLFB: 6ES7 515-2AM01-0AB0 </summary>
    SRCT_1516 = 0x000005EC, /// <summary> MLFB: 6ES7 516-3AN00-0AB0 </summary>
    SRCT_1516v2 = 0x010005EC, /// <summary> MLFB: 6ES7 516-3AN01-0AB0 </summary>
    SRCT_1517 = 0x000005ED, /// <summary> MLFB: 6ES7 517-3AP00-0AB0 </summary>
    SRCT_1518 = 0x000005EE, /// <summary> MLFB: 6ES7 518-4AP00-0AB0 </summary>
    SRCT_1511C = 0x000405E7, /// <summary> MLFB: 6ES7 511-1CK00-0AB0 </summary>
    SRCT_1512C = 0x000405E8, /// <summary> MLFB: 6ES7 512-1CK00-0AB0 </summary>
    SRCT_1511F = 0x000105E7, /// <summary> MLFB: 6ES7 511-1FK00-0AB0 </summary>
    SRCT_1511Fv2 = 0x010105E7, /// <summary> MLFB: 6ES7 511-1FK01-0AB0 </summary>
    SRCT_1513F = 0x000105E9, /// <summary> MLFB: 6ES7 513-1FL00-0AB0 </summary>
    SRCT_1513Fv2 = 0x010105E9, /// <summary> MLFB: 6ES7 513-1FL01-0AB0 </summary>
    SRCT_1515F = 0x000105EB, /// <summary> MLFB: 6ES7 515-2FM00-0AB0 </summary>
    SRCT_1515Fv2 = 0x010105EB, /// <summary> MLFB: 6ES7 515-2FM01-0AB0 </summary>
    SRCT_1516F = 0x000105EC, /// <summary> MLFB: 6ES7 516-3FN00-0AB0 </summary>
    SRCT_1516Fv2 = 0x010105EC, /// <summary> MLFB: 6ES7 516-3FN01-0AB0 </summary>
    SRCT_1517F = 0x000105ED, /// <summary> MLFB: 6ES7 517-3FP00-0AB0 </summary>
    SRCT_1518F = 0x000105EE, /// <summary> MLFB: 6ES7 518-4FP00-0AB0 </summary>
    SRCT_1511T = 0x000805E7, /// <summary> MLFB: 6ES7 511-1TK01-0AB0 </summary>
    SRCT_1515T = 0x000805EB, /// <summary> MLFB: 6ES7 515-2TM01-0AB0 </summary>
    SRCT_1517T = 0x000805ED, /// <summary> MLFB: 6ES7 517-3TP00-0AB0 </summary>
    SRCT_1517TF = 0x000905ED, /// <summary> MLFB: 6ES7 517-3UP00-0AB0 </summary>
    SRCT_1518ODK = 0x001005EE, /// <summary> MLFB: 6ES7 518-4AP00-3AB0 </summary>
    SRCT_1518FODK = 0x001105EE, /// <summary> MLFB: 6ES7 518-4FP00-3AB0 </summary>
    SRCT_ET200SP_Unspecified = 0x000205DC, /// <summary> MLFB: 6ES7 SIM-01500-VPLC </summary>
    SRCT_1510SP = 0x000205E6, /// <summary> MLFB: 6ES7 510-1DJ00-0AB0 </summary>
    SRCT_1510SPv2 = 0x010205E6, /// <summary> MLFB: 6ES7 510-1DJ01-0AB0 </summary>
    SRCT_1512SP = 0x000205E8, /// <summary> MLFB: 6ES7 512-1DK00-0AB0 </summary>
    SRCT_1512SPv2 = 0x010205E8, /// <summary> MLFB: 6ES7 512-1DK01-0AB0 </summary>
    SRCT_1510SPF = 0x000305E6, /// <summary> MLFB: 6ES7 510-1SJ00-0AB0 </summary>
    SRCT_1510SPFv2 = 0x010305E6, /// <summary> MLFB: 6ES7 510-1SJ01-0AB0 </summary>
    SRCT_1512SPF = 0x000305E8, /// <summary> MLFB: 6ES7 512-1SK00-0AB0 </summary>
    SRCT_1512SPFv2 = 0x010305E8 /// <summary> MLFB: 6ES7 512-1SK01-0AB0 </summary>
};

enum ECommunicationInterface
{
    SRCI_NONE = 0,
    SRCI_SOFTBUS = 1,
    SRCI_TCPIP = 2,
    SRCI_ENUMERATION_SIZE = 3
};

enum ELEDType
{
    SRLT_STOP = 0,
    SRLT_RUN = 1,
    SRLT_ERROR = 2,
    SRLT_MAINT = 3,
    SRLT_REDUND = 4,
    SRLT_FORCE = 5,
    SRLT_BUSF1 = 6,
    SRLT_BUSF2 = 7,
    SRLT_BUSF3 = 8,
    SRLT_BUSF4 = 9,
    SRLT_ENUMERATION_SIZE = 10
};

enum ELEDMode
{
    SRLM_OFF = 0,
    SRLM_ON = 1,
    SRLM_FLASH_FAST = 2,
    SRLM_FLASH_SLOW = 3,
    SRLM_INVALID = 4
};

enum EPrimitiveDataType
{
    SRPDT_UNSPECIFIC = 0,
    SRPDT_STRUCT = 1,
    SRPDT_BOOL = 2,
    SRPDT_INT8 = 3,
    SRPDT_INT16 = 4,
    SRPDT_INT32 = 5,
    SRPDT_INT64 = 6,
    SRPDT_UINT8 = 7,
    SRPDT_UINT16 = 8,
    SRPDT_UINT32 = 9,
    SRPDT_UINT64 = 10,
    SRPDT_FLOAT = 11,
    SRPDT_DOUBLE = 12,
    SRPDT_CHAR = 13,
    SRPDT_WCHAR = 14
};

enum EDataType
{
    SRDT_UNKNOWN = 0,
    SRDT_BOOL = 1,
    SRDT_BYTE = 2,
    SRDT_CHAR = 3,
    SRDT_WORD = 4,
    SRDT_INT = 5,
    SRDT_DWORD = 6,
    SRDT_DINT = 7,
    SRDT_REAL = 8,
    SRDT_DATE = 9,
    SRDT_TIME_OF_DAY = 10,
    SRDT_TIME = 11,
    SRDT_S5TIME = 12,
    SRDT_DATE_AND_TIME = 14,
    SRDT_STRUCT = 17,
    SRDT_STRING = 19,
    SRDT_COUNTER = 28,
    SRDT_TIMER = 29,
    SRDT_IEC_Counter = 30,
    SRDT_IEC_Timer = 31,
    SRDT_LREAL = 48,
    SRDT_ULINT = 49,
    SRDT_LINT = 50,
    SRDT_LWORD = 51,
    SRDT_USINT = 52,
    SRDT_UINT = 53,
    SRDT_UDINT = 54,
    SRDT_SINT = 55,
    SRDT_WCHAR = 61,
    SRDT_WSTRING = 62,
    SRDT_LTIME = 64,
    SRDT_LTIME_OF_DAY = 65,
    SRDT_LDT = 66,
    SRDT_DTL = 67,
    SRDT_IEC_LTimer = 68,
    SRDT_IEC_SCounter = 69,
    SRDT_IEC_DCounter = 70,
    SRDT_IEC_LCounter = 71,
    SRDT_IEC_UCounter = 72,
    SRDT_IEC_USCounter = 73,
    SRDT_IEC_UDCounter = 74,
    SRDT_IEC_ULCounter = 75,
    SRDT_ERROR_STRUCT = 97,
    SRDT_NREF = 98,
    SRDT_CREF = 101,
    SRDT_AOM_IDENT = 128,
    SRDT_EVENT_ANY = 129,
    SRDT_EVENT_ATT = 130,
    SRDT_EVENT_HWINT = 131,
    SRDT_HW_ANY = 144,
    SRDT_HW_IOSYSTEM = 145,
    SRDT_HW_DPMASTER = 146,
    SRDT_HW_DEVICE = 147,
    SRDT_HW_DPSLAVE = 148,
    SRDT_HW_IO = 149,
    SRDT_HW_MODULE = 150,
    SRDT_HW_SUBMODULE = 151,
    SRDT_HW_HSC = 152,
    SRDT_HW_PWM = 153,
    SRDT_HW_PTO = 154,
    SRDT_HW_INTERFACE = 155,
    SRDT_HW_IEPORT = 156,
    SRDT_OB_ANY = 160,
    SRDT_OB_DELAY = 161,
    SRDT_OB_TOD = 162,
    SRDT_OB_CYCLIC = 163,
    SRDT_OB_ATT = 164,
    SRDT_CONN_ANY = 168,
    SRDT_CONN_PRG = 169,
    SRDT_CONN_OUC = 170,
    SRDT_CONN_R_ID = 171,
    SRDT_PORT = 173,
    SRDT_RTM = 174,
    SRDT_PIP = 175,
    SRDT_OB_PCYCLE = 192,
    SRDT_OB_HWINT = 193,
    SRDT_OB_DIAG = 195,
    SRDT_OB_TIMEERROR = 196,
    SRDT_OB_STARTUP = 197,
    SRDT_DB_ANY = 208,
    SRDT_DB_WWW = 209,
    SRDT_DB_DYN = 210,
    SRDT_DB = 257
};

enum EOperatingMode
{
    SROM_DEFAULT = 0,
    SROM_SINGLE_STEP = 1,
    SROM_EXTENDED_SINGLE_STEP = 2,
    SROM_TIMESPAN_SYNCHNRONIZED = 3
};

enum ETagListDetails
{
    SRTLD_NONE = 0,
    SRTLD_IO = 1,
    SRTLD_M = 2,
    SRTLD_IOM = 3,
    SRTLD_CT = 4,
    SRTLD_IOCT = 5,
    SRTLD_MCT = 6,
    SRTLD_IOMCT = 7,
    SRTLD_DB = 8,
    SRTLD_IODB = 9,
    SRTLD_MDB = 10,
    SRTLD_IOMDB = 11,
    SRTLD_CTDB = 12,
    SRTLD_IOCTDB = 13,
    SRTLD_MCTDB = 14,
    SRTLD_IOMCTDB = 15
};

enum ERuntimeConfigChanged
{
    SRCC_INSTANCE_REGISTERED = 0,
    SRCC_INSTANCE_UNREGISTERED = 1,
    SRCC_CONNECTION_OPENED = 2,
    SRCC_CONNECTION_CLOSED = 3,
    SRCC_PORT_OPENED = 4,
    SRCC_PORT_CLOSED = 5
};

enum EInstanceConfigChanged
{
    SRICC_HARDWARE_SOFTWARE_CHANGED = 0,
    SRICC_IP_CHANGED = 1
};

#pragma pack(push, 1)
union UIP
{
    DWORD IP;
    BYTE IPs[4];
};
#pragma pack(pop)

#pragma pack(push, 1)
union UDataValue
{
    bool Bool;
    INT8 Int8;
    INT16 Int16;
    INT32 Int32;
    INT64 Int64;
    UINT8 UInt8;
    UINT16 UInt16;
    UINT32 UInt32;
    UINT64 UInt64;
    float Float;
    double Double;
    CHAR Char;
    WCHAR WChar;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SIPSuite4
{
    UIP IPAddress;
    UIP SubnetMask;
    UIP DefaultGateway;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SDataValue
{
    UDataValue Value;
    EPrimitiveDataType Type;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SDataValueByAddress
{
    UINT32 Offset;
    UINT8 Bit;
    SDataValue DataValue;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SDataValueByName
{
    WCHAR Name[DTAG_NAME_MAX_LENGTH];
    SDataValue DataValue;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SConnectionInfo
{
    UIP IP;
    UINT16 Port;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SInstanceInfo
{
    INT32 ID;
    WCHAR Name[DINSTANCE_NAME_MAX_LENGTH];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SDimension
{
    INT32 StartIndex;
    UINT32 Count;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct STagInfo
{
    WCHAR Name[DTAG_NAME_MAX_LENGTH];
    EArea Area;
    EDataType DataType;
    EPrimitiveDataType PrimitiveDataType;
    UINT16 Size;
    UINT32 Offset;
    UINT8 Bit;
    UINT8 DimensionCount;
    UINT32 Index;
    UINT32 ParentIndex;
    SDimension Dimension[DTAG_ARRAY_DIMENSION];
};
#pragma pack(pop)

typedef void (*EventCallback_VOID)();
typedef void (*EventCallback_IRRTM)(IRemoteRuntimeManager* in_Sender);
typedef void (*EventCallback_II_SREC_ST_SROS_SROS)(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime, EOperatingState in_PrevState, EOperatingState in_OperatingState);
typedef void (*EventCallback_II_SREC_ST)(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime);
typedef void (*EventCallback_II_SREC_ST_SRLT_SRLM)(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime, ELEDType in_LEDType, ELEDMode in_LEDMode);
typedef void (*EventCallback_II_SREC_ST_INT64_UINT32)(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime, INT64 in_CycleTime_ns, UINT32 in_CycleCount);
typedef void (*EventCallback_SRCC_UINT32_UINT32_INT32)(ERuntimeConfigChanged in_RuntimeConfigChanged, UINT32 in_Param1, UINT32 in_Param2, INT32 in_Param3);
typedef void (*EventCallback_II_SREC_ST_SRICC_UINT32_UINT32_UINT32_UINT32)(IInstance* in_Sender, ERuntimeErrorCode in_ErrorCode, SYSTEMTIME in_SystemTime, EInstanceConfigChanged in_InstanceConfigChanged, UINT32 in_Param1, UINT32 in_Param2, UINT32 in_Param3, UINT32 in_Param4);
/// <summary>
/// This is the type of the API DLL main entry point.
/// </summary>
typedef ERuntimeErrorCode(*ApiEntry_Initialize)(ISimulationRuntimeManager** out_RuntimeManagerInterface, UINT32 in_InterfaceVersion);
/// <summary>
/// This is the type of the DestroyInterface entry point.
/// </summary>
typedef ERuntimeErrorCode(*ApiEntry_DestroyInterface)(IBaseInterface* in_Interface);
static ApiEntry_DestroyInterface DestroyInterface = NULL;

static HMODULE _SimulationRuntimeApiDllHandle = NULL;

/// <summary>
/// Frees the library of the Runtime API.
/// </summary>
inline ERuntimeErrorCode FreeApi();

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_AreaSection"> The enumeration entry. </param>
inline const WCHAR* GetNameOfAreaSection(EArea in_AreaSection);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_CPUType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfCPUType(ECPUType in_CPUType);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_CommunicationInterface"> The enumeration entry. </param>
inline const WCHAR* GetNameOfCommunicationInterface(ECommunicationInterface in_CommunicationInterface);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_DataType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfDataType(EDataType in_DataType);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_ErrorCode"> The enumeration entry. </param>
inline const WCHAR* GetNameOfErrorCode(ERuntimeErrorCode in_ErrorCode);

inline const WCHAR* GetNameOfInstanceConfigChanged(EInstanceConfigChanged in_InstanceConfigChanged);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_LEDMode"> The enumeration entry. </param>
inline const WCHAR* GetNameOfLEDMode(ELEDMode in_LEDMode);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_LEDType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfLEDType(ELEDType in_LEDType);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_OperatingMode"> The enumeration entry. </param>
inline const WCHAR* GetNameOfOperatingMode(EOperatingMode in_OperatingMode);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_OperatingState"> The enumeration entry. </param>
inline const WCHAR* GetNameOfOperatingState(EOperatingState in_OperatingState);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_DataType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfPrimitiveDataType(EPrimitiveDataType in_DataType);

inline const WCHAR* GetNameOfRuntimeConfigChanged(ERuntimeConfigChanged in_RuntimeConfigChanged);

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_TagListDetails"> The enumeration entry. </param>
inline const WCHAR* GetNameOfTagListDetails(ETagListDetails in_TagListDetails);

/// <summary>
/// Loads "Siemens.Simatic.PlcSim.Advanced.Api.dll" and initializes the API.
/// </summary>
/// <param name="inout_SimulationRuntimeManagerInterface"> Pointer to a RuntimeManager interface
/// pointer. The Pointer must be initialized with NULL. The interface will be created inside the function.
/// </param>
inline ERuntimeErrorCode InitializeApi(ISimulationRuntimeManager** inout_SimulationRuntimeManagerInterface);

inline ERuntimeErrorCode InitializeApi(WCHAR* in_SimulationRuntimeApiDllPath, ISimulationRuntimeManager** inout_SimulationRuntimeManagerInterface);

/// <summary>
/// Shuts down the Runtime API, destroys the IRuntimeManager interface and frees the library of the Runtime API.
/// </summary>
/// <param name="in_SimulationRuntimeManagerInterface"> The interface of the Runtime Manager to be destroyed. </param>
inline ERuntimeErrorCode ShutdownAndFreeApi(ISimulationRuntimeManager* in_SimulationRuntimeManagerInterface);

class IBaseInterface
{
    ////    Constructors and destructors    ////
    
public :

    inline virtual ~IBaseInterface()
    {
    }
};

class ISimulationRuntimeManager : public IBaseInterface
{
    ////    Operations    ////
    
public :

    /// <summary>
    /// Closes the opened port and all open connections another Runtime Manager has established to the opened port.
    /// </summary>
    virtual ERuntimeErrorCode ClosePort() = 0;
    
    /// <summary>
    /// Creates and returns an interface of an already registered virtual controller instance.
    /// </summary>
    /// <param name="in_InstanceID"> This name of the registered instance you would like to get the interface from. </param>
    /// <param name="out_InstanceInterface"> The ID of the registered instance you would like to get the interface from. </param>
    virtual ERuntimeErrorCode CreateInterface(INT32 in_InstanceID, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Creates and returns an interface of an already registered virtual controller instance.
    /// </summary>
    /// <param name="in_InstanceName"> This name of the registered instance you would like to get the interface from. </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode CreateInterface(WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Returns the open port. Returns 0 if no port is open or if the function fails.
    /// </summary>
    virtual UINT16 GetPort() = 0;
    
    /// <summary>
    /// Returns the info about an already registered instance. You can use its ID or name to create an interface of this instance.
    /// </summary>
    /// <param name="in_Index"> Index of the created instance you want to get the info from.
    /// Must be smaller than the value you get from calling GetRegisteredInstanceCount(). </param>
    /// <param name="out_InstanceInfo"> The info of the instance containing its name and ID. </param>
    virtual ERuntimeErrorCode GetRegisteredInstanceInfoAt(UINT32 in_Index, SInstanceInfo* out_InstanceInfo) = 0;
    
    /// <summary>
    /// Returns the number of instances registered at the RuntimeManager. Return 0 if the function fails.
    /// </summary>
    virtual UINT32 GetRegisteredInstancesCount() = 0;
    
    /// <summary>
    /// Returns the info about an open connection.
    /// </summary>
    /// <param name="in_Index"> The index of the connection info to get. </param>
    /// <param name="out_ConnectionInfo"> The connection info at this index. </param>
    virtual ERuntimeErrorCode GetRemoteConnectionInfoAt(UINT32 in_Index, SConnectionInfo* out_ConnectionInfo) = 0;
    
    /// <summary>
    /// Returns the amount of open remote connections. Returns 0 if the function fails.
    /// </summary>
    virtual UINT32 GetRemoteConnectionsCount() = 0;
    
    /// <summary>
    /// Returns the version of the Runtime Manager. If the function fails the version returned is 0.
    /// </summary>
    virtual UINT32 GetVersion() = 0;
    
    /// <summary>
    /// Returns a value indicating whether the API has been initialized successfully.
    /// </summary>
    virtual bool IsInitialized() = 0;
    
    /// <summary>
    /// The function returns false if the pipe connection to the RuntimeManager
    /// is lost. This only happenes when the process of the RuntimeManager is closed.
    /// </summary>
    virtual bool IsRuntimeManagerAvailable() = 0;
    
    /// <summary>
    /// Opens a port another Runtime Manager can connect to.
    /// </summary>
    /// <param name="in_Port"> The port. Must be bigger than 1024. </param>
    virtual ERuntimeErrorCode OpenPort(UINT16 in_Port) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_VplcDll"> The complete path to the dll of the virtual controller
    /// the Siemens.Simatic.Simulation.Runtime.Instance.exe will load when powering on.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterCustomInstance(WCHAR* in_VplcDll, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_VplcDll"> The complete path to the dll of the virtual controller
    /// the Siemens.Simatic.Simulation.Runtime.Instance.exe will load when powering on.
    /// </param>
    /// <param name="in_InstanceName"> This will be the name of the instance. Each instance must have a unique
    /// name. If no name is given when registering a new instance the instance is named "Instance_#" where # is the ID of
    /// the instance. If this name already exists the name will be "Instance_#.#" where the second # is a counter that
    /// will be increased until the name is unique. The length of the name must be smaller than DINSTANCE_NAME_LENGTH.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterCustomInstance(WCHAR* in_VplcDll, WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_InstanceName"> This will be the name of the instance. Each instance must have a unique
    /// name. If no name is given when registering a new instance the instance is named "Instance_#" where # is the ID of
    /// the instance. If this name already exists the name will be "Instance_#.#" where the second # is a counter that
    /// will be increased until the name is unique. The length of the name must be smaller than DINSTANCE_NAME_LENGTH.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_CPUType"> Defines which type of CPU will be simulated
    /// when starting the instance. The default value is "SRCT_1500_Unspecified".
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(ECPUType in_CPUType, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_CPUType"> Defines which type of CPU will be simulated
    /// when starting the instance. The default value is "SRCT_1500_Unspecified".
    /// </param>
    /// <param name="in_InstanceName"> This will be the name of the instance. Each instance must have a unique
    /// name. If no name is given when registering a new instance the instance is named "Instance_#" where # is the ID of
    /// the instance. If this name already exists the name will be "Instance_#.#" where the second # is a counter that
    /// will be increased until the name is unique. The length of the name must be smaller than DINSTANCE_NAME_LENGTH.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(ECPUType in_CPUType, WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnConfigurationChangedCallback(EventCallback_SRCC_UINT32_UINT32_INT32 in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnConfigurationChangedEvent() = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnConfigurationChangedEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnRunTimeManagerLostCallback(EventCallback_VOID in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnRunTimeManagerLostEvent() = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnRunTimeManagerLostEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// Creates a new connection to a remote Runtime Manager or uses an already existing one to create an IRemoteRuntimeManager interface.
    /// </summary>
    /// <param name="in_IP"> The IP address of the remote PC. </param>
    /// <param name="in_Port"> The port open on the remote PC. </param>
    /// <param name="out_RunTimeManagerInterface"> Pointer to a remote RuntimeManager interface pointer.
    /// The pointer must be initialized with NULL. The interface will be created inside the function. </param>
    virtual ERuntimeErrorCode RemoteConnect(UIP in_IP, UINT16 in_Port, IRemoteRuntimeManager** out_RunTimeManagerInterface) = 0;
    
    /// <summary>
    /// Creates a new connection to a remote Runtime Manager or uses an already existing one to create an IRemoteRuntimeManager interface.
    /// </summary>
    /// <param name="in_IP3"> The first part of the IP address of the remote PC. </param>
    /// <param name="in_IP2"> The second part of the IP address of the remote PC. </param>
    /// <param name="in_IP1"> The third part of the IP address of the remote PC. </param>
    /// <param name="in_IP0"> The last part of the IP address of the remote PC. </param>
    /// <param name="in_Port"> The port open on the remote PC. </param>
    /// <param name="out_RunTimeManagerInterface"> Pointer to a remote RuntimeManager interface pointer.
    /// The pointer must be initialized with NULL. The interface will be created inside the function. </param>
    virtual ERuntimeErrorCode RemoteConnect(UINT8 in_IP3, UINT8 in_IP2, UINT8 in_IP1, UINT8 in_IP0, UINT16 in_Port, IRemoteRuntimeManager** out_RunTimeManagerInterface) = 0;
    
    /// <summary>
    /// Ends the communication with the Runtime Manager and cleans up the interfaces.
    /// </summary>
    virtual ERuntimeErrorCode Shutdown() = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnConfigurationChangedCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnConfigurationChangedEvent() = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnRunTimeManagerLostCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnRunTimeManagerLostEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnConfigurationChangedEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milli seconds. </param>
    virtual bool WaitForOnConfigurationChangedEvent(UINT32 in_Time_ms) = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnRunTimeManagerLostEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milli seconds. </param>
    virtual bool WaitForOnRunTimeManagerLostEvent(UINT32 in_Time_ms) = 0;
};

class IInstance : public IBaseInterface
{
    ////    Constructors and destructors    ////
    
public :

    inline virtual ~IInstance()
    {
    }
    
    ////    Operations    ////
    
    /// <summary>
    /// Stores the virtual memory card in a .zip file. The virtual controller has to be in the operatig state off.
    /// </summary>
    /// <param name="in_ArchiveStorageFile"> The full file path to the .zip file. The
    /// path is based on the directories of the computer the instance is running on.
    /// </param>
    virtual ERuntimeErrorCode ArchiveStorage(WCHAR* in_ArchiveStorageFile) = 0;
    
    /// <summary>
    /// Writes all taglist entries from the taglist storage into an XML file.
    /// </summary>
    /// <param name="in_FullFileName"> The full filename of the XML file ('path' + 'filename' + 'file extension'). </param>
    virtual ERuntimeErrorCode CreateConfigurationFile(WCHAR* in_FullFileName) = 0;
    
    /// <summary>
    /// Returns the size in bytes of the area. Returns 0 if the function fails.
    /// </summary>
    /// <param name="in_Area"> The area to get the size from. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    virtual UINT32 GetAreaSize(EArea in_Area) = 0;
    
    /// <summary>
    /// Returns the CPU type of the instance.
    /// </summary>
    virtual ECPUType GetCPUType() = 0;
    
    /// <summary>
    /// Returns the communication interface of the virtual controller. A change of the communication interface will only
    /// happen when the controller is restarted. All instances that are started have to use the same communication interface.
    /// PowerOn will be prevented, if another communication interface is selected, than the one that other running instances are
    /// using.
    /// </summary>
    virtual ECommunicationInterface GetCommunicationInterface() = 0;
    
    /// <summary>
    /// Returns a configured IP address of the instance.
    /// </summary>
    virtual UIP GetControllerIP() = 0;
    
    /// <summary>
    /// Returns a configured IP address of the instance.
    /// </summary>
    /// <param name="in_Index"> The index of the IP address to get. The index has to be
    /// smaller than the result returned by GetControllerIPCount(). The default value is
    /// 0.</param>
    virtual UIP GetControllerIP(UINT32 in_Index) = 0;
    
    /// <summary>
    /// Returns the amount of configured IP addresses of the virtual controller.Returns 0 if the function fails.
    /// </summary>
    virtual UINT32 GetControllerIPCount() = 0;
    
    virtual SIPSuite4 GetControllerIPSuite4() = 0;
    
    virtual SIPSuite4 GetControllerIPSuite4(UINT32 in_Index) = 0;
    
    /// <summary>
    /// Returns the downloaded controller name of the virtual controller.
    /// </summary>
    /// <param name="inout_Name"> A user allocated buffer for the device name. </param>
    /// <param name="in_ArrayLength"> The length of the buffer. </param>
    virtual ERuntimeErrorCode GetControllerName(WCHAR inout_Name[], UINT32 in_ArrayLength) = 0;
    
    /// <summary>
    /// Returns the downloaded short designation of the virtual controller.
    /// </summary>
    /// <param name="inout_ShortDesignation"> A user allocated buffer for the short designation. </param>
    /// <param name="in_ArrayLength"> The length of the buffer. </param>
    virtual ERuntimeErrorCode GetControllerShortDesignation(WCHAR inout_ShortDesignation[], UINT32 in_ArrayLength) = 0;
    
    /// <summary>
    /// Returns the ID of the instance. The ID is assigned by the Runtime Manager when registering the instance.
    /// </summary>
    virtual INT32 GetID() = 0;
    
    /// <summary>
    /// Returns a struct providing information about the instance.
    /// </summary>
    virtual SInstanceInfo GetInfo() = 0;
    
    /// <summary>
    /// Returns the name of the instance.
    /// </summary>
    /// <param name="inout_Name"> a user allocated buffer for the instance name.
    /// The array length should be at least as big as DINSTANCE_NAME_MAX_LENGTH.
    /// </param>
    /// <param name="in_ArrayLength"> The length of the array in wide characters. </param>
    virtual ERuntimeErrorCode GetName(WCHAR inout_Name[], UINT32 in_ArrayLength) = 0;
    
    /// <summary>
    /// Gets the operating mode of the virtual controller.
    /// </summary>
    virtual EOperatingMode GetOperatingMode() = 0;
    
    /// <summary>
    /// Returns the operating state of the virtual controller.
    /// </summary>
    virtual EOperatingState GetOperatingState() = 0;
    
    /// <summary>
    /// Gets the overwritten minimal cycle time in nanoseconds used in the extended singlestep operating mode.
    /// </summary>
    virtual INT64 GetOverwrittenMinimalCycleTime_ns() = 0;
    
    /// <summary>
    /// Gets the scale factor of the virtual clock.
    /// </summary>
    virtual double GetScaleFactor() = 0;
    
    /// <summary>
    /// Returns the full path of the directory the instance is storing its data.
    /// </summary>
    /// <param name="inout_StoragePath"> a user allocated buffer for the storage
    /// path. The array length should be at least as big as DSTARTUP_PATH_MAX_LENGTH.
    /// </param>
    /// <param name="in_ArrayLength"> The length of the array in wide characters. </param>
    virtual ERuntimeErrorCode GetStoragePath(WCHAR inout_StoragePath[], UINT32 in_ArrayLength) = 0;
    
    /// <summary>
    /// Returns the virtual system time of the virtual controller. Returns an empty struct if the function fails.
    /// </summary>
    virtual SYSTEMTIME GetSystemTime() = 0;
    
    /// <summary>
    /// Returns the amount of entries of the taglist storage. Returns 0 if the function fails.
    /// </summary>
    virtual UINT32 GetTagInfoCount() = 0;
    
    /// <summary>
    /// Get a list of all tags.
    /// </summary>
    /// <param name="in_BufferLength"> The length of the user allocated buffer to take the tags. </param>
    /// <param name="inout_TagInfos"> The user allocated buffer to take the tags. </param>
    /// <param name="out_TagCount"> Returns the amount of tags written into the buffer. </param>
    virtual ERuntimeErrorCode GetTagInfos(UINT32 in_BufferLength, STagInfo* inout_TagInfos, UINT32* out_TagCount) = 0;
    
    /// <summary>
    /// Returns the current update state of the taglist storage. "inout_TagListDetails" is "NONE" if the list needs to be updated.
    /// </summary>
    /// <param name="out_TagListDetails"> The status of the tag list details. SRTLD_NONE if the list needs to be updated. </param>
    /// <param name="out_IsHMIVisibleOnly"> If "true", only tags marked as "HMI Visible" are available in the list. </param>
    virtual ERuntimeErrorCode GetTagListStatus(ETagListDetails* out_TagListDetails, bool* out_IsHMIVisibleOnly) = 0;
    
    /// <summary>
    /// Gets the AlwaysSendOnEndOfCycle mode. Returns false if the function fails.
    /// </summary>
    virtual bool IsAlwaysSendOnEndOfCycleEnabled() = 0;
    
    /// <summary>
    /// Shuts down the virtual controller, closes its process and restarts it again.
    /// </summary>
    virtual ERuntimeErrorCode MemoryReset() = 0;
    
    /// <summary>
    /// Shuts down the virtual controller, closes its process and restarts it again.
    /// </summary>
    /// <param name="in_TimeOut_ms">
    /// A timeout value in milliseconds.
    /// - If no timeout is set the function returns immediately y. Subscribe to
    /// the OnOperatingStateChanged() event to get informed when the operation is
    /// done.
    /// - If set to a value greater than 0 (recommended is a value of 60000) the function will return after the operation is done
    /// or after the timeout value. Expected operating states for this function to return successfully are: { SROS_STOP, SROS_RUN }
    /// </param>
    virtual ERuntimeErrorCode MemoryReset(UINT32 in_TimeOut_ms) = 0;
    
    /// <summary>
    /// Shuts down the virtual controller and closes its process.
    /// </summary>
    virtual ERuntimeErrorCode PowerOff() = 0;
    
    /// <summary>
    /// Shuts down the virtual controller and closes its process.
    /// </summary>
    /// <param name="in_TimeOut_ms"> A timeout value in milliseconds.
    /// - If no timeout is set the function returns immediately. Subscribe to
    /// the OnOperatingStateChanged() event to get informed when the operation is
    /// done.
    /// - If set to a value greater than 0 (recommended is a value of 60000) the function will return after the operation is
    /// done or after the timeout value. Expected operating states for this function to return successfully are: { SROS_OFF }
    /// </param>
    virtual ERuntimeErrorCode PowerOff(UINT32 in_TimeOut_ms) = 0;
    
    /// <summary>
    /// Creates the process of the simulation runtime instance and starts the firmware of the virtual controller.
    /// </summary>
    virtual ERuntimeErrorCode PowerOn() = 0;
    
    /// <summary>
    /// Creates the process of the simulation runtime instance and starts the firmware of the virtual controller.
    /// </summary>
    /// <param name="in_TimeOut_ms"> A timeout value in milliseconds.
    /// - If no timeout is set the function returns immediately. Subscribe to
    /// the OnOperatingStateChanged() event to get informed when the operation is
    /// done.
    /// - If set to a value greater than 0 (recommended is a value of 60000) the function will return after the operation is done
    /// or after the timeout value. Expected operating states for this function to return successfully are: { SROS_STOP, SROS_RUN }
    /// </param>
    virtual ERuntimeErrorCode PowerOn(UINT32 in_TimeOut_ms) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to read. </param>
    /// <param name="inout_Value"> Contains the value and the expected type of the PLC variable.
    /// If the expected type is UNSPECIFIC the expected type will be set to the stored
    /// type when the function returns successfully. The type STRUCT is not supported.
    /// </param>
    virtual ERuntimeErrorCode Read(WCHAR* in_Tag, SDataValue* inout_Value) = 0;
    
    /// <summary>
    /// Reads a single bit from the area.
    /// </summary>
    /// <param name="in_Area"> The area to read from. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="in_Offset"> The byte offset inside the area. The value must be between 0 and the value returned by GetAreaSize(). </param>
    /// <param name="in_Bit"> The bit offset inside the byte. Must be a value between 0 and 7. </param>
    /// <param name="out_Value"> Returns the value of the bit. </param>
    virtual ERuntimeErrorCode ReadBit(EArea in_Area, UINT32 in_Offset, UINT8 in_Bit, bool* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadBool(WCHAR* in_Tag, bool* out_Value) = 0;
    
    /// <summary>
    /// Reads a single byte from the area.
    /// </summary>
    /// <param name="in_Area"> The area to read from. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="in_Offset"> The byte offset inside the area. The value must be between 0 and the value returned by GetAreaSize().</param>
    /// <param name="out_Value"> Returns the value of the byte. </param>
    virtual ERuntimeErrorCode ReadByte(EArea in_Area, UINT32 in_Offset, BYTE* out_Value) = 0;
    
    /// <summary>
    /// Reads an array of bytes from the area.
    /// </summary>
    /// <param name="in_Area"> The area to read from. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="in_Offset"> The byte offset inside the area. The value must be between 0 and the value returned by GetAreaSize().</param>
    /// <param name="in_BytesToRead"> Contains the size of the value buffer. </param>
    /// <param name="out_BytesRead"> Contains the amount of bytes that have actually been written into the value buffer. </param>
    /// <param name="inout_Values"> The buffer for the bytes to read from the area. </param>
    virtual ERuntimeErrorCode ReadBytes(EArea in_Area, UINT32 in_Offset, UINT32 in_BytesToRead, UINT32* out_BytesRead, BYTE inout_Values[]) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadChar(WCHAR* in_Tag, char* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadDouble(WCHAR* in_Tag, double* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadFloat(WCHAR* in_Tag, float* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadInt16(WCHAR* in_Tag, INT16* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadInt32(WCHAR* in_Tag, INT32* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadInt64(WCHAR* in_Tag, INT64* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadInt8(WCHAR* in_Tag, INT8* out_Value) = 0;
    
    /// <summary>
    /// Reads a list of signals at once. Only signals of the primitive data type are
    /// supported but the function is not type safe. The function is also taking care of the
    /// endianness.
    /// </summary>
    /// <param name="in_Area"> The area to read from. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="inout_Signals"> The list of the signals to read. The result will be stored in the struct. </param>
    /// <param name="in_SignalCount"> The amount of signals in the list. </param>
    virtual ERuntimeErrorCode ReadSignals(EArea in_Area, SDataValueByAddress* inout_Signals, UINT32 in_SignalCount) = 0;
    
    /// <summary>
    /// Reads multiple signals within a single API call.
    /// </summary>
    /// <param name="inout_Signals"> Contains name, the value and the expected type of the PLC variable.
    /// If the expected type is UNSPECIFIC the expected type will be set to the stored
    /// type when the function returns successfully. The type STRUCT is not supported.
    /// </param>
    /// <param name="in_SignalCount"> The amount of signals to read. </param>
    virtual ERuntimeErrorCode ReadSignals(SDataValueByName* inout_Signals, UINT32 in_SignalCount) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadUInt16(WCHAR* in_Tag, UINT16* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadUInt32(WCHAR* in_Tag, UINT32* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadUInt64(WCHAR* in_Tag, UINT64* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadUInt8(WCHAR* in_Tag, UINT8* out_Value) = 0;
    
    /// <summary>
    /// Reads the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag">The name of the PLC variable to read. </param>
    /// <param name="out_Value"> Contains the value of the PLC variable. </param>
    virtual ERuntimeErrorCode ReadWChar(WCHAR* in_Tag, WCHAR* out_Value) = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnConfigurationChangedCallback(EventCallback_II_SREC_ST_SRICC_UINT32_UINT32_UINT32_UINT32 in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnConfigurationChangedEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnConfigurationChangedEvent() = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnConfigurationChangingCallback(EventCallback_II_SREC_ST in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnConfigurationChangingEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnConfigurationChangingEvent() = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnEndOfCycleCallback(EventCallback_II_SREC_ST_INT64_UINT32 in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnEndOfCycleEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnEndOfCycleEvent() = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnLedChangedCallback(EventCallback_II_SREC_ST_SRLT_SRLM in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnLedChangedEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnLedChangedEvent() = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnOperatingStateChangedCallback(EventCallback_II_SREC_ST_SROS_SROS in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnOperatingStateChangedEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnOperatingStateChangedEvent() = 0;
    
    /// <summary>
    /// Restores a virtual memory card from a .zip file. The virtual controller has to be in the operatig state off.
    /// </summary>
    /// <param name="in_ArchiveFile"> The full file path to the .zip file. The
    /// path is based on the directories of the computer the instance is running on.
    /// </param>
    virtual ERuntimeErrorCode RetrieveStorage(WCHAR* in_ArchiveFile) = 0;
    
    /// <summary>
    /// Requests the virtual controller to switch into run mode.
    /// </summary>
    virtual ERuntimeErrorCode Run() = 0;
    
    /// <summary>
    /// Requests the virtual controller to switch into run mode.
    /// </summary>
    /// <param name="in_TimeOut_ms"> A timeout value in milliseconds.
    /// - If no timeout is set the function returns immediately. Subscribe to
    /// the OnOperatingStateChanged() event to get informed when the operation is
    /// done.
    /// - If set to a value greater than 0 (recommended is a value of 60000) the function will return after the operation is
    /// done or after the timeout value. Expected operating states for this function to return successfully are: { SROS_RUN }
    /// </param>
    virtual ERuntimeErrorCode Run(UINT32 in_TimeOut_ms) = 0;
    
    /// <summary>
    /// When operating the virtual controller in the single step or extended single step operating mode the virtual
    /// controller will go into freeze state at the end of each cycle. With this function the next cycle will be
    /// triggered.
    /// </summary>
    virtual ERuntimeErrorCode RunNextCycle() = 0;
    
    /// <summary>
    /// Sets the AlwaysSendOnEndOfCycle mode.If set, the OnEndOfCycle event will be fired at the end of each cycle
    /// for each operating mode. This has to be set to true if the event should also be received in the default operating
    /// mode.
    /// </summary>
    /// <param name="in_Enable"> If true the OnEndOfCycle event will be fired at the end of each cycle. </param>
    virtual ERuntimeErrorCode SetAlwaysSendOnEndOfCycleEnabled(bool in_Enable) = 0;
    
    /// <summary>
    /// Sets the CPU type of the instance.
    /// </summary>
    /// <param name="in_CPUType"> Defines which type of CPU will be simulated when starting the instance. </param>
    virtual void SetCPUType(ECPUType in_CPUType) = 0;
    
    /// <summary>
    /// Sets the communication interface of the virtual controller. A change of the communication interface will only
    /// happen when the controller is restarted. All instances that are started have to use the same communication interface.
    /// PowerOn will be prevented, if another communication interface is selected, than the one that other running instances are
    /// using.
    /// </summary>
    /// <param name="in_CommunicationInterface">
    /// - SRCI_NONE
    /// can not be selected. Is returned when the instance interface is not valid anymore.
    /// - SRCI_SOFTBUS
    /// the virtual controller is using the Softbus.
    /// - SRCI_TCPIP
    /// the virtual controller is using the virtual Switch.
    ///  </param>
    virtual void SetCommunicationInterface(ECommunicationInterface in_CommunicationInterface) = 0;
    
    /// <summary>
    /// Sets the IP suite of a network interface of the virtual controller.
    /// </summary>
    /// <param name="in_InterfaceID"> The ID of the network interface. </param>
    /// <param name="in_IPSuite"> The IP suite to be set to the network interface. The IP suite is containing the IP address, the subnet
    /// mask and the default gateway. If the communication interface is set to Softbus subnet mask and default gateway will be ignored.
    /// </param>
    /// <param name="in_IsRemanent"> If set to true, the IP suite will be restored after restarting the
    /// virtual controller. If the communication interface is set to Softbus this flag will be ignored.
    /// </param>
    virtual ERuntimeErrorCode SetIPSuite(UINT32 in_InterfaceID, SIPSuite4 in_IPSuite, bool in_IsRemanent) = 0;
    
    /// <summary>
    /// Sets the operating mode of the virtual controller. Changes of the value during runtime take effect on the cycle control point.
    /// </summary>
    /// <param name="in_OperatingMode"> The operating mode of the virtual controller. </param>
    virtual void SetOperatingMode(EOperatingMode in_OperatingMode) = 0;
    
    /// <summary>
    /// Sets the overwritten minimal cycle time in nanoseconds used in the extended singlestep operating mode.
    /// A valid input is a value between 0 and 6,000,000,000. Changes of the value during runtime take effect on the cycle control point.
    /// </summary>
    /// <param name="in_CycleTime_ns"> The overwritten minimal cycle time in nanoseconds. </param>
    virtual ERuntimeErrorCode SetOverwrittenMinimalCycleTime_ns(INT64 in_CycleTime_ns) = 0;
    
    /// <summary>
    /// Sets the scale factor of the virtual clock. A valid input value X is a value between 0 and 1000. The
    /// default value is 1.0.If the value is smaller than 1 the virtual time of the virtual controller runs X times
    /// slower than the wall clock time. If the value is bigger than 1 the virtual time of the virtual controller runs
    /// X times faster than the wall clock time.Changes of the value during runtime take effect at the cycle control
    /// point.
    /// </summary>
    /// <param name="in_Value"> The scale factor of the virtual clock. </param>
    virtual ERuntimeErrorCode SetScaleFactor(double in_Value) = 0;
    
    /// <summary>
    /// Sets the full path of the directory the instance is storing its data. This can also be a network share. 
    /// The path has to be set while the instance is not running. The instance will read the path during powering on only.
    /// If no path is set the default path will be
    /// 'user documents folder'\Siemens\Simatic\Simulation\Runtime\Persistence\'Instance Name'
    /// </summary>
    /// <param name="in_StoragePath"> The full name of the storage path.
    /// The length of the name has to be less than DSTARTUP_PATH_MAX_LENGTH.
    /// </param>
    virtual ERuntimeErrorCode SetStoragePath(WCHAR* in_StoragePath) = 0;
    
    /// <summary>
    /// Sets the virtual system time of the virtual controller. A valid input is a system time between
    /// Jan 1 1970 00:00:00:000 and Dec 31 2200 23:59:59:999
    /// </summary>
    /// <param name="in_SystemTime"> The system time to set. </param>
    virtual ERuntimeErrorCode SetSystemTime(SYSTEMTIME in_SystemTime) = 0;
    
    /// <summary>
    /// When operating the virtual controller in the timespan synchronized operating mode the virtual controller will go into
    /// freeze state at the cycle control point. This function will wake up the virtual controller. The virtual controller will now
    /// run for at least the requested time based on virtual time before going back into freeze state at the next cycle control
    /// point.
    /// </summary>
    /// <param name="in_MinimalTimeToRun_ns"> The minimal virtual time in
    /// nanoseconds the virtual controller will run before going into freeze state.
    /// </param>
    virtual ERuntimeErrorCode StartProcessing(INT64 in_MinimalTimeToRun_ns) = 0;
    
    /// <summary>
    /// Requests the virtual controller to switch into stop mode.
    /// </summary>
    virtual ERuntimeErrorCode Stop() = 0;
    
    /// <summary>
    /// Requests the virtual controller to switch into stop mode.
    /// </summary>
    /// <param name="in_TimeOut_ms">
    /// A timeout value in milliseconds.
    /// - If no timeout is set the function returns immediately. Subscribe to
    /// the OnOperatingStateChanged() event to get informed when the operation is
    /// done.
    /// - If set to a value greater than 0 (recommended is a value of 60000) the function will return after the operation
    /// is done or after the timeout value. Expected operating states for this function to return successfully are:
    /// { SROS_STOP }
    /// </param>
    virtual ERuntimeErrorCode Stop(UINT32 in_TimeOut_ms) = 0;
    
    /// <summary>
    /// Unregisters this instance from the Runtime Manager.
    /// </summary>
    virtual ERuntimeErrorCode UnregisterInstance() = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnConfigurationChangedCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnConfigurationChangedEvent() = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnConfigurationChangingCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnConfigurationChangingEvent() = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnEndOfCycleCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnEndOfCycleEvent() = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnLedChangedCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnLedChangedEvent() = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnOperatingStateChangedCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnOperatingStateChangedEvent() = 0;
    
    /// <summary>
    /// Reads the tags from the VPLC and writes them into the shared memory ordered by name.
    /// </summary>
    /// <param name="in_TagListDetails"> Any combination of the following four areas:
    /// IO: Inputs and Outpus
    /// M: Marker
    /// CT: Counter and Timer
    /// DB: Datablocks
    /// e.g. IOM will only read the tags from the input/output and marker area.
    /// The default value is IOMCTDB. </param>
    /// <param name="in_IsHMIVisibleOnly"> If "true", only tags marked as "HMI Visible" will be read. The default value is "false". </param>
    virtual ERuntimeErrorCode UpdateTagList(ETagListDetails in_TagListDetails, bool in_IsHMIVisibleOnly) = 0;
    
    /// <summary>
    /// Reads the tags from the VPLC and writes them into the shared memory ordered by name.
    /// </summary>
    /// <param name="in_TagListDetails"> Any combination of the following four areas:
    /// IO: Inputs and Outpus
    /// M: Marker
    /// CT: Counter and Timer
    /// DB: Datablocks
    /// e.g. IOM will only read the tags from the input/output and marker area.
    /// The default value is IOMCTDB. </param>
    virtual ERuntimeErrorCode UpdateTagList(ETagListDetails in_TagListDetails) = 0;
    
    /// <summary>
    /// Reads the tags from the VPLC and writes them into the shared memory ordered by name.
    /// </summary>
    virtual ERuntimeErrorCode UpdateTagList() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnConfigurationChangedEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milliseconds. </param>
    virtual bool WaitForOnConfigurationChangedEvent(UINT32 in_Time_ms) = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnConfigurationChangingEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milliseconds. </param>
    virtual bool WaitForOnConfigurationChangingEvent(UINT32 in_Time_ms) = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnEndOfCycleEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milliseconds. </param>
    virtual bool WaitForOnEndOfCycleEvent(UINT32 in_Time_ms) = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnLedChangedEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milliseconds. </param>
    virtual bool WaitForOnLedChangedEvent(UINT32 in_Time_ms) = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnOperatingStateChangedEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milliseconds. </param>
    virtual bool WaitForOnOperatingStateChangedEvent(UINT32 in_Time_ms) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_SymbolTag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value and the expected type of the PLC variable to write.
    /// The types UNSPECIFIC and STRUCT are not supported. </param>
    virtual ERuntimeErrorCode Write(WCHAR* in_SymbolTag, SDataValue in_Value) = 0;
    
    /// <summary>
    /// Writes a single bit to the area.
    /// </summary>
    /// <param name="in_Area"> The area to write to. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="in_Offset"> The byte offset inside the area. The value must be between 0 and the value returned by GetAreaSize(). </param>
    /// <param name="in_Bit"> The bit offset inside the byte. Must be a value between 0 and 7. </param>
    /// <param name="in_Value"> The value of the bit. </param>
    virtual ERuntimeErrorCode WriteBit(EArea in_Area, UINT32 in_Offset, UINT8 in_Bit, bool in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteBool(WCHAR* in_Tag, bool in_Value) = 0;
    
    /// <summary>
    /// Writes a single byte to the area.
    /// </summary>
    /// <param name="in_Area"> The area to write to. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="in_Offset"> The byte offset inside the area. The value must be between 0 and the value returned by GetAreaSize(). </param>
    /// <param name="in_Value"> The value of the byte. </param>
    virtual ERuntimeErrorCode WriteByte(EArea in_Area, UINT32 in_Offset, BYTE in_Value) = 0;
    
    /// <summary>
    /// Writes an array of bytes to the area.
    /// </summary>
    /// <param name="in_Area"> The area to write to. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="in_Offset"> The byte offset inside the area. The value must be between 0 and the value returned by GetAreaSize(). </param>
    /// <param name="in_BytesToWrite"> Contains the size of the value array to be written. </param>
    /// <param name="out_BytesWritten"> Contains the amount of bytes that have actually been written. </param>
    /// <param name="in_Values"> Array of bytes to be written to the area. </param>
    virtual ERuntimeErrorCode WriteBytes(EArea in_Area, UINT32 in_Offset, UINT32 in_BytesToWrite, UINT32* out_BytesWritten, BYTE in_Values[]) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteChar(WCHAR* in_Tag, char in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteDouble(WCHAR* in_Tag, double in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteFloat(WCHAR* in_Tag, float in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteInt16(WCHAR* in_Tag, INT16 in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteInt32(WCHAR* in_Tag, INT32 in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteInt64(WCHAR* in_Tag, INT64 in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteInt8(WCHAR* in_Tag, INT8 in_Value) = 0;
    
    /// <summary>
    /// Writes a list of signals at once. Only signals of the primitive data type are supported
    /// but the function is not type safe. The function is also taking care of the endianness.
    /// </summary>
    /// <param name="in_Area"> The area to write to. Allowed values are:
    /// {SRA_INPUT, SRA_MARKER, SRA_OUTPUT}. </param>
    /// <param name="inout_Signals"> The list of the signals to write. </param>
    /// <param name="in_SignalCount"> The amount of signals in the list. </param>
    virtual ERuntimeErrorCode WriteSignals(EArea in_Area, SDataValueByAddress* inout_Signals, UINT32 in_SignalCount) = 0;
    
    /// <summary>
    /// Writes multiple signals within a single API call.
    /// </summary>
    /// <param name="inout_Signals"> Contains the name, value and the expected type of the PLC variable to write.
    /// The type UNSPECIFIC and STRUCT are not supported. </param>
    /// <param name="in_SignalCount"> The amount of signals to write. </param>
    virtual ERuntimeErrorCode WriteSignals(SDataValueByName* inout_Signals, UINT32 in_SignalCount) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteUInt16(WCHAR* in_Tag, UINT16 in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteUInt32(WCHAR* in_Tag, UINT32 in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteUInt64(WCHAR* in_Tag, UINT64 in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteUInt8(WCHAR* in_Tag, UINT8 in_Value) = 0;
    
    /// <summary>
    /// Writes the value of a PLC variable.
    /// </summary>
    /// <param name="in_Tag"> The name of the PLC variable to write. </param>
    /// <param name="in_Value"> Contains the value of the PLC variable to write. </param>
    virtual ERuntimeErrorCode WriteWChar(WCHAR* in_Tag, WCHAR in_Value) = 0;
};

class IRemoteRuntimeManager : public IBaseInterface
{
    ////    Constructors and destructors    ////
    
public :

    inline virtual ~IRemoteRuntimeManager()
    {
    }
    
    ////    Operations    ////
    
    /// <summary>
    /// Creates and returns an interface of an already registered virtual controller instance.
    /// </summary>
    /// <param name="in_InstanceName"> The name of the registered instance you would like to get the interface from. </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode CreateInterface(WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Creates and returns an interface of an already registered virtual controller instance.
    /// </summary>
    /// <param name="in_InstanceID"> The ID of the registered instance you would like to get the interface from. </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode CreateInterface(INT32 in_InstanceID, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Closes the connection to the remote Runtime Manager.
    /// </summary>
    virtual ERuntimeErrorCode Disconnect() = 0;
    
    /// <summary>
    /// Returns the IP of the PC the remote Runtime Manager is running on. Returns 0 if the function fails.
    /// </summary>
    virtual UIP GetIP() = 0;
    
    /// <summary>
    /// Returns the open port of the PC the remote Runtime Manager is running on. Returns 0 if the function fails.
    /// </summary>
    virtual UINT16 GetPort() = 0;
    
    /// <summary>
    /// Returns the info about an already registered instance. You can use its ID or name to create an interface of this instance.
    /// </summary>
    /// <param name="in_Index"> Index of the created instance you want to get the info from.
    /// Must be smaller than the value you get from calling GetRegisteredInstanceCount(). </param>
    /// <param name="out_InstanceInfo"> The info of the instance containing its name and ID. </param>
    virtual ERuntimeErrorCode GetRegisteredInstanceInfoAt(UINT32 in_Index, SInstanceInfo* out_InstanceInfo) = 0;
    
    /// <summary>
    /// Returns the number of instances registered at the RuntimeManager. Returns 0 if the function fails.
    /// </summary>
    virtual UINT32 GetRegisteredInstancesCount() = 0;
    
    /// <summary>
    /// Returns the name of the PC the remote Runtime Manager is running on.
    /// </summary>
    /// <param name="inout_Name"> A user allocated array to receive the computer name. </param>
    /// <param name="in_ArrayLength"> The length of the array. The array should be bigger than MAX_COMPUTERNAME_LENGTH (Windows.h define). </param>
    virtual ERuntimeErrorCode GetRemoteComputerName(WCHAR* inout_Name, UINT32 in_ArrayLength) = 0;
    
    /// <summary>
    /// Returns the version of the remote Runtime Manager. If the function fails the version returned is 0.
    /// </summary>
    virtual UINT32 GetVersion() = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_VplcDll"> The complete path to the dll of the virtual controller
    /// the Siemens.Simatic.Simulation.Runtime.Instance.exe will load when powering on.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterCustomInstance(WCHAR* in_VplcDll, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_VplcDll"> The complete path to the dll of the virtual controller
    /// the Siemens.Simatic.Simulation.Runtime.Instance.exe will load when powering on.
    /// </param>
    /// <param name="in_InstanceName"> This will be the name of the instance. Each instance must have a unique
    /// name. If no name is given when registering a new instance the instance is named “Instance_#” where # is the ID of
    /// the instance. If this name already exists the name will be “Instance_#.#” where the second # is a counter that
    /// will be increased until the name is unique. The length of the name must be smaller than DINSTANCE_NAME_LENGTH.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterCustomInstance(WCHAR* in_VplcDll, WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_InstanceName"> This will be the name of the instance. Each instance must have a unique
    /// name. If no name is given when registering a new instance the instance is named "Instance_#" where # is the ID of
    /// the instance. If this name already exists the name will be "Instance_#.#" where the second # is a counter that
    /// will be increased until the name is unique. The length of the name must be smaller than DINSTANCE_NAME_LENGTH.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_CPUType"> Defines which type of CPU will be simulated
    /// when starting the instance. The default value is "SRCT_1500_Unspecified"
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(ECPUType in_CPUType, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// Registers a new instance of a virtual controller at the Runtime Manager. Also creates and returns an interface of this instance.
    /// </summary>
    /// <param name="in_CPUType"> Defines which type of CPU will be simulated
    /// when starting the instance. The default value is "SRCT_1500_Unspecified"
    /// </param>
    /// <param name="in_InstanceName"> This will be the name of the instance. Each instance must have a unique
    /// name. If no name is given when registering a new instance the instance is named "Instance_#" where # is the ID of
    /// the instance. If this name already exists the name will be "Instance_#.#" where the second # is a counter that
    /// will be increased until the name is unique. The length of the name must be smaller than DINSTANCE_NAME_LENGTH.
    /// </param>
    /// <param name="out_InstanceInterface"> Pointer to a SimulationRuntime interface pointer. The
    /// Pointer must be initialized with NULL. The interface will be created inside the function.
    /// </param>
    virtual ERuntimeErrorCode RegisterInstance(ECPUType in_CPUType, WCHAR* in_InstanceName, IInstance** out_InstanceInterface) = 0;
    
    /// <summary>
    /// When receiving the event the registered callback function will be called. The callback function runs in a separate thread.
    /// There can be only one callback function registered to the event. Registering a new one will automatically unregister the previous
    /// one.
    /// </summary>
    /// <param name="in_CallbackFunction"> A callback function to subscribe to the event. </param>
    virtual void RegisterOnConnectionLostCallback(EventCallback_IRRTM in_CallbackFunction) = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    virtual void RegisterOnConnectionLostEvent() = 0;
    
    /// <summary>
    /// When receiving the event, the registered event object is set to signaled state. There can be only one
    /// event object registered to the event. Registering a new one will automatically unregister the previous one.
    /// </summary>
    /// <param name="in_Event"> The handle to a user created event object. The event object will be registered. </param>
    virtual void RegisterOnConnectionLostEvent(HANDLE* in_Event) = 0;
    
    /// <summary>
    /// Unregisters the callback function. When receiving the event no callback function will be called.
    /// </summary>
    virtual void UnregisterOnConnectionLostCallback() = 0;
    
    /// <summary>
    /// Unregisters the event object.
    /// </summary>
    virtual void UnregisterOnConnectionLostEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    virtual bool WaitForOnConnectionLostEvent() = 0;
    
    /// <summary>
    /// The function will block until the registered event object is in the signaled state or the timeout interval elapses.
    /// </summary>
    /// <param name="in_Time_ms"> The value for the timeout limit in milliseconds. </param>
    virtual bool WaitForOnConnectionLostEvent(UINT32 in_Time_ms) = 0;
};

/// <summary>
/// Frees the library of the Runtime API.
/// </summary>
inline ERuntimeErrorCode FreeApi()
{
    // Init
    ERuntimeErrorCode result = (_SimulationRuntimeApiDllHandle != NULL) ? SREC_OK : SREC_API_NOT_INITIALIZED;
    
    // Free
    if (result == SREC_OK)
    {
    	FreeLibrary(_SimulationRuntimeApiDllHandle);
    	_SimulationRuntimeApiDllHandle = NULL; 
    	DestroyInterface = NULL;
    }
    
    // Done
    return result;
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_AreaSection"> The enumeration entry. </param>
inline const WCHAR* GetNameOfAreaSection(EArea in_AreaSection)
{
    switch (in_AreaSection)
    { 
    	case SRA_INVALID_AREA: 	return L"InvalidArea";
    	case SRA_INPUT: 		return L"Input";
    	case SRA_MARKER: 		return L"Marker";
    	case SRA_OUTPUT: 		return L"Output";
    	case SRA_COUNTER: 		return L"Counter";
    	case SRA_TIMER: 		return L"Timer";
    	case SRA_DATABLOCK: 	return L"DataBlock";
    }
    return L"Invalid EArea";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_CPUType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfCPUType(ECPUType in_CPUType)
{
    switch (in_CPUType)
    { 
    	case SRCT_1500_Unspecified: 	return L"1500_Unspecified";	
    	
    	case SRCT_1511: 				return L"1511";
    	case SRCT_1511v2: 				return L"1511v2";
    	case SRCT_1513: 				return L"1513";
    	case SRCT_1513v2: 				return L"1513v2";
    	case SRCT_1515: 				return L"1515";
    	case SRCT_1515v2: 				return L"1515v2";
    	case SRCT_1516: 				return L"1516";
    	case SRCT_1516v2: 				return L"1516v2";
    	case SRCT_1517: 				return L"1517";
    	case SRCT_1518: 				return L"1518";
    	
    	case SRCT_1511C: 				return L"1511C";
    	case SRCT_1512C: 				return L"1512C";
    
    	case SRCT_1511F: 				return L"1511F";
    	case SRCT_1511Fv2: 				return L"1511Fv2";
    	case SRCT_1513F: 				return L"1513F";
    	case SRCT_1513Fv2: 				return L"1513Fv2";
    	case SRCT_1515F: 				return L"1515F";  
    	case SRCT_1515Fv2: 				return L"1515Fv2";
    	case SRCT_1516F: 				return L"1516F";
    	case SRCT_1516Fv2: 				return L"1516Fv2";
    	case SRCT_1517F: 				return L"1517F";
    	case SRCT_1518F: 				return L"1518F";
    	
    	case SRCT_1511T: 				return L"1511T";
    	case SRCT_1515T: 				return L"1515T";
    	case SRCT_1517T: 				return L"1517T";
    	
    	case SRCT_1517TF: 				return L"1517TF";
    	
    	case SRCT_1518ODK: 				return L"1518ODK"; 
    	case SRCT_1518FODK: 			return L"1518FODK";
    	
    	case SRCT_ET200SP_Unspecified: 	return L"ET200SP_Unspecified";	
    	case SRCT_1510SP: 				return L"1510SP";
    	case SRCT_1510SPv2: 			return L"1510SPv2";
    	case SRCT_1512SP: 				return L"1512SP";
    	case SRCT_1512SPv2: 			return L"1512SPv2";
    	
    	case SRCT_1510SPF: 				return L"1510SPF";
    	case SRCT_1510SPFv2: 			return L"1510SPFv2";   
    	case SRCT_1512SPF: 				return L"1512SPF";
    	case SRCT_1512SPFv2: 			return L"1512SPFv2";  
    }
    return L"Invalid ECPUType";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_CommunicationInterface"> The enumeration entry. </param>
inline const WCHAR* GetNameOfCommunicationInterface(ECommunicationInterface in_CommunicationInterface)
{
    switch (in_CommunicationInterface)
    { 
    	case SRCI_NONE: 	return L"None";
    	case SRCI_SOFTBUS: 	return L"Softbus";
    	case SRCI_TCPIP: 	return L"TCPIP";
    }
    return L"Invalid ECommunicationInterface";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_DataType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfDataType(EDataType in_DataType)
{
    switch (in_DataType)
    { 
    	case SRDT_UNKNOWN:			return L"Unknown";	
    	
    	case SRDT_BOOL:				return L"Bool";	
    	case SRDT_BYTE: 			return L"Byte";	
    	case SRDT_CHAR:				return L"Char";	
    	case SRDT_WORD: 			return L"Word";	
    	case SRDT_INT:				return L"Int";	
    	case SRDT_DWORD: 			return L"DWord";	
    	case SRDT_DINT: 			return L"DInt";	
    	case SRDT_REAL: 			return L"Real";	
    	case SRDT_DATE: 			return L"Date";	
    	case SRDT_TIME_OF_DAY: 		return L"TimeOfDay";	
    	case SRDT_TIME: 			return L"Time";	 
    	case SRDT_S5TIME: 			return L"S5Time";	
    
    	case SRDT_DATE_AND_TIME: 	return L"DateAndTime";	
    	
    	case SRDT_STRUCT:			return L"Struct";	
    	 
    	case SRDT_STRING:           return L"String";	
    	
    	case SRDT_COUNTER: 			return L"Counter";	
    	case SRDT_TIMER: 			return L"Timer";	 
    	case SRDT_IEC_Counter: 		return L"IEC_Counter";	
    	case SRDT_IEC_Timer: 		return L"IEC_Timer";	
    	
    	case SRDT_LREAL: 			return L"LReal";	
    	case SRDT_ULINT: 			return L"ULInt";	
    	case SRDT_LINT: 			return L"LInt";	
    	case SRDT_LWORD: 			return L"LWord";	
    	case SRDT_USINT: 			return L"USInt";	
    	case SRDT_UINT: 			return L"UInt";	
    	case SRDT_UDINT: 			return L"UDInt";	
    	case SRDT_SINT: 			return L"SInt";		 
       
        case SRDT_WCHAR: 			return L"WChar";	
    	case SRDT_WSTRING:          return L"WString";	
    	  
    	case SRDT_LTIME: 			return L"LTime";	
        case SRDT_LTIME_OF_DAY: 	return L"LTimeOfDay";	
        case SRDT_LDT:				return L"LDT";	
    	case SRDT_DTL:				return L"DTL";	
    	case SRDT_IEC_LTimer:		return L"IEC_LTimer";	
    	case SRDT_IEC_SCounter:		return L"IEC_SCounter";	
    	case SRDT_IEC_DCounter:		return L"IEC_DCounter";	
    	case SRDT_IEC_LCounter:		return L"IEC_LCounter";	
    	case SRDT_IEC_UCounter:		return L"IEC_UCounter";	
    	case SRDT_IEC_UDCounter:	return L"IEC_UDCounter";	
    	case SRDT_IEC_USCounter:	return L"IEC_USCounter";	
    	case SRDT_IEC_ULCounter:	return L"IEC_ULCounter";	
    	
    	case SRDT_ERROR_STRUCT:		return L"ErrorStruct";
    	case SRDT_NREF:				return L"NREF";
    	case SRDT_CREF:				return L"CREF";
    	  
    	case SRDT_AOM_IDENT: 		return L"Aom_Ident";	
        case SRDT_EVENT_ANY:		return L"Event_Any";	
        case SRDT_EVENT_ATT: 		return L"Event_Att";	
        case SRDT_EVENT_HWINT: 		return L"Event_HwInt";	
       
        case SRDT_HW_ANY: 			return L"Hw_Any";	
        case SRDT_HW_IOSYSTEM: 		return L"Hw_IoSystem";	
        case SRDT_HW_DPMASTER: 		return L"Hw_DpMaster";	
        case SRDT_HW_DEVICE: 		return L"Hw_Device";	
        case SRDT_HW_DPSLAVE: 		return L"Hw_DpSlave";	
        case SRDT_HW_IO: 			return L"Hw_Io";	
        case SRDT_HW_MODULE: 		return L"Hw_Module";	
        case SRDT_HW_SUBMODULE: 	return L"Hw_SubModule";	     
        case SRDT_HW_HSC: 			return L"Hw_Hsc";	     
        case SRDT_HW_PWM: 			return L"Hw_Pwm";	      
        case SRDT_HW_PTO: 			return L"Hw_Pto";	  
        case SRDT_HW_INTERFACE: 	return L"Hw_Interface";	
    	case SRDT_HW_IEPORT: 		return L"Hw_IEPort";	
        
        case SRDT_OB_ANY: 			return L"OB_Any";	
    	case SRDT_OB_DELAY: 		return L"OB_Delay";	
        case SRDT_OB_TOD: 			return L"OB_Tod";	
        case SRDT_OB_CYCLIC:		return L"OB_Cyclic";	
        case SRDT_OB_ATT:			return L"OB_Att";	
               
        case SRDT_CONN_ANY:			return L"Conn_Any";	
        case SRDT_CONN_PRG:			return L"Conn_Prg";	
        case SRDT_CONN_OUC:			return L"Conn_Ouc";	
        case SRDT_CONN_R_ID:		return L"Conn_R_ID";	
        
        case SRDT_PORT:				return L"Port";	    
        case SRDT_RTM:				return L"Rtm";	       
        case SRDT_PIP:				return L"Pip";	
        
        case SRDT_OB_PCYCLE:		return L"OB_PCycle";	
        case SRDT_OB_HWINT:			return L"OB_HwInt";	  
        case SRDT_OB_DIAG:			return L"OB_Diag";	     
        case SRDT_OB_TIMEERROR:		return L"OB_TimeError";	     
        case SRDT_OB_STARTUP:		return L"OB_Startup";	     
              
        case SRDT_DB_ANY:			return L"DB_Any";	 
        case SRDT_DB_WWW:			return L"DB_WWW";	
        case SRDT_DB_DYN:			return L"DB_Dyn";	  
        
        case SRDT_DB:				return L"DB";	  	
    }
    return L"Invalid EDataType";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_ErrorCode"> The enumeration entry. </param>
inline const WCHAR* GetNameOfErrorCode(ERuntimeErrorCode in_ErrorCode)
{
    switch (in_ErrorCode)
    { 
    	case SREC_OK: 										return L"Ok";
    	case SREC_INVALID_ERROR_CODE:						return L"InvalidErrorCode";
    	case SREC_NOT_IMPLEMENTED:							return L"NotImplemented";
    	case SREC_INDEX_OUT_OF_RANGE:						return L"IndexOutOfRange";
    	case SREC_DOES_NOT_EXIST:							return L"DoesNotExist";
    	case SREC_ALREADY_EXISTS:							return L"AlreadyExists";
    	case SREC_UNKNOWN_MESSAGE_TYPE:						return L"UnknownMessageType";
    	case SREC_INVALID_MESSAGE_ID:						return L"InvalidMessageID";
    	case SREC_WRONG_ARGUMENT:							return L"WrongArgument";
    	case SREC_WRONG_PIPE:								return L"WrongPipe";
    	case SREC_CONNECTION_ERROR:							return L"ConnectionError";  
    	case SREC_TIMEOUT:									return L"Timeout";	
    	case SREC_MESSAGE_CORRUPT:							return L"MessageCorrupt";
    	case SREC_WRONG_VERSION:							return L"WrongVersion";
    	case SREC_INSTANCE_NOT_RUNNING:						return L"InstanceNotRunning";
    	case SREC_INTERFACE_REMOVED:						return L"InterfaceRemoved";         
    	case SREC_SHARED_MEMORY_NOT_INITIALIZED:			return L"SharedMemoryNotInitialized";
    	case SREC_API_NOT_INITIALIZED:						return L"ApiNotInitialized";
    	case SREC_WARNING_ALREADY_EXISTS:					return L"WarningAlreadyExists";
    	case SREC_NOT_SUPPORTED:							return L"NotSupported";
    	case SREC_WARNING_INVALID_CALL:						return L"WarningInvalidCall";
    	case SREC_ERROR_LOADING_DLL:						return L"ErrorLoadingDll";
    	case SREC_SIGNAL_NAME_DOES_NOT_EXIST:				return L"SignalNameDoesNotExist";
    	case SREC_SIGNAL_TYPE_MISMATCH:						return L"SignalTypeMismatch";
    	case SREC_SIGNAL_CONFIGURATION_ERROR:				return L"SignalConfigurationError";
    	case SREC_NO_SIGNAL_CONFIGURATION_LOADED:			return L"NoSignalConfigurationLoaded";
    	case SREC_CONFIGURED_CONNECTION_NOT_FOUND:			return L"ConfiguredConnectionNotFound";
    	case SREC_CONFIGURED_DEVICE_NOT_FOUND:				return L"ConfiguredDeviceNotFound";
    	case SREC_INVALID_CONFIGURATION:					return L"InvalidConfiguration";
    	case SREC_TYPE_MISMATCH:							return L"TypeMismatch";
    	case SREC_LICENSE_NOT_FOUND:						return L"LicenseNotFound";         
    	case SREC_NO_LICENSE_AVAILABLE:						return L"NoLicenseAvailable";
    	case SREC_WRONG_COMMUNICATION_INTERFACE:			return L"WrongCommunicationInterface";
    	case SREC_LIMIT_REACHED:							return L"LimitReached";	
    	case SREC_NO_STORAGE_PATH_SET:						return L"NoStoragePathSet";
    	case SREC_STORAGE_PATH_ALREADY_IN_USE:				return L"StoragePathAlreadyInUse"; 
    	case SREC_MESSAGE_INCOMPLETE:						return L"MessageIncomplete";
    	case SREC_ARCHIVE_STORAGE_FAILED:					return L"ArchiveStorageFailed";
    	case SREC_RETRIEVE_STORAGE_FAILURE:					return L"RetrieveStorageFailure";
    	case SREC_INVALID_OPERATING_STATE:					return L"InvalidOperatingState";
    	case SREC_INVALID_ARCHIVE_PATH:						return L"InvalidArchivePath";
    	case SREC_DELETE_EXISTING_STORAGE_FAILED:			return L"DeleteExistingStorageFailed";
    	case SREC_CREATE_DIRECTORIES_FAILED:				return L"CreateDirectoriesFailed";
    	case SREC_NOT_ENOUGH_MEMORY:						return L"NotEnoughMemory";	    
    	case SREC_WARNING_TRIAL_MODE_ACTIVE:				return L"WarningTrialModeActive";
    	case SREC_NOT_RUNNING:								return L"NotRunning";
    	case SREC_NOT_EMPTY: 								return L"NotEmpty"; 
    	case SREC_NOT_UP_TO_DATE: 							return L"NotUpToDate";
    	case SREC_COMMUNICATION_INTERFACE_NOT_AVAILABLE: 	return L"CommunicationInterfaceNotAvailable"; 
    	case SREC_WARNING_NOT_COMPLETE: 					return L"WarningNotComplete";    
    	case SREC_VIRTUAL_SWITCH_MISCONFIGURED: 			return L"VirtualSwitchMisconfigured";     
    	case SREC_RUNTIME_NOT_AVAILABLE: 					return L"RuntimeNotAvailable";
    	case SREC_IS_EMPTY: 								return L"IsEmpty";       
    }
    return L"Invalid ERuntimeErrorCode";
}

inline const WCHAR* GetNameOfInstanceConfigChanged(EInstanceConfigChanged in_InstanceConfigChanged)
{
    switch (in_InstanceConfigChanged)
    { 
    	case SRICC_HARDWARE_SOFTWARE_CHANGED: 	return L"HardwareSoftwareChanged";   
    	case SRICC_IP_CHANGED: 					return L"IPChanged";
    }
    return L"Invalid EInstanceConfigChanged";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_LEDMode"> The enumeration entry. </param>
inline const WCHAR* GetNameOfLEDMode(ELEDMode in_LEDMode)
{
    switch (in_LEDMode)
    { 
    	case SRLM_OFF: 			return L"Off";
    	case SRLM_ON: 			return L"On";
    	case SRLM_FLASH_FAST: 	return L"FastFlash";
    	case SRLM_FLASH_SLOW: 	return L"SlowFlash";
    	case SRLM_INVALID: 		return L"Invalid";
    }
    return L"Invalid ELEDMode";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_LEDType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfLEDType(ELEDType in_LEDType)
{
    switch (in_LEDType)
    { 
    	case SRLT_STOP: 	return L"Stop";
    	case SRLT_RUN: 		return L"Run";     
    	case SRLT_ERROR: 	return L"Error";
    	case SRLT_MAINT: 	return L"Maintanance";
    	case SRLT_REDUND: 	return L"Redund";
    	case SRLT_FORCE: 	return L"Force";
    	case SRLT_BUSF1: 	return L"BusF1";
    	case SRLT_BUSF2: 	return L"BusF2";
    	case SRLT_BUSF3: 	return L"BusF3";
    	case SRLT_BUSF4: 	return L"BusF4";
    }
    return L"Invalid ELEDTYpe";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_OperatingMode"> The enumeration entry. </param>
inline const WCHAR* GetNameOfOperatingMode(EOperatingMode in_OperatingMode)
{
    switch (in_OperatingMode)
    { 
    	case SROM_DEFAULT: 					return L"Default";
    	case SROM_SINGLE_STEP: 				return L"SingleStep";
    	case SROM_EXTENDED_SINGLE_STEP: 	return L"ExtendedSingleStep";
    	case SROM_TIMESPAN_SYNCHNRONIZED: 	return L"TimespanSynchronized";
    }
    return L"Invalid EOperatingMode";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_OperatingState"> The enumeration entry. </param>
inline const WCHAR* GetNameOfOperatingState(EOperatingState in_OperatingState)
{
    switch (in_OperatingState)
    { 
    	case SROS_INVALID_OPERATING_STATE: 	return L"InvalidOperatingState";
    	case SROS_OFF: 						return L"Off";
    	case SROS_BOOTING: 					return L"Booting";
    	case SROS_STOP: 					return L"Stop";
    	case SROS_STARTUP: 					return L"Startup";
    	case SROS_RUN: 						return L"Run";
    	case SROS_FREEZE: 					return L"Freeze";
    	case SROS_SHUTTING_DOWN: 			return L"ShuttingDown";
    }
    return L"Invalid EOperatingState";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_DataType"> The enumeration entry. </param>
inline const WCHAR* GetNameOfPrimitiveDataType(EPrimitiveDataType in_DataType)
{
    switch (in_DataType)
    { 
    	case SRPDT_UNSPECIFIC: 	return L"Unspecific";
    	case SRPDT_STRUCT: 		return L"Struct";
    	case SRPDT_BOOL: 		return L"Bool";
    	case SRPDT_INT8: 		return L"Int8";
    	case SRPDT_INT16: 		return L"Int16";
    	case SRPDT_INT32: 		return L"Int32";
    	case SRPDT_INT64: 		return L"Int64";
    	case SRPDT_UINT8: 		return L"UInt8";
    	case SRPDT_UINT16: 		return L"UInt16";
    	case SRPDT_UINT32: 		return L"UInt32";
    	case SRPDT_UINT64: 		return L"UInt64";
    	case SRPDT_FLOAT: 		return L"Float";
    	case SRPDT_DOUBLE: 		return L"Double";
    	case SRPDT_CHAR: 		return L"Char";
    	case SRPDT_WCHAR: 		return L"WChar";
    }
    return L"Invalid EPrimitiveDataType";
}

inline const WCHAR* GetNameOfRuntimeConfigChanged(ERuntimeConfigChanged in_RuntimeConfigChanged)
{
    switch (in_RuntimeConfigChanged)
    { 
    	case SRCC_INSTANCE_REGISTERED: 		return L"InstanceRegistered";
    	case SRCC_INSTANCE_UNREGISTERED: 	return L"InstanceUnregistered";
    	case SRCC_CONNECTION_OPENED: 		return L"ConnectionOpened";
    	case SRCC_CONNECTION_CLOSED: 		return L"ConnectionClosed";
    	case SRCC_PORT_OPENED: 				return L"PortOpened";  
    	case SRCC_PORT_CLOSED: 				return L"PortClosed";
    }
    return L"Invalid ERuntimeConfigChanged";
}

/// <summary>
/// Returns the name of the enumeration entry.
/// </summary>
/// <param name="in_TagListDetails"> The enumeration entry. </param>
inline const WCHAR* GetNameOfTagListDetails(ETagListDetails in_TagListDetails)
{
    switch (in_TagListDetails)
    { 
    	case SRTLD_NONE: 		return L"None";
    	case SRTLD_IO: 			return L"IO";
    	case SRTLD_M: 			return L"M";
    	case SRTLD_IOM: 		return L"IOM";
    	case SRTLD_CT: 			return L"CT";
    	case SRTLD_IOCT: 		return L"IOCT";
    	case SRTLD_MCT: 		return L"MCT";
    	case SRTLD_IOMCT: 		return L"IOMCT";
    	case SRTLD_DB: 			return L"DB";
    	case SRTLD_IODB: 		return L"IODB";
    	case SRTLD_MDB: 		return L"MDB";
    	case SRTLD_IOMDB: 		return L"IOMDB";
    	case SRTLD_CTDB: 		return L"CTDB";
    	case SRTLD_IOCTDB: 		return L"IOCTDB";
    	case SRTLD_MCTDB: 		return L"MCTDB";  
    	case SRTLD_IOMCTDB: 	return L"IOMCTDB";
    }
    return L"Invalid ETagListDetails";
}

/// <summary>
/// Loads "Siemens.Simatic.PlcSim.Advanced.Api.dll" and initializes the API.
/// </summary>
/// <param name="inout_SimulationRuntimeManagerInterface"> Pointer to a RuntimeManager interface
/// pointer. The Pointer must be initialized with NULL. The interface will be created inside the function.
/// </param>
inline ERuntimeErrorCode InitializeApi(ISimulationRuntimeManager** inout_SimulationRuntimeManagerInterface)
{
    // Init
    ERuntimeErrorCode result = SREC_ERROR_LOADING_DLL;
    ApiEntry_Initialize initializeApi = NULL;
    ApiEntry_DestroyInterface destroyInterface = NULL;
    
    if (_SimulationRuntimeApiDllHandle == NULL)
    {
    	// Load The DLL From The Current Working Directory
    	#ifdef _M_X64
    	_SimulationRuntimeApiDllHandle = LoadLibraryW(DAPI_DLL_NAME_X64); 
    	#else
    	_SimulationRuntimeApiDllHandle = LoadLibraryW(DAPI_DLL_NAME_X86); 
    	#endif 
    
    	// Load The DLL From The Install Directory
    	if (_SimulationRuntimeApiDllHandle == NULL)
    	{
    		// Prepare
    		WCHAR installPath[MAX_PATH] = {};
    		WCHAR fullSimulationRuntimeApiDllName[MAX_PATH] = {};
    		DWORD dwDataSize = sizeof(WCHAR) * MAX_PATH;
    		HKEY hKey = NULL;
    
    		// Open The Key
    		LSTATUS lResult = RegOpenKeyExW(HKEY_LOCAL_MACHINE, DAPI_DLL_PATH_REGISTRY_KEY_NAME, 0, KEY_READ, &hKey);
    		if (lResult == ERROR_SUCCESS)
    		{
    			// Read The Path
    			lResult = RegQueryValueExW(hKey, DAPI_DLL_PATH_REGISTRY_VALUE_NAME, NULL, NULL, (BYTE*)installPath, &dwDataSize);
    		}
    
    		// Combine The Path And Load The DLL
    		if (lResult == ERROR_SUCCESS)
    		{
    			// Combine The Directories
    			WCHAR* combineResult = NULL;
    			combineResult = PathCombineW(fullSimulationRuntimeApiDllName, installPath, DAPI_DLL_PATH_VERSION_SUBDIRECTORY);
    			if (combineResult != NULL)
    			{
    				// Attach The DLL Name					
    				#ifdef _M_X64
    				combineResult = PathCombineW(fullSimulationRuntimeApiDllName, fullSimulationRuntimeApiDllName, DAPI_DLL_NAME_X64);
    				#else
    				combineResult = PathCombineW(fullSimulationRuntimeApiDllName, fullSimulationRuntimeApiDllName, DAPI_DLL_NAME_X86);
    				#endif 
    				
    				// Load The DLL
    				if (combineResult != NULL)
    				{
    					_SimulationRuntimeApiDllHandle = LoadLibraryW(fullSimulationRuntimeApiDllName);
    				}
    			}
    		}
    	}
        
        // Initialize The API
    	if (_SimulationRuntimeApiDllHandle != NULL)
    	{
    		// Import The API Functions
    		initializeApi = (ApiEntry_Initialize)GetProcAddress(_SimulationRuntimeApiDllHandle, DAPI_ENTRY_INITIALIZE);   
    		destroyInterface = (ApiEntry_DestroyInterface)GetProcAddress(_SimulationRuntimeApiDllHandle, DAPI_ENTRY_DESTROY_INTERFACE); 
    		
    		// Initialize The API
    		if (initializeApi != NULL && destroyInterface != NULL)
    		{
    			result = initializeApi(inout_SimulationRuntimeManagerInterface, DAPI_DLL_INTERFACE_VERSION);   
    			if (result == SREC_OK)
    			{
    				DestroyInterface = destroyInterface; 	
    			} 
    		}
    		
    		// Cleanup If The Initialization Failed
    		if (result != SREC_OK)
    		{
    			FreeLibrary(_SimulationRuntimeApiDllHandle);
    			_SimulationRuntimeApiDllHandle = NULL;
    		}
    	}
    } 
    
    // Done
    return result;
}

inline ERuntimeErrorCode InitializeApi(WCHAR* in_SimulationRuntimeApiDllPath, ISimulationRuntimeManager** inout_SimulationRuntimeManagerInterface)
{
    // Init
    ERuntimeErrorCode result = SREC_ERROR_LOADING_DLL;
    ApiEntry_Initialize initializeApi = NULL;
    ApiEntry_DestroyInterface destroyInterface = NULL;
    
    // Check The Path
    #ifdef _M_X64
    UINT32 totalPathLength = 2 + (UINT32)wcslen(in_SimulationRuntimeApiDllPath) + (UINT32)wcslen(DAPI_DLL_NAME_X64);
    #else
    UINT32 totalPathLength = 2 + wcslen(in_SimulationRuntimeApiDllPath) + wcslen(DAPI_DLL_NAME_X86);
    #endif 
    
    if (totalPathLength < MAX_PATH)
    {
    	if (_SimulationRuntimeApiDllHandle == NULL)
    	{
    		// Create The Full File Name
    		WCHAR fullSimulationRuntimeApiDllName[MAX_PATH] = {}; 
    		WCHAR* combineResult = NULL;
    		#ifdef _M_X64
    		combineResult = PathCombineW(fullSimulationRuntimeApiDllName, in_SimulationRuntimeApiDllPath, DAPI_DLL_NAME_X64); 
    		#else
    		combineResult = PathCombineW(fullSimulationRuntimeApiDllName, in_SimulationRuntimeApiDllPath, DAPI_DLL_NAME_X86); 
    		#endif 
    		
    		// Load The DLL
    		if (combineResult != NULL)
    		{ 
    			_SimulationRuntimeApiDllHandle = LoadLibraryW(fullSimulationRuntimeApiDllName); 
    		}
    		
    		if (_SimulationRuntimeApiDllHandle != NULL)
    		{
    			// Import The API Functions
    			initializeApi = (ApiEntry_Initialize)GetProcAddress(_SimulationRuntimeApiDllHandle, DAPI_ENTRY_INITIALIZE);   
    			destroyInterface = (ApiEntry_DestroyInterface)GetProcAddress(_SimulationRuntimeApiDllHandle, DAPI_ENTRY_DESTROY_INTERFACE); 
    			
    			// Init The API
    			if (initializeApi != NULL && destroyInterface != NULL)
    			{
    				result = initializeApi(inout_SimulationRuntimeManagerInterface, DAPI_DLL_INTERFACE_VERSION);   
    				if (result == SREC_OK)
    				{
    					DestroyInterface = destroyInterface; 	
    				} 
    			}
    			
    			// Cleanup If The Initialization Failed
    			if (result != SREC_OK)
    			{
    				FreeLibrary(_SimulationRuntimeApiDllHandle);
    				_SimulationRuntimeApiDllHandle = NULL;
    			}
    		}
    	} 
    }
    else
    {
    	result = SREC_INDEX_OUT_OF_RANGE;
    }
    
    // Done
    return result;
}

/// <summary>
/// Shuts down the Runtime API, destroys the IRuntimeManager interface and frees the library of the Runtime API.
/// </summary>
/// <param name="in_SimulationRuntimeManagerInterface"> The interface of the Runtime Manager to be destroyed. </param>
inline ERuntimeErrorCode ShutdownAndFreeApi(ISimulationRuntimeManager* in_SimulationRuntimeManagerInterface)
{
    // Init
    ERuntimeErrorCode result = (_SimulationRuntimeApiDllHandle != NULL && DestroyInterface != NULL) ? SREC_OK : SREC_API_NOT_INITIALIZED;
    if (result == SREC_OK)
    {
    	if (in_SimulationRuntimeManagerInterface == NULL)
    	{
    		result = SREC_WRONG_ARGUMENT;
    	}
    }
    
    if (result == SREC_OK)
    {
    	// Shutdown
    	in_SimulationRuntimeManagerInterface->Shutdown();
    	DestroyInterface(in_SimulationRuntimeManagerInterface); 
    	
    	// Free
    	FreeLibrary(_SimulationRuntimeApiDllHandle);
    	_SimulationRuntimeApiDllHandle = NULL; 
    	DestroyInterface = NULL;
    }
    
    // Done
    return result;
}

#endif
