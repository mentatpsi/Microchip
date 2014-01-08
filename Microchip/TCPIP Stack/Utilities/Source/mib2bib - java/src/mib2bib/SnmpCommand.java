/**********************************************************************
*
*            MIB2BIB - MIB to Binary Information Base Builder
*
**********************************************************************
* FileName:        SnmpCommand.java
* Complier:        JAVA version "1.6.0_20 and onwards"
* IDE :            Netbean
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
import java.util.*;
import java.io.*;
import java.text.*;

public class SnmpCommand {

    public int  MAX_DATA_LEN =   (256);
    public int  MAX_INDEX_LEN =  (255);
    public final int MAX_DYNAMIC_ID_SIZE = 1024;
    public int INDEX_CONSTANT  = (0x00);
    public int INDEX_OID = (0x01);
    public enum DATA_TYPE
    {
        INT8_VAL ,
        INT16_VAL,
        INT32_VAL,
        OCTET_STRING,
        ASCII_STRING,
        IP_ADDRESS,
        COUNTER32,
        TIME_TICKS,
        GAUGE32,
        OID_VAL,
        DATA_TYPE_UNKNOWN
    }
    String [] dataTypeTable =
    {
        "BYTE",
        "WORD",
        "DWORD",
        "OCTET_STRING",
        "ASCII_STRING",
        "IP_ADDRESS",
        "COUNTER32",
        "TIME_TICKS",
        "GAUGE32",
        "OID",
    };
    public enum OID_TYPE
    {
        SINGLE,
        SEQUENCE,
        OID_TYPE_UNKNOWN
    }
    String[] oidTypes=
    {
        "SINGLE",
        "SEQUENCE",
    };

    public int  OID_TYPES_COUNT=2;

    public enum OID_ACCESS
    {
        OID_ACCESS_RO,
        OID_ACCESS_RW,
        OID_ACCESS_UNKNOWN
    }

    String[] oidAccessDictionary =
    {
        "READONLY",
        "READWRITE"
    };
    public int OID_STRING_LEN    =  (256);
    enum OID_INFO
    {
        bIsDistantSibling,
        bIsConstant,
        bIsSequence,
        bIsSibling,
        bIsParent,
        bIsEditable,
        bIsAgentID,
        bIsIDPresent,
    }
    class MIB_STATISTICS
    {
        long totalBytes;
        long totalStaticOIDs;
        long totalDynamicOIDs;
        long totalSequenceOIDs;
        long totalAgentIDs;
        long totalReadOnlyOIDs;
        long totalReadWriteOIDs;
        long totalDataBytes;
    } 
    mib2bib mainMib2bib;
    Map<String,DATA_TYPE> dataTypeInfoMap;
    HashMap<String,OID_TYPE> NodeTypeMap;
    HashMap<String,OID_ACCESS> AccessTypeMap;
    //HashMap dataTypeMapWithVal;
    Map<DATA_TYPE,Byte> dataTypeMapWithVal = new HashMap<DATA_TYPE,Byte>();
    public OID_NODE mibRoot;
    public SnmpCommand()
    {
        System.out.println("Snmp mib");
    }

/*
 * Constructor
 */
    public SnmpCommand(mib2bib parent)
    {
        mainMib2bib = parent;

        dataTypeInfoMap = new HashMap<String,DATA_TYPE>();
        dataTypeInfoMap.put("BYTE",DATA_TYPE.INT8_VAL);
        dataTypeInfoMap.put("WORD",DATA_TYPE.INT16_VAL);
        dataTypeInfoMap.put("DWORD",DATA_TYPE.INT32_VAL);
        dataTypeInfoMap.put("OCTET_STRING",DATA_TYPE.OCTET_STRING);
        dataTypeInfoMap.put("ASCII_STRING",DATA_TYPE.ASCII_STRING);
        dataTypeInfoMap.put("IP_ADDRESS",DATA_TYPE.IP_ADDRESS);
        dataTypeInfoMap.put("COUNTER32",DATA_TYPE.COUNTER32);
        dataTypeInfoMap.put("TIME_TICKS",DATA_TYPE.TIME_TICKS);
        dataTypeInfoMap.put("GAUGE32",DATA_TYPE.GAUGE32);
        dataTypeInfoMap.put("OID",DATA_TYPE.OID_VAL);

        NodeTypeMap = new HashMap<String,OID_TYPE>();
        NodeTypeMap.put("SINGLE",OID_TYPE.SINGLE);
        NodeTypeMap.put("SEQUENCE",OID_TYPE.SEQUENCE);

        AccessTypeMap = new HashMap<String,OID_ACCESS>();
        AccessTypeMap.put("READONLY",OID_ACCESS.OID_ACCESS_RO);
        AccessTypeMap.put("READWRITE",OID_ACCESS.OID_ACCESS_RW);



        dataTypeMapWithVal.put(DATA_TYPE.INT8_VAL,(Byte.valueOf((byte)0)));
        dataTypeMapWithVal.put(DATA_TYPE.INT16_VAL,(Byte.valueOf((byte)1)));
        dataTypeMapWithVal.put(DATA_TYPE.INT32_VAL,(Byte.valueOf((byte)2)));
        dataTypeMapWithVal.put(DATA_TYPE.OCTET_STRING,(Byte.valueOf((byte)3)));
        dataTypeMapWithVal.put(DATA_TYPE.ASCII_STRING,(Byte.valueOf((byte)4)));
        dataTypeMapWithVal.put(DATA_TYPE.IP_ADDRESS,(Byte.valueOf((byte)5)));
        dataTypeMapWithVal.put(DATA_TYPE.COUNTER32,(Byte.valueOf((byte)6)));
        dataTypeMapWithVal.put(DATA_TYPE.TIME_TICKS,(Byte.valueOf((byte)7)));
        dataTypeMapWithVal.put(DATA_TYPE.GAUGE32,(Byte.valueOf((byte)8)));
        dataTypeMapWithVal.put(DATA_TYPE.OID_VAL,(Byte.valueOf((byte)9)));


        //mibRoot = new OID_NODE();
    }

    /*
     * OID_NODE class is used as a tree node.
     */
    public class OID_NODE //extends SnmpCommand
    {
        String name;
        String oidASCIIString;
        String dataTypeString;
        long id;
        byte oid;
        DATA_TYPE dataType;
        long siblingOffset;
        long distantSiblingOffset;
        short dataLen;
        short oid_info_val=0;

        long int32;
        long counter32;
        long gauge32;
        long timeTicks;
        byte[] octets = new byte[MAX_DATA_LEN];
        byte[] ipAddress = new byte[4];
        int int16;
        byte Byte;
        byte[] oidString =  new byte[MAX_DATA_LEN];
        byte[] data = new byte[MAX_DATA_LEN];
        

        short indexLen;
        short indexBytes;
        /*
         * Class index is used for sequence variable.
         */
        public class INDEX
        {
        short indexType;
        String indexName;
        byte constant;
        long id;
        DATA_TYPE dataType;
        }


        OID_TYPE oidType;
        List<OID_NODE> pChilds;
        List<String> pIndex;
        private  OID_NODE pParent;
        long recordOffset;
        INDEX[] idx = new INDEX[MAX_INDEX_LEN];
        int line;

        /*
         * constructor
         */
        public OID_NODE()
        {
            for(int i =0; i<MAX_INDEX_LEN;i++)
            {
                idx[i] = new INDEX();
            }
        }
    }
    
    short setOidInfo(OID_INFO oidInfo,short oid_info_val)
    {
        switch(oidInfo)
        {
            case bIsDistantSibling:
                oid_info_val |= (1<<0);
                break;
            case bIsConstant:
                oid_info_val |= (1<<1);
                break;
            case bIsSequence:
                oid_info_val |= (1<<2);
                break;
            case bIsSibling:
                oid_info_val |= (1<<3);
                break;
            case bIsParent:
                oid_info_val |= (1<<4);
                break;
            case bIsEditable:
                oid_info_val |= (1<<5);
                break;
            case bIsAgentID:
                oid_info_val |= (1<<6);
                break;
            case bIsIDPresent:
                oid_info_val |= (1<<7);
                break;
        }
        return oid_info_val;
    }

    void clearOidInfo(OID_INFO oidInfo,byte oid_info_val)
    {
        switch(oidInfo)
        {
            case bIsDistantSibling:
                oid_info_val &= ~(1<<0);
                break;
            case bIsConstant:
                oid_info_val &= ~(1<<1);
                break;
            case bIsSequence:
                oid_info_val &= ~(1<<2);
                break;
            case bIsSibling:
                oid_info_val &= ~(1<<3);
                break;
            case bIsParent:
                oid_info_val &= ~(1<<4);
                break;
            case bIsEditable:
                oid_info_val &= ~(1<<5);
                break;
            case bIsAgentID:
                oid_info_val &= ~(1<<6);
                break;
            case bIsIDPresent:
                oid_info_val &= ~(1<<7);
                break;

        }
    }

    boolean getOidInfo(OID_INFO oidInfo,short oid_info_val)
    {
        boolean oidInfoVal = true;

        switch(oidInfo)
        {
            case bIsDistantSibling:
                oidInfoVal = (oid_info_val & (1<<0)) != 0;
                break;
            case bIsConstant:
                oidInfoVal = (oid_info_val & (1<<1)) != 0;
                break;
            case bIsSequence:
                oidInfoVal = (oid_info_val & (1<<2)) != 0;
                break;
            case bIsSibling:
                oidInfoVal = (oid_info_val & (1<<3)) != 0;
                break;
            case bIsParent:
                oidInfoVal = (oid_info_val & (1<<4)) != 0;
                break;
            case bIsEditable:
                oidInfoVal = (oid_info_val & (1<<5)) != 0;
                break;
            case bIsAgentID:
                oidInfoVal = (oid_info_val & (1<<6)) != 0;
                break;
            case bIsIDPresent:
                oidInfoVal = (oid_info_val & (1<<7)) != 0;
                break;
        }
        return oidInfoVal;
    }

    // A node with at least one child is called parent.
    int IsThisParent(OID_NODE r)
    {
        return r.pChilds.size();
    }

    // A node is called sibling if there is at least one other node on its right at
    // same level.
    boolean IsThisSibling(OID_NODE r)
    {
        OID_NODE p;

        p = r.pParent;

        if ( p != null)
        {
            for ( int i = 0; i < p.pChilds.size(); i++ )
            {
                if ( p.pChilds.get(i) == r )
                {
                    if ( i < (p.pChilds.size()-1) )
                        return true;
                }
            }
        }
        return false;
    }
    boolean IsValidName(String name)
    {
        int i;
        char c;
        int len=name.length();
        len = len-1;
        i = 0;
        for(;len>=0;len--)
        {
            // OID name should not start with Digit
            if(len == 0)
            {
                if(Character.isDigit(name.charAt(len)))
                    return false;
            }
            if((name.charAt(len)== ' ') ||
               (name.charAt(len)== '+') ||
               (name.charAt(len)== '?') ||
               (name.charAt(len)== '-') ||
               (name.charAt(len)== '.') ||
               (name.charAt(len)== '\\')||
               (name.charAt(len)== '/') == true)
                return false;

        }

       return true;
    }

    DATA_TYPE GetDataType(String type)
    {
        Object dataType = dataTypeInfoMap.get(type);
        if(dataType != null)
            return (DATA_TYPE)dataType;
        else
            return DATA_TYPE.DATA_TYPE_UNKNOWN;
    }

    OID_TYPE GetNodeType(String typeString)
    {
        Object oidType = NodeTypeMap.get(typeString);
        if(oidType != null)
            return (OID_TYPE)oidType;
        else
            return OID_TYPE.OID_TYPE_UNKNOWN;
    }

    OID_ACCESS GetOIDAccess(String accessString)
    {
        Object accessType = AccessTypeMap.get(accessString);
        if(accessType != null)
            return (OID_ACCESS)accessType;
        else
            return OID_ACCESS.OID_ACCESS_UNKNOWN;
    }

    int ParseOIDString(String oidString, byte[] oid, int oidLen)
    {
        //String tempOIDStr;
        //boolean lbEndOfString;
        byte[] adjustedOID = new byte[10];
        //int adjustedOIDLen;
        long tempOID;
        int i=0,j=0;
        //tempOIDStr = oidString;
        oidLen = 0;
        //lbEndOfString = false;
        String delimiters = "[.]";
        String[] tokens = oidString.split(delimiters);
        for(;i < tokens.length;i++)
        {
            int len = tokens[i].length();
            for(;j < len;j++)
            {
                if(Character.isDigit(tokens[i].charAt(j)) == false)
                    return 0;
            }
            j = 0;
            tempOID = Integer.decode(tokens[i]);
            // If number is <= than 127, it need not be adjusted.
            if ( tempOID <= 127 )
            {
                oid[oidLen++] = (byte)tempOID;
                continue;
            }

            // Number is more than 127. In that case, bit 7 would be set to '1'.
            long d = tempOID;
            int tempOidlen = 0;

            do
            {
                if ( d > 127 )
                {
                    if ( tempOidlen == 0 )
                        adjustedOID[tempOidlen] = (byte)(d % 128);
                    else
                        adjustedOID[tempOidlen] = (byte)((d % 128) + 0x80);
                    tempOidlen++;
                }
                else
                    adjustedOID[tempOidlen++] = (byte)(d + 0x80);

                d = d / 128;
            } while( d > 0 );
            while( tempOidlen > 0 )
               oid[oidLen++] = adjustedOID[--tempOidlen];
        }
        return oidLen;
    }


    OID_NODE AddRoot(byte oid)
    {
        //OID_NODE newoidNode ;
        if(mibRoot == null)
        {
            mibRoot = new OID_NODE();
            //assert(mibRoot);

            mibRoot.oid = oid;
            mibRoot.pChilds = new LinkedList<OID_NODE>();
            //assert(mibRoot->pChilds);

            mibRoot.pIndex = new ArrayList<String>();
            //assert(mibRoot->pIndex);
        }
        else
        {
            /// All oid must have same root.
            if ( mibRoot.oid != oid )
                return null;
        }

        mibRoot.id = 0x7fffffff;
        mibRoot.oid_info_val = 0;
        mibRoot.oid_info_val = setOidInfo(OID_INFO.bIsParent,mibRoot.oid_info_val);
        mibRoot.siblingOffset = 0;
        mibRoot.dataLen = 0;
        mibRoot.indexLen = 0;
        mibRoot.indexBytes = 0;
        mibRoot.oidType = OID_TYPE.OID_TYPE_UNKNOWN;
        mibRoot.recordOffset = 0;

        return mibRoot;
    }

    OID_NODE AddChild(OID_NODE root, byte oid, boolean bIsDuplicate)
    {
        OID_NODE t;
        int i;

        bIsDuplicate = false;

        // Given root must be valid.
        //assert(root);
        if(root == null)
            return null;

        // It's child list must be valid too.
        //assert(root->pChilds);
        if(root.pChilds == null)
            return null;

        // Go through each child and see if given node already exists.
        for ( i = 0; i < root.pChilds.size(); i++ )
        {
            // Each item in a child list must be valid item.
            t = root.pChilds.get(i);
            if(t == null)
                return null;

            // Position this node in binary ascending order.  If a node already
            // exists, use that.
            if ( oid < t.oid )
                break;

            else if ( t.oid == oid )
            {
                bIsDuplicate = true;
                return t;
            }
        }

        // New node must be child of non-leaf node, or else addition of this
        // node will non-leaf node parent causing firmware logic to fail.
        if(getOidInfo(OID_INFO.bIsParent,root.oid_info_val) == false)
            return null;

        if (root.pParent !=null && root.pParent.name!= null) // != "" ))
            return null;


        // There is no matching node.  We need to create new one.
        t = new OID_NODE();
        //assert(t);

        // Create empty childs list.
        t.pChilds = new LinkedList<OID_NODE>();
        //assert(t->pChilds);

        // Create empty index list.
        t.pIndex = new ArrayList<String>();
        //assert(t->pIndex);


        t.id = 0x7fffffff;
        t.oid_info_val = 0;
        t.oid_info_val = setOidInfo(OID_INFO.bIsParent,t.oid_info_val);
        //t->oidInfo.bIsParent = TRUE;
        t.siblingOffset = 0;
        t.dataLen = 0;
        t.indexLen = 0;
        t.indexBytes = 0;
        t.oidType = OID_TYPE.OID_TYPE_UNKNOWN;
        t.recordOffset = 0;

        // Assign given oid to this new node.
        t.oid = oid;

        // Remember this child's parent, so that we can determine
        // who are the sibling of this child.
        t.pParent = root;

        // Add new node to given root
        if (i < root.pChilds.size())
            root.pChilds.add(i, t);
        else
            root.pChilds.add(t);
        return t;
    }


    OID_NODE AddNode(String name, DATA_TYPE type, OID_TYPE oidType,OID_ACCESS access, byte[] oidBytes, int oidLen)
    {
        OID_NODE root;
        boolean lbIsDuplicate;

        lbIsDuplicate = false;

        root = AddRoot(oidBytes[0]);
        if ( root == null )
            return null;

        for ( int i = 1;  i < oidLen; i++ )
        {
            root = AddChild(root, oidBytes[i], lbIsDuplicate);
            if ( root == null )
                return null;
        }

        // Very last node i.e.leaf of this OID string must not be duplicate.
        // i.e. There must not have been previous node using same oid string.
        if ( lbIsDuplicate )
            return null;

        // Copy information into this node.
        root.oid_info_val = 0;

        root.dataType = type;
        if ( oidType == OID_TYPE.SEQUENCE )
        {
            root.oid_info_val = setOidInfo(OID_INFO.bIsSequence,root.oid_info_val);
        }


        if ( access == OID_ACCESS.OID_ACCESS_RW )
        {
            root.oid_info_val = setOidInfo(OID_INFO.bIsEditable,root.oid_info_val);
        }

        root.siblingOffset = 0x00;
        root.dataLen = 0x00;
        root.indexLen = 0;
        root.indexBytes = 0;
        root.id = 0x7fffffff;

        root.name = name;
        root.oidType  = oidType;

        return root;
    }


    OID_NODE LookupOIDName(OID_NODE root, String name)
    {
        OID_NODE t = new OID_NODE();

        if ( root == null)
            return null;

        if (root.pChilds != null)
        {
            for ( int i = 0; i < root.pChilds.size(); i++ )
            {
                t = LookupOIDName(root.pChilds.get(i), name);
                if ( t != null )
                    return t;
            }
        }
        if(root.name != null)
        {
            if ( root.name.compareTo(name) == 0 )
                return root;
        }

        return null;
    }
    int intSwap(short val)
    {
        return (val<<8+(val>>8 & 0xff));
    }
    long intSwap(int val)
    {
        byte[] p =new byte[4];
        p[0] = (byte)(val & 0xff);
        p[1] = (byte)(val>>8 & 0xff);
        p[2] = (byte)(val>>16 & 0xff);
        p[3] = (byte)(val>>24 & 0xff);
         return ((long)p[0] << 24) +
           ((long)p[1] << 16) +
           ((long)p[2] << 8) +
            (long)p[3];

    }

    OID_NODE LookupID(OID_NODE r, long id)
    {
        int i;
        OID_NODE t;

        if ( r == null )
            return null;

        for ( i = 0; i < r.pChilds.size(); i++ )
        {
            t = LookupID(r.pChilds.get(i), id);
            if ( t != null )
                return t;
        }

        if ( r.id == id )
            return r;

        return null;
    }

    int ValidateFile(String file)
    {
        File hFile = new File(file);
        if ( mibRoot == null )
        {
            System.out.println(hFile.getName()+": No MIB was created.\n");
            return 1;
        }

        return ValidateMIB(hFile, mibRoot);
    }

    int ValidateMIB(File file, OID_NODE r)
    {
        int i;
        int liResult;
        OID_NODE duplicateIDRec;

        liResult = 0;

        if ( r == null )
            return 1;

        for ( i = 0; i < r.pChilds.size(); i++ )
        {
            liResult += ValidateMIB(file,r.pChilds.get(i));
        }

        // At this point, node must not be parent.
        if ( getOidInfo(OID_INFO.bIsParent,r.oid_info_val))
            return liResult;

        // All dynamic variable must have id assigned to it.
        if ( !getOidInfo(OID_INFO.bIsConstant,r.oid_info_val))
        {
            if ( r.id > MAX_DYNAMIC_ID_SIZE )
            {
                mainMib2bib.ShowError(file, r.line, mib2bib.FILE_ERROR_CODE.OID_DATA_SCOPE_UNKNOWN);
                liResult++;
            }
        }

        // All static variable must have value assigned to it.
        if ( getOidInfo(OID_INFO.bIsConstant,r.oid_info_val) )
        {
            if ( r.dataLen == 0 )
            {
                mainMib2bib.ShowError(file, r.line, mib2bib.FILE_ERROR_CODE.NO_OID_STATIC_VALUE);
                liResult++;
            }
        }
        // All sequence OID must have index defined.
        if ( getOidInfo(OID_INFO.bIsSequence,r.oid_info_val))
        {
            if ( r.indexLen == 0 )
            {
                mainMib2bib.ShowError(file, r.line, mib2bib.FILE_ERROR_CODE.NO_OID_INDEX);
                liResult++;
            }
        }
        return liResult;
    }
    
    boolean ProcessFile(String binFile, String incFile, boolean bOverwrite)
    {
        boolean lbResult = true;
        File hBinOutput = new File(binFile);
        File hIncOutput = new File(incFile);
        MIB_STATISTICS stat = new MIB_STATISTICS();
        FileOutputStream bin_file_output;
        FileOutputStream inc_file_output;
        DataOutputStream data_out;
        BufferedWriter fincout;
        // Make sure that mib is created.
        if ( mibRoot == null )
        {
            System.out.println("No MIB database was created.\n");
            return false;
        }
        
        // Open output file.
        if ( hBinOutput.exists() )
        {
            if ( !bOverwrite )
            {
                System.out.println("Output BIB file " + hBinOutput.getName()+" already exists.\n");
                return false;
            }
        }

        if (hIncOutput.exists())
        {
            if ( !bOverwrite )
            {
                    System.out.println("Output Inc file " + hIncOutput.getName()+" already exists.\n");
                    return false;
            }
        }
        try{
        // Create an output stream to the file.
        bin_file_output = new FileOutputStream (binFile);
        if ( bin_file_output == null )
        {
            System.out.println("Error opening output BIB file " + hBinOutput.getName()+ " for Write.\n");
            return false;
        }
        // Wrap the FileOutputStream with a DataOutputStream
        inc_file_output = new FileOutputStream (incFile);
        data_out = new DataOutputStream (inc_file_output);
        fincout = new BufferedWriter(new OutputStreamWriter(data_out));
        if ( fincout == null )
        {
            bin_file_output.close();
            System.out.println("Error opening output inc file" + hIncOutput.getName()+" for Write.\n");
            return false;
        }
        Date date = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("E MMM dd yyyy hh:mm:ss");
        fincout.write("/*******************************************************************\n" +
            " * FileName: mib.h\r\n"+
            " * Processor:       PIC18,PIC24E, PIC24F, PIC24H, dsPIC30F, dsPIC33F, dsPIC33E,PIC32\r\n" +
            " * Compiler:        Microchip C18, C30, C32\r\n" +
            " * This file was automatically generated on " + sdf.format(date)+ "\r\n"+
            " * by mib2bib utility.\r\n" +
            " * This file contains 'C' defines for dynamic OIDs and AgentID only.\r\n" +
            " * Do not modify this file manually.\r\n" +
            " * Include this file in your application source file that handles SNMP callbacks and TRAP.\r\n" +
            " * \r\n" +
            " * Software License Agreement\r\n" +
            " *\r\n" +
            " * Copyright (C) 2012 Microchip Technology Inc.  All rights\r\n" +
            " * reserved.\r\n" +
            " *\r\n" +
            " * Microchip licenses to you the right to use, modify, copy, and \r\n " +
            " * distribute: \r\n" +
            " * (i)  the Software when embedded on a Microchip microcontroller or \r\n" +
            " *      digital signal controller product (\"Device\") which is \r\n" +
            " *      integrated into Licensee's product; or \r\n" +
            " * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,\r\n" +
            " *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device\r\n" +
            " *		used in conjunction with a Microchip ethernet controller for\r\n" +
            " *		the sole purpose of interfacing with the ethernet controller.\r\n" +
            " *\r\n" +
            " * You should refer to the license agreement accompanying this \r\n" +
            " * Software for additional information regarding your rights and \r\n" +
            " * obligations.\r\n" +
            " *\r\n" +
            " * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED \"AS IS\" WITHOUT\r\n" +
            " * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT\r\n" +
            " * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A \r\n" +
            " * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL\r\n" +
            " * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR\r\n" +
            " * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF\r\n" +
            " * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS\r\n" +
            " * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE \r\n" +
            " * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER \r\n" +
            " * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT\r\n" +
            " * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.\r\n" +
            " *\r\n"+
            " *******************************************************************/\n");

        PreprocessOutput(mibRoot, 0);
        AssignOffset(mibRoot);

        //memset(&stat, 0, sizeof(stat));
        lbResult = GenerateOutput(bin_file_output, fincout, mibRoot, stat);

        if ( stat.totalAgentIDs < 1 )
            System.out.println("Warning: No AgentID was defined.  Without AgentID, TRAP functionality is not available.\n");

        if ( lbResult )
        {
            System.out.println("BIB File Statistics: ");
            System.out.println("    Total Static OIDs         : "+ stat.totalStaticOIDs);
            System.out.println("    Total Static data bytes   : "+ stat.totalDataBytes);
            System.out.println("    Total Dynamic OIDs        : "+stat.totalDynamicOIDs );
            System.out.println("    (" + hIncOutput.getName()+"   entries)");
            System.out.println("    Total Read-Only OIDs      : " + stat.totalReadOnlyOIDs);
            System.out.println("    Total Read-Write OIDs     : "+ stat.totalReadWriteOIDs);
            System.out.println("-------------------------------------------");
            System.out.println("    Total OIDs                : "+ (stat.totalStaticOIDs + stat.totalDynamicOIDs));
            System.out.println("");
            System.out.println("    Total Sequence OIDs       : "+stat.totalSequenceOIDs);
            System.out.println("    Total AgentIDs            : "+ stat.totalAgentIDs);
            System.out.println("===========================================");
            System.out.println("    Total MIB bytes           : "+ stat.totalBytes);
            System.out.println("    (" + hBinOutput.getName()+"   size)\n");
        }

        bin_file_output.close();
        fincout.close();
        }catch(IOException e){}

        DeleteMIBTree(mibRoot);
        return lbResult;
    }
    
    int PreprocessOutput(OID_NODE r, long currentOffset)
    {
       
        r.recordOffset = currentOffset;

        // oid
        currentOffset++;

        // oidInfo
        currentOffset++;

        // Count node id only if this is leaf node with dynamic data.
        if ( IsThisParent(r) == 0 )
        {
            if ( getOidInfo(OID_INFO.bIsIDPresent, r.oid_info_val))
            {
                currentOffset++;
                if(r.id <= 0xFF)
                    currentOffset++;
                else if(r.id > 0xFF && r.id < 0xFFFF)
                {
                    currentOffset++;
                    currentOffset++;
                }
            }
        }
        else
        {
            r.oid_info_val = setOidInfo(OID_INFO.bIsParent, r.oid_info_val);
        }
           

        if ( GetSibling(r) != null )
        {
            r.oid_info_val = setOidInfo(OID_INFO.bIsSibling, r.oid_info_val);
            currentOffset += 2;
        }
        // Only leaf node with no sibling may have distant sibling
        else if ( (IsThisParent(r)==0) && (GetDistantSibling(r) != null) )
        {
            r.oid_info_val= setOidInfo(OID_INFO.bIsDistantSibling, r.oid_info_val);
            currentOffset += 2;
        }

        // Only leaf node has data type byte.
        if ( getOidInfo(OID_INFO.bIsParent, r.oid_info_val)== false)
            // dataType
            currentOffset++;

        if ( r.dataLen > 0)
        {
            // dataLen
            currentOffset++;

            // Actual data bytes
            currentOffset += r.dataLen;
        }

        if ( r.indexBytes > 0)
        {
            // IndexCount
            currentOffset++;

            // Actual Index bytes
            currentOffset += r.indexBytes;
        }

        for ( int i =0; i < r.pChilds.size(); i++ )
            currentOffset = PreprocessOutput(r.pChilds.get(i), currentOffset);

        return (int)currentOffset;
    }

