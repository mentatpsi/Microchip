/*********************************************************************
*
*            MIB2BIB - MIB to Binary Information Base Builder
*
**********************************************************************
* FileName:        mib2bib.java
* Complier:        JAVA version "1.6.0_20 and onwards"
* IDE :            Netbean
*
* Software License Agreement
*
* Copyright (C) 2012 Microchip Technology Inc.  All rights reserved.
*
* Microchip licenses to you the right to use, modify, copy, and
* distribute the Software when used with a Microchip microcontroller or
* digital signal controller product which is integrated into Licensee's product.
*
* You should refer to the license agreement accompanying this
* Software for additional information regarding your rights and
* obligations.
*
* THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
* WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
* LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
* MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
* THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
* SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
* (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE
**********************************************************************/

package mib2bib;
import java.io.*;
import java.util.*;

public class mib2bib {

static String VERSION_INFO = "  v2.0.0  ";
final int OID_STRING_LEN =  (256);

/*
 * Help Command
 */
final static String commandHelp =
        "Detailed command help.\n" +
        "\n" +
        "$DeclareVar(oidName, oidDataType, oidType, oidAccess, oidString)\n" +
        "oidName: \n" +
        "        Up to 32 characters long name of this OID. "  +
        "This name is also used in mib.h file for application usage.\n" +
        "oidDataType: \n" +
        "        Any one of following keywords: \n"  +
        "        BYTE - 8-bit data type.\n" +
        "        WORD - 16-bit data type.\n" +
        "        DWORD - 32-bit data type.\n" +
        "        OCTET_STRING - Byte array.\n" +
        "        ASCII_STRING - ASCII String.\n" +
        "        IP_ADDRESS - 4-byte long IP address.\n" +
        "        COUNTER32 - 32-bit counter data type.\n" +
        "        TIME_TICKS - 32-bit time tick data type.\n"  +
        "        GAUGE32 - 32-bit gauge data type.\n" +
        "        OID - Dotted decimal OID string.\n" +
        "Access: \n" +
        "        Any one of following keywords:\n" +
        "        READONLY - This is read-only OID.\n" +
        "        READWRITE - This is read-write OID.\n" +
        "oidString: \n" +
        "        OID String value in \"dotted-decimal\" notation. " +
        "All OID string must being with same root OID. " +
        "If specifying internet OID, first decimal value must 43. " +
        "e.g. 43.6.1.2.1.1.5.\n" +
        "\n" +
        "$StaticVar(oidName, oidValue...)\n" +
        "oidName: \n" +
        "        oidName as defined by previous DeclareVar.\n" +
        "oidValue: \n" +
        "        Static data value of data type as defined by previous DeclareVar.\n" +
        "\n" +
        "$DynamicVar(oidName, ID)\n" +
        "oidName: \n" +
        "        oidName as defined by previous DeclareVar.\n" +
        "ID: \n" +
        "        A 0-255 identifier value to uniquely identify this variable.\n" +
        "\n" +
        "$SequenceVar(oidName, indexOID)\n" +
        "oidName: \n" +
        "        oidName as defined by previous DeclareVar.\n" +
        "indexOID: \n" +
        "        An oid name as defined by previous DeclareVar that will be index for oidName\n" +
        "\n" +
        "$AgentID(oidName, ID)\n" +
        "oidName: \n" +
        "        oidName as defined by previous DeclareVar. " +
        "This variable must be of static OID data type.\n" +
        "ID: \n" +
        "        A 0-255 identifier value to uniquely identify this AgentID. " +
        "Application must define one and only one AgentID if SNMPNotify() " +
        "is to be used.Application must pass this ID in SNMPNotify().\n";

/*
 * Version Message
 */
static String VERSION_MESSAGE = "mib2bib" ;

/*
 * Help Command
 */
final static String help =
        "Converts Microchip Stack MIB data file into Binary Information Base " +
        "and generates 'C' definition file.\n" +
        "\n" +
        "mib2bib [/?] [/h] [/q] <MIBFile> [/b=<OutputBIBDir>] /i=<OutputIncDir>]\n" +
        "        /? : Displays this message\n" +
        "        /h : Displays detailed help for all commands.\n" +
        "        /q : Quitely overwrites output file \"snmp.bib\"\n" +
        "        MIBFile: Microchip Stack MIB input data file.\n" +
        "        /b=<OutputBIBDir> : Output directory where output BIB file should be generated.\n" +
        "                   If no OutputBIBDir is specified, current directory will be used.\n"+
        "        /i=<OutputIncDir> : Output directory where Inc file should be generated.\n" +
        "                   If no OutputIncDir is specified, current directory will be used.\n" +
        "\n" +
        "Output BIB file name is internally set to \"snmp.bib\".\n" +
        "'C' definition file name is set to \"mib.h\".\n" +
        "Include \"snmp.bib\" file in MPFS image and \"mib.h\" in your " +
        "application file that handles SNMP callbacks.\n" +
        "\n" +
        "Examples:  1. mib2bib MySNMP.mib\n" +
        "              Generates snmp.bib file from MySNMP.mib file.\n\n" +
        "           2. mib2bib MySNMP.mib /q\n" +
        "              Overwrites existing snmp.bib file in current directory.\n\n" +
        "           3. mib2bib MySNMP.mib /b=WebPages /q\n" +
        "              Overwrites existing snmp.bib file in WebPages directory.\n";

public PROGRAM_ARGS programArgs = new PROGRAM_ARGS();
public static ERROR_INFO errorInfo;
public SnmpCommand snmpParamCommands;
SnmpCommand.declareVar SnmpDeclareVarCmd;
SnmpCommand.staticVar SnmpStaticVarCmd;
SnmpCommand.dynamicVar SnmpDynamicVarCmd;
SnmpCommand.sequenceVar SnmpSequenceVarCmd;
SnmpCommand.agentID SnmpAgentIDCmd;

/*
 * Mib2bib argument class
 * bDebug - to enable debug option
 * bOverwrite - to overwrite exiting snmp.bib and  mib.h file
 * bInputFile - to avoid more than one input MIB file
 * InputFile - MIB file path name
 * binFile - snmp.bib file path name
 * incFile -  mib.h file path name
 */
public class PROGRAM_ARGS
{
    boolean bDebug;
    boolean bOverwrite;
    boolean binputFile;
    String InputFile;
    String binFile;
    String incFile;
}

public static String INPUT_FILE  =  "snmp.mib";
public static String OUTPUT_FILE  =  "snmp.bib";
public static String OUTPUT_INC_FILE = "mib.h";

/*
 * program argumnet processing error
 */
public enum PROGRAM_RESULT
{
    RESULT_NO_ERROR,
    RESULT_EXIT,
    RESULT_INSUFFICIENT_ARGS,
    RESULT_EXTRA_ARGS,
    RESULT_SHOW_HELP,
    RESULT_FILE_NOT_FOUND,
    RESULT_INVALID_FILE,
    RESULT_INPUT_ERROR,
    RESULT_OUTPUT_ERROR,
}

/*
 * Parse Result error
 */
public enum PARSE_RESULT
{
    PARSE_OK ,
    PARSE_NO_MORE,
    PARSE_ERROR
}

/*
 * process input argument
 */
public  mib2bib(String argc[])
{
    //programArgs = new PROGRAM_ARGS();
    errorInfo = new ERROR_INFO();
    snmpParamCommands = new SnmpCommand(this);
    SnmpDeclareVarCmd = snmpParamCommands.new declareVar();
    SnmpStaticVarCmd = snmpParamCommands.new staticVar();
    SnmpSequenceVarCmd = snmpParamCommands.new sequenceVar();
    SnmpDynamicVarCmd = snmpParamCommands.new dynamicVar();
    SnmpAgentIDCmd = snmpParamCommands.new agentID();
    mib2BibStart(argc);
}

/******************************************************************************
  Function:
	public PROGRAM_RESULT  ProcessArguments(String args[])
  Summary:
	Updates programArgs  class objects.
  Description:
	This function parse all the arguments and updates programArgs class objects.
  Parameters:
	String args[]
  Returns:
  	PROGRAM_RESULT
*******************************************************************************/
public PROGRAM_RESULT  ProcessArguments(String args[])
{
    String fullPath;
    String pFullPath;
    String mibFilePath = new File(".").getAbsolutePath();
    // Load default program options.
    programArgs.bOverwrite = false;
    programArgs.bDebug = false;
    programArgs.binputFile = false;
    programArgs.InputFile = "";
    programArgs.binFile = "";
    programArgs.incFile = "";

    //args[] = "F:\\RC Release\\Microchip Solutions\\TCPIP Demo App\\snmp.mib" +
    //        " -i=\"F:\RC Release\Microchip Solutions\TCPIP Demo App\"";

    if(args.length > 0)
    {
        for ( int i = 0; i < args.length; i++ )
        {
            char ch;
            String arg = args[i];
            if(arg.isEmpty())
                continue;
            ch = arg.charAt(0);
          
            switch(ch)
            {
                case '/':
                case '-':
                    char ch2 = arg.charAt(1);
                    switch(ch2)
                    {
                        case '?':
                        {
                            System.out.println(help);
                            return PROGRAM_RESULT.RESULT_SHOW_HELP;
                        }
                        case 'q': case 'Q':
                        {
                            programArgs.bOverwrite = true;
                        }
                        break;
                        case 'd': case 'D':
                        {
                            programArgs.bDebug = true;
                        }
                        break;
                        case 'h': case 'H':
                        {
                            System.out.println(commandHelp);
                            return PROGRAM_RESULT.RESULT_EXIT;
                        }
                        case 'i': case 'I':
                        {
                            String tempStr[] = arg.split("=");
                            if(tempStr.length > 2)
                                return PROGRAM_RESULT.RESULT_INVALID_FILE;
                            programArgs.incFile = tempStr[1];
                            File file = new File(programArgs.incFile);
                            if(!file.exists())
                            {
                                System.out.println("Output Inc directory "+ programArgs.incFile.toString()+
                                        " does not exist.\n");
                                return PROGRAM_RESULT.RESULT_INVALID_FILE;
                            }
                            programArgs.incFile = programArgs.incFile + "\\"+OUTPUT_INC_FILE;
                        }
                        break;
                        case 'b': case 'B':
                        {
                            String tempStr[] = arg.split("=");
                            if(tempStr.length > 2)
                                return PROGRAM_RESULT.RESULT_INVALID_FILE;
                            programArgs.binFile = tempStr[1];
                            File file = new File(programArgs.binFile);
                            if(!file.exists())
                            {
                                System.out.println("Output Bib directory" + programArgs.binFile.toString()+
                                        " does not exist.\n" );
                                return PROGRAM_RESULT.RESULT_INVALID_FILE;
                            }
                            programArgs.binFile = programArgs.binFile + "\\"+OUTPUT_FILE;
                        }
                        break;
                        default:
                            System.out.println("Unknown option " + arg+" received.\n");
                            return PROGRAM_RESULT.RESULT_EXTRA_ARGS;
                    }
                    break;
                default:
                    if((programArgs.InputFile.isEmpty() != true )&& (programArgs.binputFile))
                    {
                        System.out.println("More than one Input MIB file received.\n");
                        return PROGRAM_RESULT.RESULT_EXTRA_ARGS;
                    }
                    else
                    {
                        pFullPath = new File(args[i]).getAbsolutePath();
                        programArgs.InputFile = pFullPath;
                        programArgs.binputFile = true;
                    }


                    break;
            }
        }
    }
    if ( programArgs.bDebug )
        PrintErrorMessages();

    // Make sure that required information is supplied.
    // Input file name must be present and it muset be valid.
    if ((programArgs.InputFile.isEmpty() == true) && (args.length == 0))
    {
        System.out.println("No input MIB file was specified.\n\n");
        System.out.println(help);
        return PROGRAM_RESULT.RESULT_SHOW_HELP;
    }
    else if(programArgs.binputFile == false)
    {
        programArgs.InputFile = args[0];
    }

    File tempFile = new File(programArgs.InputFile).getAbsoluteFile();
    //System.out.println("tempFile : " + tempFile.getPath());
    if ( !tempFile.exists())
    {
            System.out.println("Input file " + programArgs.InputFile.toString()
                    + " does not exist.\n\n");
            return PROGRAM_RESULT.RESULT_FILE_NOT_FOUND;
    }

    if ( programArgs.binFile.isEmpty() == true )
        programArgs.binFile = OUTPUT_FILE;

    if ( programArgs.incFile.isEmpty() == true )
        programArgs.incFile = OUTPUT_INC_FILE;


    // Create full path for all input and output files so that
    // accurate error/warning messages are generated.
    pFullPath = new File(programArgs.InputFile).getAbsoluteFile().toString();
    programArgs.InputFile = pFullPath;

    pFullPath = new File(programArgs.binFile).getAbsoluteFile().toString();
    programArgs.binFile = pFullPath;

    pFullPath = new File(programArgs.incFile).getAbsoluteFile().toString();
    programArgs.incFile = pFullPath;

//    System.out.println("Input file file " + programArgs.InputFile.toString()
//                    + "\n\n");
//    System.out.println("BIN file " + programArgs.binFile.toString()
//                    + "\n\n");
//    System.out.println("INC file " + programArgs.incFile.toString()
//                    + "\n\n");

    return PROGRAM_RESULT.RESULT_NO_ERROR;
}

/******************************************************************************
  Function:
	void PrintErrorMessages()
  Summary:
	Print Error Messages.
  Description:
	This function parse all the arguments and updates programArgs class objects.
 
  Parameters:
	none
  Returns:
  	none
*******************************************************************************/

void PrintErrorMessages()
{
    int i;
    ERROR_INFO errorMsg = new ERROR_INFO();

    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNEXPECTED_EOF);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNEXPECTED_EOL);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_ESC_SEQUENCE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.EMPTY_COMMAND_STRING);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNEXPECTED_RIGHT_PAR);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_COMMAND_INFO);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNEXPECTED_ESC_CHAR);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNKNOWN_COMMAND);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.DEFINE_INVALID_ARGS);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.DUPLICATE_DEFINE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNKNOWN_DATA_TYPE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNKNOWN_OID_TYPE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_OID_LEN);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.DYNAMIC_OID_INVALID_ARGS);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.OID_NOT_DEFINED);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_OID_ID);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.SEQUENCE_OID_INVALID_ARGS);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.OID_VALUE_ASSIGNED);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_INDEX_COUNT);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.STATIC_SEQUENCE_OID);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.ROOT_NOT_SAME_OR_DUPLICATE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_INDEX_DEFINED);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.UNKNOWN_OID_ACCESS);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.OID_ID_ASSIGNED);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.OID_ID_DUPLICATE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.NO_OID_STATIC_VALUE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.NO_OID_INDEX);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.OID_DATA_SCOPE_UNKNOWN);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_DATA);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.AGENT_ID_INVALID_ARGS);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.NOT_OID_DATA_TYPE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.NOT_A_CONSTANT_DATA_TYPE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.ALREADY_AN_AGENT_ID);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.AGENT_ID_ALREADY_DEFINED);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.OID_WITH_READWRITE_ACCESS);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.DYNAMIC_OID_TYPE);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.ALREADY_DYNAMIC);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.ALREADY_STATIC);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.NOT_AN_INTERNET_ROOT);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.VALUE_TRUNCATED);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.TOO_LONG_STRING);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.INVALID_OID_NAME);
    errorMsg.printErrorInfo(FILE_ERROR_CODE.TOO_MANY_DYNAMIC_OIDS);
}

