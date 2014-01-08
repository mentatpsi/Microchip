/*********************************************************************
*
*  MPFS entry point for both GUI and CLI.Launch for Generate and upload
*
**********************************************************************
* FileName:        MainMPFS.java
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
import javax.swing.*;
import java.util.*;
import java.util.List;
import java.io.*;
import java.net.*;
import sun.misc.*;
import javax.swing.ImageIcon;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import javax.swing.Action;
//import java.text.*;
//import java.lang.*;

public class MainMPFS extends javax.swing.JFrame {
    public enum outputFileFormat
    {
        BIN,
        C18_32,
        ASM30,
        MDD,
    };
    public enum MPFS_OUTPUT_VERSION
    {
       MPFS2,
       MPFS_CLASSIC,
    };

    public enum UPLOAD_PROTOCOL_OPTION
    {
       HTTP,
       FTP,
    };

    public enum DIRECTORY_OPTION
    {
        SOURCE,
        PROJECT,
    };
    public String uploadExceptionString = null;
    public int percen=0;
    public int progressByteCount=0;
    public int uploadFileLength=0;
    public static Thread manageUpload;
    AdvanceSettings advSetting;
    UploadSettings uploadSettings;
    AboutBox aboutBox;
    public boolean generationResult;
    public List<String> generateLog;
    public MPFS_OUTPUT_VERSION outPutVersion = MPFS_OUTPUT_VERSION.MPFS2;
    public UPLOAD_PROTOCOL_OPTION uploadProtocol = UPLOAD_PROTOCOL_OPTION.HTTP;
    public String defaultSourceDirectoryPath =
            "C:\\Microchip Solutions\\TCPIP\\Demo App\\WebPages2";
    public String defaultProjectBinFilePath =
            "C:\\Microchip Solutions\\TCPIP\\Demo App\\MPFSImg2.bin";
    public String defaultProjectDirectoryPath =
             "C:\\Microchip Solutions\\TCPIP\\Demo App";

    public String sourceDirectoryPath =
            "../../../TCPIP/Demo App/WebPages2";
    public String sourceDirectoryPath_old =
            "../../../TCPIP Demo App/WebPages2";
            //"C:\\Microchip Solutions\\TCPIP\\Demo App\\WebPages2";
            //"C:\\Microchip Solutions\\TCPIP Demo App\\WebPages2";

    public String projectBinFilePath =
            "../../../TCPIP/Demo App/MPFSImg2.bin";
    public String projectBinFilePath_old =
            "../../../TCPIP Demo App/MPFSImg2.bin";
    // "C:\\Microchip Solutions\\TCPIP\\Demo App\\MPFSImg2.bin";
    //         "C:\\Microchip Solutions\\TCPIP Demo App\\MPFSImg2.bin";

    public String projectDirectoryPath =
            "../../../TCPIP/Demo App";
    public String projectDirectoryPath_old =
            "../../../TCPIP Demo App";
    //   "C:\\Microchip Solutions\\TCPIP\\Demo App";
    //    "C:\\Microchip Solutions\\TCPIP Demo App";
    final ImageIcon icon = new ImageIcon(getClass().getResource("/Resource/mchpIcon.png"));
    String aboutStr;
    int progressVal = 0;
    int progressVal_temp=0;
    URL url;
    public Properties prop = System.getProperties();

    public String[] XMLNodeName=
    {
        "Setting"
    };
    public String[] XMLNodeSettingAttribute=
    {
    //    "DefaultSourceDirectory",
        "SourceDirectory",
   //     "SourceDirectory_old",
   //     "DefaultProjectDirectory",
        "ProjectDirectory",
   //     "ProjectDirectory_old",
   //     "DefaultProjectBinFile",
        "ProjectBinFile",
   //     "ProjectBinFile_old"
        "StartWithDirectory",
        "ImageName",
        "UploadImageAfterGenerate",
        "OutputFormat",
        "UploadProtocol",
        "DynamicFiles",
        "NoCompressFiles",
        "UploadAddress",
        "UploadPath",
        "UploadUser",
        "UploadPass"

    };
    public enum eXmlNodeAttribute
    {
        SOURCE_DIRECTORY_PATH,
        PROJECT_DIRECTORY_PATH,
        PROJECT_BIN_FILE_PATH,
        START_DIRECTORY_RAD_BOTTON,
        IMAGE_NAME,
        UPLOAD_BIN_IMAGE_CHKBOX,
        OUTPUT_IMAGE_FORMAT_RAD_BOTTON,
        UPLOAD_PROTOCOL,
        DYNAMIC_FILES,
        NO_COMPRESS_FILE_LIST,
        UPLOAD_ADDRESS,
        UPLOAD_IMAGE_PATH,
        UPLOAD_USER_NAME,
        UPLOAD_PASSWORD
    }
    xmlInterface xmlIntrf;
    public Map<eXmlNodeAttribute,String> xmlAttrInfoMap;

    public Map<Byte,outputFileFormat> ouputFormat;

   /*
    * XML interface file to store the Configuration details
    */
   public String xmlOuputfile = "MPFS2SettingDetails.xml";

    /** Creates new form MainMPFS */
    public MainMPFS() {
        //Date date = new Date();
        //SimpleDateFormat sdf = new SimpleDateFormat("MMMMM dd, yyyy");
        //this.setIconImage(icon.getImage());
       
         // XML interface class
        xmlAttrInfoMap = new HashMap<eXmlNodeAttribute,String>();
        xmlAttrInfoMap.put(eXmlNodeAttribute.SOURCE_DIRECTORY_PATH,XMLNodeSettingAttribute[0]);
        xmlAttrInfoMap.put( eXmlNodeAttribute.PROJECT_DIRECTORY_PATH,XMLNodeSettingAttribute[1]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.PROJECT_BIN_FILE_PATH,XMLNodeSettingAttribute[2]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.START_DIRECTORY_RAD_BOTTON,XMLNodeSettingAttribute[3]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.IMAGE_NAME,XMLNodeSettingAttribute[4]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.UPLOAD_BIN_IMAGE_CHKBOX,XMLNodeSettingAttribute[5]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.OUTPUT_IMAGE_FORMAT_RAD_BOTTON,XMLNodeSettingAttribute[6]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.UPLOAD_PROTOCOL,XMLNodeSettingAttribute[7]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.DYNAMIC_FILES,XMLNodeSettingAttribute[8]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.NO_COMPRESS_FILE_LIST,XMLNodeSettingAttribute[9]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.UPLOAD_ADDRESS,XMLNodeSettingAttribute[10]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.UPLOAD_IMAGE_PATH,XMLNodeSettingAttribute[11]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.UPLOAD_USER_NAME,XMLNodeSettingAttribute[12]);
        xmlAttrInfoMap.put(eXmlNodeAttribute.UPLOAD_PASSWORD,XMLNodeSettingAttribute[13]);

        ouputFormat = new HashMap<Byte,outputFileFormat>();
        ouputFormat.put(Byte.valueOf((byte)0),outputFileFormat.BIN);
        ouputFormat.put(Byte.valueOf((byte)1),outputFileFormat.C18_32);
        ouputFormat.put(Byte.valueOf((byte)2),outputFileFormat.ASM30);
        ouputFormat.put(Byte.valueOf((byte)3),outputFileFormat.MDD);

        JDialog.setDefaultLookAndFeelDecorated(true);
        File xmlFile = new File(xmlOuputfile);
        xmlIntrf = new xmlInterface(xmlFile,this);
        if(xmlFile.isFile() && xmlFile.exists())
        {
            //xmlFile.delete();
            //xmlIntrf = new xmlInterface(xmlFile,this);
            for(int iLoop=0;iLoop<XMLNodeName.length;iLoop++)
            {
                //System.out.println("File already present \n");
                if(xmlIntrf.validateSettingsNodeInfo(XMLNodeName[iLoop])!= true)
                {
                   //System.out.println("if File has problem \n");
                    File invalidXmlFile = new File("MPFS2SettingDetails.bak");
                    xmlFile.renameTo(invalidXmlFile);
                    xmlFile.delete();
                    xmlIntrf.defaultSettingXmlFileCreation();
                    JOptionPane.showMessageDialog(null, "Invalid XML File.\n" +
                           "File saved as MPFS2SettingDetails.bak ."); //, "System Alert:"); //, JOptionPane.OK_OPTION,JOptionPane.INFORMATION_MESSAGE);
                   //int response = JOptionPane.showConfirmDialog(null, "Invalid XML File.\n" +
                  //         "File saved as MPFS2SettingDetails.bak .", "System Alert:", JOptionPane.OK_OPTION,JOptionPane.INFORMATION_MESSAGE);
//                   if(response == JOptionPane.YES_OPTION)
//                   {
//                        xmlFile.delete();
//                        xmlIntrf.defaultSettingXmlFileCreation();
//                   }
//                   else
//                   {
//                       return;
//                   }
                }
            }
        }
        else
        {
            //System.out.println("Dafault file is created \n");
            xmlIntrf.defaultSettingXmlFileCreation();
        }
       
        initComponents();
        jProgressBar1.setIndeterminate(false);
        this.setIconImage(icon.getImage());
        toolTipSettings();

        this.setSize(638,520);
        
        //advSetting = new AdvanceSettings(new JFrame(),true,this);
        //uploadSettings =  new UploadSettings(new JFrame(),true,this);
        advSetting = new AdvanceSettings(this,true);
        uploadSettings =  new UploadSettings(this,true);
        aboutBox = new AboutBox(this,true);
        txtUploadPath.setText(uploadSettings.getUploadPathStr());
        aboutStr = "<html>"+"Date June,26 2012"+"<br>";
        String version = "Version MPFS 2.2.1";
        lebelAbout.setText(aboutStr+version+"</html>");
        UIManager.put("Button.defaultButtonFollowsFocus", Boolean.TRUE);
        MainKeyEventActionIntialization();
        xmlIntrf.loadSettingsNodeInfo(XMLNodeName[0]);
        if(radWebDir.isSelected())
        {
            radWebDirActionPerformed(null);
        }
        else
        {
            radPreBuildDirActionPerformed(null);
        }
        File sourceFile=new File(sourceDirectoryPath);
        File projectBinFile = new File(projectBinFilePath);
        File projectDir = new File(projectDirectoryPath);

        File sourceFile_old=new File(sourceDirectoryPath_old);
        File projectBinFile_old = new File(projectBinFilePath_old);
        File projectDir_old = new File(projectDirectoryPath_old);
        // source directory
        try
        {
            sourceDirectoryPath = sourceFile.getCanonicalPath();
            //System.out.println("initial sourceDirectoryPath =" + sourceDirectoryPath);
            if(sourceFile.exists() == false)
            {
                if(sourceFile_old.exists() == false)
                {
                    //System.out.println("incorrect sourceDirectoryPath =" + sourceFile_old.getCanonicalPath());
                    sourceDirectoryPath = defaultSourceDirectoryPath;
                }
                else
                {
                    //System.out.println("correct sourceDirectoryPath =" + sourceFile_old.getCanonicalPath());
                    sourceDirectoryPath = sourceFile_old.getCanonicalPath();
                }
            }
            saveXmlNodeValue(eXmlNodeAttribute.SOURCE_DIRECTORY_PATH,sourceDirectoryPath,0,true);

        }catch(IOException E){
        //System.out.println("Catch sourceDirectoryPath =" + sourceDirectoryPath);
        }

        // project Bin File Path
        try
        {
            projectBinFilePath = projectBinFile.getCanonicalPath();
            if(projectBinFile.exists() == false)
            {
                 if(projectBinFile_old.exists() == false)
                    projectBinFilePath = defaultProjectBinFilePath;
                 else
                 {
                     projectBinFilePath = projectBinFile_old.getCanonicalPath();
                 }
            }
            //xmlIntrf.modifyExistingXmlFile("Setting", "ProjectBinFile", projectBinFilePath);
            saveXmlNodeValue(eXmlNodeAttribute.PROJECT_BIN_FILE_PATH,projectBinFilePath,0,true);
            //System.out.println("projectBinFilePath =" + projectBinFilePath);
        }catch(IOException E){}

        // project directory path
        try
        {
            projectDirectoryPath = projectDir.getCanonicalPath();
            if(projectDir.exists() == false)
            {
               if(projectDir_old.exists() == false)
                    projectDirectoryPath = defaultProjectDirectoryPath;
               else
               {
                   projectDirectoryPath = projectDir_old.getCanonicalPath();
               }
            }
            //xmlIntrf.modifyExistingXmlFile("Setting", "ProjectDirectory", projectDirectoryPath);
            saveXmlNodeValue(eXmlNodeAttribute.PROJECT_DIRECTORY_PATH,projectDirectoryPath,0,true);
            //System.out.println("projectDirectoryPath =" + projectDirectoryPath);
        }catch(IOException E){}
         // Soure directory Image path
        TextSrcDir.setText(sourceDirectoryPath);
        txtProjectDir.setText(projectDirectoryPath);
    }

    /*
     * Get protocol As per the Selection
     * HTTP ==  MPFS2
     * FTP ==  MPFSClassic
     */
    public String GetProtocol()
    {
        // For images being built, base on the output version
        if (radWebDir.isSelected())
        {
            if ((outPutVersion == MPFS_OUTPUT_VERSION.MPFS2)&&
                    (uploadProtocol == UPLOAD_PROTOCOL_OPTION.HTTP))
                return "http";
            else
                return "ftp";
        }
        // For images being uploaded from disk, try to read the file header
        // If file does not exist, default to http
        else
        {
            try
            {
                FileInputStream bin = new FileInputStream(projectBinFilePath);
                if (bin.read() == (byte)'M' && bin.read() == (byte)'P' &&
                    bin.read() == (byte)'F' && bin.read() == (byte)'S' &&
                    bin.read() == (byte)0x02)
                {
                    // Upload an MPFS2 image
                    bin.close();
                    if ((outPutVersion == MPFS_OUTPUT_VERSION.MPFS2)&&
                            (uploadProtocol == UPLOAD_PROTOCOL_OPTION.HTTP))
                        return "http";
                    else
                        return "ftp";
                    //return "http";
                }
                else
                {
                    // Upload an MPFS Classic image
                    bin.close();
                    generateLog.add("Error : Invalid Source file.");
                   //return "ftp";
                    return null;
                }
            }
            catch(IOException e)
            {
                // Will trap if the file did not exist or was unreadable
                return "http";
            }
        }
    }