long GetDistantSiblingOffset(OID_NODE n)
{
    OID_NODE s;

    s = GetDistantSibling(n);
    if ( s != null )
        return s.recordOffset;

    else
        return 0;
}

OID_NODE GetFirstChildLeaf(OID_NODE n)
{
    if ( n.pChilds.size() > 0 )
        return GetFirstChildLeaf(n.pChilds.get(0));

    return n;
}



OID_NODE GetDistantSibling(OID_NODE n)
{
    OID_NODE p;
    OID_NODE s;

    p = n.pParent;

    if ( p == null )
        return null;

    s = GetSibling(p);
    if ( s == null )
    {
        //p = p->pParent;
        //if ( p )
            return GetDistantSibling(p);
        //else
        //    return NULL;
    }
    else if ( getOidInfo(OID_INFO.bIsParent, s.oid_info_val))
        return GetFirstChildLeaf(s);

   return s;
}

long GetSiblingOffset(OID_NODE n)
{
    OID_NODE s;

    s = GetSibling(n);
    if ( s != null )
        return s.recordOffset;
    else
        return 0;
}

OID_NODE GetSibling(OID_NODE n)
{
    OID_NODE p;
    boolean lbLocated;

    p = n.pParent;
    if ( p == null )
        return null;

    lbLocated = false;

    for ( int i = 0; i < p.pChilds.size(); i++ )
    {
        if (p.pChilds.get(i) == n )
            lbLocated = true;
        else if ( lbLocated )
            return p.pChilds.get(i);
    }
    return null;
}