/*
 * This Enum is used as state machine to validate each valid line of the
 * snmp.mib file.
 */
enum GetLineState
{
  SM_COMMAND,
  SM_COMMENT,
  SM_NEW_LINE,
  SM_ARGS,
  SM_ESCAPE,
  SM_LINE_CONTINUE,
  SM_END_OF_COMMAND
}
public static int lineNumber = 1;
/******************************************************************************
  Function:
	public PARSE_RESULT GetLine(File file,List<String> pParamList,int line,String newLine)
  Summary:
	Parse each line which starts with $.
  Description:
	Parse Each line which starts with $ and store all the string in the
        pParamList.
  Parameters:
	File file ----- snmp.mib
        List<String> pParamList ---- List to collect all the token strings from
        the line.
        int line ---- keep line number to print error message.
        String newLine ---- $ line.
  Returns:
  	PARSE_RESULT
*******************************************************************************/
public PARSE_RESULT GetLine(File file,List<String> pParamList,int line,String newLine)
{
    char[] tempParam= new char[1024] ;
    int paramLen;
    boolean lbEndOfFile;
    PARSE_RESULT lenumResult;
    boolean lbDone;
    char c;
    char COMMAND_CHAR = '$';
    int index=0;
    GetLineState state = GetLineState.SM_NEW_LINE;

    pParamList.clear();
    paramLen = 0;
    lbEndOfFile = false;
    lbDone = false;
    lenumResult = PARSE_RESULT.PARSE_ERROR;
    for(int i=0;i<1024;i++)
    {
        tempParam[i]='\0';
    }
    //System.out.println(" newLine : "+newLine +"  \t   Length"+newLine.length());
    while( !lbEndOfFile && !lbDone)
    {
        if(newLine == null)
            lbEndOfFile = true;
        if(index >= newLine.length())
        {
            lbDone = true;
            break;
        }
        c = newLine.charAt(index++);
        //System.out.println(" Index " + (index-1) + "\tchar" + Character.toString(c)+ "\n");
//        if ( c == -1 )
//        {
//            lbDone = true;
//            break;
//        }

        switch(state)
        {
        case SM_NEW_LINE:
            // End-of-file during this state is normal.
            if ( lbEndOfFile )
            {
                lbDone = true;
                //lbReturn = FALSE;
                lenumResult = PARSE_RESULT.PARSE_NO_MORE;
                break;
            }

            if ( c == ' ' || c == '\t' )
                break;
            else if ( c == '\r' || c == '\n')
            {
                lineNumber++;
                break;
            }
            else if (c == COMMAND_CHAR )
            {
                paramLen = 0;
                state = GetLineState.SM_COMMAND;
            }
            else if ( c != ' ' && c != '\t' )
                state = GetLineState.SM_COMMENT;
            break;

        case SM_COMMAND:
            // End-of-file during this state is a syntax error.
            if ( lbEndOfFile )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_EOF);
                lbDone = true;
                lenumResult = PARSE_RESULT.PARSE_NO_MORE;
                break;
            }

            if ( c == '(' )
            {
                // When we encounter '(', current parameter must be non-zero bytes long.
                if ( paramLen == 0 )
                {
                    // Or else, this is an syntax error.
                    ShowError(file, lineNumber, FILE_ERROR_CODE.EMPTY_COMMAND_STRING);
                    lbDone = true;
                    lenumResult = PARSE_RESULT.PARSE_NO_MORE;
                    break;
                }
                else
                {
                    // '(' after non-zero long parameter is a valid syntax.
                    tempParam[paramLen]= '\0';
                    String tempStr = new String(tempParam,0,paramLen);
                    paramLen = 0;
                    pParamList.add(tempStr);
                    //System.out.println("( SM_COMMAND tempParam:" + tempStr + "    length" + tempStr.length()+"\n");
                }

                state = GetLineState.SM_ARGS;
            }
            // There must not be ')', as we have not even started the command yet.
            else if ( c == ')' )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_RIGHT_PAR);
                lbDone = true;
                lenumResult = PARSE_RESULT.PARSE_ERROR;
                break;
            }
            else if ( c == '\\' )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_ESC_CHAR);
                lbDone = true;
                lenumResult = PARSE_RESULT.PARSE_ERROR;
                break;
            }
            else
                tempParam[paramLen++] = c;
            break;

        case SM_ARGS:
            // End-of-file during state is a syntax error.
            if ( lbEndOfFile )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_EOF);
                lenumResult = PARSE_RESULT.PARSE_ERROR;
                lbDone = true;
                break;
            }

            // Skip spaces...
            if ( paramLen == 0 && (c == ' ' || c == '\t') )
                break;

            // if encountered '\', see if it needs to be escaped.
            if ( c == '\\' )
                state = GetLineState.SM_ESCAPE;

            // ',' means end of parameter, there would be more parameters.
            if ( c == ',' )
            {
                tempParam[paramLen] = 0;                
                String tempStr = new String(tempParam,0,paramLen);
                paramLen = 0;
                pParamList.add(tempStr);
                //System.out.println(", SM_ARGS tempParam: "+tempStr+"\n");
            }

            // ')' also means end of parameter and end of comand.
            else if ( c == ')' )
            {
                tempParam[paramLen] = 0;                
                String tempStr = new String(tempParam,0,paramLen);
                paramLen = 0;
                pParamList.add(tempStr);
                //System.out.println(" ) SM_ARGS tempParam: "+tempStr+"\n");
                //state = SM_END_OF_COMMAND;

                lbDone = true;

                if ( pParamList.size() > 0 )
                    lenumResult = PARSE_RESULT.PARSE_OK;
                else
                {
                    ShowError(file, lineNumber, FILE_ERROR_CODE.INVALID_COMMAND_INFO);
                    lenumResult = PARSE_RESULT.PARSE_ERROR;
                    break;
                }

            }
            else if ( c == '\r' || c == '\n' )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_EOL);
                lbDone = true;
                lenumResult = PARSE_RESULT.PARSE_ERROR;
            }

            else
                tempParam[paramLen++] = c;
            break;

        case SM_ESCAPE:
            // End-of-file during state is a syntax error.
            if ( lbEndOfFile )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_EOF);
                lbDone = true;
                lenumResult = PARSE_RESULT.PARSE_ERROR;
                break;
            }

            // Only following characters are escaped.
            if ( c == '\\' || c == ',' || c == ')' || c == '(' )
            {
                tempParam[paramLen++] = c;
                state = GetLineState.SM_ARGS;
            }
            else if ( c == '\n' || c == '\r' )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_EOF);
                lbDone = true;
                lenumResult = PARSE_RESULT.PARSE_ERROR;
            }
            else
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.INVALID_ESC_SEQUENCE);
                lbDone = true;
                lenumResult = PARSE_RESULT.PARSE_ERROR;

                // or else, it is considered line continuation.
                //state = SM_LINE_CONTINUE;
            }
            break;

        case SM_COMMENT:
            // It is okay to have end-of-file at this time.
            if ( lbEndOfFile )
            {
                lbDone = true;
                //lbReturn = FALSE;
                lenumResult = PARSE_RESULT.PARSE_NO_MORE;
                break;
            }

            if ( c == '\r' || c == '\n' )
            {
                lineNumber++;
                state = GetLineState.SM_NEW_LINE;
            }
            break;


        case SM_LINE_CONTINUE:
            // End-of-file at this stage, is a syntax error.
            if ( lbEndOfFile )
            {
                ShowError(file, lineNumber, FILE_ERROR_CODE.UNEXPECTED_EOF);
                lbDone = true;
                //lbReturn = FALSE;
                lenumResult = PARSE_RESULT.PARSE_ERROR;
                break;
            }

            if ( c == '\r' || c == '\n' )
            {
                lineNumber++;
                state = GetLineState.SM_ARGS;
            }

            break;

        case SM_END_OF_COMMAND:
            // It is okay to have end-of-file at this time.
            if ( lbEndOfFile )
            {
                lbDone = true;
                //lbReturn = !!(pParamList->Count);
                if ( pParamList.size() > 0 )
                    lenumResult = PARSE_RESULT.PARSE_NO_MORE;
                else
                {
                    ShowError(file, lineNumber, FILE_ERROR_CODE.INVALID_COMMAND_INFO);
                    lenumResult = PARSE_RESULT.PARSE_ERROR;
                }

                break;
            }

            if ( c == '\r' || c == '\n')
            {
                lineNumber++;
                state = GetLineState.SM_NEW_LINE;
                lbDone = true;
                //lbReturn = TRUE;
                lenumResult = PARSE_RESULT.PARSE_OK;
                break;
            }
            break;
        }
    }

    line = lineNumber;
    return lenumResult;
}
/*
 * snmp Command list
 */
