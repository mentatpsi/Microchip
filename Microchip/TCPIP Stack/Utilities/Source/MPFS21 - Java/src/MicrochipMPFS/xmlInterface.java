/*********************************************************************
*
*  XML interface and MPFSConfigDetails.xml file creation.
*
**********************************************************************
* FileName:        xmlInterface.java
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

package MicrochipMPFS;


/**
 *
 * @author i00265
 */
import java.util.*;
import java.io.File;
//import java.io.FileNotFoundException;
//import java.io.FileOutputStream;
import java.io.IOException;
import javax.xml.parsers.*;
import javax.xml.transform.Transformer;
//import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
//import org.w3c.dom.Attr;
import org.w3c.dom.DOMException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


public class xmlInterface {
    private String errorMessage;
    //private Document document;
    private DocumentBuilderFactory docFactory;
    private DocumentBuilder docBuilder;
    private Document doc;
    private File xmlOutputFile;
    MainMPFS MPFS2;
    private NodeList currentNodeList;
    private int listCount;
    private int currentIteratorCount;
    private Node nodeIterator;
    private Node currentNode;
    private Vector<String> nodeValueList;
    private Vector<String> nodeNameList;
    private Vector<String> nodeAttrNameList;
    private Vector<String> nodeAttrValueList;

    

    xmlInterface(File XmlOuputFile,javax.swing.JFrame parent)
    {
        try
        {
            docFactory= DocumentBuilderFactory.newInstance();
            docFactory.setIgnoringComments(true);
            docFactory.setNamespaceAware(true);
            docFactory.setValidating(false);
            docBuilder = docFactory.newDocumentBuilder();
            doc = docBuilder.newDocument();
            xmlOutputFile = XmlOuputFile;
            MPFS2 = (MainMPFS)parent;
            nodeNameList = new Vector<String>();
            nodeValueList = new Vector<String>();
            nodeAttrNameList = new Vector<String>();
            nodeAttrValueList = new Vector<String>();
        }
        catch (ParserConfigurationException pce) {
		pce.printStackTrace();
        }
    }

    void defaultSettingXmlFileCreation()
    {
       // FileOutputStream fos;
        Transformer transformer;
        TransformerFactory transformerFactory;
      
        doc = docBuilder.newDocument();
        Element rootElement = doc.createElement("Settings");
        doc.appendChild(rootElement);
// Source directory details ----
//        Element DefaultSourceDirectory = doc.createElement("Setting");
//        DefaultSourceDirectory.setAttribute("Name", "DefaultSourceDirectory");
//        DefaultSourceDirectory.setAttribute("Type", "String");
//        DefaultSourceDirectory.setAttribute("Scope", "User");
//        Element value = doc.createElement("Value");
//        //value.appendChild(doc.createTextNode("C:\\Microchip Solutions\\TCPIP\\Demo App\\WebPages2"));
//        value.appendChild(doc.createTextNode(MPFS2.defaultSourceDirectoryPath));
//        DefaultSourceDirectory.appendChild(value);
//        rootElement.appendChild(DefaultSourceDirectory);

        Element SourceDirectory_new = doc.createElement("Setting");
        //SourceDirectory_new.appendChild(value);
        SourceDirectory_new.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.SOURCE_DIRECTORY_PATH));
        SourceDirectory_new.setAttribute("Type", "String");
        //SourceDirectory_new.setAttribute("Scope", "User");
        Element value = doc.createElement("Value");
        //value.appendChild(doc.createTextNode("..//..//..//TCPIP//Demo App//WebPages2"));
        value.appendChild(doc.createTextNode(MPFS2.sourceDirectoryPath));
        SourceDirectory_new.appendChild(value);
        rootElement.appendChild(SourceDirectory_new);

