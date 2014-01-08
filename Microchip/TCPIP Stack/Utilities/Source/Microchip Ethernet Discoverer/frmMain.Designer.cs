/*********************************************************************
 *
 *     Microchip Ethernet Device Discoverer
 *
 *********************************************************************
 * FileName:        frmMain.Designer.cs
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
namespace Embedded_Device_Discoverer
{
    partial class Form1
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
            this.cmdDiscoverDevices = new System.Windows.Forms.Button();
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnIPAddress = new System.Windows.Forms.ColumnHeader();
            this.columnHostName = new System.Windows.Forms.ColumnHeader();
            this.columnMACAddress = new System.Windows.Forms.ColumnHeader();
            this.columnOther = new System.Windows.Forms.ColumnHeader();
            this.SuspendLayout();
            // 
            // cmdDiscoverDevices
            // 
            this.cmdDiscoverDevices.Location = new System.Drawing.Point(12, 12);
            this.cmdDiscoverDevices.Name = "cmdDiscoverDevices";
            this.cmdDiscoverDevices.Size = new System.Drawing.Size(143, 23);
            this.cmdDiscoverDevices.TabIndex = 0;
            this.cmdDiscoverDevices.Text = "&Discover Devices";
            this.cmdDiscoverDevices.UseVisualStyleBackColor = true;
            this.cmdDiscoverDevices.Click += new System.EventHandler(this.cmdDiscoverDevices_Click);
            // 
            // listView1
            // 
            this.listView1.Activation = System.Windows.Forms.ItemActivation.TwoClick;
            this.listView1.Alignment = System.Windows.Forms.ListViewAlignment.Default;
            this.listView1.AllowColumnReorder = true;
            this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnIPAddress,
            this.columnHostName,
            this.columnMACAddress,
            this.columnOther});
            this.listView1.FullRowSelect = true;
            this.listView1.HideSelection = false;
            this.listView1.Location = new System.Drawing.Point(12, 41);
            this.listView1.MultiSelect = false;
            this.listView1.Name = "listView1";
            this.listView1.ShowGroups = false;
            this.listView1.Size = new System.Drawing.Size(614, 218);
            this.listView1.TabIndex = 4;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            this.listView1.ItemActivate += new System.EventHandler(this.listView1_ItemActivate);
            // 
            // columnIPAddress
            // 
            this.columnIPAddress.Text = "IP Address";
            this.columnIPAddress.Width = 90;
            // 
            // columnHostName
            // 
            this.columnHostName.Text = "Host Name";
            this.columnHostName.Width = 140;
            // 
            // columnMACAddress
            // 
            this.columnMACAddress.Text = "MAC Address";
            this.columnMACAddress.Width = 140;
            // 
            // columnOther
            // 
            this.columnOther.Text = "Other Info";
            this.columnOther.Width = 240;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(638, 271);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.cmdDiscoverDevices);
            this.Name = "Form1";
            this.Text = "Microchip Ethernet Device Discoverer";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cmdDiscoverDevices;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader columnIPAddress;
        private System.Windows.Forms.ColumnHeader columnHostName;
        private System.Windows.Forms.ColumnHeader columnMACAddress;
        private System.Windows.Forms.ColumnHeader columnOther;
    }
}