static String[] CommandList =
{
    "DeclareVar",
    "DynamicVar",
    "StaticVar",
    "SequenceVar",
    "AgentID",
};

/******************************************************************************
  Function:
	public boolean ProcessCommand(List pList, File file, int line)
  Summary:
	Process each command.
  Description:
	Parse Each line which starts with $ and store all the string in the
        pParamList.
  Parameters:
	File file ----- snmp.mib
        List pList ---- Collection of tokens of a String.
        int line ---- keep line number to print error message.

  Returns:
  	true or false.
*******************************************************************************/
public boolean ProcessCommand(List<String> pList, File file, int line)
{
    int paramCount;

    // Extract total number of paramters into this command including command itself.
    paramCount = pList.size();
    // If there is none, that would be syntax error.
    if ( paramCount <= 0 )
    {
        ShowError(file, line, FILE_ERROR_CODE.INVALID_COMMAND_INFO);
        return false;
    }

    // Now go through all known commands and handle it accordingly.
//    for ( int i = 0; i < CommandList.length; i++ )
//    {
        if ( pList.get(0).toString().compareTo("DeclareVar") == 0)
        {
            return SnmpDeclareVarCmd.HandleDeclareVar(pList, file, line);
        }
        else if(pList.get(0).toString().compareTo("StaticVar") == 0)
        {
            return SnmpStaticVarCmd.HandleStaticVar(pList, file, line);
        }
        else if(pList.get(0).toString().compareTo("DynamicVar") == 0)
        {
            return SnmpDynamicVarCmd.HandleDynamicVar(pList, file, line);
        }
        else if(pList.get(0).toString().compareTo("SequenceVar") == 0)
        {
            return SnmpSequenceVarCmd.HandleSequenceVar(pList, file, line);
        }
        else if(pList.get(0).toString().compareTo("AgentID") == 0)
        {
            return SnmpAgentIDCmd.HandleAgentID(pList, file, line);
        }
        else
        {
           ShowError(file, line, FILE_ERROR_CODE.INVALID_COMMAND_INFO);
           //return false;
        }
            //return CommandList[i].CommandHandler(pList, file, line);
           
   // }

    //ShowError(file, line, FILE_ERROR_CODE.UNKNOWN_COMMAND);
    return false;
}