//        Element SourceDirectory_old = doc.createElement("Setting");
//        //SourceDirectory_new.appendChild(value);
//        SourceDirectory_old.setAttribute("Name", "SourceDirectory_old");
//        SourceDirectory_old.setAttribute("Type", "String");
//        SourceDirectory_old.setAttribute("Scope", "User");
//        value = doc.createElement("Value");
//        //value.appendChild(doc.createTextNode("..//..//..//TCPIP Demo App//WebPages2"));
//        value.appendChild(doc.createTextNode(MPFS2.sourceDirectoryPath_old));
//        SourceDirectory_old.appendChild(value);
//        rootElement.appendChild(SourceDirectory_old);
// Project Directory Path details ----
//        Element DefaultProjectDirectory = doc.createElement("Setting");
//        DefaultProjectDirectory.setAttribute("Name", "DefaultProjectDirectory");
//        DefaultProjectDirectory.setAttribute("Type", "String");
//        DefaultProjectDirectory.setAttribute("Scope", "User");
//        value = doc.createElement("Value");
//        //value.appendChild(doc.createTextNode("C:\\Microchip Solutions\\TCPIP\\Demo App\\WebPages2"));
//        value.appendChild(doc.createTextNode(MPFS2.defaultProjectDirectoryPath));
//        DefaultProjectDirectory.appendChild(value);
//        rootElement.appendChild(DefaultProjectDirectory);

        Element projectDirectory_new = doc.createElement("Setting");
        //SourceDirectory_new.appendChild(value);
        projectDirectory_new.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.PROJECT_DIRECTORY_PATH));
        projectDirectory_new.setAttribute("Type", "String");
        //projectDirectory_new.setAttribute("Scope", "User");
        value = doc.createElement("Value");
        //value.appendChild(doc.createTextNode("..//..//..//TCPIP//Demo App//WebPages2"));
        value.appendChild(doc.createTextNode(MPFS2.projectDirectoryPath));
        projectDirectory_new.appendChild(value);
        rootElement.appendChild(projectDirectory_new);

//        Element projectDirectory_old = doc.createElement("Setting");
//        //SourceDirectory_new.appendChild(value);
//        projectDirectory_old.setAttribute("Name", "ProjectDirectory_old");
//        projectDirectory_old.setAttribute("Type", "String");
//        projectDirectory_old.setAttribute("Scope", "User");
//        value = doc.createElement("Value");
//        //value.appendChild(doc.createTextNode("..//..//..//TCPIP Demo App//WebPages2"));
//        value.appendChild(doc.createTextNode(MPFS2.projectDirectoryPath_old));
//        projectDirectory_old.appendChild(value);
//        rootElement.appendChild(projectDirectory_old);
//Project Bin file Path
//        Element DefaultProjectBinFile = doc.createElement("Setting");
//        DefaultProjectBinFile.setAttribute("Name", "DefaultProjectBinFile");
//        DefaultProjectBinFile.setAttribute("Type", "String");
//        DefaultProjectBinFile.setAttribute("Scope", "User");
//        value = doc.createElement("Value");
//        //value.appendChild(doc.createTextNode("C:\\Microchip Solutions\\TCPIP\\Demo App\\WebPages2"));
//        value.appendChild(doc.createTextNode(MPFS2.defaultProjectBinFilePath));
//        DefaultProjectBinFile.appendChild(value);
//        rootElement.appendChild(DefaultProjectBinFile);

        Element projectBinFile_new = doc.createElement("Setting");
        //SourceDirectory_new.appendChild(value);
        projectBinFile_new.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.PROJECT_BIN_FILE_PATH));
        projectBinFile_new.setAttribute("Type", "String");
        //projectBinFile_new.setAttribute("Scope", "User");
        value = doc.createElement("Value");
        //value.appendChild(doc.createTextNode("..//..//..//TCPIP//Demo App//WebPages2"));
        value.appendChild(doc.createTextNode(MPFS2.projectBinFilePath));
        projectBinFile_new.appendChild(value);
        rootElement.appendChild(projectBinFile_new);

//        Element projectBinFile_old = doc.createElement("Setting");
//        //SourceDirectory_new.appendChild(value);
//        projectBinFile_old.setAttribute("Name", "ProjectBinFile_old");
//        projectBinFile_old.setAttribute("Type", "String");
//        projectBinFile_old.setAttribute("Scope", "User");
//        value = doc.createElement("Value");
//        //value.appendChild(doc.createTextNode("..//..//..//TCPIP Demo App//WebPages2"));
//        value.appendChild(doc.createTextNode(MPFS2.projectBinFilePath_old));
//        projectBinFile_old.appendChild(value);
//        rootElement.appendChild(projectBinFile_old);

// <Setting Name="StartWithDirectory" Type="boolean" Scope="User">
//      <Value>True</Value>
//    </Setting>
        Element radStartDirectory = doc.createElement("Setting");
        radStartDirectory.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.START_DIRECTORY_RAD_BOTTON));
        radStartDirectory.setAttribute("Type","Boolean");
       // radStartDirectory.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("True"));
        radStartDirectory.appendChild(value);
        rootElement.appendChild(radStartDirectory);