boolean AssignOffset(OID_NODE r)
{
    int i;
    OID_NODE matchingIndex;
    boolean lbReturn;

    
    if ( getOidInfo(OID_INFO.bIsSibling, r.oid_info_val))
    {
        r.siblingOffset = GetSiblingOffset(r);
        if ( r.siblingOffset > 0xffff )
            return false;
    }

    if ( getOidInfo(OID_INFO.bIsDistantSibling, r.oid_info_val))
    {
        r.distantSiblingOffset = GetDistantSiblingOffset(r);
        if ( r.distantSiblingOffset > 0xffff )
            return false;
    }

    for ( i = 0; i < r.pChilds.size(); i++ )
    {
        lbReturn = AssignOffset(r.pChilds.get(i));
        if ( lbReturn == false )
            return false;
    }
    return true;
}


boolean GenerateOutput( FileOutputStream hBinOutput, BufferedWriter hIncOutput,
                            OID_NODE r, MIB_STATISTICS stat)
{
    byte[] oidRecord = new byte[512];
    int index;
    int i;
    //int bytesWritten=0;
    boolean lbReturn;

    //assert(r);

    index = 0;

    oidRecord[index++] = r.oid;
    oidRecord[index++] = (byte)r.oid_info_val;
try{
        // Insert oid only, if this is a leaf oid with dynamic data.
    if ( r.pChilds.size() == 0 && getOidInfo(OID_INFO.bIsIDPresent, r.oid_info_val))
    {

        hIncOutput.write("#define " + r.name.toString()+"      " +r.id);

        if ( getOidInfo(OID_INFO.bIsAgentID, r.oid_info_val))
        {
            stat.totalAgentIDs++;
            hIncOutput.write("\t\t// This is an Agent ID for use in SNMPNotify() only.\n");
        }
        else
        {
            stat.totalDynamicOIDs++;
            hIncOutput.write( "\t\t\t//" + r.oidASCIIString.toString() + " :  ");
            

            if ( getOidInfo(OID_INFO.bIsEditable, r.oid_info_val))
            {
                stat.totalReadWriteOIDs++;
                hIncOutput.write("READWRITE ");
            }
            else
            {
                stat.totalReadOnlyOIDs++;
                hIncOutput.write("READONLY ");
            }
            hIncOutput.write(r.dataTypeString + ".\n");
        }

        if(r.id <= 0xFF)
        {
            oidRecord[index++] = (byte)1;
            oidRecord[index++] = (byte)r.id;
        }
        else if(r.id > 0xFF && r.id < 0xFFFF)
        {
            oidRecord[index++] = (byte)2;
            for (int j = 0; j < 2; j++)
            {
                int offset = (1 - j) * 8;
                //matchingNode.data[j] =
                //byte a = (byte)((matchingNode.Byte >> (byte)offset) & (byte)0xff);
                oidRecord[index++] = (byte)((r.id >> (byte)offset) & (byte)0xff);
            }
        }
    }

    if ( getOidInfo(OID_INFO.bIsSibling,r.oid_info_val))
    {
        oidRecord[index++] = (byte)(r.siblingOffset & 0xff);
        oidRecord[index++] = (byte)((r.siblingOffset >> 8) & 0xff);
    }

    if ( getOidInfo(OID_INFO.bIsDistantSibling, r.oid_info_val))
    {
        oidRecord[index++] = (byte)(r.distantSiblingOffset & 0xff);
        oidRecord[index++] = (byte)((r.distantSiblingOffset >> 8) & 0xff);
    }

    // Only leaf oid has dataType byte.
    if (getOidInfo(OID_INFO.bIsParent,r.oid_info_val) == false)
    {
            Byte test=dataTypeMapWithVal.get(r.dataType);
            oidRecord[index++] = test.byteValue(); //(byte)dataTypeMapWithVal.get((DATA_TYPE)r.dataType);        
    }

    if ( getOidInfo(OID_INFO.bIsConstant, r.oid_info_val))
    {
        stat.totalStaticOIDs++;

        oidRecord[index++] = (byte)r.dataLen;
        for ( i = 0; i < r.dataLen; i++ )
        {
            stat.totalDataBytes++;
            oidRecord[index++] = r.data[i];
        }
    }

    if ( getOidInfo(OID_INFO.bIsSequence, r.oid_info_val))
    {
        stat.totalSequenceOIDs++;

        oidRecord[index++] = (byte)r.indexLen;

        for ( i = 0; i < r.indexLen; i++ )
        {
            oidRecord[index++] = (byte)r.idx[i].indexType;

            if ( r.idx[i].indexType == INDEX_CONSTANT )
                oidRecord[index++] = r.idx[i].constant;

            else
            {
                if(r.idx[i].id <= 0xFF)
                {
                    oidRecord[index++] = 1;
                    oidRecord[index++] = (byte)r.idx[i].id;
                }
                else if(r.idx[i].id > 0xFF && r.idx[i].id < 0xFFFF)
                {
                    oidRecord[index++] = 2;                    
                    oidRecord[index++] = (byte)((r.idx[i].id >> 8) & 0xff);
                    oidRecord[index++] = (byte)(r.idx[i].id & 0xff);
                }
                Byte test=dataTypeMapWithVal.get(r.idx[i].dataType);
                oidRecord[index++] = test.byteValue();
            }
        }
    }

    hBinOutput.write(oidRecord, 0, index);

//    bytesWritten = fwrite(oidRecord, sizeof(BYTE), index, hBinOutput);
//    if ( bytesWritten != index )
//        return false;

    stat.totalBytes += index;

    for ( i = 0; i < r.pChilds.size(); i++ )
    {
        lbReturn = GenerateOutput(hBinOutput, hIncOutput,
                                r.pChilds.get(i), stat);
        if ( lbReturn == false )
            return false;
    }

    }catch (IOException e){}
    return true;

}