/******************************************************************************
  Function:
	public int ParseFile(String input)
  Summary:
	Parse Snmp.mib file
  Description:
	Read the file and find a valid line which starts with $ and process
        all the tokens of that line.
  Parameters:
	String input - input Source file.

  Returns:
  	error count .
*******************************************************************************/
public int ParseFile(String input)
{
    File hFile;
    List<String> pCommandParams;
    int line=0;
    int errorCount;
    PARSE_RESULT parseResult = PARSE_RESULT.PARSE_ERROR;
    String strLine;
    errorCount = 0;

    hFile = new File(input);
    if(hFile.exists() == false)
        return 0;
    pCommandParams = new ArrayList<String>();

    try
    {
        FileInputStream inputFile = new FileInputStream(hFile);
        DataInputStream in = new DataInputStream(inputFile);
        BufferedReader br = new BufferedReader(new InputStreamReader(in));
        do
        {
            strLine = br.readLine();
            line++;
            if(strLine == null)
            {
                parseResult = PARSE_RESULT.PARSE_NO_MORE;
                if((line == 0) && (pCommandParams.size() == 0))
                {
                      ShowError(hFile, lineNumber, FILE_ERROR_CODE.UNEXPECTED_EOF);
                      errorCount++;
                }
                break;
            }
            strLine.concat("\r\n");
            if(strLine.startsWith("$", 0) == false)
            {                
                continue;
            }
            parseResult = GetLine(hFile, pCommandParams, line,strLine);

            if ( parseResult ==  PARSE_RESULT.PARSE_OK )
            {
                if ( programArgs.bDebug )
                {
                    for ( int i = 0; i < pCommandParams.size(); i++ )
                        System.out.println(pCommandParams.get(i).toString());
                    System.out.println("\n");
                }

                if ( !ProcessCommand(pCommandParams, hFile, line) )
                {
                    errorCount++;
                }
            }
            else if ( parseResult == PARSE_RESULT.PARSE_ERROR )
            {
                errorCount++;
                //break;
            }
            //if(errorCount > 0)
              //  break;

        } while ( parseResult != PARSE_RESULT.PARSE_NO_MORE );
        br.close();
    }catch(IOException e) {
       System.out.println ("IO exception = " + e );
    }
    return errorCount;
}