//<Setting Name="ImageName" Type="String" Scope="User">
//      <Value>MPFSImg2</Value>
//    </Setting>
        Element generateImageName = doc.createElement("Setting");
        generateImageName.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.IMAGE_NAME));
        generateImageName.setAttribute("Type","String");
       // generateImageName.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("MPFSImg2"));
        generateImageName.appendChild(value);
        rootElement.appendChild(generateImageName);

// <Setting Name="UploadImageAfterGenerate" Type="Boolean" Scope="User">
//      <Value>True</Value>
//    </Setting>
        Element uploadImgAfterGenerate = doc.createElement("Setting");
        uploadImgAfterGenerate.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.UPLOAD_BIN_IMAGE_CHKBOX));
        uploadImgAfterGenerate.setAttribute("Type","Boolean");
        //uploadImgAfterGenerate.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("True"));
        uploadImgAfterGenerate.appendChild(value);
        rootElement.appendChild(uploadImgAfterGenerate);

//<Setting Name="OutputFormat" Type="Byte" Scope="User">
//      <Value>0</Value>
//    </Setting>
        Element outputImageFormat = doc.createElement("Setting");
        outputImageFormat.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.OUTPUT_IMAGE_FORMAT_RAD_BOTTON));
        outputImageFormat.setAttribute("Type","Byte");
        //outputImageFormat.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("0"));
        outputImageFormat.appendChild(value);
        rootElement.appendChild(outputImageFormat);
//    <Setting Name="UploadProtocol" Type="Byte" Scope="User">
//      <Value">1</Value>
//    </Setting>
        Element uploadProtocol = doc.createElement("Setting");
        uploadProtocol.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.UPLOAD_PROTOCOL));
        uploadProtocol.setAttribute("Type","Byte");
        //uploadProtocol.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("1"));
        uploadProtocol.appendChild(value);
        rootElement.appendChild(uploadProtocol);
//    <Setting Name="DynamicFiles" Type="System.String" Scope="User">
//      <Value Profile="(Default)">*.htm, *.html, *.cgi, *.xml</Value>
//    </Setting>
        Element dynamicFiles = doc.createElement("Setting");
        dynamicFiles.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.DYNAMIC_FILES));
        dynamicFiles.setAttribute("Type","String");
        //dynamicFiles.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("*.htm, *.html, *.cgi, *.xml"));
        dynamicFiles.appendChild(value);
        rootElement.appendChild(dynamicFiles);

//    <Setting Name="NoCompressFiles" Type="System.String" Scope="User">
//      <Value Profile="(Default)">*.inc, snmp.bib</Value>
//    </Setting>
        Element noCompressFiles = doc.createElement("Setting");
        noCompressFiles.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.NO_COMPRESS_FILE_LIST));
        noCompressFiles.setAttribute("Type","String");
       // noCompressFiles.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("*.inc, snmp.bib"));
        noCompressFiles.appendChild(value);
        rootElement.appendChild(noCompressFiles);

//<Setting Name="UploadAddress" Type="String" Scope="User">
//      <Value>MCHPBOARD</Value>
//    </Setting>
        Element uploadAddress = doc.createElement("Setting");
        uploadAddress.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.UPLOAD_ADDRESS));
        uploadAddress.setAttribute("Type","String");
       // uploadAddress.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("MCHPBOARD"));
        uploadAddress.appendChild(value);
        rootElement.appendChild(uploadAddress);
//    <Setting Name="UploadPath" Type="String" Scope="User">
//      <Value>mpfsupload</Value>
//    </Setting>
        Element uploadPath = doc.createElement("Setting");
        uploadPath.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.UPLOAD_IMAGE_PATH));
        uploadPath.setAttribute("Type","String");
       // uploadPath.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("mpfsupload"));
        uploadPath.appendChild(value);
        rootElement.appendChild(uploadPath);
//    <Setting Name="UploadUser" Type="String" Scope="User">
//      <Value>admin</Value>
//    </Setting>
        Element uploadUserName = doc.createElement("Setting");
        uploadUserName.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.UPLOAD_USER_NAME));
        uploadUserName.setAttribute("Type","String");
       // uploadUserName.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("admin"));
        uploadUserName.appendChild(value);
        rootElement.appendChild(uploadUserName);
