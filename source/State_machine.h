#include <stdint.h>
#include <stdbool.h>


enum { EVENT_IGNORED = 0xFE, CANNOT_HAPPEN = 0xFF };

typedef void NoEventData;

// State machine constant data
typedef struct
{
    const char* name;
    const BYTE maxStates;
    const struct SM_StateStruct* stateMap;
    const struct SM_StateStructEx* stateMapEx;
} SM_StateMachineConst;

// State machine instance data
typedef struct 
{
    const CHAR* name;
    void* pInstance;
    BYTE newState;
    BYTE currentState;
    bool eventGenerated;
    void* pEventData;
} SM_StateMachine;

// Generic state function signatures
typedef void (*SM_StateFunc)(SM_StateMachine* self, void* pEventData);
typedef bool (*SM_GuardFunc)(SM_StateMachine* self, void* pEventData);
typedef void (*SM_EntryFunc)(SM_StateMachine* self, void* pEventData);
typedef void (*SM_ExitFunc)(SM_StateMachine* self);

typedef struct SM_StateStruct
{
    SM_StateFunc pStateFunc;
} SM_StateStruct;

typedef struct SM_StateStructEx
{
    SM_StateFunc pStateFunc;
    SM_GuardFunc pGuardFunc;
    SM_EntryFunc pEntryFunc;
    SM_ExitFunc pExitFunc;
} SM_StateStructEx;

// Public functions
#define SM_Event(_smName_, _eventFunc_, _eventData_) \
    _eventFunc_(&_smName_##Obj, _eventData_)

// Protected functions
#define SM_InternalEvent(_newState_, _eventData_) \
    _SM_InternalEvent(self, _newState_, _eventData_)
#define SM_GetInstance(_instance_) \
    (_instance_*)(self->pInstance);

// Private functions
void _SM_ExternalEvent(SM_StateMachine* self, const SM_StateMachineConst* selfConst, BYTE newState, void* pEventData);
void _SM_InternalEvent(SM_StateMachine* self, BYTE newState, void* pEventData);
void _SM_StateEngine(SM_StateMachine* self, const SM_StateMachineConst* selfConst);
void _SM_StateEngineEx(SM_StateMachine* self, const SM_StateMachineConst* selfConst);

#define SM_DECLARE(_smName_) \
    extern SM_StateMachine _smName_##Obj; 

#define SM_DEFINE(_smName_, _instance_) \
    SM_StateMachine _smName_##Obj = { #_smName_, _instance_, \
        0, 0, 0, 0 }; 

#define EVENT_DECLARE(_eventFunc_, _eventData_) \
    void _eventFunc_(SM_StateMachine* self, _eventData_* pEventData);

#define EVENT_DEFINE(_eventFunc_, _eventData_) \
    void _eventFunc_(SM_StateMachine* self, _eventData_* pEventData)

#define STATE_DECLARE(_stateFunc_, _eventData_) \
    static void ST_##_stateFunc_(SM_StateMachine* self, _eventData_* pEventData);

#define STATE_DEFINE(_stateFunc_, _eventData_) \
    static void ST_##_stateFunc_(SM_StateMachine* self, _eventData_* pEventData)