void DeleteMIBTree(OID_NODE root)
{
    if ( root == null )
        return;

    if ( root.pChilds != null )
    {
        for ( int i = 0; i < root.pChilds.size(); i++ )
        {
            DeleteMIBTree(root.pChilds.get(i));
            if((root.pChilds != null)&& root.pChilds.size()>0)
                root.pChilds.remove(i);
            if((root.pIndex != null) && root.pIndex.size()>0)
                root.pIndex.remove(i);
        }
    }
}
    //boolean exec();
    public List<OID_NODE> newOidNode;
//}

class declareVar //extends SnmpCommand
{
    public boolean lbIsAlreadyWarned = false;
    boolean HandleDeclareVar(List<String> pList, File file, int line)
    {
        OID_NODE r;
        DATA_TYPE dataType;
        OID_TYPE oidType;
        OID_ACCESS oidAccess;
        byte[] oidBytes = new byte[OID_STRING_LEN];
        //AnsiString tempString;
        int oidLen=0;
        String temp;
       


        // Make sure that we only have required number of parameters.
        if ( pList.size() != 6 )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.DEFINE_INVALID_ARGS);
            return false;
        }

        // Is this a valid name?
        if ( IsValidName(pList.get(1)) == false )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_OID_NAME);
            return false;
        }


        dataType = GetDataType(pList.get(2));
        if ( dataType == DATA_TYPE.DATA_TYPE_UNKNOWN )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.UNKNOWN_DATA_TYPE);
            return false;
        }

        oidType = GetNodeType(pList.get(3));
        if ( oidType == OID_TYPE.OID_TYPE_UNKNOWN )
        {
            mainMib2bib.ShowError(file, line,mib2bib.FILE_ERROR_CODE.UNKNOWN_OID_TYPE);
            return false;
        }

        oidAccess = GetOIDAccess(pList.get(4));
        if ( oidAccess == OID_ACCESS.OID_ACCESS_UNKNOWN )
        {
            mainMib2bib.ShowError(file, line,mib2bib.FILE_ERROR_CODE.UNKNOWN_OID_ACCESS);
            return false;
        }


        // Remember OID string so that we can use that in mib.h file.
        String tempString =pList.get(5);
        oidLen = ParseOIDString(pList.get(5), oidBytes, oidLen);
        if ( oidLen == 0 )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_OID_LEN);
            return false;
        }


        // Is this name already defined?
        if ( LookupOIDName(mibRoot,pList.get(1)) == null)
        {
            r = AddNode(pList.get(1).toString(), dataType, oidType, oidAccess, oidBytes, oidLen);
            if ( r == null )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.ROOT_NOT_SAME_OR_DUPLICATE);
                return false;
            }
        }
        else
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.DUPLICATE_DEFINE);
            return false;
        }

        // Check to see if root node has OID of decimal 43.  This is must if this MIB
        // is of Internet type.  If not, warn user only once.
        if (mibRoot.oid != 43 && !lbIsAlreadyWarned )
        {
            lbIsAlreadyWarned = true;
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.NOT_AN_INTERNET_ROOT);
        }

        // Remember OID string so that we can use that in mib.h file.
        r.oidASCIIString = tempString;

        // Remember data type keyword for later display..
        r.dataTypeString = pList.get(2).toString();

        r.line = line;

        return true;
    }
}
// $DeclareVar(<Name>, <dataType>, <nodeType>, <nodeAccess>, <oid>)