//    <Setting Name="UploadPass" Type="String" Scope="User">
//      <Value>microchip</Value>
//    </Setting>
        Element uploadUserPasswd = doc.createElement("Setting");
        uploadUserPasswd.setAttribute("Name",
                (String)MPFS2.xmlAttrInfoMap.get(MainMPFS.eXmlNodeAttribute.UPLOAD_PASSWORD));
        uploadUserPasswd.setAttribute("Type","String");
       // uploadUserPasswd.setAttribute("Scope","User");
        value = doc.createElement("Value");
        value.appendChild(doc.createTextNode("microchip"));
        uploadUserPasswd.appendChild(value);
        rootElement.appendChild(uploadUserPasswd);

        try
        {
        // write the content into xml file
            //fos = new FileOutputStream(xmlOutputFile);
            transformerFactory = TransformerFactory.newInstance();
            transformer = transformerFactory.newTransformer();
            DOMSource source = new DOMSource(doc);
            StreamResult result = new StreamResult(xmlOutputFile);

            // Output to console for testing
            // StreamResult result = new StreamResult(System.out);

            transformer.transform(source, result);

            //System.out.println("File saved!");
	} 
        catch (TransformerException tfe) 
        {
		tfe.printStackTrace();
        }
//        catch (FileNotFoundException ex)
//        {
//            errorMessage = ex.getMessage();
//        }
    }
// Node Name = " Setting" , Attr =  " DefaultSourceSetting ", Value = " Path "
    boolean modifyExistingXmlFile(String nodeName,String sAttr, String sValue)
    {
        Vector v_AttributeNames, v_AttributeValues;
        //FileOutputStream fos;
        Transformer transformer;
        TransformerFactory transformerFactory;
        
        if(xmlOutputFile.exists() == false)
            return false;
        try
        {
            doc = docBuilder.parse(xmlOutputFile);
        }
        catch (SAXException sae) 
        {
            sae.printStackTrace();
        }
        catch (IOException ioe) 
        {
            ioe.printStackTrace();
	}
        // get the Node element
        if(selectNodes(nodeName)!= true)
            return false;
        try
        {
            while(getNext() == true)
            {
                v_AttributeNames=getNodeAttrNameList();
                v_AttributeValues=getNodeAttrValueList();
                for(int iLoop=0;iLoop<v_AttributeNames.size();iLoop++){
                    if((v_AttributeNames.get(iLoop).toString().compareToIgnoreCase("Name")==0)
                            && (v_AttributeValues.get(iLoop).toString().compareToIgnoreCase(sAttr)==0))
                        if(setChildNode("Value", sValue) != true)
                            return false;
                        else
                        {
                            try
                            {
                            // write the content into xml file
                               // fos = new FileOutputStream(xmlOutputFile);
                                transformerFactory = TransformerFactory.newInstance();
                                transformer = transformerFactory.newTransformer();
                                DOMSource source = new DOMSource(doc);
                                StreamResult result = new StreamResult(xmlOutputFile);

                                // Output to console for testing
                                // StreamResult result = new StreamResult(System.out);

                                transformer.transform(source, result);

                                //System.out.println("File saved!");
                            }
                            catch (TransformerException tfe)
                            {
                                    tfe.printStackTrace();
                            }
                            
                            return true;
                        }
                }
            }
        }catch(Exception e){
          //  JOptionPane.showMessageDialog(null, "processNodeInfo failed", "System Alert:", JOptionPane.INFORMATION_MESSAGE);
            return false;
        }  
       // Node rootSettings = doc.getFirstChild();
        return true;
    }

    public boolean setChildNode(String childNodeName, String childNodeValue)
    {
        try
        {
            NodeList nodeList;
            nodeList = currentNode.getChildNodes();
            for (int jLoop = 0;jLoop<nodeList.getLength();jLoop++)
            {
                if(nodeList.item(jLoop).getNodeType() == nodeList.item(jLoop).ELEMENT_NODE)
                {
                    if(nodeList.item(jLoop).getNodeName().compareToIgnoreCase(childNodeName)==0){
                        Node curChildNode=nodeList.item(jLoop);
                        curChildNode.setTextContent(childNodeValue);
                    }
                }
            }
        }
        catch(DOMException ex)
        {
            errorMessage = ex.getMessage();
            return false;
        }
        return true;  
     }
    public boolean getNext()
    {
        if(currentIteratorCount >= currentNodeList.getLength())
        {
            return false;
        }
        if(currentNodeList.item(currentIteratorCount).getNodeType() == currentNodeList.item(currentIteratorCount).ELEMENT_NODE)
        {
            nodeIterator = currentNodeList.item(currentIteratorCount);
            currentNode = nodeIterator;
            if(nodeIterator.hasChildNodes())
            {
                NodeList nodeList;

                nodeAttrNameList.clear();
                nodeAttrValueList.clear();
                nodeNameList.clear();
                nodeValueList.clear();

                nodeList = nodeIterator.getChildNodes();
                for (int j = 0;j<nodeList.getLength();j++)
                {
                    if(nodeList.item(j).getNodeType() == nodeList.item(j).ELEMENT_NODE)
                    {
                        nodeNameList.add(nodeList.item(j).getNodeName());
                        nodeValueList.add(nodeList.item(j).getTextContent());
                    }
                }
                //Attribute List
                if(nodeIterator.hasAttributes())
                {
                    NamedNodeMap nm = nodeIterator.getAttributes();
                    for (int j = 0;j<nm.getLength();j++)
                    {
                        if(nm.item(j).getNodeType() == nm.item(j).ATTRIBUTE_NODE)
                        {
                            nodeAttrNameList.add(nm.item(j).getNodeName());
                            nodeAttrValueList.add(nm.item(j).getTextContent());
                        }
                    }
                }
                nodeIterator = nodeIterator.getNextSibling();
            }
        }
        currentIteratorCount++;
        return true;
     }
    /**
      getNodeValueList: Returns the Node Value List
      */
     public Vector getNodeValueList()
     {
        return nodeValueList;
     }
