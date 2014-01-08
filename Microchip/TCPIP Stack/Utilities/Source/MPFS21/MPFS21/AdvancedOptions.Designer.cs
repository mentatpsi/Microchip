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
namespace MPFS21
{
    partial class AdvancedOptions
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AdvancedOptions));
            this.label1 = new System.Windows.Forms.Label();
            this.radMPFS2 = new System.Windows.Forms.RadioButton();
            this.radMPFSClassic = new System.Windows.Forms.RadioButton();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.txtNoCompress = new System.Windows.Forms.TextBox();
            this.txtDynamicFiles = new System.Windows.Forms.TextBox();
            this.txtReserveBlockClassic = new System.Windows.Forms.TextBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnDefaults = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.pnlMPFS2 = new System.Windows.Forms.Panel();
            this.label6 = new System.Windows.Forms.Label();
            this.pnlMPFSClassic = new System.Windows.Forms.Panel();
            this.label7 = new System.Windows.Forms.Label();
            this.pnlMPFS2.SuspendLayout();
            this.pnlMPFSClassic.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Output Format:";
            // 
            // radMPFS2
            // 
            this.radMPFS2.AutoSize = true;
            this.radMPFS2.Location = new System.Drawing.Point(58, 25);
            this.radMPFS2.Name = "radMPFS2";
            this.radMPFS2.Size = new System.Drawing.Size(60, 17);
            this.radMPFS2.TabIndex = 1;
            this.radMPFS2.TabStop = true;
            this.radMPFS2.Text = "MPFS2";
            this.toolTip1.SetToolTip(this.radMPFS2, "Build an MPFS2 image that is compatible with the HTTP2 web server.");
            this.radMPFS2.UseVisualStyleBackColor = true;
            this.radMPFS2.CheckedChanged += new System.EventHandler(this.radMPFS2_CheckedChanged);
            // 
            // radMPFSClassic
            // 
            this.radMPFSClassic.AutoSize = true;
            this.radMPFSClassic.Location = new System.Drawing.Point(147, 25);
            this.radMPFSClassic.Name = "radMPFSClassic";
            this.radMPFSClassic.Size = new System.Drawing.Size(90, 17);
            this.radMPFSClassic.TabIndex = 2;
            this.radMPFSClassic.TabStop = true;
            this.radMPFSClassic.Text = "MPFS Classic";
            this.toolTip1.SetToolTip(this.radMPFSClassic, "Build a classic MPFS image that is compatible with the original HTTP web server.");
            this.radMPFSClassic.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 3);
            this.label2.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(75, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Dynamic Files:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 49);
            this.label3.Margin = new System.Windows.Forms.Padding(3, 7, 3, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(93, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Do Not Compress:";
            // 
            // toolTip1
            // 
            this.toolTip1.AutoPopDelay = 10000;
            this.toolTip1.InitialDelay = 500;
            this.toolTip1.IsBalloon = true;
            this.toolTip1.ReshowDelay = 100;
            this.toolTip1.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.toolTip1.ToolTipTitle = "Advanced Settings Help";
            // 
            // txtNoCompress
            // 
            this.txtNoCompress.Location = new System.Drawing.Point(6, 65);
            this.txtNoCompress.Name = "txtNoCompress";
            this.txtNoCompress.Size = new System.Drawing.Size(242, 20);
            this.txtNoCompress.TabIndex = 6;
            this.txtNoCompress.Text = "*.inc, snmp.bib";
            this.toolTip1.SetToolTip(this.txtNoCompress, resources.GetString("txtNoCompress.ToolTip"));
            // 
            // txtDynamicFiles
            // 
            this.txtDynamicFiles.Location = new System.Drawing.Point(6, 19);
            this.txtDynamicFiles.Name = "txtDynamicFiles";
            this.txtDynamicFiles.Size = new System.Drawing.Size(242, 20);
            this.txtDynamicFiles.TabIndex = 4;
            this.txtDynamicFiles.Text = "*.htm, *.html, *.cgi, *.xml";
            this.toolTip1.SetToolTip(this.txtDynamicFiles, "These files will be searched for\r\ndynamic variables and indexed for \r\nyour applic" +
                    "ation.  Enter file names or\r\nextensions, separated by commas.");
            // 
            // txtReserveBlockClassic
            // 
            this.txtReserveBlockClassic.Location = new System.Drawing.Point(29, 37);
            this.txtReserveBlockClassic.Name = "txtReserveBlockClassic";
            this.txtReserveBlockClassic.Size = new System.Drawing.Size(50, 20);
            this.txtReserveBlockClassic.TabIndex = 3;
            this.toolTip1.SetToolTip(this.txtReserveBlockClassic, "The MPFS image will begin at this address in your external storage.  Memory befor" +
                    "e this address will be reserved for your own application\'s use.");
            // 
            // btnOK
            // 
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(12, 174);
            this.btnOK.Margin = new System.Windows.Forms.Padding(3, 10, 3, 3);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 7;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnDefaults
            // 
            this.btnDefaults.Location = new System.Drawing.Point(103, 174);
            this.btnDefaults.Name = "btnDefaults";
            this.btnDefaults.Size = new System.Drawing.Size(75, 23);
            this.btnDefaults.TabIndex = 8;
            this.btnDefaults.Text = "Defaults";
            this.btnDefaults.UseVisualStyleBackColor = true;
            this.btnDefaults.Click += new System.EventHandler(this.btnDefaults_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(194, 174);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 9;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(26, 21);
            this.label4.Margin = new System.Windows.Forms.Padding(3, 3, 3, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(80, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Reserve Block:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(82, 40);
            this.label5.Margin = new System.Windows.Forms.Padding(0, 0, 3, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 13);
            this.label5.TabIndex = 12;
            this.label5.Text = "bytes";
            // 
            // pnlMPFS2
            // 
            this.pnlMPFS2.Controls.Add(this.label6);
            this.pnlMPFS2.Controls.Add(this.label2);
            this.pnlMPFS2.Controls.Add(this.txtDynamicFiles);
            this.pnlMPFS2.Controls.Add(this.label3);
            this.pnlMPFS2.Controls.Add(this.txtNoCompress);
            this.pnlMPFS2.Location = new System.Drawing.Point(12, 50);
            this.pnlMPFS2.Name = "pnlMPFS2";
            this.pnlMPFS2.Size = new System.Drawing.Size(257, 111);
            this.pnlMPFS2.TabIndex = 13;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(3, 95);
            this.label6.Margin = new System.Windows.Forms.Padding(3, 7, 3, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(245, 13);
            this.label6.TabIndex = 7;
            this.label6.Text = "(Reserve block is only configured in TCPIPConfig.)";
            // 
            // pnlMPFSClassic
            // 
            this.pnlMPFSClassic.Controls.Add(this.label7);
            this.pnlMPFSClassic.Controls.Add(this.label4);
            this.pnlMPFSClassic.Controls.Add(this.txtReserveBlockClassic);
            this.pnlMPFSClassic.Controls.Add(this.label5);
            this.pnlMPFSClassic.Location = new System.Drawing.Point(12, 48);
            this.pnlMPFSClassic.Name = "pnlMPFSClassic";
            this.pnlMPFSClassic.Size = new System.Drawing.Size(257, 110);
            this.pnlMPFSClassic.TabIndex = 14;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(3, 97);
            this.label7.Margin = new System.Windows.Forms.Padding(3, 7, 3, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(215, 13);
            this.label7.TabIndex = 13;
            this.label7.Text = "(Reserve block MUST match TCPIPConfig.)";
            // 
            // AdvancedOptions
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(280, 207);
            this.Controls.Add(this.pnlMPFSClassic);
            this.Controls.Add(this.pnlMPFS2);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnDefaults);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.radMPFSClassic);
            this.Controls.Add(this.radMPFS2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AdvancedOptions";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Advanced Settings";
            this.Load += new System.EventHandler(this.AdvancedOptions_Load);
            this.pnlMPFS2.ResumeLayout(false);
            this.pnlMPFS2.PerformLayout();
            this.pnlMPFSClassic.ResumeLayout(false);
            this.pnlMPFSClassic.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton radMPFS2;
        private System.Windows.Forms.RadioButton radMPFSClassic;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtDynamicFiles;
        private System.Windows.Forms.TextBox txtNoCompress;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnDefaults;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox txtReserveBlockClassic;
        private System.Windows.Forms.Panel pnlMPFS2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Panel pnlMPFSClassic;
        private System.Windows.Forms.Label label7;
    }
}