// $DynamicVar(<NodeName>, <NodeID>)
class dynamicVar //extends SnmpCommand
{
    //final int MAX_DYNAMIC_OID_COUNT =  (256);
    public int DynamicOIDCount=0;
    boolean HandleDynamicVar(List<String> pList,File file, int line)
    {
        OID_NODE matchingNode;
        OID_NODE duplicateIDRec;
        long id;
        String intVal;

        // Even before we process this command, warn use if total number of
        // dynamic OIDs are exceeding the allowable limit.

        if ( DynamicOIDCount > MAX_DYNAMIC_ID_SIZE )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.TOO_MANY_DYNAMIC_OIDS);
            return false;
        }

        if ( pList.size() != 3 )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.DYNAMIC_OID_INVALID_ARGS);
            return false;
        }

        matchingNode = LookupOIDName(mibRoot, pList.get(1).toString());
        if ( matchingNode == null )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_NOT_DEFINED);
            return false;
        }

        // Make sure that this OID is not delcared as Static
        if (getOidInfo(OID_INFO.bIsConstant, matchingNode.oid_info_val))
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.ALREADY_STATIC);
            return false;
        }


        // An OID of OID data type cannot be dynamic.
        if ( matchingNode.dataType == DATA_TYPE.OID_VAL )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.DYNAMIC_OID_TYPE);
            return false;
        }

        // This must not be multiple declaration.
        if ( matchingNode.id <= MAX_DYNAMIC_ID_SIZE )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_ID_ASSIGNED);
            return false;
        }

        intVal = pList.get(2).toString().replace(" ","");
        id = Integer.decode(intVal);
        if ( id > MAX_DYNAMIC_ID_SIZE )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_OID_ID);
            return false;
        }

        // This must not be duplicate id.
        duplicateIDRec = LookupID(mibRoot, id);
        if ( duplicateIDRec != null )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_ID_DUPLICATE);
            return false;
        }

        // Remember id.
        matchingNode.id = id;

        // Mark that this node does contain id.
        matchingNode.oid_info_val = setOidInfo(OID_INFO.bIsIDPresent,matchingNode.oid_info_val);

        // Remember number of dynamic OIDs we have processed.
        DynamicOIDCount++;

        return true;
    }
}