/**
      getNodeValueList: Returns the Node Name List
      */
     public Vector getNodeNameList()
     {
        return nodeNameList;
     }
      /**
      getNodeValueList: Returns the Node Attribute Name List
      */
     public Vector getNodeAttrNameList()
     {
        return nodeAttrNameList;
     }

     public Vector getNodeAttrValueList()
     {
        return nodeAttrValueList;
     }
// This method will validate the XML file for a perticular node and collect all the node value and attribute.
     // And update the global variable.
    public boolean validateSettingsNodeInfo(String nodeName)
    {
        Vector v_AttributeNames, v_AttributeValues;
        Vector v_NodeName,v_NodeValue;
        String sName,sValue;
        String sType;
        try
        {
            doc = docBuilder.parse(xmlOutputFile);
        }
        catch (SAXException sae)
        {
            sae.printStackTrace();
        }
        catch (IOException ioe)
        {
            ioe.printStackTrace();
	}
        try
        {
            sName=new String("");
            sValue=new String("");
            sType = new String("");
            if(selectNodes(nodeName) != true)
                return false;
            while(getNext()==true)
            {
                v_AttributeNames  = getNodeAttrNameList();
                v_AttributeValues = getNodeAttrValueList();
                v_NodeName = getNodeNameList();
                v_NodeValue = getNodeValueList();

                // check the number of attributes
                //if(v_AttributeNames.size() > 3)
                  //  return false;
                for(int iLoop=0;iLoop<v_AttributeNames.size();iLoop++){
                    if(v_AttributeNames.get(iLoop).toString().compareToIgnoreCase("Name")==0){
                        sValue=v_AttributeValues.get(iLoop).toString();
                    }
                   
                    // check First attribute of the Node  tag "Setting" 
                    if(sValue.equals(MPFS2.XMLNodeSettingAttribute[currentIteratorCount-1]) != true)
                        return false;

                    if(v_AttributeNames.get(iLoop).toString().compareToIgnoreCase("Type")==0){
                        sType = v_AttributeValues.get(iLoop).toString();
                    
                        if((sType.equals("String") != true) && (sType.equals("Byte") != true)&&
                            (sType.equals("Boolean") != true))
                            return false;
                    }
                }
            }
            return true;
        }catch(Exception e){
            //JOptionPane.showMessageDialog(null, "processNodeInfo failed", "System Alert:", JOptionPane.INFORMATION_MESSAGE);
            return false;
        }
       // return true;
    }

    // This method will validate the XML file for a perticular node and collect all the node value and attribute.
     // And update the global variable.
    public boolean loadSettingsNodeInfo(String nodeName)
    {
        Vector v_AttributeNames, v_AttributeValues;
        Vector v_NodeName,v_NodeValue;
        String sName,sValue;
        String sType;
        int nIteration = 0;
        boolean nodeValue = true;
        int bNodeValue=1;
        int iLoop = 0;
        try
        {
            doc = docBuilder.parse(xmlOutputFile);
        }
        catch (SAXException sae)
        {
            sae.printStackTrace();
        }
        catch (IOException ioe)
        {
            ioe.printStackTrace();
	}
        try
        {
            sName=new String("");
            sValue=new String("");
            sType = new String("");
            if(selectNodes(nodeName) != true)
                return false;
            while(getNext()==true)
            {
                v_AttributeNames  = getNodeAttrNameList();
                v_AttributeValues = getNodeAttrValueList();
                v_NodeName = getNodeNameList();
                v_NodeValue = getNodeValueList();
                // Attribute name
                sName = MPFS2.XMLNodeSettingAttribute[nIteration];
                sType = v_AttributeValues.get(iLoop+1).toString();
                sValue = v_NodeValue.get(iLoop).toString();
                switch(nIteration)
                {
                    case 0 : //SOURCE_DIRECTORY_PATH
                        if(sType.contains("String"))
                            MPFS2.sourceDirectoryPath = v_NodeValue.get(iLoop).toString();
                        else
                            return false;
                        break;
                    case 1: //PROJECT_DIRECTORY_PATH
                        if(sType.contains("String"))
                            MPFS2.projectDirectoryPath = v_NodeValue.get(iLoop).toString();
                        else
                            return false;
                        break;
                    case 2:  //PROJECT_BIN_FILE_PATH
                        if(sType.contains("String"))
                            MPFS2.projectBinFilePath = v_NodeValue.get(iLoop).toString();
                        else
                            return false;
                        break;
                    case 3:  //START_DIRECTORY_RAD_BOTTON
                        if(sType.contains("Boolean"))
                        {
                            if(sValue.contains("True"))
                                nodeValue = true;
                            else
                                nodeValue = false;
                            MPFS2.setConfiguration(nodeValue, sValue, bNodeValue, nIteration);//.setConfiguration(nodeValue,"",0,3);
                        }
                        break;
                    case 4:  // IMAGE_NAME
                        if(sType.contains("String"))
                            MPFS2.setConfiguration(nodeValue, sValue, bNodeValue, nIteration);
                        else
                            return false;
                        break;
                    case 5:  // UPLOAD_BIN_IMAGE_CHKBOX
                        if(sType.contains("Boolean"))
                        {
                            if(sValue.contains("True"))
                                nodeValue = true;
                            else
                                nodeValue = false;
                            MPFS2.setConfiguration(nodeValue, sValue, bNodeValue, nIteration);
                        }
                        break;
                    case 6:  // OUTPUT_IMAGE_FORMAT_RAD_BOTTON
                        if(sType.contains("Byte"))
                        {
                            bNodeValue = Integer.parseInt(sValue);
                            if(bNodeValue < 0 && bNodeValue > 3)
                                return false;
                            MPFS2.setConfiguration(nodeValue, sValue, bNodeValue, nIteration);
                        }
                        break;
                    case 7:  // UPLOAD_PROTOCOL
                       if(sType.contains("Byte"))
                        {
                            bNodeValue = Integer.parseInt(sValue);
                            if(bNodeValue < 1 && bNodeValue > 2)
                                return false;
                            MPFS2.setConfiguration(nodeValue, sValue, bNodeValue, nIteration);
                        }
                        break;
                    case 8:  // DYNAMIC_FILES
                    case 9: // NO_COMPRESS_FILE_LIST
                    case 10: // UPLOAD_ADDRESS
                    case 11: // UPLOAD_IMAGE_PATH
                    case 12: // UPLOAD_USER_NAME
                    case 13: // UPLOAD_PASSWORD
                        if(sType.contains("String"))
                            MPFS2.setConfiguration(nodeValue, sValue, bNodeValue, nIteration);
                        else
                            return false;
                        break;
                    default :
                        return false;
                }
                nIteration = nIteration+1;               
            }
            return true;
        }catch(Exception e){
            //JOptionPane.showMessageDialog(null, "processNodeInfo failed", "System Alert:", JOptionPane.INFORMATION_MESSAGE);
            return false;
        }
       // return true;
    }
     /**
      selectNodes: User wants to select a node before each operation.This function will
       * load the Node Names,Node Values,Attribute Names,Attribute Values in the Vector
       * initialized using the initialize function.Before each opertion on a node user
       * should call this function.
      */
    // nodeName  == "Setting"
     public boolean selectNodes(String nodeName)
     {
        //nodeNameList.clear();
        //nodeValueList.clear();
        //nodeAttrNameList.clear();
        //nodeAttrValueList.clear();
        currentNodeList = doc.getElementsByTagName(nodeName);
        //currentNode = currentNodeList.item(0);
        listCount = currentNodeList.getLength();
        try
        {
            currentIteratorCount = 0;
        }
        catch(DOMException ex)
        {
            errorMessage = ex.getMessage();
        }
        if(listCount>0)
            return true;
        else
            return false;
    }

}
