/*********************************************************************
 *
 *    Main Dialog for MPFS21
 *
 *********************************************************************
 * FileName:        MPFS21Form.cs
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
    partial class MPFS21Form
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MPFS21Form));
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.txtSourceImage = new System.Windows.Forms.TextBox();
            this.btnSourceDir = new System.Windows.Forms.Button();
            this.txtSourceDir = new System.Windows.Forms.TextBox();
            this.lblInput = new System.Windows.Forms.Label();
            this.radStartImg = new System.Windows.Forms.RadioButton();
            this.radStartDir = new System.Windows.Forms.RadioButton();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.btnAdvanced = new System.Windows.Forms.Button();
            this.radOutputMDD = new System.Windows.Forms.RadioButton();
            this.radOutputASM30 = new System.Windows.Forms.RadioButton();
            this.radOutputC18 = new System.Windows.Forms.RadioButton();
            this.radOutputBIN = new System.Windows.Forms.RadioButton();
            this.label7 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.lblType = new System.Windows.Forms.Label();
            this.txtImageName = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.btnProjectDir = new System.Windows.Forms.Button();
            this.txtProjectDir = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.txtUploadDestination = new System.Windows.Forms.TextBox();
            this.chkUpload = new System.Windows.Forms.CheckBox();
            this.label11 = new System.Windows.Forms.Label();
            this.btnUploadSettings = new System.Windows.Forms.Button();
            this.btnGenerate = new System.Windows.Forms.Button();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.lblAbout = new System.Windows.Forms.Label();
            this.myProgress = new System.Windows.Forms.ProgressBar();
            this.myStatusMsg = new System.Windows.Forms.Label();
            this.myToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(5, 16);
            this.label1.Margin = new System.Windows.Forms.Padding(0, 0, 5, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(27, 24);
            this.label1.TabIndex = 1;
            this.label1.Text = "1.";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold);
            this.label2.Location = new System.Drawing.Point(5, 13);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(27, 24);
            this.label2.TabIndex = 2;
            this.label2.Text = "2.";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold);
            this.label3.Location = new System.Drawing.Point(5, 18);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(27, 24);
            this.label3.TabIndex = 3;
            this.label3.Text = "3.";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold);
            this.label4.Location = new System.Drawing.Point(5, 18);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 24);
            this.label4.TabIndex = 4;
            this.label4.Text = "4.";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.txtSourceImage);
            this.groupBox1.Controls.Add(this.btnSourceDir);
            this.groupBox1.Controls.Add(this.txtSourceDir);
            this.groupBox1.Controls.Add(this.lblInput);
            this.groupBox1.Controls.Add(this.radStartImg);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.radStartDir);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(15, 5);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(15, 5, 5, 10);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(5, 5, 15, 10);
            this.groupBox1.Size = new System.Drawing.Size(545, 85);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Source Settings";
            // 
            // txtSourceImage
            // 
            this.txtSourceImage.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.txtSourceImage.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystem;
            this.txtSourceImage.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtSourceImage.Location = new System.Drawing.Point(47, 52);
            this.txtSourceImage.Name = "txtSourceImage";
            this.txtSourceImage.Size = new System.Drawing.Size(389, 20);
            this.txtSourceImage.TabIndex = 6;
            this.txtSourceImage.TextChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // btnSourceDir
            // 
            this.btnSourceDir.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSourceDir.Location = new System.Drawing.Point(442, 50);
            this.btnSourceDir.Name = "btnSourceDir";
            this.btnSourceDir.Size = new System.Drawing.Size(75, 23);
            this.btnSourceDir.TabIndex = 5;
            this.btnSourceDir.Text = "Browse...";
            this.btnSourceDir.UseVisualStyleBackColor = true;
            this.btnSourceDir.Click += new System.EventHandler(this.btnSourceDir_Click);
            // 
            // txtSourceDir
            // 
            this.txtSourceDir.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.txtSourceDir.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystemDirectories;
            this.txtSourceDir.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtSourceDir.Location = new System.Drawing.Point(47, 52);
            this.txtSourceDir.Name = "txtSourceDir";
            this.txtSourceDir.Size = new System.Drawing.Size(389, 20);
            this.txtSourceDir.TabIndex = 4;
            this.myToolTip.SetToolTip(this.txtSourceDir, "Selects the source file(s) for the remainder of the process.");
            // 
            // lblInput
            // 
            this.lblInput.AutoSize = true;
            this.lblInput.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblInput.Location = new System.Drawing.Point(44, 36);
            this.lblInput.Name = "lblInput";
            this.lblInput.Size = new System.Drawing.Size(89, 13);
            this.lblInput.TabIndex = 3;
            this.lblInput.Text = "Source Directory:";
            // 
            // radStartImg
            // 
            this.radStartImg.AutoSize = true;
            this.radStartImg.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radStartImg.Location = new System.Drawing.Point(274, 16);
            this.radStartImg.Name = "radStartImg";
            this.radStartImg.Size = new System.Drawing.Size(128, 17);
            this.radStartImg.TabIndex = 2;
            this.radStartImg.Text = "Pre-Built MPFS Image";
            this.myToolTip.SetToolTip(this.radStartImg, "Upload an existing BIN image to a device.");
            this.radStartImg.UseVisualStyleBackColor = true;
            this.radStartImg.CheckedChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // radStartDir
            // 
            this.radStartDir.AutoSize = true;
            this.radStartDir.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radStartDir.Location = new System.Drawing.Point(135, 16);
            this.radStartDir.Name = "radStartDir";
            this.radStartDir.Size = new System.Drawing.Size(117, 17);
            this.radStartDir.TabIndex = 1;
            this.radStartDir.TabStop = true;
            this.radStartDir.Text = "Webpage Directory";
            this.myToolTip.SetToolTip(this.radStartDir, "Generate a new image from a directory of files.");
            this.radStartDir.UseVisualStyleBackColor = true;
            this.radStartDir.CheckedChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(44, 18);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(57, 13);
            this.label5.TabIndex = 0;
            this.label5.Text = "Start With:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label8);
            this.groupBox2.Controls.Add(this.btnAdvanced);
            this.groupBox2.Controls.Add(this.radOutputMDD);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.radOutputASM30);
            this.groupBox2.Controls.Add(this.radOutputC18);
            this.groupBox2.Controls.Add(this.radOutputBIN);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(15, 105);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(15, 5, 15, 5);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(5, 5, 15, 10);
            this.groupBox2.Size = new System.Drawing.Size(545, 80);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Processing Options";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(44, 49);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(62, 13);
            this.label8.TabIndex = 6;
            this.label8.Text = "Processing:";
            // 
            // btnAdvanced
            // 
            this.btnAdvanced.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAdvanced.Location = new System.Drawing.Point(135, 44);
            this.btnAdvanced.Name = "btnAdvanced";
            this.btnAdvanced.Size = new System.Drawing.Size(125, 23);
            this.btnAdvanced.TabIndex = 5;
            this.btnAdvanced.Text = "Advanced Settings";
            this.btnAdvanced.UseVisualStyleBackColor = true;
            this.btnAdvanced.Click += new System.EventHandler(this.btnAdvanced_Click);
            // 
            // radOutputMDD
            // 
            this.radOutputMDD.AutoSize = true;
            this.radOutputMDD.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radOutputMDD.Location = new System.Drawing.Point(432, 21);
            this.radOutputMDD.Name = "radOutputMDD";
            this.radOutputMDD.Size = new System.Drawing.Size(50, 17);
            this.radOutputMDD.TabIndex = 4;
            this.radOutputMDD.Text = "MDD";
            this.myToolTip.SetToolTip(this.radOutputMDD, "Use this option to store pages on drive using MDD file system.");
            this.radOutputMDD.UseVisualStyleBackColor = true;
            this.radOutputMDD.CheckedChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // radOutputASM30
            // 
            this.radOutputASM30.AutoSize = true;
            this.radOutputASM30.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radOutputASM30.Location = new System.Drawing.Point(321, 21);
            this.radOutputASM30.Name = "radOutputASM30";
            this.radOutputASM30.Size = new System.Drawing.Size(92, 17);
            this.radOutputASM30.TabIndex = 3;
            this.radOutputASM30.Text = "ASM30 Image";
            this.myToolTip.SetToolTip(this.radOutputASM30, "Output an image compatible with C30 \r\nto be stored in internal Flash program \r\nme" +
                    "mory.  Remember to add the \r\ngenerated .S file to your project.");
            this.radOutputASM30.UseVisualStyleBackColor = true;
            this.radOutputASM30.CheckedChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // radOutputC18
            // 
            this.radOutputC18.AutoSize = true;
            this.radOutputC18.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radOutputC18.Location = new System.Drawing.Point(221, 21);
            this.radOutputC18.Name = "radOutputC18";
            this.radOutputC18.Size = new System.Drawing.Size(100, 17);
            this.radOutputC18.TabIndex = 2;
            this.radOutputC18.Text = "C18/C32 Image";
            this.myToolTip.SetToolTip(this.radOutputC18, "Output an image compatible with \r\nC18/C32 to be stored in internal Flash \r\nprogra" +
                    "m memory.");
            this.radOutputC18.UseVisualStyleBackColor = true;
            this.radOutputC18.CheckedChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // radOutputBIN
            // 
            this.radOutputBIN.AutoSize = true;
            this.radOutputBIN.Checked = true;
            this.radOutputBIN.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.radOutputBIN.Location = new System.Drawing.Point(135, 21);
            this.radOutputBIN.Name = "radOutputBIN";
            this.radOutputBIN.Size = new System.Drawing.Size(75, 17);
            this.radOutputBIN.TabIndex = 1;
            this.radOutputBIN.TabStop = true;
            this.radOutputBIN.Text = "BIN Image";
            this.myToolTip.SetToolTip(this.radOutputBIN, "Output a BIN image to be \r\nuploaded to external storage,\r\nsuch as EEPROM or seria" +
                    "l Flash.");
            this.radOutputBIN.UseVisualStyleBackColor = true;
            this.radOutputBIN.CheckedChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(44, 23);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(42, 13);
            this.label7.TabIndex = 0;
            this.label7.Text = "Output:";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.lblType);
            this.groupBox3.Controls.Add(this.txtImageName);
            this.groupBox3.Controls.Add(this.label10);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.btnProjectDir);
            this.groupBox3.Controls.Add(this.txtProjectDir);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox3.Location = new System.Drawing.Point(15, 195);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(15, 5, 15, 5);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(5, 5, 15, 10);
            this.groupBox3.Size = new System.Drawing.Size(545, 93);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Output Files";
            // 
            // lblType
            // 
            this.lblType.AutoSize = true;
            this.lblType.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblType.Location = new System.Drawing.Point(266, 63);
            this.lblType.Name = "lblType";
            this.lblType.Size = new System.Drawing.Size(34, 13);
            this.lblType.TabIndex = 5;
            this.lblType.Text = "(*.bin)";
            // 
            // txtImageName
            // 
            this.txtImageName.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtImageName.Location = new System.Drawing.Point(135, 60);
            this.txtImageName.Name = "txtImageName";
            this.txtImageName.Size = new System.Drawing.Size(125, 20);
            this.txtImageName.TabIndex = 4;
            this.myToolTip.SetToolTip(this.txtImageName, "File name for the image \r\nyou\'d like to create.");
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(44, 63);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(70, 13);
            this.label10.TabIndex = 3;
            this.label10.Text = "Image Name:";
            // 
            // btnProjectDir
            // 
            this.btnProjectDir.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnProjectDir.Location = new System.Drawing.Point(442, 32);
            this.btnProjectDir.Name = "btnProjectDir";
            this.btnProjectDir.Size = new System.Drawing.Size(75, 23);
            this.btnProjectDir.TabIndex = 2;
            this.btnProjectDir.Text = "Browse...";
            this.btnProjectDir.UseVisualStyleBackColor = true;
            this.btnProjectDir.Click += new System.EventHandler(this.btnProjectDir_Click);
            // 
            // txtProjectDir
            // 
            this.txtProjectDir.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.txtProjectDir.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.FileSystemDirectories;
            this.txtProjectDir.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtProjectDir.Location = new System.Drawing.Point(47, 34);
            this.txtProjectDir.Name = "txtProjectDir";
            this.txtProjectDir.Size = new System.Drawing.Size(389, 20);
            this.txtProjectDir.TabIndex = 1;
            this.myToolTip.SetToolTip(this.txtProjectDir, "Select your MPLAB project directory.  \r\nYour image file will be placed here, \r\nal" +
                    "ong with HTTPPrint.h if applicable.");
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(44, 18);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(88, 13);
            this.label9.TabIndex = 0;
            this.label9.Text = "Project Directory:";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.txtUploadDestination);
            this.groupBox4.Controls.Add(this.label4);
            this.groupBox4.Controls.Add(this.chkUpload);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Controls.Add(this.btnUploadSettings);
            this.groupBox4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox4.Location = new System.Drawing.Point(15, 298);
            this.groupBox4.Margin = new System.Windows.Forms.Padding(15, 5, 15, 10);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Padding = new System.Windows.Forms.Padding(5, 5, 15, 10);
            this.groupBox4.Size = new System.Drawing.Size(545, 67);
            this.groupBox4.TabIndex = 8;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Upload Settings";
            // 
            // txtUploadDestination
            // 
            this.txtUploadDestination.BackColor = System.Drawing.SystemColors.Window;
            this.txtUploadDestination.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtUploadDestination.Location = new System.Drawing.Point(68, 34);
            this.txtUploadDestination.Name = "txtUploadDestination";
            this.txtUploadDestination.ReadOnly = true;
            this.txtUploadDestination.Size = new System.Drawing.Size(368, 20);
            this.txtUploadDestination.TabIndex = 3;
            this.myToolTip.SetToolTip(this.txtUploadDestination, "Your MPFS image will be uploaded \r\nhere.  Use the Settings button to \r\nmodify thi" +
                    "s destination.");
            // 
            // chkUpload
            // 
            this.chkUpload.AutoSize = true;
            this.chkUpload.Checked = global::MPFS21.Properties.Settings.Default.UploadImageAfterBuild;
            this.chkUpload.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkUpload.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::MPFS21.Properties.Settings.Default, "UploadImageAfterBuild", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this.chkUpload.Location = new System.Drawing.Point(47, 37);
            this.chkUpload.Name = "chkUpload";
            this.chkUpload.Size = new System.Drawing.Size(15, 14);
            this.chkUpload.TabIndex = 2;
            this.myToolTip.SetToolTip(this.chkUpload, "Select this box to upload \r\nyour image upon generation.");
            this.chkUpload.UseVisualStyleBackColor = true;
            this.chkUpload.CheckedChanged += new System.EventHandler(this.CorrectDisplay);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(44, 18);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(92, 13);
            this.label11.TabIndex = 1;
            this.label11.Text = "Upload Image To:";
            // 
            // btnUploadSettings
            // 
            this.btnUploadSettings.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnUploadSettings.Location = new System.Drawing.Point(442, 32);
            this.btnUploadSettings.Name = "btnUploadSettings";
            this.btnUploadSettings.Size = new System.Drawing.Size(75, 23);
            this.btnUploadSettings.TabIndex = 0;
            this.btnUploadSettings.Text = "Settings";
            this.btnUploadSettings.UseVisualStyleBackColor = true;
            this.btnUploadSettings.Click += new System.EventHandler(this.btnUploadSettings_Click);
            // 
            // btnGenerate
            // 
            this.btnGenerate.BackColor = System.Drawing.SystemColors.Control;
            this.btnGenerate.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnGenerate.Location = new System.Drawing.Point(210, 378);
            this.btnGenerate.Margin = new System.Windows.Forms.Padding(210, 3, 3, 3);
            this.btnGenerate.Name = "btnGenerate";
            this.btnGenerate.Size = new System.Drawing.Size(160, 25);
            this.btnGenerate.TabIndex = 9;
            this.btnGenerate.Text = "Generate and Upload";
            this.btnGenerate.UseVisualStyleBackColor = false;
            this.btnGenerate.Click += new System.EventHandler(this.btnGenerate_Click);
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPanel1.Controls.Add(this.groupBox1);
            this.flowLayoutPanel1.Controls.Add(this.groupBox2);
            this.flowLayoutPanel1.Controls.Add(this.groupBox3);
            this.flowLayoutPanel1.Controls.Add(this.groupBox4);
            this.flowLayoutPanel1.Controls.Add(this.btnGenerate);
            this.flowLayoutPanel1.Controls.Add(this.lblAbout);
            this.flowLayoutPanel1.Controls.Add(this.myProgress);
            this.flowLayoutPanel1.Controls.Add(this.myStatusMsg);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(574, 435);
            this.flowLayoutPanel1.TabIndex = 10;
            // 
            // lblAbout
            // 
            this.lblAbout.AutoSize = true;
            this.lblAbout.Cursor = System.Windows.Forms.Cursors.Help;
            this.lblAbout.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblAbout.ForeColor = System.Drawing.Color.MediumBlue;
            this.lblAbout.Location = new System.Drawing.Point(473, 378);
            this.lblAbout.Margin = new System.Windows.Forms.Padding(100, 3, 3, 0);
            this.lblAbout.Name = "lblAbout";
            this.lblAbout.Size = new System.Drawing.Size(56, 26);
            this.lblAbout.TabIndex = 12;
            this.lblAbout.Text = "Version\r\nBuild Date";
            this.lblAbout.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this.lblAbout.Click += new System.EventHandler(this.lblAbout_Click);
            // 
            // myProgress
            // 
            this.myProgress.Location = new System.Drawing.Point(3, 416);
            this.myProgress.Margin = new System.Windows.Forms.Padding(3, 10, 3, 3);
            this.myProgress.MarqueeAnimationSpeed = 50;
            this.myProgress.Maximum = 120;
            this.myProgress.Name = "myProgress";
            this.myProgress.Size = new System.Drawing.Size(200, 18);
            this.myProgress.TabIndex = 10;
            // 
            // myStatusMsg
            // 
            this.myStatusMsg.AutoSize = true;
            this.myStatusMsg.Location = new System.Drawing.Point(209, 416);
            this.myStatusMsg.Margin = new System.Windows.Forms.Padding(3, 10, 3, 0);
            this.myStatusMsg.Name = "myStatusMsg";
            this.myStatusMsg.Size = new System.Drawing.Size(80, 13);
            this.myStatusMsg.TabIndex = 11;
            this.myStatusMsg.Text = "[Generator Idle]";
            // 
            // myToolTip
            // 
            this.myToolTip.AutoPopDelay = 10000;
            this.myToolTip.InitialDelay = 500;
            this.myToolTip.IsBalloon = true;
            this.myToolTip.ReshowDelay = 100;
            this.myToolTip.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.myToolTip.ToolTipTitle = "MPFS Generator Help";
            // 
            // MPFS21Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(574, 435);
            this.Controls.Add(this.flowLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MPFS21Form";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "Microchip MPFS Generator";
            this.Load += new System.EventHandler(this.MPFS21Form_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MPFS21Form_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnSourceDir;
        private System.Windows.Forms.TextBox txtSourceDir;
        private System.Windows.Forms.Label lblInput;
        private System.Windows.Forms.RadioButton radStartImg;
        private System.Windows.Forms.RadioButton radStartDir;
        private System.Windows.Forms.RadioButton radOutputBIN;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.RadioButton radOutputMDD;
        private System.Windows.Forms.RadioButton radOutputASM30;
        private System.Windows.Forms.RadioButton radOutputC18;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button btnAdvanced;
        private System.Windows.Forms.TextBox txtImageName;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button btnProjectDir;
        private System.Windows.Forms.TextBox txtProjectDir;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox txtUploadDestination;
        private System.Windows.Forms.CheckBox chkUpload;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button btnUploadSettings;
        private System.Windows.Forms.Button btnGenerate;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.ProgressBar myProgress;
        private System.Windows.Forms.Label myStatusMsg;
        private System.Windows.Forms.ToolTip myToolTip;
        private System.Windows.Forms.Label lblType;
        private System.Windows.Forms.TextBox txtSourceImage;
        private System.Windows.Forms.Label lblAbout;
    }
}