/*
 * error enums
 */
public enum FILE_ERROR_CODE
{
    UNEXPECTED_EOF,
    UNEXPECTED_EOL,
    INVALID_ESC_SEQUENCE,
    EMPTY_COMMAND_STRING,
    UNEXPECTED_RIGHT_PAR,
    INVALID_COMMAND_INFO,
    UNEXPECTED_ESC_CHAR,
    UNKNOWN_COMMAND,
    DEFINE_INVALID_ARGS,
    DUPLICATE_DEFINE,
    UNKNOWN_DATA_TYPE,
    UNKNOWN_OID_TYPE,
    INVALID_OID_LEN,
    DYNAMIC_OID_INVALID_ARGS,
    OID_NOT_DEFINED,
    INVALID_OID_ID,
    STATIC_OID_INVALID_ARGS,
    SEQUENCE_OID_INVALID_ARGS,
    OID_VALUE_ASSIGNED,
    INVALID_INDEX_COUNT,
    STATIC_SEQUENCE_OID,
    ROOT_NOT_SAME_OR_DUPLICATE,
    INVALID_INDEX_DEFINED,
    UNKNOWN_OID_ACCESS,
    OID_ID_ASSIGNED,
    OID_ID_DUPLICATE,
    //NO_OID_DYN_ID,
    NO_OID_STATIC_VALUE,
    NO_OID_INDEX,
    OID_DATA_SCOPE_UNKNOWN,
    INVALID_DATA,
    AGENT_ID_INVALID_ARGS,
    NOT_OID_DATA_TYPE,
    NOT_A_CONSTANT_DATA_TYPE,
    ALREADY_AN_AGENT_ID,
    AGENT_ID_ALREADY_DEFINED,
    OID_WITH_READWRITE_ACCESS,
    DYNAMIC_OID_TYPE,
    ALREADY_DYNAMIC,
    ALREADY_STATIC,
    NOT_AN_INTERNET_ROOT,
    VALUE_TRUNCATED,
    TOO_LONG_STRING,
    INVALID_OID_NAME,
    TOO_MANY_DYNAMIC_OIDS,
}
/*
 * error info class.
 */
