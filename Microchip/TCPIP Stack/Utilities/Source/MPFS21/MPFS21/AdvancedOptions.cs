/*********************************************************************
 *
 *    Advanced Options dialog for MPFS21
 *
 *********************************************************************
 * FileName:        AdvancedOptions.cs
 * Dependencies:    Microsoft .NET Framework 2.0
 * Processor:       x86
 * Complier:        Microsoft Visual C# 2008 Express Edition
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * This software is owned by Microchip Technology Inc. ("Microchip") 
 * and is supplied to you for use exclusively as described in the 
 * associated software agreement.  This software is protected by 
 * software and other intellectual property laws.  Any use in 
 * violation of the software license may subject the user to criminal 
 * sanctions as well as civil liability.  Copyright 2008 Microchip
 * Technology Inc.  All rights reserved.
 *
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
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date   		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Elliott Wood		4/17/2008	    Original
 ********************************************************************/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MPFS21
{
    public partial class AdvancedOptions : Form
    {
        public AdvancedOptions()
        {
            InitializeComponent();
        }

        private void radMPFS2_CheckedChanged(object sender, EventArgs e)
        {
            pnlMPFS2.Visible = radMPFS2.Checked;
            pnlMPFSClassic.Visible = radMPFSClassic.Checked;
        }

        /// <summary>
        /// Restores the default settings
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDefaults_Click(object sender, EventArgs e)
        {
            radMPFS2.Checked = true;
            txtReserveBlockClassic.Text = "64";
            txtDynamicFiles.Text = "*.htm, *.html, *.cgi, *.xml";
            txtNoCompress.Text = "*.inc, snmp.bib";
        }

        /// <summary>
        /// Do manual data marshalling
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void AdvancedOptions_Load(object sender, EventArgs e)
        {
            // Manual data marshalling
            if (Settings.Default.OutputVersion == 1)
                radMPFSClassic.Checked = true;
            else
                radMPFS2.Checked = true;
            txtReserveBlockClassic.Text = Settings.Default.ReserveBlockClassic.ToString();
            txtDynamicFiles.Text = Settings.Default.DynamicFiles;
            txtNoCompress.Text = Settings.Default.NoCompressFiles;
        }

        /// <summary>
        /// Do manual marshalling data back to settings file
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOK_Click(object sender, EventArgs e)
        {
            if (radMPFSClassic.Checked)
                Settings.Default.OutputVersion = 1;
            else
                Settings.Default.OutputVersion = 2;
            Settings.Default.ReserveBlockClassic = Convert.ToInt32(txtReserveBlockClassic.Text);
            Settings.Default.DynamicFiles = txtDynamicFiles.Text;
            Settings.Default.NoCompressFiles = txtNoCompress.Text;
        }
    }
}