// $StaticVar(<NodeName>, <Value>)
class staticVar //extends SnmpCommand
{
    boolean HandleStaticVar(List<String> pList, File file, int line)
    {
        OID_NODE matchingNode;
        long i, j, b1, b2, b3, b4;
        int temp;
        String intVal;
        // Must have at least 3 (2+1) arguments.
        if ( pList.size() < 3 )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
            return false;
        }

        matchingNode = LookupOIDName(mibRoot, pList.get(1).toString());
        if ( matchingNode == null )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_NOT_DEFINED);
            return false;
        }

        // Sequence variable cannot be static
        if ( getOidInfo(OID_INFO.bIsSequence,matchingNode.oid_info_val))
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_SEQUENCE_OID);
            return false;
        }

        // Make sure that this variable is not already defined as dynamic.
        if ( matchingNode.id <= MAX_DYNAMIC_ID_SIZE )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.ALREADY_DYNAMIC);
            return false;
        }


        // One can do StaticVar only once to a given variable.
        if ( getOidInfo(OID_INFO.bIsConstant,matchingNode.oid_info_val))
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_VALUE_ASSIGNED);
            return false;
        }

        // Static variable cannot be READWRITE.
        if (getOidInfo(OID_INFO.bIsEditable,matchingNode.oid_info_val))
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_WITH_READWRITE_ACCESS);
            return false;
        }

        // Based on datatype for this node, decode given data.
        switch ( matchingNode.dataType )
        {
        case INT8_VAL:
            if ( pList.size() != 3 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }
            intVal = pList.get(2).toString().replace(" ","");
            i = Integer.decode(intVal);
            if ( i > 0xff )
                mainMib2bib.ShowWarning(file, line, mib2bib.FILE_ERROR_CODE.VALUE_TRUNCATED);

            matchingNode.Byte = (byte)i;
            matchingNode.dataLen = 1; //sizeof(matchingNode.Byte.);
            for (j = 0; j < matchingNode.dataLen; j++)
            {
                int offset = (int)(matchingNode.dataLen - 1 - j) * 8;
                //matchingNode.data[j] =
                //byte a = (byte)((matchingNode.Byte >> (byte)offset) & (byte)0xff);
                matchingNode.data[(int)j] = (byte)((matchingNode.Byte >> (byte)offset) & (byte)0xff);
            }
            break;

        case INT16_VAL:
            if ( pList.size() != 3 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }
            intVal = pList.get(2).toString().replace(" ","");
            i = Integer.decode(intVal);
            if ( i > 0xffff )
                mainMib2bib.ShowWarning(file, line, mib2bib.FILE_ERROR_CODE.VALUE_TRUNCATED);

            matchingNode.int16 = intSwap((short)i);
            matchingNode.dataLen = 2;
            for (j = 0; j < matchingNode.dataLen; j++)
            {
                int offset = (int)(matchingNode.dataLen - 1 - j) * 8;
                //matchingNode.data[j] =
                //byte a = (byte)((matchingNode.Byte >> (byte)offset) & (byte)0xff);
                matchingNode.data[(int)j] = (byte)((matchingNode.int16 >> (byte)offset) & (byte)0xff);
            }
            break;

        case INT32_VAL:
        case COUNTER32:
        case GAUGE32:
        case TIME_TICKS:
            if ( pList.size() != 3 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }
            intVal = pList.get(2).toString().replace(" ","");
            matchingNode.int32 = intSwap(Integer.decode(intVal));
            matchingNode.dataLen = 4;
            for (j = 0; j < matchingNode.dataLen; j++)
            {
                int offset = (int)(matchingNode.dataLen - 1 - j) * 8;
                //matchingNode.data[j] =
                //byte a = (byte)((matchingNode.Byte >> (byte)offset) & (byte)0xff);
                matchingNode.data[(int)j] = (byte)((matchingNode.int32 >> (byte)offset) & (byte)0xff);
            }
            break;

        case ASCII_STRING:
            if ( pList.size()!= 3 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }
            i = pList.get(2).toString().length();
            if ( i > 127 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.TOO_LONG_STRING);
                return false;
            }
            matchingNode.dataLen = (byte)i;
            matchingNode.octets = pList.get(2).toString().getBytes();
            matchingNode.dataType = DATA_TYPE.OCTET_STRING;
            matchingNode.data = matchingNode.octets;