public class ERROR_INFO
{
    String errorMessages[];
    FILE_ERROR_CODE errorCode;

/*
 * print error info for perticular error code.
 */
    void printErrorInfo(FILE_ERROR_CODE errorCode)
    {
        switch(errorCode)
        {
            case UNEXPECTED_EOF:
                System.out.println("Unexpected end-of-file found.");
                break;
            case UNEXPECTED_EOL:
                System.out.println("Unexpected end-of-line found.");
                break;
            case INVALID_ESC_SEQUENCE:
                System.out.println("Invalid escape sequence detected;  Only ',', '\\', '(', or ')' may follow '\\'");
                break;
            case EMPTY_COMMAND_STRING:
                System.out.println("Unexpected empty command string received.");
                break;
            case UNEXPECTED_RIGHT_PAR:
                System.out.println("Unexpected empty command string received.");
                break;
            case INVALID_COMMAND_INFO:
                System.out.println("Invalid or empty command received.");
                break;
            case UNEXPECTED_ESC_CHAR:
                System.out.println("Unexpected escape character received.");
                break;
            case UNKNOWN_COMMAND:
                System.out.println("Unknown command received.");
                break;
            case DEFINE_INVALID_ARGS:
                System.out.println("Invalid parameters: Expected $DeclareVar(oidName, dataType, oidType, access, oid)");
                break;
            case DUPLICATE_DEFINE:
                System.out.println("Duplicate OID name found.");
                break;
            case UNKNOWN_DATA_TYPE:
                System.out.println("Unknown data type received.");
                break;
            case UNKNOWN_OID_TYPE:
                System.out.println("Unknown OID type received.");
                break;
            case INVALID_OID_LEN:
                System.out.println("Empty OID string received.");
                break;
            case DYNAMIC_OID_INVALID_ARGS:
                System.out.println("Invalid parameters: Expected $DynamicVar(oidName, id)");
                break;
            case OID_NOT_DEFINED:
                System.out.println("OID name is not defined.");
                break;
            case INVALID_OID_ID:
                System.out.println("Invalid OID id received - must be between 0-255 inclusive.");
                break;
            case STATIC_OID_INVALID_ARGS:
                System.out.println("Invalid parameters: Expected $StaticVar(oidName, value)");
                break;
            case SEQUENCE_OID_INVALID_ARGS:
                System.out.println("Invalid parameters: Expected $SequenceVar(oidName, index)");
                break;
            case OID_VALUE_ASSIGNED:
                System.out.println("Invalid number of index parameters received.");
                break;
            case INVALID_INDEX_COUNT:
                System.out.println("Current OID already contains a Static value.");
                break;
            case STATIC_SEQUENCE_OID:
                System.out.println("OID of Sequence type cannot contain static data.");
                break;
            case ROOT_NOT_SAME_OR_DUPLICATE:
                System.out.println("This is duplicate OID or Root of this OID is not same as previous OID(s), " +
                                "or this OID is a child of a previously defined OID.");
                break;
            case INVALID_INDEX_DEFINED:
                System.out.println("Invalid index received: Must be BYTE data value.");
                break;
            case UNKNOWN_OID_ACCESS:
                System.out.println("Invalid OID access type received: Must be \"READONLY\" or \"READWRITE\".");
                break;
            case OID_ID_ASSIGNED:
                System.out.println("Current OID is already assigned an ID value.");
                break;
            case OID_ID_DUPLICATE:
                System.out.println("Duplicate dynamic ID found.");
                break;
    //NO_OID_DYN_ID,
            case NO_OID_STATIC_VALUE:
                System.out.println("No static value found for this OID.");
                break;
            case NO_OID_INDEX:
                System.out.println("No index value found for this OID.");
                break;
            case OID_DATA_SCOPE_UNKNOWN:
                System.out.println("OID data scope (dynamic/static) is not defined.");
                break;
            case INVALID_DATA:
                System.out.println("Invalid data value found.");
                break;
            case AGENT_ID_INVALID_ARGS:
                System.out.println("Invalid parameters: Expected $AgentID(oidName, id)");
                break;
            case NOT_OID_DATA_TYPE:
                System.out.println("Only OID data type is allowed for this command.");
                break;
            case NOT_A_CONSTANT_DATA_TYPE:
                System.out.println("This OID must contain static OID data.");
                break;
            case ALREADY_AN_AGENT_ID:
                System.out.println("This OID is already declared as an Agent ID.");
                break;
            case AGENT_ID_ALREADY_DEFINED:
                System.out.println("An Agent ID is already assigned.");
                break;
            case OID_WITH_READWRITE_ACCESS:
                System.out.println("OID with READWRITE access cannot be Static.");
                break;
            case DYNAMIC_OID_TYPE:
                System.out.println("OID of OID data type cannot be Dynamic.");
                break;
            case ALREADY_DYNAMIC:
                System.out.println("This OID is already declared as Dynamic.");
                break;
            case ALREADY_STATIC:
                System.out.println("This OID is already declared as Static.");
                break;
            case NOT_AN_INTERNET_ROOT:
                System.out.println("This OID does not contain Internet root. " +
                                "Internet root of '43' must be used if this Internet MIB.");
                break;
            case VALUE_TRUNCATED:
                System.out.println("Given value was truncated to fit in specified data type.");
                break;
            case TOO_LONG_STRING:
                System.out.println("Given string exceeds maximum length of 127.");
                break;
            case INVALID_OID_NAME:
                System.out.println("Invalid OID name detected.  OID Name " +
                                            "must follow standard 'C' variable naming convention.");
                break;
            case TOO_MANY_DYNAMIC_OIDS:
                System.out.println("Total number of Dynamic OIDs exceeds 256.\n");
                break;
        }
    }

