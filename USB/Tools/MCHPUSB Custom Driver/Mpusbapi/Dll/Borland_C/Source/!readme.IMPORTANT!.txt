
When compiling with C++ Builder,

Make sure that the Data alignment option is set to "byte"

This can be done by:

click Project\Options...\Advanced Compiler\Data Alignment[Byte]

The default is Quad word.
This will fail the sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) in MPUSBGetDeviceLink