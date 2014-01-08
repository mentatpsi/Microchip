Many of the demos require a pushbutton to do anything useful.  However,
the starter kit board does not have any physcial pushbutton(s) on it.  It
has capacitive touch sensitive "buttons", however, the code to make use
of these cap touch buttons is not implemented in the current MCHPFSUSB
framework release.  For demo code using the cap touch buttons, see the
materials that came with the starter kit.

Several pre-compiled hex files are included here anyway, although many
of the demos will either function with reduced functionality (no pushbutton
input), or in cases where a pushbutton is required, will not be useful,
other than to demonstrate enumeration of the firmware.

Demos that still provide some or full functionality are in this directory.
Demos that provide no functionality, other than enumeration, are in
	the "Demos That Need Pushbutton" directory.