    /*
     * Get error String from a errorcode.
     */
    String ErrorInfoStr(FILE_ERROR_CODE errorCode)
    {
        switch(errorCode)
        {
            case UNEXPECTED_EOF:
                 return("Unexpected end-of-file found.");

            case UNEXPECTED_EOL:
               return ("Unexpected end-of-line found.");

            case INVALID_ESC_SEQUENCE:
                return ("Invalid escape sequence detected;  Only ',', '\\', '(', or ')' may follow '\\'");

            case EMPTY_COMMAND_STRING:
                return ("Unexpected empty command string received.");

            case UNEXPECTED_RIGHT_PAR:
                return ("Unexpected empty command string received.");

            case INVALID_COMMAND_INFO:
                return ("Invalid or empty command received.");

            case UNEXPECTED_ESC_CHAR:
                return ("Unexpected escape character received.");

            case UNKNOWN_COMMAND:
                return ("Unknown command received.");

            case DEFINE_INVALID_ARGS:
                return ("Invalid parameters: Expected $DeclareVar(oidName, dataType, oidType, access, oid)");

            case DUPLICATE_DEFINE:
                return ("Duplicate OID name found.");

            case UNKNOWN_DATA_TYPE:
                return ("Unknown data type received.");

            case UNKNOWN_OID_TYPE:
                return ("Unknown OID type received.");

            case INVALID_OID_LEN:
                return ("Empty OID string received.");

            case DYNAMIC_OID_INVALID_ARGS:
                return ("Invalid parameters: Expected $DynamicVar(oidName, id)");

            case OID_NOT_DEFINED:
                return ("OID name is not defined.");

            case INVALID_OID_ID:
                return ("Invalid OID id received - must be between 0-255 inclusive.");

            case STATIC_OID_INVALID_ARGS:
                return ("Invalid parameters: Expected $StaticVar(oidName, value)");

            case SEQUENCE_OID_INVALID_ARGS:
                return ("Invalid parameters: Expected $SequenceVar(oidName, index)");

            case OID_VALUE_ASSIGNED:
                return ("Invalid number of index parameters received.");

            case INVALID_INDEX_COUNT:
                return ("Current OID already contains a Static value.");

            case STATIC_SEQUENCE_OID:
                return ("OID of Sequence type cannot contain static data.");

            case ROOT_NOT_SAME_OR_DUPLICATE:
                return ("This is duplicate OID or Root of this OID is not same as previous OID(s), " +
                                "or this OID is a child of a previously defined OID.");

            case INVALID_INDEX_DEFINED:
                return ("Invalid index received: Must be BYTE data value.");

            case UNKNOWN_OID_ACCESS:
                return ("Invalid OID access type received: Must be \"READONLY\" or \"READWRITE\".");

            case OID_ID_ASSIGNED:
                return ("Current OID is already assigned an ID value.");

            case OID_ID_DUPLICATE:
                return ("Duplicate dynamic ID found.");

    //NO_OID_DYN_ID,
            case NO_OID_STATIC_VALUE:
                return ("No static value found for this OID.");

            case NO_OID_INDEX:
                return ("No index value found for this OID.");

            case OID_DATA_SCOPE_UNKNOWN:
                return ("OID data scope (dynamic/static) is not defined.");

            case INVALID_DATA:
                return ("Invalid data value found.");

            case AGENT_ID_INVALID_ARGS:
                return ("Invalid parameters: Expected $AgentID(oidName, id)");

            case NOT_OID_DATA_TYPE:
                return ("Only OID data type is allowed for this command.");

            case NOT_A_CONSTANT_DATA_TYPE:
                return ("This OID must contain static OID data.");

            case ALREADY_AN_AGENT_ID:
                return ("This OID is already declared as an Agent ID.");

            case AGENT_ID_ALREADY_DEFINED:
                return ("An Agent ID is already assigned.");

            case OID_WITH_READWRITE_ACCESS:
                return ("OID with READWRITE access cannot be Static.");

            case DYNAMIC_OID_TYPE:
                return ("OID of OID data type cannot be Dynamic.");

            case ALREADY_DYNAMIC:
                return ("This OID is already declared as Dynamic.");

            case ALREADY_STATIC:
                return ("This OID is already declared as Static.");

            case NOT_AN_INTERNET_ROOT:
                return ("This OID does not contain Internet root. " +
                                "Internet root of '43' must be used if this Internet MIB.");

            case VALUE_TRUNCATED:
                return ("Given value was truncated to fit in specified data type.");

            case TOO_LONG_STRING:
                return ("Given string exceeds maximum length of 127.");

            case INVALID_OID_NAME:
                return ("Invalid OID name detected.  OID Name " +
                                            "must follow standard 'C' variable naming convention.");

            case TOO_MANY_DYNAMIC_OIDS:
                return ("Total number of Dynamic OIDs exceeds 256.\n");                
        }
        return null;
    }
}

