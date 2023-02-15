from ctypes import CDLL, c_uint, byref, create_string_buffer
from ctypes.util import find_library
libc = CDLL(find_library("c"))

def sysctlRead(name):
    size = c_uint(0)
    # Find out how big our buffer will be
    libc.sysctlbyname(name, None, byref(size), None, 0)
    # Make the buffer
    value = c_uint(0)
    # Re-run, but provide the buffer
    libc.sysctlbyname(name, byref(value), byref(size), None, 0)
    return value

def sysctlWrite(name, value):
    size = c_uint(0)
    # Find out how big our buffer will be
    rv = libc.sysctlbyname(name, None, byref(size), None, 0)
    assert(rv == 0)
    # Make the buffer
    oldValue = c_uint(0)
    newValue = c_uint(value)
    # Re-run, but provide the buffer
    rv = libc.sysctlbyname(name, 
            byref(oldValue), byref(size), 
            byref(newValue), size)
    assert(rv == 0)
    return value

name = b"vm.v_prefetches"
print("Reading ", name)
value = sysctlRead(name)
print("\t", value)

print("Writing ", name)
sysctlWrite(name, 0)

print("Reading ", name)
value = sysctlRead(name)
print("\t", value)
