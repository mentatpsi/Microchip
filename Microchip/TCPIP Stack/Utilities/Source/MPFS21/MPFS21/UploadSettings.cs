/*********************************************************************
 *
 *    Upload Settings dialog for MPFS21
 *
 *********************************************************************
 * FileName:        UploadSettings.cs
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
    public partial class UploadSettings : Form
    {
        public UploadSettings()
        {
            InitializeComponent();
        }

        private void btnDefaults_Click(object sender, EventArgs e)
        {
            txtUploadAddress.Text = "MCHPBOARD";
            txtUploadPath.Text = "mpfsupload";
            txtUploadUser.Text = "admin";
            txtUploadPass.Text = "microchip";
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Settings.Default.UploadAddress = txtUploadAddress.Text;
            Settings.Default.UploadPath = txtUploadPath.Text;
            Settings.Default.UploadUser = txtUploadUser.Text;
            Settings.Default.UploadPass = txtUploadPass.Text;
        }

        private void UploadSettings_Load(object sender, EventArgs e)
        {
            txtUploadAddress.Text = Settings.Default.UploadAddress;
            txtUploadPath.Text = Settings.Default.UploadPath;
            txtUploadUser.Text = Settings.Default.UploadUser;
            txtUploadPass.Text = Settings.Default.UploadPass;
        }

    }
}