//            for (j = 0; j < matchingNode.dataLen; j++)
//            {
//                int offset = (int)(matchingNode.dataLen - 1 - j) * 8;
//                //matchingNode.data[j] =
//                //byte a = (byte)((matchingNode.Byte >> (byte)offset) & (byte)0xff);
//                matchingNode.data[(int)j] = (byte)((matchingNode.int16 >> (byte)offset) & (byte)0xff);
//            }
            break;

        case OID_VAL:
            if ( pList.size() != 3 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }
            temp = 0;
            intVal = pList.get(2).toString().replace(" ","");
            temp = ParseOIDString(intVal,matchingNode.oidString, temp);
            if ( temp == 0)
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }
            matchingNode.dataLen = (byte)temp;
            matchingNode.dataType = DATA_TYPE.OID_VAL;
            matchingNode.data = matchingNode.oidString;
            break;


        case IP_ADDRESS:
            if ( pList.size() != 3 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }
            intVal = pList.get(2).toString().replace(" ","");
            matchingNode.dataLen = (byte)intVal.length();
            String delimiters = "[.]";
            String[] ipString = intVal.split(delimiters);
            b1 = Integer.decode(ipString[0]);
            b2 = Integer.decode(ipString[1]);
            b3 = Integer.decode(ipString[2]);
            b4 = Integer.decode(ipString[3]);
            matchingNode.dataLen = (byte)ipString.length;
            if ( matchingNode.dataLen != 4 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_DATA);
                return false;
            }

            if ( b1 > 0xff || b1 < 0 ||
                 b2 > 0xff || b2 < 0 ||
                 b3 > 0xff || b3 < 0 ||
                 b4 > 0xff || b4 < 0 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_DATA);
                return false;
            }


            matchingNode.ipAddress[0] = (byte)b4;
            matchingNode.ipAddress[1] = (byte)b3;
            matchingNode.ipAddress[2] = (byte)b2;
            matchingNode.ipAddress[3] = (byte)b1;
            matchingNode.data = matchingNode.ipAddress;
            break;

        case OCTET_STRING:
            if ( pList.size() < 3 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.STATIC_OID_INVALID_ARGS);
                return false;
            }

            i = pList.size() - 2;
            if ( i > 127 )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.TOO_LONG_STRING);
                return false;
            }

            for ( j = 0, i = 2; i < pList.size(); i++, j++ )
            {
                temp = Integer.decode(pList.get((int)i).toString());
                if ( temp > 0xff )
                {
                    mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_DATA);
                    return false;
                }

                matchingNode.octets[(int)j] = (byte)temp;
            }
            matchingNode.dataLen = (byte)j;
            matchingNode.data = matchingNode.octets;

            break;
        }

        matchingNode.oid_info_val = setOidInfo(OID_INFO.bIsConstant, matchingNode.oid_info_val);
        matchingNode.id = 0x7fffffff;
        return true;
    }
}





