from ctypes import CDLL, c_uint, c_uint64, byref
from ctypes.util import find_library
libc = CDLL(find_library("c"))

def sysctlRead(name):
    
    # Find out how big our buffer will be
    size = c_uint(0)
    rv = libc.sysctlbyname(name, None, byref(size), None, 0)
    assert(rv == 0)
    
    # Make the buffer
    value = c_uint64(0)
    
    # Re-run, but provide the buffer
    rv = libc.sysctlbyname(name, byref(value), byref(size), None, 0)
    assert(rv == 0)
    
    return value.value

def sysctlWrite(name, value):
    
    # Find out how big our buffer will be
    size = c_uint(0)
    rv = libc.sysctlbyname(name, None, byref(size), None, 0)
    assert(rv == 0)
    
    # Make the buffer for old and new value
    oldValue = c_uint64(0)
    newValue = c_uint64(value)
    
    # Re-run, but provide the buffer
    rv = libc.sysctlbyname(name, 
            byref(oldValue), byref(size), 
            byref(newValue), size)
    assert(rv == 0)
    assert(value == newValue.value)


    return newValue.value