/******************************************************************************
  Function:
	void ShowWarning(File file, int line, FILE_ERROR_CODE code)
  Summary:
	Print only warnings.
  Description:
	This Method will print only warnings.
  Parameters:
	File file ----- snmp.mib
        FILE_ERROR_CODE code -- error code
        int line ---- keep line number to print error message.
  Returns:
  	none
*******************************************************************************/
void ShowWarning(File file, int line, FILE_ERROR_CODE code)
{
    String str = errorInfo.ErrorInfoStr(code);
    if(str != null)
    {
        System.out.println(file.getName()+ ": " + line + ": " + "Warning [" +
            code + "] " + str);
    }
    else
    {
       System.out.println(file.getName()+ ": " + line + ": " + "Warning [" +
            code + "] Unknown warning code.\n");
    }
}

/******************************************************************************
  Function:
	void ShowError(File file, int line, FILE_ERROR_CODE code)
  Summary:
	Print only Errors.
  Description:
	This Method will print only Errors.
  Parameters:
	File file ----- snmp.mib
        FILE_ERROR_CODE code -- error code
        int line ---- keep line number to print error message.
  Returns:
  	none
*******************************************************************************/
public void ShowError(File file, int line, FILE_ERROR_CODE code)
{
    String str = errorInfo.ErrorInfoStr(code);
    if(str != null)
    {
        System.out.println(file.getName()+ ": " + line + ": "+ "Error [" +
            code + "] " + str);
    }
    else
    {
         System.out.println(file.getName()+ ": " + line + ": " + "Error [" +
            code + "]Unknown error code.\n ");
    }
  
}

/******************************************************************************
  Function:
	void mib2BibStart(String argc[])
  Summary:
	Initial method
  Description:
	Initial Method
  Parameters:
	String argc[] - input arguments.
  Returns:
  	none
*******************************************************************************/
void mib2BibStart(String argc[])
{
    int liResult;
    PROGRAM_RESULT lenmResults;

    programArgs = new PROGRAM_ARGS();

    lenmResults = ProcessArguments(argc);
    if ( lenmResults != PROGRAM_RESULT.RESULT_NO_ERROR )
            return ;

    System.out.println("Input MIB File :" + programArgs.InputFile + "\n");
    System.out.println("Output BIB File:" + programArgs.binFile + "\n");
    System.out.println("Output Inc File:" + programArgs.incFile + "\n");
    System.out.println("\n");

    liResult =  ParseFile(programArgs.InputFile);
    if ( liResult > 0 )
        System.out.println("\nTotal of " + liResult +"error(s) detected.\n");

    if ( liResult == 0 )
        liResult = snmpParamCommands.ValidateFile(programArgs.InputFile);


    if ( liResult == 0 )
        snmpParamCommands.ProcessFile(programArgs.binFile, programArgs.incFile, programArgs.bOverwrite);

}

public static void main(String argc[])
{
    VERSION_INFO  = VERSION_INFO + " <Tuesday, Nov 01, 2011>\n";
    VERSION_MESSAGE = VERSION_MESSAGE + VERSION_INFO + "Copyright (c) 2003 Microchip Technology Inc.\n";
    System.out.println(VERSION_MESSAGE);

    new mib2bib(argc);

    return ;
}
}