/*
    public JToolTip createToolTip()
    {
        tip = new JToolTip();
       // tip.setComponent(this);
        tip.setPreferredSize(new Dimension(100,50));
        tip.setBounds(20,5,20, 3);
        tip.repaint();
        return tip;
    }
*/
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {
        bindingGroup = new org.jdesktop.beansbinding.BindingGroup();

        sourceSettingButGrp = new javax.swing.ButtonGroup();
        processOpnButGrp = new javax.swing.ButtonGroup();
        jPanel1 = new javax.swing.JPanel();
        groupLbl1 = new javax.swing.JLabel();
        LblStart = new javax.swing.JLabel();
        LblSrcDir = new javax.swing.JLabel();
        TextSrcDir = new javax.swing.JTextField();
        radWebDir = new javax.swing.JRadioButton();
        radPreBuildDir = new javax.swing.JRadioButton();
        btnSrcBrowse = new javax.swing.JButton();
        jPanel3 = new javax.swing.JPanel();
        groupLbl3 = new javax.swing.JLabel();
        lblProjectDir = new javax.swing.JLabel();
        txtProjectDir = new javax.swing.JTextField();
        btnProjectDir = new javax.swing.JButton();
        lblProjectImg = new javax.swing.JLabel();
        txtProjectImageName = new javax.swing.JTextField();
        lbnImageType = new javax.swing.JLabel();
        jPanel2 = new javax.swing.JPanel();
        groupLbl2 = new javax.swing.JLabel();
        lblOuput = new javax.swing.JLabel();
        lblProcess = new javax.swing.JLabel();
        radBin = new javax.swing.JRadioButton();
        radC18C32 = new javax.swing.JRadioButton();
        radAsm = new javax.swing.JRadioButton();
        radMdd = new javax.swing.JRadioButton();
        btnAdvSetting = new javax.swing.JButton();
        jPanel4 = new javax.swing.JPanel();
        groupLbl4 = new javax.swing.JLabel();
        lblUploadImg = new javax.swing.JLabel();
        chkBoxUpload = new javax.swing.JCheckBox();
        txtUploadPath = new javax.swing.JTextField();
        btnUploadSetting = new javax.swing.JButton();
        jPanel6 = new javax.swing.JPanel();
        btnGenAndUpload = new javax.swing.JButton();
        lebelAbout = new javax.swing.JLabel();
        jProgressBar1 = new javax.swing.JProgressBar();
        lblMessage = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Microchip MPFS Generator");
        setLocationByPlatform(true);

        org.jdesktop.beansbinding.Binding binding = org.jdesktop.beansbinding.Bindings.createAutoBinding(org.jdesktop.beansbinding.AutoBinding.UpdateStrategy.READ_WRITE, this, org.jdesktop.beansbinding.ELProperty.create("false"), this, org.jdesktop.beansbinding.BeanProperty.create("resizable"));
        bindingGroup.addBinding(binding);

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Source Settings", javax.swing.border.TitledBorder.LEFT, javax.swing.border.TitledBorder.TOP, new java.awt.Font("Microsoft Sans Serif", 1, 11))); // NOI18N
        jPanel1.setFont(new java.awt.Font("MS Reference Sans Serif", 0, 11));
        jPanel1.setName("Source Settings"); // NOI18N
        jPanel1.setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        groupLbl1.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 14));
        groupLbl1.setText("1.");
        groupLbl1.setName("SrcSetting"); // NOI18N
        jPanel1.add(groupLbl1, new org.netbeans.lib.awtextra.AbsoluteConstraints(10, 50, -1, -1));

        LblStart.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 12));
        LblStart.setText("Start With:");
        jPanel1.add(LblStart, new org.netbeans.lib.awtextra.AbsoluteConstraints(70, 25, 100, -1));

        LblSrcDir.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 12));
        LblSrcDir.setText("Source Directory:");
        jPanel1.add(LblSrcDir, new org.netbeans.lib.awtextra.AbsoluteConstraints(70, 50, 124, -1));

        TextSrcDir.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        TextSrcDir.setToolTipText("Selects the source file(s) for the remainder of the process.");
        TextSrcDir.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                TextSrcDirActionPerformed(evt);
            }
        });
        jPanel1.add(TextSrcDir, new org.netbeans.lib.awtextra.AbsoluteConstraints(70, 70, 399, -1));

        sourceSettingButGrp.add(radWebDir);
        radWebDir.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        radWebDir.setSelected(true);
        radWebDir.setText("Webpage Directory");
        radWebDir.setToolTipText("Generate a new image from a directory of files.");
        radWebDir.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radWebDirActionPerformed(evt);
            }
        });
        jPanel1.add(radWebDir, new org.netbeans.lib.awtextra.AbsoluteConstraints(220, 20, -1, -1));

        sourceSettingButGrp.add(radPreBuildDir);
        radPreBuildDir.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        radPreBuildDir.setText("Pre-Built MPFS Image");
        radPreBuildDir.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radPreBuildDirActionPerformed(evt);
            }
        });
        jPanel1.add(radPreBuildDir, new org.netbeans.lib.awtextra.AbsoluteConstraints(429, 20, 140, -1));

        btnSrcBrowse.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        btnSrcBrowse.setText("Browse");
        btnSrcBrowse.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSrcBrowseActionPerformed(evt);
            }
        });
        jPanel1.add(btnSrcBrowse, new org.netbeans.lib.awtextra.AbsoluteConstraints(480, 68, 96, -1));

        jPanel3.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Output Files", javax.swing.border.TitledBorder.LEFT, javax.swing.border.TitledBorder.TOP, new java.awt.Font("Microsoft Sans Serif", 1, 11))); // NOI18N
        jPanel3.setFont(new java.awt.Font("MS Reference Sans Serif", 0, 11));
        jPanel3.setName("Source Settings"); // NOI18N
        jPanel3.setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        groupLbl3.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 14));
        groupLbl3.setText("3.");
        jPanel3.add(groupLbl3, new org.netbeans.lib.awtextra.AbsoluteConstraints(6, 31, -1, -1));

        lblProjectDir.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 12));
        lblProjectDir.setText("Project Directory:");
        jPanel3.add(lblProjectDir, new org.netbeans.lib.awtextra.AbsoluteConstraints(75, 20, 127, -1));

        txtProjectDir.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        txtProjectDir.setToolTipText("<html>Select your MPLAB project directory.<br>Your image file will be placed here, along with HTTPPrint.h if applicable.<\\html>");
        jPanel3.add(txtProjectDir, new org.netbeans.lib.awtextra.AbsoluteConstraints(75, 41, 390, -1));

        btnProjectDir.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        btnProjectDir.setText("Browse");
        btnProjectDir.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnProjectDirActionPerformed(evt);
            }
        });
        jPanel3.add(btnProjectDir, new org.netbeans.lib.awtextra.AbsoluteConstraints(475, 38, 100, -1));

        lblProjectImg.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 12));
        lblProjectImg.setText("Image Name:");
        jPanel3.add(lblProjectImg, new org.netbeans.lib.awtextra.AbsoluteConstraints(80, 70, -1, -1));

        txtProjectImageName.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        txtProjectImageName.setText("MPFSImg2");
        txtProjectImageName.setToolTipText("File name for the image you'd like to create.");
        txtProjectImageName.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                txtProjectImageNameActionPerformed(evt);
            }
        });
        txtProjectImageName.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txtProjectImageNameKeyReleased(evt);
            }
        });
        jPanel3.add(txtProjectImageName, new org.netbeans.lib.awtextra.AbsoluteConstraints(179, 68, 164, -1));

        lbnImageType.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        lbnImageType.setText("[.bin]");
        jPanel3.add(lbnImageType, new org.netbeans.lib.awtextra.AbsoluteConstraints(347, 71, 33, -1));

        jPanel2.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Processing Options", javax.swing.border.TitledBorder.LEFT, javax.swing.border.TitledBorder.TOP, new java.awt.Font("Microsoft Sans Serif", 1, 11))); // NOI18N
        jPanel2.setFont(new java.awt.Font("MS Reference Sans Serif", 0, 11));
        jPanel2.setName("Source Settings"); // NOI18N
        jPanel2.setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        groupLbl2.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 14));
        groupLbl2.setText("2.");
        jPanel2.add(groupLbl2, new org.netbeans.lib.awtextra.AbsoluteConstraints(6, 43, -1, -1));

        lblOuput.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 12));
        lblOuput.setText("Output:");
        jPanel2.add(lblOuput, new org.netbeans.lib.awtextra.AbsoluteConstraints(74, 31, -1, -1));

        lblProcess.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 12));
        lblProcess.setText("Processing:");
        jPanel2.add(lblProcess, new org.netbeans.lib.awtextra.AbsoluteConstraints(74, 63, -1, -1));

        processOpnButGrp.add(radBin);
        radBin.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11)); // NOI18N
        radBin.setText("BIN Image");
        radBin.setToolTipText("Output a BIN image to be uploaded to external storage, such as EEPROM or serial Flash.");
        radBin.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radBinActionPerformed(evt);
            }
        });
        jPanel2.add(radBin, new org.netbeans.lib.awtextra.AbsoluteConstraints(150, 30, -1, -1));

        processOpnButGrp.add(radC18C32);
        radC18C32.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11)); // NOI18N
        radC18C32.setSelected(true);
        radC18C32.setText("PIC18/PIC32 Image");
        radC18C32.setToolTipText("Output an image compatible with PIC18/PIC32 to be stored in internal Flash program memory.");
        radC18C32.setName("radC18C30Image"); // NOI18N
        radC18C32.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radC18C32ActionPerformed(evt);
            }
        });
        jPanel2.add(radC18C32, new org.netbeans.lib.awtextra.AbsoluteConstraints(240, 30, -1, -1));

        processOpnButGrp.add(radAsm);
        radAsm.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11)); // NOI18N
        radAsm.setText("PIC24/dsPIC Image");
        radAsm.setToolTipText("Output an image compatible with 16-bit parts to be stored in internal Flash program memory.  Remember to add the generated .S file to your project.");
        radAsm.setName("radASM30Image"); // NOI18N
        radAsm.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radAsmActionPerformed(evt);
            }
        });
        jPanel2.add(radAsm, new org.netbeans.lib.awtextra.AbsoluteConstraints(370, 30, -1, -1));

        processOpnButGrp.add(radMdd);
        radMdd.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        radMdd.setText("MDD");
        radMdd.setToolTipText("Use this option to store pages on drive using MDD file system.");
        radMdd.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                radMddActionPerformed(evt);
            }
        });
        jPanel2.add(radMdd, new org.netbeans.lib.awtextra.AbsoluteConstraints(500, 30, -1, -1));

        btnAdvSetting.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 14));
        btnAdvSetting.setText("Advanced Settings");
        btnAdvSetting.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnAdvSettingActionPerformed(evt);
            }
        });
        jPanel2.add(btnAdvSetting, new org.netbeans.lib.awtextra.AbsoluteConstraints(182, 58, 157, -1));

        jPanel4.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Upload Settings", javax.swing.border.TitledBorder.LEFT, javax.swing.border.TitledBorder.TOP, new java.awt.Font("Microsoft Sans Serif", 1, 11))); // NOI18N
        jPanel4.setFont(new java.awt.Font("MS Reference Sans Serif", 0, 11));
        jPanel4.setName("Source Settings"); // NOI18N
        jPanel4.setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        groupLbl4.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 14));
        groupLbl4.setText("4.");
        jPanel4.add(groupLbl4, new org.netbeans.lib.awtextra.AbsoluteConstraints(6, 31, -1, -1));

        lblUploadImg.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 12));
        lblUploadImg.setText("Upload Image To:");
        jPanel4.add(lblUploadImg, new org.netbeans.lib.awtextra.AbsoluteConstraints(73, 20, -1, -1));

        chkBoxUpload.setSelected(true);
        chkBoxUpload.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                chkBoxUploadStateChanged(evt);
            }
        });
        chkBoxUpload.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chkBoxUploadActionPerformed(evt);
            }
        });
        jPanel4.add(chkBoxUpload, new org.netbeans.lib.awtextra.AbsoluteConstraints(74, 42, 20, 20));

        txtUploadPath.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        txtUploadPath.setToolTipText("Your MPFS image will be uploaded here.  Use the Settings button to modify this destination.");
        txtUploadPath.setFocusable(false);
        txtUploadPath.setMinimumSize(new java.awt.Dimension(400, 400));
        jPanel4.add(txtUploadPath, new org.netbeans.lib.awtextra.AbsoluteConstraints(100, 41, 360, -1));

        btnUploadSetting.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        btnUploadSetting.setText("Settings");
        btnUploadSetting.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnUploadSettingActionPerformed(evt);
            }
        });
        jPanel4.add(btnUploadSetting, new org.netbeans.lib.awtextra.AbsoluteConstraints(470, 37, 100, -1));

        jPanel6.setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        btnGenAndUpload.setText("Generate and Upload");
        btnGenAndUpload.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnGenAndUploadActionPerformed(evt);
            }
        });
        jPanel6.add(btnGenAndUpload, new org.netbeans.lib.awtextra.AbsoluteConstraints(200, 10, 155, -1));

        lebelAbout.setBackground(new java.awt.Color(153, 153, 153));
        lebelAbout.setFont(new java.awt.Font("Microsoft Sans Serif", 1, 11));
        lebelAbout.setForeground(new java.awt.Color(0, 0, 204));
        lebelAbout.setText("<html> date <br> version </html>");
        lebelAbout.setBorder(javax.swing.BorderFactory.createEmptyBorder(1, 1, 1, 1));
        lebelAbout.setCursor(new java.awt.Cursor(java.awt.Cursor.HAND_CURSOR));
        lebelAbout.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                lebelAboutMouseClicked(evt);
            }
        });
        jPanel6.add(lebelAbout, new org.netbeans.lib.awtextra.AbsoluteConstraints(390, 0, 210, 38));

        jProgressBar1.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11));
        jProgressBar1.setIndeterminate(true);
        jProgressBar1.setMaximumSize(new java.awt.Dimension(16, 16));
        jProgressBar1.setMinimumSize(new java.awt.Dimension(16, 16));
        jPanel6.add(jProgressBar1, new org.netbeans.lib.awtextra.AbsoluteConstraints(10, 50, 173, 20));

        lblMessage.setText("[Generator Idle]");
        jPanel6.add(lblMessage, new org.netbeans.lib.awtextra.AbsoluteConstraints(190, 50, 410, 20));

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jPanel6, javax.swing.GroupLayout.Alignment.LEADING, 0, 0, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                        .addComponent(jPanel4, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jPanel3, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jPanel2, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jPanel1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 606, Short.MAX_VALUE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, 103, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, 93, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel4, javax.swing.GroupLayout.PREFERRED_SIZE, 89, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel6, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );

        bindingGroup.bind();

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnGenAndUploadActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnGenAndUploadActionPerformed
        // TODO add your handling code here:
        final MPFS2Lib builder = new MPFS2Lib();
        MPFS2Lib.MPFSClassicBuilder classicBuilder = null;
     
        //AdvanceSettings settings;
        if(radWebDir.isSelected()){
        if(outPutVersion == MPFS_OUTPUT_VERSION.MPFS2)
        {
            //builder ;
            //settings = new AdvanceSettings(this);
            builder.MPFS2Builder(txtProjectDir.getText(),txtProjectImageName.getText());
            builder.DynamicTypes(advSetting.DynVarStr);
            builder.NonGZipTypes(advSetting.NoCompStr);
            lblMessage.setText("Adding source files to image...");
            builder.AddDirectory(TextSrcDir.getText());
        }
        else
        {
            //settings = new AdvanceSettings(this);
            classicBuilder = builder.new MPFSClassicBuilder
                    (txtProjectDir.getText(),txtProjectImageName.getText());
            classicBuilder.ReserveBlock = advSetting.reserveBlock;
            lblMessage.setText("Adding source files to image...");
            classicBuilder.AddDirectory(TextSrcDir.getText(), "");
        }

        // Generate the image
        lblMessage.setText("Generating output image...");
        //jProgressBar1.setIndeterminate(false);
//        jProgressBar1.setStringPainted(true);
//        jProgressBar1.setIndeterminate(true);
//        try{
//        Thread.sleep(500);
//        }catch(Exception ex){}
        jProgressBar1.setStringPainted(true);
        jProgressBar1.setMaximum(120);
        jProgressBar1.setMinimum(0);
        jProgressBar1.setValue((radBin.isSelected() && chkBoxUpload.isSelected()) ? 20 : 70);
        if(outPutVersion == MPFS_OUTPUT_VERSION.MPFS2)
        {
            if (radBin.isSelected())
                generationResult = builder.Generate(MPFS2Lib.MPFSOutputFormat.BIN);//.Generate(MPFSOutputFormat.BIN);
            else if (radC18C32.isSelected())
                generationResult = builder.Generate(MPFS2Lib.MPFSOutputFormat.C18);
            else if (radAsm.isSelected())
                generationResult = builder.Generate(MPFS2Lib.MPFSOutputFormat.ASM30);
            else if (radMdd.isSelected())
                generationResult = builder.Generate(MPFS2Lib.MPFSOutputFormat.MDD);
        }
        else
        {
            if (radBin.isSelected())
                generationResult = classicBuilder.Generate(MPFS2Lib.MPFSOutputFormat.BIN);
            else if (radC18C32.isSelected())
                generationResult = classicBuilder.Generate(MPFS2Lib.MPFSOutputFormat.C18);
            else if (radAsm.isSelected())
                generationResult = classicBuilder.Generate(MPFS2Lib.MPFSOutputFormat.ASM30);
            else if (radMdd.isSelected())
                generationResult = classicBuilder.Generate(MPFS2Lib.MPFSOutputFormat.MDD);
        }
        jProgressBar1.setIndeterminate(false);

        // Indicate full progress for non-uploads
        jProgressBar1.setValue((radBin.isSelected() && chkBoxUpload.isSelected()) ? 20 : 120);
             
        // Retrieve the log
        generateLog = builder.log;

        // Perform the upload if needed
        if (radBin.isSelected() && chkBoxUpload.isSelected() && generationResult)
        {
            manageUpload = new Thread()
            {
                public void run()
                {
                    UploadImage(builder.GeneratedImageFileName());
                }
            };
            manageUpload.start();

            try{
            Thread.sleep(10);
            }catch(InterruptedException ie){
            }
        }
        else
        {
            if (generationResult)
                ShowResultDialog("The MPFS" +/* ((Settings.Default.OutputVersion == 1) ? "" : */"2"/*)*/ +
                    " image was successfully generated.");
            else
                ShowResultDialog("Errors were encountered while generating the MPFS image.");
        }
        jProgressBar1.setValue(0);
        // Show a warning if index has changed
        if (builder.indexUpdated)
        {
            JOptionPane.showMessageDialog(null,"The dynamic variables in your web pages have changed!\n\n" +
                            "Remember to recompile your MPLAB project before continuing\n" +
                            "to ensure that the project is in sync.",
                            "MPFS2 Warning", JOptionPane.INFORMATION_MESSAGE);
        }// This is just an upload
        }
        else
        {
            generationResult = true;
            generateLog = new ArrayList<String>();
            manageUpload = new Thread()
            {
                public void run()
                {
                    UploadImage(projectBinFilePath);
                }
            };
            manageUpload.start();

            try{
            Thread.sleep(10);
            }catch(InterruptedException ie){
            }
            
         }
        //if(manageUpload.isAlive() == true)
          //      manageUpload = null;
      
    }//GEN-LAST:event_btnGenAndUploadActionPerformed

    /// <summary>
    /// Upload a file from disk to the board
    /// </summary>
    /// <param name="filename"></param>
    public PasswordAuthentication pa ;
    public int passwordAuthRetryCnt=0;
    private void UploadImage(String fileName)
    {
        //UploadSettings uploadVal = new UploadSettings(this);
        final String pass ;
        final String userName;      
        passwordAuthRetryCnt = 0;
      
        File imageFile = new File(fileName);
        if (!imageFile.exists())
        {
            generateLog.add("ERROR: Could not open " + fileName);
            generationResult = false;
            ShowResultDialog("The image could not be uploaded.");
        }
        String protocol = GetProtocol();
        if(protocol == null)
        {
            generateLog.add("ERROR: Could not open " + fileName);
            generationResult = false;
            ShowResultDialog("The image could not be uploaded.");
        }
        uploadFileLength = (int)imageFile.length();
        userName = uploadSettings.getUserName();
        pass = uploadSettings.getPasswordName();
        lblMessage.setText("Contacting device for upload...");
        if (protocol.contains("http") == true )
        {
            generateLog.add("Uploading MPFS2 image: " + imageFile.length() + " bytes");
            httpImageUpload(userName,pass,uploadSettings.getIpAddress(),
                    uploadSettings.getUploadPath(),imageFile,protocol);
        }
        else
        {
            generateLog.add("Uploading MPFS2 image: " + imageFile.length() + " bytes");
            ftpImageUpload(userName,pass,uploadSettings.getIpAddress(),
                    uploadSettings.getUploadPath(),imageFile,protocol);
        }
        
    }

    /*
     * upload Image using HTTP protocol
     * includes HTTP client header
     */
   void httpImageUpload(final String userName , final String pass,
           String IPAddr, String uploadPath,File imageFile,
           String protocol)
   {
       String urlPath;
       int byteRead;
        try{
             
        if (userName.length() > 0)
        {
            Authenticator.setDefault(new Authenticator() {
            protected PasswordAuthentication getPasswordAuthentication() {
                if(passwordAuthRetryCnt>0)
                    return null;
                passwordAuthRetryCnt++;
                return new PasswordAuthentication(userName, pass.toCharArray());
            }
            });            
        }
        lblMessage.setText("Contacting device for upload...");
        String boundary = Long.toString(System.currentTimeMillis(), 16);
        String auth = userName+":"+pass;
        urlPath = protocol + "://" + IPAddr + ":80/" + uploadPath;
        URL url = new URL(urlPath);
        URLConnection conn = url.openConnection();
        HttpURLConnection  uConn = (HttpURLConnection)conn;
       
        uConn.setRequestProperty("Authorization", "Basic " + new BASE64Encoder().encode(auth.getBytes()));
        uConn.setDoOutput(true);
        uConn.setDoInput(true);
        uConn.setRequestProperty("MIME-version", "1.0");
        uConn.setRequestProperty("Content-Type", "multipart/form-data;  boundary=" + boundary);
        uConn.setRequestProperty("Expect","100-continue");
        uConn.setRequestProperty("Accept","");
        HttpURLConnection.setFollowRedirects(false);
        uConn.setRequestProperty("Cache-Control", "no-cache");
        uConn.setInstanceFollowRedirects(false);
        //uConn.setRequestProperty("Content-Length",Integer.toString(uploadFileLength));
        conn.connect();
        InputStream input = uConn.getInputStream();
//         pa =
//               Authenticator.requestPasswordAuthentication(uploadVal.getIpAddress(),
//               InetAddress.getByName(uploadVal.getIpAddress()), 80, "http",null,null);//,"", "ntlm");
//
//         if (pa != null)
//         {
//             System.out.println(pa.getUserName());
//             System.out.println(pa.getPassword());
//         }
//         else
//         {
//            System.out.println("PasswordAuthentication was null !");
//            //generateLog.add
//         }
         input.close();
         uConn.disconnect();
         if(passwordAuthRetryCnt > 1)
         {  
            generationResult = false;
            generateLog.add("ERROR: Could not contact remote device for upload.");
            generateLog.add("ERROR: " + "Authentication failure");
            ShowResultDialog("The MPFS image could not be uploaded.");
            return;
         }
      
        // to avoid java.net.ProtocolException:
        //Cannot write output after reading input.
        urlPath = protocol + "://" + IPAddr + "/"+uploadPath;
        int totalNumberOfBytes=0;
        String initialBoundaryStr = "-------------------"+boundary+"\r\n";
        String contentDisposition = "Content-Disposition: form-data; name=\"file\"; filename=\""
                                + imageFile.getName() + "\"\r\n";
        String contentType = "Content-Type: application/octet-stream\r\n";
        String newLine =  "\r\n";
        String endBoundaryStr ="\r\n"+"-----------------------"+boundary+"--\r\n";
        totalNumberOfBytes = initialBoundaryStr.length()
                + contentDisposition.length()
                + contentType.length()
                + newLine.length()
                + endBoundaryStr.length();
        URL url1 = new URL(urlPath);
        HttpURLConnection uConn1 = (HttpURLConnection)url1.openConnection();
        uConn1.setFixedLengthStreamingMode(uploadFileLength+totalNumberOfBytes);//.setChunkedStreamingMode(1024);
        //uConn1.setChunkedStreamingMode(0);
        uConn1.setRequestProperty("Authorization", "Basic " +
                new BASE64Encoder().encode(auth.getBytes()));
        uConn1.setDoOutput(true);
        uConn1.setDoInput(true);
        uConn1.setRequestProperty("MIME-version", "1.0");
        uConn1.setRequestProperty("Content-Type", "multipart/form-data;  boundary=" + boundary);
        uConn1.setRequestProperty("Expect","100-continue");
        uConn1.setRequestProperty("Accept","");
        uConn1.setRequestProperty("Cache-Control", "no-cache");
        //uConn1.setRequestProperty("Content-Length",Integer.toString(uploadFileLength+2048));
        
        lblMessage.setText("Waiting for upload to complete...");
        FileInputStream inputFile = new FileInputStream(imageFile);
        DataInputStream in = new DataInputStream(inputFile);
        //uConn1.getInputStream();
        DataOutputStream outStream = new DataOutputStream( uConn1.getOutputStream() );
        //System.out.println("initial outputstream Size:"+outStream.size());
        
        outStream.write(initialBoundaryStr.getBytes(),0,initialBoundaryStr.length());
        outStream.write(contentDisposition.getBytes(),0,contentDisposition.length());
        outStream.write(contentType.getBytes(),0,contentType.length());
        outStream.write(newLine.getBytes(),0,newLine.length());
        int bufByte = 10000;
        jProgressBar1.setStringPainted(false);
        jProgressBar1.setMaximum(100);
        jProgressBar1.setMinimum(0);
        jProgressBar1.setValue(0);
        if(uploadFileLength < bufByte)
            progressVal = uploadFileLength;
        else
            progressVal = uploadFileLength/bufByte;
        progressVal_temp = progressVal;
        byte[] tempbuf = new byte[bufByte];
        while((byteRead = in.read(tempbuf))!= -1)
        {
            outStream.write(tempbuf,0,byteRead);
            progressByteCount = progressByteCount+byteRead;
            percen++;
            lblMessage.setText("Uploading image (" + progressByteCount + " / " + uploadFileLength + " bytes)");
            jProgressBar1.setValue((100/progressVal)*percen);
        }
        outStream.write(endBoundaryStr.getBytes(),0,endBoundaryStr.length());        
        in.close();       
        outStream.flush();
        outStream.close();
        jProgressBar1.setValue(100);
        //System.out.println("outputstream Size:"+outStream.size());
        //InputStream input1 = uConn1.getInputStream();
        lblMessage.setText("Uploading image (" + uploadFileLength + " / " + uploadFileLength + " bytes)");
        uConn1.disconnect();
        progressByteCount = 0;
        percen = 0;
        lblMessage.setText("Process Complete... See status dialog.");
        }catch(Exception uploadException)
        {
            generationResult = false;
            //System.out.println("\r\nERROR: " + uploadException.getMessage());
            uploadExceptionString = uploadException.getMessage();
            lblMessage.setText("Waiting for upload to complete...");
        }
        java.awt.EventQueue.invokeLater(new Runnable(){
            public void run(){                
            // Display the results
            if ((uploadExceptionString == null) && generationResult)
                ShowResultDialog("The MPFS2 image upload was successfully completed.");
            else
            {
                generationResult = false;
                generateLog.add("ERROR: Could not contact remote device for upload.");
                generateLog.add("ERROR: " + uploadExceptionString);
                ShowResultDialog("The MPFS2 image could not be uploaded.");
            }
             uploadExceptionString = null;
             //jProgressBar1.setValue(0);
            }
          });
   }
