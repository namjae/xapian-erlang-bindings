#include "xapian_exception.h"
#include <sstream>

/* These macroses help to transform an argument into a string. */
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define REG_TYPE(CLASS) const char CLASS::TYPE[] = STR(CLASS);

// ===================================================================
// Exceptions
// ===================================================================


// -------------------------------------------------------------------
// DriverRuntimeError
// -------------------------------------------------------------------
DriverRuntimeError::DriverRuntimeError(
    const char * type, const std::string& str):
    runtime_error(str) { m_type = type; }

const char* 
DriverRuntimeError::get_type() const
{
   return m_type;
}



// -------------------------------------------------------------------
// MemoryAllocationDriverError
// -------------------------------------------------------------------
MemoryAllocationDriverError::MemoryAllocationDriverError(size_t size) : 
    DriverRuntimeError(TYPE, buildString(size)) {}

const std::string 
MemoryAllocationDriverError::buildString(size_t size)
{
    std::stringstream ss;
    ss << "Cannot allocate " << size << " bytes.";
    return ss.str();
}


// -------------------------------------------------------------------
// BadCommandDriverError
// -------------------------------------------------------------------
BadCommandDriverError::BadCommandDriverError(int command_id) : 
    DriverRuntimeError(TYPE, buildString(command_id)) {}

const std::string 
BadCommandDriverError::buildString(int command_id)
{
    std::stringstream ss;
    ss << "Unknown command with id = " << command_id << ".";
    return ss.str();
}


// -------------------------------------------------------------------
// OverflowDriverError
// -------------------------------------------------------------------
OverflowDriverError::OverflowDriverError() : 
    DriverRuntimeError(TYPE, "Too short binary.") {}


// -------------------------------------------------------------------
// NotWritableDatabaseError
// -------------------------------------------------------------------
NotWritableDatabaseError::NotWritableDatabaseError() : 
     DriverRuntimeError(TYPE, "The database is open as read only.") {}


// -------------------------------------------------------------------
// DbAlreadyOpenedDriverError
// -------------------------------------------------------------------
DbAlreadyOpenedDriverError::DbAlreadyOpenedDriverError() : 
    DriverRuntimeError(TYPE, 
        "This port cannot open second DB. Use another port.") {}


// -------------------------------------------------------------------
// DbIsNotReadyDriverError
// -------------------------------------------------------------------
DbIsNotReadyDriverError::DbIsNotReadyDriverError() : 
    DriverRuntimeError(TYPE, 
        "Call xapian_drv:port_open.") {}


// -------------------------------------------------------------------
// ElementNotFoundDriverError
// -------------------------------------------------------------------
ElementNotFoundDriverError::ElementNotFoundDriverError(uint32_t num) : 
    DriverRuntimeError(TYPE, buildString(num)) {}

const std::string 
ElementNotFoundDriverError::buildString(uint32_t num)
{
    std::stringstream ss;
    ss << "Element with number = " << num << " is not found.";
    return ss.str();
}



REG_TYPE(MemoryAllocationDriverError)
REG_TYPE(BadCommandDriverError)
REG_TYPE(OverflowDriverError)
REG_TYPE(NotWritableDatabaseError)
REG_TYPE(DbAlreadyOpenedDriverError)
REG_TYPE(DbIsNotReadyDriverError)
REG_TYPE(ElementNotFoundDriverError)
