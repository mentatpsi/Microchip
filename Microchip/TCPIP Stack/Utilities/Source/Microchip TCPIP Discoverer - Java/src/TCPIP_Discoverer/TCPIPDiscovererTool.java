/*********************************************************************
*
*  TCPIP Microchip devices discoverer GUI
*
**********************************************************************
* FileName:        TCPIPDiscovererTool.java
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

package TCPIP_Discoverer;
import java.awt.*;
import java.net.*;
import java.io.*;
import javax.swing.*;
import java.awt.Desktop;
import javax.swing.table.*;
import java.util.concurrent.locks.*;

public class TCPIPDiscovererTool extends javax.swing.JFrame {
    public static DatagramSocket socket;
    public static DatagramPacket packet;
    public static DefaultTableModel model;
    public static Thread recvThread=null;
    public static Thread manageTable=null;
    public static String mchipUrl;
    String str = new String("Discovery: Who is out there?\0\n");
    String received = " ";
    long threadSleep = 500;
    ReentrantLock lock = new ReentrantLock();
    ImageIcon icon = new ImageIcon(getClass().getResource
            ("/Resource/mchpIcon.png"));
    /** Creates new form TCPIPDiscovererTool */
    public TCPIPDiscovererTool() {
        initComponents();
        changeTableColumnWidth();
        this.setIconImage(icon.getImage());
         // call dtatgram socket here and create the thread here
        udpAction();
        UIManager.put("Button.defaultButtonFollowsFocus", Boolean.TRUE);
        this.setLocation(200, 150);
        
    }
 /* Table updation Thread */
    public void addDiscoveryEntry(String receivePacket)
    {
        String[] s = receivePacket.split("\n");
        if(s[3].contains
            ("                                                                ")
                                                                        == true)
            s[3] = "";
        //if(s[1].contentEquals("MCHPBOARD      ")!=true)
          //  return;
        model  = (DefaultTableModel)jTable1.getModel();
        int rowCnt = model.getRowCount();
        // to avoid the same mac
        for(int index=0;index<rowCnt;index++)
        {
            String macAddrs = (String)model.getValueAt(index,2);
            if(macAddrs.isEmpty() == false)
            {
               //System.out.println("macaddres"+macAddrs);
                if(s[2].contentEquals(macAddrs))
                {
                    //System.out.println("equal macaddress");
                    model.removeRow(index);
                    model.insertRow(index,s);
                    return;
                }
            }
        }
        model.insertRow(0,s);
       /* try
        {
            manageTable.sleep(threadSleep);
        }catch(InterruptedException ie)
        {
            close_action();
        }*/
    }

    /* Receive thread */
    public void receiveUDPPacket()
    {
        try
        {
            byte[] buf = new byte[100];
            for(int i=0;i<buf.length;i++)
                buf[i]=' ';
            packet = new DatagramPacket(buf, buf.length);

            //ignore first packet which is a loopback one;
            socket.receive(packet);

            received = new String(packet.getData());
            for(;;)
            {
                for(int i=0;i<buf.length;i++)
                buf[i]=' ';
                packet = new DatagramPacket(buf, buf.length);

                try{
                socket.receive(packet);
                }
                catch(SocketTimeoutException  e){
                 // System.out.println("lengthcatch"+packet.getLength());
                }
                // * */
                if(new String(packet.getData()).trim().length() == 0)
                    continue;
                if(packet.getLength() < 36)
                {
                    continue;
                }

                received = new String(packet.getData());
                if(received.isEmpty())
                    continue;
               // System.out.println("inside received thread " + received);
                lock.lock();
                received = packet.getAddress().getHostAddress()+
                                    "\n"+received.replace("\r\n", "\n");
                addDiscoveryEntry(received);
                received = " ";
                lock.unlock();
            }
        }
        catch (IOException e)
        {
            close_action();
        }
    }
    // create dtatgram socket and send the broadcast packet to all the listeners
    public void udpAction()
    {
        try
        {
            recvThread = new Thread()
            {
                public void run()
                {
                    receiveUDPPacket();
                }
            };
            manageTable = new Thread()
            {
                public void run()
                {
                    String[] s;
                    while(true)
                    {
                        if(received.trim().isEmpty())
                        {
                            continue;
                        }
                        if(received.isEmpty())
                            continue;
                        s = received.split("\n");
                        if(s[1].trim().isEmpty())
                        {
                            received = " ";
                            continue;
                        }
                        //if(s[1].contentEquals("MCHPBOARD      ")!=true)
                          //  continue;
                        lock.lock();
                        addDiscoveryEntry(received);
                        received = " ";
                        lock.unlock();
                    }
                }
            };
            socket = new DatagramSocket(30303);
            socket.setBroadcast(true);
            InetAddress address = InetAddress.getByName("255.255.255.255");
            packet = new DatagramPacket(str.getBytes(),
                    str.length(), address, 30303);

            socket.send(packet);
            recvThread.start();
            //manageTable.start();
        }
        catch(IOException e)
        {
            String optionStr = "<html>"+"Unable to transmit discovery message. <br> Check network " +
                    "connectivity and ensure that no other instances of this " +
                    "program are running.";

            JOptionPane.showMessageDialog(null,optionStr,
                            "Error", JOptionPane.ERROR_MESSAGE);
            close_action();
        }
    }
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        btnExit = new javax.swing.JButton();
        btnDiscover = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        jTable1 = new javax.swing.JTable();
        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        jMenuItem1 = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Microchip TCPIP Discoverer");
        setResizable(false);

        btnExit.setText("Exit");
        btnExit.setMaximumSize(new java.awt.Dimension(115, 23));
        btnExit.setMinimumSize(new java.awt.Dimension(115, 23));
        btnExit.setPreferredSize(new java.awt.Dimension(115, 23));
        btnExit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnExitActionPerformed(evt);
            }
        });

        btnDiscover.setText("Discover Devices");
        btnDiscover.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        btnDiscover.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnDiscoverActionPerformed(evt);
            }
        });

        jTable1.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "IP Address", "Host Name", "MAC Address", "Other Info"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.String.class, java.lang.String.class, java.lang.String.class
            };
            boolean[] canEdit = new boolean [] {
                false, false, false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jTable1.setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        jTable1.getTableHeader().setReorderingAllowed(false);
        jTable1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jTable1MouseClicked(evt);
            }
        });
        jScrollPane1.setViewportView(jTable1);

        jMenu1.setText("Help");

        jMenuItem1.setText("About...");
        jMenuItem1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem1ActionPerformed(evt);
            }
        });
        jMenu1.add(jMenuItem1);

        jMenuBar1.add(jMenu1);

        setJMenuBar(jMenuBar1);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(24, 24, 24)
                .addComponent(btnDiscover, javax.swing.GroupLayout.PREFERRED_SIZE, 158, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 255, Short.MAX_VALUE)
                .addComponent(btnExit, javax.swing.GroupLayout.PREFERRED_SIZE, 150, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(63, 63, 63))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addContainerGap()
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 630, Short.MAX_VALUE)
                    .addContainerGap()))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(26, 26, 26)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnExit, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(btnDiscover, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(293, Short.MAX_VALUE))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGap(71, 71, 71)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 255, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addContainerGap(21, Short.MAX_VALUE)))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnExitActionPerformed
        //close_action();
        System.exit(0);
}//GEN-LAST:event_btnExitActionPerformed

    private void btnDiscoverActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnDiscoverActionPerformed
        // TODO add your handling code here:
        try {
            if(socket.isBound() != true) {
                socket = new DatagramSocket(30303);
                System.out.println("EthDiscoverActionPerformed:after socket");
            }
            InetAddress address = InetAddress.getByName("255.255.255.255");
            model  = (DefaultTableModel)jTable1.getModel();
            int rowcount = model.getRowCount();
            for(int index1=0;index1 < rowcount;index1++) {
                model.removeRow(0);
            }
            packet = new DatagramPacket(str.getBytes(),str.length(),
                    address, 30303);

            socket.send(packet);
            jTable1.addMouseListener(null);
        }catch(IOException soEx) {
            int res1;
            String optionStr = "<html>"+"Unable to transmit discovery message."+
                    " <br> Check network " +
                    "connectivity and ensure that no other instances of this " +
                    "program are running.";

            JOptionPane.showMessageDialog(null,optionStr,
                    "Error", JOptionPane.ERROR_MESSAGE);
            close_action();
        }
}//GEN-LAST:event_btnDiscoverActionPerformed

    private void jTable1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jTable1MouseClicked
        // TODO add your handling code here:
        Point pt = evt.getPoint();
        int row = jTable1.rowAtPoint(pt);
        int col = jTable1.columnAtPoint(pt);
        String url = (String)jTable1.getValueAt(row,0);
        mchipUrl = (String)"http://"+url;
        try{
            URL myUrl = new URL(mchipUrl);
            Desktop.getDesktop().browse(myUrl.toURI());
        }catch (Exception ex) {}
}//GEN-LAST:event_jTable1MouseClicked

    private void jMenuItem1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem1ActionPerformed
        // TODO add your handling code here:
        TCPIPAboutBox aboutBox = new TCPIPAboutBox(this,true);
        aboutBox.setVisible(true);
    }//GEN-LAST:event_jMenuItem1ActionPerformed

    public void close_action()
    {
        if(socket != null)
        {
            if(socket.isConnected() == true)
            {
                socket.close();
            }
        }
        if(recvThread != null)
        {
            if(recvThread.isAlive() == true)
            {
                recvThread = null;
            }
        }
        if(manageTable != null)
        {
            if(manageTable.isAlive() == true)
            {
                manageTable = null;
            }
        }
    }

    void changeTableColumnWidth()
    {
        TableColumn column = null;
        final TableCellRenderer tcr = jTable1.getTableHeader().getDefaultRenderer();
        jTable1.getTableHeader().setDefaultRenderer(new TableCellRenderer() {
            @Override
            public Component getTableCellRendererComponent(JTable table,
                    Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                JLabel lbl = (JLabel) tcr.getTableCellRendererComponent
                        (table, value, isSelected, hasFocus, row, column);
                lbl.setBorder(BorderFactory.createCompoundBorder
                        (lbl.getBorder(), BorderFactory.createEmptyBorder(0,5, 0, 0)));
                lbl.setHorizontalAlignment(SwingConstants.LEFT);
                return lbl;
            }
        });
        /*
         * ((DefaultTableCellRenderer) jTable1.getTableHeader().getDefaultRenderer()).
                setHorizontalAlignment(SwingConstants.LEFT);
         */
        jTable1.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
        jTable1.setDragEnabled(false);
        jTable1.setColumnSelectionAllowed(false);
        int colNumber = jTable1.getColumnCount();
        for (int i = 0; i < colNumber; i++) {
        column = jTable1.getColumnModel().getColumn(i);
        //col = jTable1.getColumnModel();
        switch(i)
        {
            case 0:
                //col.getColumn(i).setPreferredWidth(15);
                column.setPreferredWidth(100);
                column.setMaxWidth(120);
                //column.setWidth(15);
                break;
            case 1:
                //col.getColumn(i).setPreferredWidth(16);
                column.setPreferredWidth(100);
                column.setMaxWidth(120);
                //column.setWidth(16);
                break;
            case 2:
                //col.getColumn(i).setPreferredWidth(17);
                column.setPreferredWidth(130);
                column.setMaxWidth(180);
                //column.setWidth(17);
                break;
            case 3:
                column.setPreferredWidth(100);//.setPreferedWidth(100);
                //column.setMaxWidth(1000);
                break;
        }
        column.setResizable(true);
        }
    }

    /**
    * @param args the command line arguments
    */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                try{
                UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
                //UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                // UIManager.setLookAndFeel(new com.sun.java.swing.plaf.windows.WindowsLookAndFeel());
                }catch (Exception e) {}
                new TCPIPDiscovererTool().setVisible(true);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnDiscover;
    private javax.swing.JButton btnExit;
    private javax.swing.JMenu jMenu1;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JTable jTable1;
    // End of variables declaration//GEN-END:variables

}