/*
 * Upload Image Using FTP protocol
 */
   void ftpImageUpload(final String userName , final String pass,
           String IPAddr, String uploadPath,File imageFile,
           String protocol)
   {
        String urlPath;
        int byteRead;
        try{

        lblMessage.setText("Contacting device for upload...");
       // String boundary = Long.toString(System.currentTimeMillis(), 16);
        String auth = userName+":"+pass;
        urlPath = protocol + "://" + auth+ "@"+IPAddr + "/" + uploadPath;
      
        URL url1 = new URL(urlPath);
        URLConnection uConn1 = url1.openConnection();

        uConn1.setDoOutput(true);
        uConn1.setDoInput(true);

        uConn1.setRequestProperty("Cache-Control", "no-cache");
        lblMessage.setText("Waiting for upload to complete...");
        FileInputStream inputFile = new FileInputStream(imageFile);
        DataInputStream in = new DataInputStream(inputFile);

        DataOutputStream outStream = new DataOutputStream( uConn1.getOutputStream() );
        int bufByte = 10000;
        jProgressBar1.setStringPainted(false);
        jProgressBar1.setMaximum(100);
        jProgressBar1.setMinimum(0);
        jProgressBar1.setValue(0);
        progressVal = uploadFileLength/bufByte;
        progressVal_temp = progressVal;
        byte[] tempbuf = new byte[bufByte];
        while((byteRead = in.read(tempbuf))!= -1)
        {
            outStream.write(tempbuf,0,byteRead);
            progressByteCount = progressByteCount+byteRead;
            percen++;
            lblMessage.setText("Uploading image (" + progressByteCount + " / " + uploadFileLength + " bytes)");
            jProgressBar1.setValue((100/progressVal)*percen);
        }
        in.close();
        outStream.flush();
        outStream.close();
        jProgressBar1.setValue(100);
        System.out.println("outputstream Size:"+outStream.size());
        progressByteCount = 0;
        percen = 0;
        lblMessage.setText("Process Complete... See status dialog.");
        }catch(Exception uploadException)
        {
            lblMessage.setText("Waiting for upload to complete...");
            generationResult = false;
            //System.out.println("\r\nERROR: " + uploadException.getMessage()+
             //       uploadException.hashCode());
            uploadExceptionString = uploadException.getMessage();
        }
        java.awt.EventQueue.invokeLater(new Runnable(){
        public void run(){
              // First, stop the marquee
         lblMessage.setText("Process Complete... See status dialog.");

        // Display the results
        if ((uploadExceptionString == null) && generationResult)
            ShowResultDialog("The MPFS image upload was successfully completed.");
        else
        {
            generationResult = false;
            generateLog.add("ERROR: Could not contact remote device for upload.");
            generateLog.add("The remote server returned an error: " + uploadExceptionString);
            ShowResultDialog("The MPFS image could not be uploaded.");
        }
        uploadExceptionString = null;
        //jProgressBar1.setValue(0);
        }
        });
   }
    private void ShowResultDialog(String message)
    {
        /*
     warningIcon = (IconUIResource) defaults.getIcon("OptionPane.warningIcon");
     infoIcon = (IconUIResource) defaults.getIcon("OptionPane.informationIcon");
     errorIcon = (IconUIResource) defaults.getIcon("OptionPane.errorIcon");
     questionIcon = (IconUIResource) defaults.getIcon("OptionPane.questionIcon");
     */
        LogWindow dlg;// = new LogWindow("OptionPane.warningIcon");
        if (generationResult)
            dlg = new LogWindow(this,true,"OptionPane.informationIcon");
        else
            dlg= new LogWindow(this,true,"OptionPane.errorIcon");
        //dlg.setVisible(true);
        dlg.setListArray(generateLog);
        dlg.setLabelMsg(message);
        dlg.setVisible(true);
        
        lblMessage.setText("[Generator Idle]");
        jProgressBar1.setValue(0);
        
        /*
        String[] utilityLog = new String[generateLog.size()];
        generateLog.toArray(utilityLog);
        if (generationResult)
            JOptionPane.showInputDialog(null, message, "MPFS Utility Log",//JOptionPane.OK_OPTION,
                JOptionPane.INFORMATION_MESSAGE, null,utilityLog,null);
        else
        {
           JOptionPane.showInputDialog(null, message, "MPFS Utility Log",//JOptionPane.OK_OPTION,
                JOptionPane.ERROR_MESSAGE,null,utilityLog,null);
        }
         *
         */
    }
    private void btnSrcBrowseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSrcBrowseActionPerformed
        // TODO add your handling code here:
        String srcPath;
        SourceSettings browseButton = new SourceSettings(this,true,DIRECTORY_OPTION.SOURCE);
        //browseButton.setVisible(true);
        //browseButton.setTitle("MPFS Browse Source Directory");
        if(getRadBotWebPageDirStatus() == true)
        {
           if((srcPath = browseButton.getDirctoryPath()) != null)
            {
                TextSrcDir.setText(srcPath);
                txtProjectDir.setText(browseButton.getParentDirctoryPath());
                sourceDirectoryPath = TextSrcDir.getText();
                projectDirectoryPath = txtProjectDir.getText();
                saveXmlNodeValue(eXmlNodeAttribute.SOURCE_DIRECTORY_PATH,sourceDirectoryPath,0,true);
                saveXmlNodeValue(eXmlNodeAttribute.PROJECT_DIRECTORY_PATH,projectDirectoryPath,0,true);
            }
        }
        else if(getRadBotPreBuildDirStatus()== true)
        {
            if((srcPath = browseButton.getDirctoryPath()) != null)
            {
                TextSrcDir.setText(srcPath);
                projectBinFilePath = TextSrcDir.getText();
                saveXmlNodeValue(eXmlNodeAttribute.PROJECT_BIN_FILE_PATH,projectBinFilePath,0,true);
            }
        }
        txtUploadPath.setText(uploadSettings.getUploadPathStr());
    }//GEN-LAST:event_btnSrcBrowseActionPerformed

    private void radC18C32ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radC18C32ActionPerformed
        // TODO add your handling code here:
        lbnImageType.setText("[.c]");
        jPanel4.setVisible(false);
        btnGenAndUpload.setText("Generate");
        lblProjectImg.setVisible(true);
        txtProjectImageName.setVisible(true);
        lbnImageType.setVisible(true);
         this.setSize(638, 420);
         radPreBuildDir.setEnabled(true);
         btnAdvSetting.setEnabled(true);
         btnAdvSetting.setFocusable(true);
         saveXmlNodeValue(eXmlNodeAttribute.OUTPUT_IMAGE_FORMAT_RAD_BOTTON,null,1,true);
    }//GEN-LAST:event_radC18C32ActionPerformed

    private void radAsmActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radAsmActionPerformed
        // TODO add your handling code here:
         lbnImageType.setText("[.s]");
         jPanel4.setVisible(false);
         btnGenAndUpload.setText("Generate");
         lblProjectImg.setVisible(true);
        txtProjectImageName.setVisible(true);
        lbnImageType.setVisible(true);
        this.setSize(638, 420);
        radPreBuildDir.setEnabled(true);
        btnAdvSetting.setEnabled(true);
        btnAdvSetting.setFocusable(true);
        saveXmlNodeValue(eXmlNodeAttribute.OUTPUT_IMAGE_FORMAT_RAD_BOTTON,null,2,true);
    }//GEN-LAST:event_radAsmActionPerformed

    private void chkBoxUploadActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chkBoxUploadActionPerformed
        // TODO add your handling code here:
        String str = new String();
        str = txtUploadPath.getText();
        txtUploadPath.setText(txtUploadPath.getText());
        if(chkBoxUpload.isSelected())
            saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_BIN_IMAGE_CHKBOX,null,0,true);
        else
            saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_BIN_IMAGE_CHKBOX,null,0,false);
    }//GEN-LAST:event_chkBoxUploadActionPerformed

    private void radWebDirActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radWebDirActionPerformed
        // TODO add your handling code here:
       // java.awt.event.ActionEvent evt;
         jPanel2.setEnabled(true);
         jPanel3.setEnabled(true);
         jPanel2.setVisible(true);
         jPanel3.setVisible(true);
         this.setSize(638, 520);
         TextSrcDir.setText(sourceDirectoryPath);
         if(radBin.isSelected() == true)
         {
            radBinActionPerformed(null);
         }
         else if(radC18C32.isSelected() == true)
         {
             radC18C32ActionPerformed(null);
         }
         else if(radAsm.isSelected() == true)
         {
            radAsmActionPerformed(null);
         }
         else if(radMdd.isSelected() == true)
         {
             radMddActionPerformed(null);
         }
         chkBoxUpload.setVisible(true);
         chkBoxUpload.setEnabled(true);
         if((chkBoxUpload.isSelected()== true) && (radBin.isSelected() == true))
         {
             btnGenAndUpload.setText("Generate and Upload");
         }
         else if(chkBoxUpload.isSelected() == false)
         {
             btnGenAndUpload.setText("Generate");
         }
         txtUploadPath.setText(uploadSettings.getUploadPathStr());
         if(radWebDir.isSelected())
             saveXmlNodeValue(eXmlNodeAttribute.START_DIRECTORY_RAD_BOTTON,null,1,true);
         else
             saveXmlNodeValue(eXmlNodeAttribute.START_DIRECTORY_RAD_BOTTON,null,1,false);
        //this.setSize(null);
    }//GEN-LAST:event_radWebDirActionPerformed

    private void btnProjectDirActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnProjectDirActionPerformed
        String projectPath;
        SourceSettings ProjectDir = new SourceSettings(this,true,DIRECTORY_OPTION.PROJECT);
        //ProjectDir.setVisible(true);
        //ProjectDir.setTitle("MPFS Browse Project Directory");
        //jTextField1.setText(browseButton.getDirctoryPath());
        if((projectPath = ProjectDir.getDirctoryPath()) != null)
        {
            txtProjectDir.setText(projectPath);
            projectDirectoryPath = projectPath;
            saveXmlNodeValue(eXmlNodeAttribute.PROJECT_DIRECTORY_PATH,projectDirectoryPath,0,true);
        }
       //if(ProjectDir.getOutputDirctoryPath() != null)
         //   txtProjectDir.setText(ProjectDir.getParentDirctoryPath());
    }//GEN-LAST:event_btnProjectDirActionPerformed

    private void btnAdvSettingActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnAdvSettingActionPerformed
        // TODO add your handling code here:
        //AdvanceSettings advSetting = new AdvanceSettings(new JFrame(),true,this);
        advSetting.setVisible(true);
        advSetting.defaultAdvanceSetting();
        outPutVersion = MPFS_OUTPUT_VERSION.MPFS2;
        /*
        if(advSetting.getRadBtnMpfs2Status())
        {
            outPutVersion = MPFS_OUTPUT_VERSION.MPFS2;
        }
        else
        {
            outPutVersion = MPFS_OUTPUT_VERSION.MPFS_CLASSIC;
        }
         * 
         */
        txtUploadPath.setText(uploadSettings.getUploadPathStr());
        saveXmlNodeValue(eXmlNodeAttribute.DYNAMIC_FILES,advSetting.getDynamicFileStr(),0,true);
        saveXmlNodeValue(eXmlNodeAttribute.NO_COMPRESS_FILE_LIST,advSetting.getNoCompressionFileStr(),0,true);
    }//GEN-LAST:event_btnAdvSettingActionPerformed

    private void btnUploadSettingActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnUploadSettingActionPerformed
        // TODO add your handling code here
        //UploadSettings uploadSet = new UploadSettings(this);
        uploadSettings.setVisible(true);
        txtUploadPath.setText(uploadSettings.getUploadPathStr());
        if(uploadSettings.getUploadProtocolradBut())
        {
            uploadProtocol = UPLOAD_PROTOCOL_OPTION.HTTP;
        }
        else
        {
            uploadProtocol = UPLOAD_PROTOCOL_OPTION.FTP;
        }
        txtUploadPath.setText(uploadSettings.getUploadPathStr());
        saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_ADDRESS,uploadSettings.getIpAddress(),0,true);
        saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_IMAGE_PATH,uploadSettings.getUploadPath(),0,true);
        saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_USER_NAME,uploadSettings.getUserName(),0,true);
        saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_PASSWORD,uploadSettings.getPasswordName(),0,true);
        if(uploadProtocol == UPLOAD_PROTOCOL_OPTION.HTTP)
            saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_PROTOCOL,null,1,true);
        else
            saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_PROTOCOL,null,2,true);
    }//GEN-LAST:event_btnUploadSettingActionPerformed

    private void chkBoxUploadStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_chkBoxUploadStateChanged
        // TODO add your handling code here:
        if(chkBoxUpload.isSelected())
        {
            txtUploadPath.setEnabled(true);
            btnUploadSetting.setEnabled(true);
            String str = new String();
            str = txtUploadPath.getText();
            txtUploadPath.setText(txtUploadPath.getText());
            btnGenAndUpload.setText("Generate and Upload");
            saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_BIN_IMAGE_CHKBOX,null,0,true);
        }
        else
        {
            txtUploadPath.setEnabled(false);
            btnUploadSetting.setEnabled(false);
            btnGenAndUpload.setText("Generate");
            saveXmlNodeValue(eXmlNodeAttribute.UPLOAD_BIN_IMAGE_CHKBOX,null,0,false);
        }
    }//GEN-LAST:event_chkBoxUploadStateChanged

    private void radPreBuildDirActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radPreBuildDirActionPerformed
        // TODO add your handling code here:
        jPanel4.setVisible(true);
        jPanel2.setEnabled(false);
        jPanel3.setEnabled(false);
        jPanel2.setVisible(false);
        jPanel3.setVisible(false);
        this.setSize(638, 300);
        TextSrcDir.setText(projectBinFilePath);
        //if(jCheckBox1.isEnabled() == false)
            //chkBoxUpload.setVisible(false);
        chkBoxUpload.setEnabled(false);
        chkBoxUpload.setVisible(false);
       // else
         //   jCheckBox1.setVisible(true);
        btnGenAndUpload.setText("Upload");
        txtUploadPath.setText(uploadSettings.getUploadPathStr());
        txtUploadPath.setEnabled(true);
        btnUploadSetting.setEnabled(true);
       if(radWebDir.isSelected())
             saveXmlNodeValue(eXmlNodeAttribute.START_DIRECTORY_RAD_BOTTON,null,1,true);
         else
             saveXmlNodeValue(eXmlNodeAttribute.START_DIRECTORY_RAD_BOTTON,null,1,false);

    }//GEN-LAST:event_radPreBuildDirActionPerformed

    private void radBinActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radBinActionPerformed
        // TODO add your handling code here:
        lbnImageType.setText("[.bin]");
        jPanel4.setVisible(true);
        if(chkBoxUpload.isSelected())
            btnGenAndUpload.setText("Generate and Upload");
        else
             btnGenAndUpload.setText("Generate");
        lblProjectImg.setVisible(true);
        txtProjectImageName.setVisible(true);
        lbnImageType.setVisible(true);
        this.setSize(638, 520);
        radPreBuildDir.setEnabled(true);
        btnAdvSetting.setEnabled(true);
        btnAdvSetting.setFocusable(true);
        saveXmlNodeValue(eXmlNodeAttribute.OUTPUT_IMAGE_FORMAT_RAD_BOTTON,null,0,true);
        txtUploadPath.setText(uploadSettings.getUploadPathStr());
    }//GEN-LAST:event_radBinActionPerformed

    private void radMddActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_radMddActionPerformed
        // TODO add your handling code here:
         //jLabel12.setText("[.mdd]");
         jPanel4.setVisible(false);
         btnGenAndUpload.setText("Generate");
         lblProjectImg.setVisible(false);
         txtProjectImageName.setVisible(false);
         lbnImageType.setVisible(false);
         this.setSize(638, 420);
         radPreBuildDir.setEnabled(false);
         btnAdvSetting.setEnabled(false);
         saveXmlNodeValue(eXmlNodeAttribute.OUTPUT_IMAGE_FORMAT_RAD_BOTTON,null,3,true);

    }//GEN-LAST:event_radMddActionPerformed

    private void lebelAboutMouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_lebelAboutMouseClicked
         aboutBox.setVisible(true);       
    }//GEN-LAST:event_lebelAboutMouseClicked

    private void TextSrcDirActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_TextSrcDirActionPerformed
        // TODO add your handling code here:
        File dir = new File(TextSrcDir.getText());
        String imageName = txtProjectImageName.getText();
        if(imageName.endsWith(".bin")== false)
            imageName = imageName.concat(".bin");
        if(radWebDir.isEnabled())
        {
            txtProjectDir.setText(dir.getParent());
            sourceDirectoryPath = TextSrcDir.getText();
            projectDirectoryPath = txtProjectDir.getText();
            projectBinFilePath = projectDirectoryPath+File.separator+imageName;
            //save the setting
            saveXmlNodeValue(eXmlNodeAttribute.PROJECT_BIN_FILE_PATH,projectBinFilePath,0,true);
            saveXmlNodeValue(eXmlNodeAttribute.SOURCE_DIRECTORY_PATH,sourceDirectoryPath,0,true);
            saveXmlNodeValue(eXmlNodeAttribute.PROJECT_DIRECTORY_PATH,projectDirectoryPath,0,true);
        }
    }//GEN-LAST:event_TextSrcDirActionPerformed

    private void txtProjectImageNameActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_txtProjectImageNameActionPerformed
        // TODO add your handling code here:
        saveXmlNodeValue(eXmlNodeAttribute.IMAGE_NAME,txtProjectImageName.getText(),0,true);
    }//GEN-LAST:event_txtProjectImageNameActionPerformed

    private void txtProjectImageNameKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txtProjectImageNameKeyReleased
        // TODO add your handling code here:
        saveXmlNodeValue(eXmlNodeAttribute.IMAGE_NAME,txtProjectImageName.getText(),0,true);
    }//GEN-LAST:event_txtProjectImageNameKeyReleased

    void toolTipSettings()
    {
        String infoString;
        /*infoString = "<html><body bgcolor=\"#FFFFCC\"><img src=\""+ getClass()
                     .getResource("/Resource/info.gif") + "\"> <b>";*/
        infoString = "<html><body bgcolor=\"#FFFFCC\"> <b>";
      
        infoString = infoString + "<headings> <font size=\"4\">"
                +" MPFS Generator Help </font></headings></b><br>";
        radWebDir.setToolTipText(infoString +
                "Generate a new image from a directory of files." + "</body></html>");

        radPreBuildDir.setToolTipText(infoString +
                "Upload an existing BIN image to a device." + "</body></html>");

        TextSrcDir.setToolTipText(infoString + "Selects the source file(s) " +
                "for the remainder of the process."+"</body></html>");

        radBin.setToolTipText(infoString +"Output a BIN image to be <br>" +
             "uploaded to external storage,<br>such as EEPROM or serial Flash."
             +"</body></html>" );
        radC18C32.setToolTipText(infoString+"Output an image compatible with " +
                "<br>PIC18/PIC32 to be stored in internal Flash <br>progra" +
                "m memory."+"</body></html>");
        radAsm.setToolTipText(infoString+ "Output an image compatible with 16-bit parts" +
                "<br>to be stored in internal Flash program<br> memory. " +
                "Remember to add the <br>generated .S file to your project."
                +"</body></html>");
        radMdd.setToolTipText(infoString+"Use this option to store pages " +
                "<br>on drive using MDD file system."+"</body></html>");

        txtProjectDir.setToolTipText(infoString +"Select your MPLAB project " +
                "directory. <br>Your image file will be placed here, <br>al" +
                    "ong with HTTPPrint.h if applicable."+"</body></html>");
        txtProjectImageName.setToolTipText(infoString+"File name for the image"+
                "<br>you\'d like to create."+"</body></html>");

        txtUploadPath.setToolTipText(infoString+"Your MPFS image will be " +
                "uploaded <br>here.  Use the Settings button to <br>modify thi"+
                    "s destination."+"</body></html>");
        chkBoxUpload.setToolTipText(infoString+"Select this box to upload <br>" +
                "your image upon generation."+"</body></html>");

    }
    public boolean getRadBotWebPageDirStatus()
    {
        return radWebDir.isSelected();
    }
    public boolean getRadBotPreBuildDirStatus()
    {
        return radPreBuildDir.isSelected();
    }

    //xmlIntrf.modifyExistingXmlFile("Setting", "SourceDirectory", sourceDirectoryPath);
    public void saveXmlNodeValue(eXmlNodeAttribute xmlNodeValue,String sNodeValue,int cNodeValue,boolean bNodevalue)
    {
        switch(xmlNodeValue)
        {
            case SOURCE_DIRECTORY_PATH :
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.SOURCE_DIRECTORY_PATH), sNodeValue);
                break;
            case PROJECT_DIRECTORY_PATH:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.PROJECT_DIRECTORY_PATH), sNodeValue);
                break;
            case PROJECT_BIN_FILE_PATH:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.PROJECT_BIN_FILE_PATH), sNodeValue);
                break;
            case START_DIRECTORY_RAD_BOTTON:
                if(bNodevalue == true)
                    sNodeValue = "True";
                else
                    sNodeValue = "False";
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.START_DIRECTORY_RAD_BOTTON), sNodeValue);
                break;
            case IMAGE_NAME:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.IMAGE_NAME), sNodeValue);
                break;
            case UPLOAD_BIN_IMAGE_CHKBOX:
                if(bNodevalue == true)
                    sNodeValue = "True";
                else
                    sNodeValue = "False";
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.UPLOAD_BIN_IMAGE_CHKBOX), sNodeValue);
                break;
            case OUTPUT_IMAGE_FORMAT_RAD_BOTTON:
                sNodeValue = Integer.toString(cNodeValue);
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.OUTPUT_IMAGE_FORMAT_RAD_BOTTON), sNodeValue);
                break;
            case UPLOAD_PROTOCOL:
                sNodeValue = Integer.toString(cNodeValue);
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.UPLOAD_PROTOCOL), sNodeValue);
                break;
            case DYNAMIC_FILES:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.DYNAMIC_FILES), sNodeValue);
                break;
            case NO_COMPRESS_FILE_LIST:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.NO_COMPRESS_FILE_LIST), sNodeValue);
                break;
            case UPLOAD_ADDRESS:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.UPLOAD_ADDRESS), sNodeValue);
                break;
            case UPLOAD_IMAGE_PATH:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.UPLOAD_IMAGE_PATH), sNodeValue);
                break;
            case UPLOAD_USER_NAME:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.UPLOAD_USER_NAME), sNodeValue);
                break;
            case UPLOAD_PASSWORD:
                xmlIntrf.modifyExistingXmlFile(XMLNodeName[0], xmlAttrInfoMap.get(xmlNodeValue.UPLOAD_PASSWORD), sNodeValue);
                break;
        }
    }

    public void setConfiguration(boolean bNodeValue,String sNodeValue,int cNodeValue,int xmlNodeValue)
    {
        switch(xmlNodeValue)
        {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3: //START_DIRECTORY_RAD_BOTTON
                if(bNodeValue)
                {
                    radWebDir.setSelected(true);
                    radPreBuildDir.setSelected(false);
                    //radWebDirActionPerformed(null);
                }
                else
                {
                    radWebDir.setSelected(false);
                    radPreBuildDir.setSelected(true);
                    //radPreBuildDirActionPerformed(null);
                }
                break;
            case 4:  // IMAGE_NAME
                txtProjectImageName.setText(sNodeValue);
                break;
            case 5:  // UPLOAD_BIN_IMAGE_CHKBOX
                if(bNodeValue)
                {
                    chkBoxUpload.setSelected(bNodeValue);
                }
                else
                {
                    chkBoxUpload.setSelected(bNodeValue);
                }
                chkBoxUploadStateChanged(null);
                break;
            case 6:  // OUTPUT_IMAGE_FORMAT_RAD_BOTTON
                if(ouputFormat.get((byte)cNodeValue) == outputFileFormat.BIN)
                {
                    radBin.setSelected(true);
                    //radBinActionPerformed(null);
                }
                else if(ouputFormat.get((byte)cNodeValue) == outputFileFormat.C18_32)
                {
                    radC18C32.setSelected(true);
                   // radC18C32ActionPerformed(null);
                }
                else if(ouputFormat.get((byte)cNodeValue) == outputFileFormat.ASM30)
                {
                    radAsm.setSelected(true);
                    //radAsmActionPerformed(null);
                }
                else if(ouputFormat.get((byte)cNodeValue) == outputFileFormat.MDD)
                {
                    radMdd.setSelected(true);                
                   // radMddActionPerformed(null);
                }
                break;
            case 7:  // UPLOAD_PROTOCOL
                uploadSettings.setUploadProtocolRadBtn(cNodeValue);
                break;
            case 8:  // DYNAMIC_FILES
                advSetting.setDynamicFileStr(sNodeValue);
                break;
            case 9: // NO_COMPRESS_FILE_LIST
                advSetting.setNoCompressionFileStr(sNodeValue);
                break;
            case 10: // UPLOAD_ADDRESS
                uploadSettings.setUploadAddress(sNodeValue);
                break;
            case 11: // UPLOAD_IMAGE_PATH
                uploadSettings.setUploadPath(sNodeValue);
                break;
            case 12: // UPLOAD_USER_NAME
                uploadSettings.setUploadUserName(sNodeValue);
                break;
            case 13: // UPLOAD_PASSWORD
                uploadSettings.setUploadPass(sNodeValue);
                break;
        }
    }
    private void MainKeyEventActionIntialization()
    {
        Action  ESCactionListener = new AbstractAction () {
          public void actionPerformed(ActionEvent actionEvent) {
            setVisible(false);
            System.exit(0);//exit();
          }
        };
        KeyStroke stroke = KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0, true);
        JComponent comp = this.getRootPane();
        comp.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(stroke, "ESCAPE");
        ActionMap actionMap = comp.getActionMap();
        actionMap.put("ESCAPE", ESCactionListener);
        btnAdvSetting.getRootPane().setDefaultButton(btnAdvSetting);
        btnUploadSetting.getRootPane().setDefaultButton(btnUploadSetting);
        btnSrcBrowse.getRootPane().setDefaultButton(btnSrcBrowse);
        btnGenAndUpload.getRootPane().setDefaultButton(btnGenAndUpload);
        btnProjectDir.getRootPane().setDefaultButton(btnProjectDir);

        /* Advance Setting button Action Solves MAC ,
         Linux and Window enter key issue*/
        /*
        Action  advSettingactionListener = new AbstractAction () {
          public void actionPerformed(ActionEvent actionEvent) {
            btnAdvSettingActionPerformed(actionEvent);
          }
        };
        KeyStroke enter_advSetting = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,
                                                                       0, true);
        btnAdvSetting.getInputMap(JButton.WHEN_FOCUSED).put(enter_advSetting,
                                                            "AdvSettingAction");
        actionMap = btnAdvSetting.getActionMap();
        actionMap.put("AdvSettingAction", advSettingactionListener);
        btnAdvSetting.setActionMap(actionMap);
         *
         */
        /* Upload Setting button Action Solves MAC , 
         Linux and Window enter key issue*/
        /*
        Action  uploadSettingactionListener = new AbstractAction () {
          public void actionPerformed(ActionEvent actionEvent) {
            btnUploadSettingActionPerformed(actionEvent);
          }
        };
        KeyStroke enter_updSetting = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,
                                                                       0, true);
        btnUploadSetting.getInputMap(JButton.WHEN_FOCUSED).put(enter_updSetting,
                                                        "UploadSettingAction");
        actionMap = btnUploadSetting.getActionMap();
        actionMap.put("UploadSettingAction", uploadSettingactionListener);
        btnUploadSetting.setActionMap(actionMap);

         *
         */
         /* Source Browse Setting button Action Solves MAC ,
         Linux and Window enter key issue*/
        /*
        Action  SrcBrowseactionListener = new AbstractAction () {
          public void actionPerformed(ActionEvent actionEvent) {
            btnSrcBrowseActionPerformed(actionEvent);
          }
        };
        KeyStroke enter_srcBrowse = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,
                                                                       0, true);
        btnSrcBrowse.getInputMap(JButton.WHEN_FOCUSED).put(enter_srcBrowse,
                                                        "srcBrowseAction");
        actionMap = btnSrcBrowse.getActionMap();
        actionMap.put("srcBrowseAction", SrcBrowseactionListener);
        btnSrcBrowse.setActionMap(actionMap);

         *
         */
         /* Project Dir Browse Setting button Action Solves MAC ,
         Linux and Window enter key issue*/
        /*
        Action  projectBrowseactionListener = new AbstractAction () {
          public void actionPerformed(ActionEvent actionEvent) {
            btnProjectDirActionPerformed(actionEvent);
          }
        };
        KeyStroke enter_projectDir = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,
                                                                       0, true);
        btnProjectDir.getInputMap(JButton.WHEN_FOCUSED).put(enter_projectDir,
                                                      "ProjectDirBrowseAction");
        actionMap = btnProjectDir.getActionMap();
        actionMap.put("ProjectDirBrowseAction", projectBrowseactionListener);
        btnProjectDir.setActionMap(actionMap);

         *
         */
        /* Project Dir Browse Setting button Action Solves MAC ,
         Linux and Window enter key issue*/
        /*
        Action  genUploadactionListener = new AbstractAction () {
          public void actionPerformed(ActionEvent actionEvent) {
            btnGenAndUploadActionPerformed(actionEvent);
          }
        };
        KeyStroke enter_genUpd = KeyStroke.getKeyStroke(KeyEvent.VK_ENTER,
                                                                       0, true);
        btnGenAndUpload.getInputMap(JButton.WHEN_FOCUSED).put(enter_genUpd,
                                                      "GenAndUploadAction");
        actionMap = btnGenAndUpload.getActionMap();
        actionMap.put("GenAndUploadAction", genUploadactionListener);
        btnGenAndUpload.setActionMap(actionMap);
         *
         */
         
    }
   
    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        if(args.length == 0)
        {
            java.awt.EventQueue.invokeLater(new Runnable() {
                public void run() {
                     try{
                    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                    }catch(Exception e){}
                    new MainMPFS().setVisible(true);
                }
            });
       }
       else
       {
         // Make sure we got at least 3 parameters
            if (args.length < 3)
            {
                System.out.println("\n" +
                    "Usage: MPFS2 [options] <SourceDir> <ProjectDir> <OutputFile>\n" +
                    "    /BIN\t\t(/b)\t: Output a BIN image (Default)\n" +
                    "    /C18_C32\t\t(/c)\t: Output a PIC18 or PIC32 image\n" +
                    "    /ASM30\t\t(/s)\t: Output an ASM16 image\n" +
                   // "    /C32\t\t(/x)\t: Output a C32 image\n" +
                 //   "    /classic\t\t(/1)\t: MPFSClassic format\n" +
                    "    /mpfs2\t\t(/2)\t: MPFS2 format (Default)\n" +
                //    "    /reserve #\t\t(/r #)\t: Reserved space for Classic BINs (Default 64)\n" +
                    "    /html \"...\"\t\t(/h)\t: Dynamic file types (\"*.htm, *.html, *.xml, *.cgi\")\n" +
                    "    /xgzip \"...\"\t(/z)\t: Non-compressible types (\"snmp.bib, *.inc\")\n\n" +
                    "SourceDir, ProjectDir, and OutputFile are required and should be enclosed in quotes.\n" +
                    "OutputFile is placed relative to ProjectDir and *CANNOT* be a full path name.");
                return;
            }

//long cmdstartTime  =  System.currentTimeMillis();
            // Locate the parameters
            String sourceDir = args[args.length - 3];
            String projectDir = args[args.length - 2];
            String outputFile = args[args.length - 1];
//            System.out.println("arg len :"+args.length);
//            System.out.println("projectDir :"+projectDir);
//            System.out.println("sourceDir :"+sourceDir);
//            System.out.println("outputFile :"+outputFile);
            boolean genResult;
            final MPFS2Lib builder = new MPFS2Lib();
            MPFS2Lib.MPFSClassicBuilder classicBuilder = null;
            // Set up some defaults
            MPFS2Lib.MPFSOutputFormat fmt = MPFS2Lib.MPFSOutputFormat.BIN;
            byte version = 2;
            int reserveBlock = 64;
            String htmlTypes = "*.htm, *.html, *.xml, *.cgi";
            String noGZipTypes = "*.inc, snmp.bib";

            // Process each command line argument
            for(int i =0; i < (args.length - 3); i++)
            {
                
                String arg = args[i].toLowerCase();
//                System.out.println("arguments : "+ arg);
                // Check for output format parameters
                if(arg.compareTo("/bin")==0 || arg.compareTo("/b")==0)
                        fmt = MPFS2Lib.MPFSOutputFormat.BIN;
                else if(arg.compareTo("/c18_c32")==0 || arg.compareTo("/c")==0)
                        fmt = MPFS2Lib.MPFSOutputFormat.C18;
                else if(arg.compareTo("/asm30")==0 || arg.compareTo("/s")==0)
                        fmt = MPFS2Lib.MPFSOutputFormat.ASM30;
//                else if(arg.contains("/c32") || arg.contains("/x"))
//                        fmt = MPFS2Lib.MPFSOutputFormat.C32;
//                else if(arg.contains("/classic") || arg.contains("/1"))
  //                      version = 1;
                else if(arg.compareTo("/mpfs2")==0 || arg.compareTo("/2")==0)
                        version = 2;

                // Check for string parameters
//                else if(arg.contains("/reserve") || arg.contains("/r"))
//                        reserveBlock = Integer.parseInt(args[++i]);
                else if(arg.compareTo("/html")==0 || arg.compareTo("/h")==0)
                        htmlTypes = args[++i];
                else if(arg.compareTo("/xgzip")==0 || arg.compareTo("/z")==0)
                        noGZipTypes = args[++i];
                else
                {
                    System.out.println("The command-line option \""+args[i]+"\" was not recognized.");
                    return;
                }
            }

            if (version == 2)
            {
                builder.MPFS2Builder(projectDir,outputFile);
                builder.DynamicTypes(htmlTypes);
                builder.NonGZipTypes(noGZipTypes);
                // Add the files to the image and generate the image
                builder.AddDirectory(sourceDir);
                genResult = builder.Generate(fmt);
            }
            else
            {
                classicBuilder = builder.new MPFSClassicBuilder(projectDir,outputFile);
                classicBuilder.ReserveBlock = (int)reserveBlock;
                 // Add the files to the image and generate the image
                classicBuilder.AddDirectory(sourceDir, "");
                genResult = classicBuilder.Generate(fmt);
            }
//            long cmdendTime = System.currentTimeMillis();
//            long cmddiffTime = cmdendTime -  cmdstartTime;
//            System.out.println(" time elapsed : "+ cmddiffTime);
            if (genResult)
            {
//                LogWindow dlg;// = new LogWindow("OptionPane.warningIcon");
//                dlg = new LogWindow(null,true,"OptionPane.informationIcon");
//                //dlg.setVisible(true);
//                dlg.setListArray(builder.log);
//                dlg.setLabelMsg("The MPFS" +"2" +" image was successfully generated.");
//                dlg.setVisible(true);
                System.out.println("The MPFS2 image was successfully generated.\n");
                return;
            }
            else
            {
//                LogWindow dlg;// = new LogWindow("OptionPane.warningIcon");
//                dlg = new LogWindow(null,true,"OptionPane.informationIcon");
//                //dlg.setVisible(true);
//                dlg.setListArray(builder.log);
//                dlg.setLabelMsg("Errors were encountered while generating the MPFS image.");
//                dlg.setVisible(true);
                System.out.println("Errors were encountered while generating the MPFS2 image.\n");
                return;
            }
            //return;
       }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLabel LblSrcDir;
    private javax.swing.JLabel LblStart;
    private javax.swing.JTextField TextSrcDir;
    private javax.swing.JButton btnAdvSetting;
    private javax.swing.JButton btnGenAndUpload;
    private javax.swing.JButton btnProjectDir;
    private javax.swing.JButton btnSrcBrowse;
    private javax.swing.JButton btnUploadSetting;
    private javax.swing.JCheckBox chkBoxUpload;
    private javax.swing.JLabel groupLbl1;
    private javax.swing.JLabel groupLbl2;
    private javax.swing.JLabel groupLbl3;
    private javax.swing.JLabel groupLbl4;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    public javax.swing.JPanel jPanel6;
    public javax.swing.JProgressBar jProgressBar1;
    public static javax.swing.JLabel lblMessage;
    private javax.swing.JLabel lblOuput;
    private javax.swing.JLabel lblProcess;
    private javax.swing.JLabel lblProjectDir;
    private javax.swing.JLabel lblProjectImg;
    private javax.swing.JLabel lblUploadImg;
    private javax.swing.JLabel lbnImageType;
    private javax.swing.JLabel lebelAbout;
    private javax.swing.ButtonGroup processOpnButGrp;
    private javax.swing.JRadioButton radAsm;
    private javax.swing.JRadioButton radBin;
    private javax.swing.JRadioButton radC18C32;
    private javax.swing.JRadioButton radMdd;
    private javax.swing.JRadioButton radPreBuildDir;
    private javax.swing.JRadioButton radWebDir;
    private javax.swing.ButtonGroup sourceSettingButGrp;
    private javax.swing.JTextField txtProjectDir;
    public javax.swing.JTextField txtProjectImageName;
    public javax.swing.JTextField txtUploadPath;
    private org.jdesktop.beansbinding.BindingGroup bindingGroup;
    // End of variables declaration//GEN-END:variables

}