// $AgentID(<OIDName>, <id>)
// <OIDName> must be of data type OID, it also must be static
// <id> is a special case where id is assigned to static variable
class agentID //extends SnmpCommand
{
    int GetAgentIDCount(OID_NODE r)
    {
        int i;
        int count;

        count = 0;

        if ( r == null )
            return 0;

        for ( i = 0; i < r.pChilds.size(); i++ )
            count += GetAgentIDCount(r.pChilds.get(i));

        if ( getOidInfo(OID_INFO.bIsAgentID,r.oid_info_val))
            return 1;

        return 0;
    }
    boolean HandleAgentID(List<String> pList, File file, int line)
    {
        OID_NODE matchingNode;
        OID_NODE duplicateIDRec;
        long id;
        String intVal;

        // There must be 3 (2+1) arguments.
        if ( pList.size() != 3 )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.AGENT_ID_INVALID_ARGS);
            return false;
        }

        // Find matching node.
        matchingNode = LookupOIDName(mibRoot, pList.get(1).toString());
        if ( matchingNode == null )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_NOT_DEFINED);
            return false;
        }

        // Make sure that matching node is of data type OID.
        if ( matchingNode.dataType != DATA_TYPE.OID_VAL )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.NOT_OID_DATA_TYPE);
            return false;
        }

        // Make sure that this is a static node with some value assigned.
        if ( (getOidInfo(OID_INFO.bIsConstant, matchingNode.oid_info_val)== false) || (matchingNode.dataLen == 0) )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.NOT_A_CONSTANT_DATA_TYPE);
            return false;
        }

        // Convert id into number.
        intVal = pList.get(2).toString().replace(" ","");
        id = Integer.decode(intVal);
        if ( id > MAX_DYNAMIC_ID_SIZE )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_OID_ID);
            return false;
        }

        // This must not be duplicate id.
        duplicateIDRec = LookupID(mibRoot, id);
        if ( duplicateIDRec != null )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_ID_DUPLICATE);
            return false;
        }

        // Make sure that this is not already declared as an AgentID;
        if ( getOidInfo(OID_INFO.bIsAgentID, matchingNode.oid_info_val))
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.ALREADY_AN_AGENT_ID);
            return false;
        }

        // Only one AgentID is allowed per agent.
        if ( GetAgentIDCount(mibRoot) > 0 )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.AGENT_ID_ALREADY_DEFINED);
            return false;
        }


        // Now that this OID meets all rules, flag it as an AgentID:
        matchingNode.oid_info_val = setOidInfo(OID_INFO.bIsAgentID, matchingNode.oid_info_val);
        
        // Remember its id.
        matchingNode.id = id;

        matchingNode.oid_info_val = setOidInfo(OID_INFO.bIsIDPresent, matchingNode.oid_info_val);
        

        return true;
    }
}

// $SequenceVar(<NodeName>, <Index1>, [<Index2>])
class sequenceVar //extends SnmpCommand
{
    public int MAX_NO_OF_INDEX = 1;
    boolean HandleSequenceVar(List<String> pList, File file, int line)
    {
        OID_NODE matchingNode;
        OID_NODE matchingIndex;
        //OID_NODE.INDEX [] matchingIndexTemp;
        String temp;
        int i;
        String  strVal;

        // In this version, only one index is allowed.
        if ( pList.size() != (MAX_NO_OF_INDEX+2)  )
        {
            mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.SEQUENCE_OID_INVALID_ARGS);
            return false;
        }

        matchingNode = LookupOIDName(mibRoot, pList.get(1).toString());
        if ( matchingNode == null )
        {
            mainMib2bib.ShowError(file, line,  mib2bib.FILE_ERROR_CODE.OID_NOT_DEFINED);
            return false;
        }

        matchingNode.indexLen = 0;
        matchingNode.indexBytes = 0;
        for ( i = 2; i < pList.size(); i++ )
        {
            if ( matchingNode.indexLen >= MAX_INDEX_LEN )
            {
                mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_INDEX_COUNT);
                return false;
            }
            strVal = pList.get(i).toString().replace(" ","");
            matchingIndex = LookupOIDName(mibRoot,strVal);

            matchingNode.oid_info_val = setOidInfo(OID_INFO.bIsSequence, matchingNode.oid_info_val);

            if ( matchingIndex == null)
            {
                // Decided not to support constant index values.
                //if ( !IsNumber(pList->Strings[i].c_str()) )
                {
                    mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.OID_NOT_DEFINED);
                    return false;
                }
            }
            else
            {
                // Index must be of type INT8_VAL.
                if ( matchingIndex.dataType != DATA_TYPE.INT8_VAL )
                {
                    mainMib2bib.ShowError(file, line, mib2bib.FILE_ERROR_CODE.INVALID_INDEX_DEFINED);
                    return false;
                }
                
                //matchingNode.idx[matchingNode.indexLen] =  new snmpComand();
                //OID_NODE.INDEX [] matchingIndexTemp = matchingNode.new INDEX[MAX_INDEX_LEN];
                matchingNode.idx[matchingNode.indexLen].indexType = (byte)INDEX_OID;
                matchingNode.idx[matchingNode.indexLen].id = matchingIndex.id;
                matchingNode.idx[matchingNode.indexLen].dataType = matchingIndex.dataType;

    //#if 0
                //matchingNode.idx[matchingNode.indexLen].indexName = pList.get(i).toString();
                //matchingNode.idx[matchingNode.indexLen].offset = 0;
    //#endif
                // 3 == INDEX_OID+MatchingIndex.id+matchingIndex.dataType
                // 1 == extra byte for the matchingIndex.id size.
                if(matchingIndex.id <= 0xFF)
                {
                    matchingNode.indexBytes +=3+1;
                }
                else if((matchingIndex.id > 0xFF) && (matchingIndex.id < 0xFFFF))
                {
                     // 4 == INDEX_OID+MatchingIndex.id[0]+MatchingIndex.id[1]+matchingIndex.dataType
                     // 1 == extra byte for the matchingIndex.id size.
                    matchingNode.indexBytes +=4+1;
                }
            }
            matchingNode.indexLen++;
        }

        return true;
    }
}
}