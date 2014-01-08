/*********************************************************************
*
*  Source and project Directory  selection.
*
**********************************************************************
* FileName:        SourceSettings.java
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
import javax.swing.filechooser.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

public class SourceSettings extends javax.swing.JDialog {

    //private javax.swing.JFileChooser jFileChooser2;
    MainMPFS  mainMpfs;
    ImageIcon icon = new ImageIcon(getClass().getResource("/Resource/mchpIcon.png"));
    /** Creates new form SourceSettings */
    public SourceSettings(javax.swing.JFrame parent, boolean modal,MainMPFS.DIRECTORY_OPTION directoryOpt) {
        super(parent, modal);
        initComponents();
        this.add(jFileChooser2);
        mainMpfs = (MainMPFS)parent;

        setIconImage(icon.getImage());
        EscKeyEventActionIntialization();
        //jFileChooser2 = new javax.swing.JFileChooser();
        jFileChooser2.setDialogTitle("Browse For Folder");
        jFileChooser2.setFont(new java.awt.Font("Microsoft Sans Serif", 0, 11)); // NOI18N
        jFileChooser2.setName("Browse For Folder"); // NOI18N

        if(mainMpfs.getRadBotWebPageDirStatus() == true)
        {
            jFileChooser2.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
            if(directoryOpt == MainMPFS.DIRECTORY_OPTION.SOURCE)
            {
                jFileChooser2.setCurrentDirectory(new
                            java.io.File(mainMpfs.sourceDirectoryPath));
            }
            else if(directoryOpt == MainMPFS.DIRECTORY_OPTION.PROJECT)
            {
                jFileChooser2.setCurrentDirectory(new
                            java.io.File(mainMpfs.projectDirectoryPath));
            }
        }
        else if(mainMpfs.getRadBotPreBuildDirStatus() == true)
        {
            FileFilter binFile = new FileNameExtensionFilter("*.BIN",
                    new String[] {"bin"});
            jFileChooser2.setFileFilter(binFile);
            jFileChooser2.setFileSelectionMode(JFileChooser.FILES_ONLY);
            jFileChooser2.setCurrentDirectory(new
                        java.io.File(mainMpfs.projectBinFilePath));
            jFileChooser2.setDialogTitle("Browse For BIN File");


        }

    }
    public String getDirctoryPath()
    {
        int retval = jFileChooser2.showOpenDialog(this);
        if(retval  == JFileChooser.APPROVE_OPTION)
        {
            return jFileChooser2.getSelectedFile().getAbsolutePath().toString();
        }
        else
        {
            dispose();
            return null;
        }
    }

    public String getParentDirctoryPath()
    {
        return jFileChooser2.getCurrentDirectory().getAbsolutePath().toString();
    }

    public String getOutputDirctoryPath()
    {
        if(jFileChooser2.showOpenDialog(this)  == JFileChooser.APPROVE_OPTION)
        {
           return jFileChooser2.getCurrentDirectory().getAbsolutePath().toString();
        }
        else
        {
           dispose();
           return null;
        }
    }

    private void EscKeyEventActionIntialization()
    {
        Action  ESCactionListener = new AbstractAction () {
          public void actionPerformed(ActionEvent actionEvent) {
            setVisible(false);
            dispose();
          }
        };
        KeyStroke stroke = KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0, true);
        JComponent comp = this.getRootPane();
        comp.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(stroke, "ESCAPE");
        ActionMap actionMap = comp.getActionMap();
        actionMap.put("ESCAPE", ESCactionListener);
    }
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jFileChooser2 = new javax.swing.JFileChooser();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setLocationByPlatform(true);
        setModal(true);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 541, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 388, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    /**
    * @param args the command line arguments
    */
    /*
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                SourceSettings dialog = new SourceSettings(new javax.swing.JFrame(), true);
                dialog.addWindowListener(new java.awt.event.WindowAdapter() {
                    public void windowClosing(java.awt.event.WindowEvent e) {
                        System.exit(0);
                    }
                });
                dialog.setVisible(true);
            }
        });
    }
     *
     */

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JFileChooser jFileChooser2;
    // End of variables declaration//GEN-END:variables

}
