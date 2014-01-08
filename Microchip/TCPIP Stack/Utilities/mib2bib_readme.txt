mib2bib.jar is a command line Java program.

Java Runtime Environment is required.

To use in Windows OS:
At command line prompt, navigate to where mib2bib.jar is located, this is typically -
<install>\Microchip\TCPIP Stack\Utilities\

Then type:

java - jar mib2bib.jar

The usage details and options will be displayed.

Example: java -jar mib2bib.jar snmp.mib

Where snmp.mib is the input file.
The ouput files are snmp.bib and mib.h.
mib.h is the list of dynamic variables present in snmp.mib file.

- This jar file supports a dynamic ID upto 1024 numbers.


