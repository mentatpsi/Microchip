/*********************************************************************
 *
 *     Microchip Ethernet Discoverer
 *      -Provides a PC interface for the Announce.c Microchip TCP/IP 
 *       module
 *
 *********************************************************************
 * FileName:        frmMain.cs
 * Dependencies:    Microsoft .NET Framework 2.0
 * Processor:       x86
 * Complier:        Microsoft Visual C# 2005 Express Edition
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * This software is owned by Microchip Technology Inc. ("Microchip") 
 * and is supplied to you for use exclusively as described in the 
 * associated software agreement.  This software is protected by 
 * software and other intellectual property laws.  Any use in 
 * violation of the software license may subject the user to criminal 
 * sanctions as well as civil liability.  Copyright 2006 Microchip
 * Technology Inc.  All rights reserved.
 *
 * This software is provided "AS IS."  MICROCHIP DISCLAIMS ALL 
 * WARRANTIES, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, NOT LIMITED 
 * TO MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
 * INFRINGEMENT.  Microchip shall in no event be liable for special, 
 * incidental, or consequential damages.
 *
 *
 * Author               Date   		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder		7/31/06	    Original
********************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;

namespace Embedded_Device_Discoverer
{
    public partial class Form1 : Form
    {
        public delegate void AsyncCallback(IAsyncResult ar);
        public delegate void AddTolstDiscoveredDevices(object o);

        private UdpState GlobalUDP;

        public Form1()
        {
            InitializeComponent();
        }

        struct UdpState
        {
            public System.Net.IPEndPoint EP;
            public System.Net.Sockets.UdpClient UDPClient;
        }

        private void cmdDiscoverDevices_Click(object sender, EventArgs e)
        {
            try
            {
                // Clear the listbox of all current discovery responses
                listView1.Items.Clear();

                // Try to send the discovery request message
                byte[] DiscoverMsg = Encoding.ASCII.GetBytes("Discovery: Who is out there?");
                GlobalUDP.UDPClient.Send(DiscoverMsg, DiscoverMsg.Length, new System.Net.IPEndPoint(System.Net.IPAddress.Parse("255.255.255.255"), 30303));
            }
            catch
            {
                MessageBox.Show("Unable to transmit discovery message.  Check network connectivity and ensure that no other instances of this program are running.", "Error", MessageBoxButtons.OK);
                return;
            }
        }

        public void AddDiscoveryEntry(object o)
        {
            //lstDiscoveredDevices.Items.Add(o);
            listView1.Items.Add(new ListViewItem(((string)(o)).Split('\n')));
        }

        public void ReceiveCallback(IAsyncResult ar)
        {
            UdpState MyUDP = (UdpState)ar.AsyncState;

            // Obtain the UDP message body and convert it to a string, with remote IP address attached as well
            string ReceiveString = Encoding.ASCII.GetString(MyUDP.UDPClient.EndReceive(ar, ref MyUDP.EP));
            ReceiveString = MyUDP.EP.Address.ToString() + "\n" + ReceiveString.Replace("\r\n", "\n");

            // Configure the UdpClient class to accept more messages, if they arrive
            MyUDP.UDPClient.BeginReceive(ReceiveCallback, MyUDP);

            // Write the received UDP message text to the listbox in a thread-safe manner
            //lstDiscoveredDevices.Invoke(new AddTolstDiscoveredDevices(AddDiscoveryEntry), ReceiveString);
            listView1.Invoke(new AddTolstDiscoveredDevices(AddDiscoveryEntry), ReceiveString);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                GlobalUDP.UDPClient = new UdpClient();
                GlobalUDP.EP = new System.Net.IPEndPoint(System.Net.IPAddress.Parse("255.255.255.255"), 30303);
                System.Net.IPEndPoint BindEP = new System.Net.IPEndPoint(System.Net.IPAddress.Any, 30303);
                byte[] DiscoverMsg = Encoding.ASCII.GetBytes("Discovery: Who is out there?");

                // Set the local UDP port to listen on
                GlobalUDP.UDPClient.Client.Bind(BindEP);

                // Enable the transmission of broadcast packets without having them be received by ourself
                GlobalUDP.UDPClient.EnableBroadcast = true;
                GlobalUDP.UDPClient.MulticastLoopback = false;

                // Configure ourself to receive discovery responses
                GlobalUDP.UDPClient.BeginReceive(ReceiveCallback, GlobalUDP);

                // Transmit the discovery request message
                GlobalUDP.UDPClient.Send(DiscoverMsg, DiscoverMsg.Length, new System.Net.IPEndPoint(System.Net.IPAddress.Parse("255.255.255.255"), 30303));
            }
            catch
            {
                MessageBox.Show("Unable to transmit discovery message.  Check network connectivity and ensure that no other instances of this program are running.", "Error", MessageBoxButtons.OK);
                return;
            }
        }

        private void listView1_ItemActivate(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("http://" + listView1.SelectedItems[0].Text);
        }
    }
}