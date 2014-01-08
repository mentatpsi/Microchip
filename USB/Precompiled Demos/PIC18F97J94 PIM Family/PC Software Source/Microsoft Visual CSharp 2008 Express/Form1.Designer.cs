namespace HID_PnP_Demo
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
            this.components = new System.ComponentModel.Container();
            this.PushbuttonState_lbl = new System.Windows.Forms.Label();
            this.ToggleLEDs_btn = new System.Windows.Forms.Button();
            this.ANxVoltage_lbl = new System.Windows.Forms.Label();
            this.StatusBox_lbl = new System.Windows.Forms.Label();
            this.StatusBox_txtbx = new System.Windows.Forms.TextBox();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.ReadWriteThread = new System.ComponentModel.BackgroundWorker();
            this.FormUpdateTimer = new System.Windows.Forms.Timer(this.components);
            this.ANxVoltageToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.ToggleLEDToolTip = new System.Windows.Forms.ToolTip(this.components);
            this.PushbuttonStateTooltip = new System.Windows.Forms.ToolTip(this.components);
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.toolTip2 = new System.Windows.Forms.ToolTip(this.components);
            this.RTCC_dtpkr = new System.Windows.Forms.DateTimePicker();
            this.LCD_Banner = new System.Windows.Forms.TextBox();
            this.LCD_lbl = new System.Windows.Forms.Label();
            this.RTCC_lbl = new System.Windows.Forms.Label();
            this.TogglePixels_btn = new System.Windows.Forms.Button();
            this.DisplayDate_chbx = new System.Windows.Forms.CheckBox();
            this.RTCC_Sync_btn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // PushbuttonState_lbl
            // 
            this.PushbuttonState_lbl.AutoSize = true;
            this.PushbuttonState_lbl.Enabled = false;
            this.PushbuttonState_lbl.Location = new System.Drawing.Point(131, 65);
            this.PushbuttonState_lbl.Name = "PushbuttonState_lbl";
            this.PushbuttonState_lbl.Size = new System.Drawing.Size(141, 13);
            this.PushbuttonState_lbl.TabIndex = 25;
            this.PushbuttonState_lbl.Text = "Pushbutton State: Unknown";
            // 
            // ToggleLEDs_btn
            // 
            this.ToggleLEDs_btn.Enabled = false;
            this.ToggleLEDs_btn.Location = new System.Drawing.Point(12, 60);
            this.ToggleLEDs_btn.Name = "ToggleLEDs_btn";
            this.ToggleLEDs_btn.Size = new System.Drawing.Size(96, 23);
            this.ToggleLEDs_btn.TabIndex = 24;
            this.ToggleLEDs_btn.Text = "ToggleLED(s)";
            this.ToggleLEDs_btn.UseVisualStyleBackColor = true;
            this.ToggleLEDs_btn.Click += new System.EventHandler(this.ToggleLEDs_btn_Click);
            // 
            // ANxVoltage_lbl
            // 
            this.ANxVoltage_lbl.AutoSize = true;
            this.ANxVoltage_lbl.Enabled = false;
            this.ANxVoltage_lbl.Location = new System.Drawing.Point(122, 105);
            this.ANxVoltage_lbl.Name = "ANxVoltage_lbl";
            this.ANxVoltage_lbl.Size = new System.Drawing.Size(93, 13);
            this.ANxVoltage_lbl.TabIndex = 23;
            this.ANxVoltage_lbl.Text = "ANx/POT Voltage";
            // 
            // StatusBox_lbl
            // 
            this.StatusBox_lbl.AutoSize = true;
            this.StatusBox_lbl.Location = new System.Drawing.Point(316, 15);
            this.StatusBox_lbl.Name = "StatusBox_lbl";
            this.StatusBox_lbl.Size = new System.Drawing.Size(37, 13);
            this.StatusBox_lbl.TabIndex = 22;
            this.StatusBox_lbl.Text = "Status";
            // 
            // StatusBox_txtbx
            // 
            this.StatusBox_txtbx.BackColor = System.Drawing.SystemColors.Window;
            this.StatusBox_txtbx.Location = new System.Drawing.Point(12, 12);
            this.StatusBox_txtbx.Name = "StatusBox_txtbx";
            this.StatusBox_txtbx.ReadOnly = true;
            this.StatusBox_txtbx.Size = new System.Drawing.Size(298, 20);
            this.StatusBox_txtbx.TabIndex = 21;
            // 
            // progressBar1
            // 
            this.progressBar1.BackColor = System.Drawing.Color.White;
            this.progressBar1.ForeColor = System.Drawing.Color.Red;
            this.progressBar1.Location = new System.Drawing.Point(12, 121);
            this.progressBar1.Maximum = 1024;
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(298, 23);
            this.progressBar1.Step = 1;
            this.progressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar1.TabIndex = 20;
            // 
            // ReadWriteThread
            // 
            this.ReadWriteThread.WorkerReportsProgress = true;
            this.ReadWriteThread.DoWork += new System.ComponentModel.DoWorkEventHandler(this.ReadWriteThread_DoWork);
            // 
            // FormUpdateTimer
            // 
            this.FormUpdateTimer.Enabled = true;
            this.FormUpdateTimer.Interval = 6;
            this.FormUpdateTimer.Tick += new System.EventHandler(this.FormUpdateTimer_Tick);
            // 
            // ANxVoltageToolTip
            // 
            this.ANxVoltageToolTip.AutomaticDelay = 20;
            this.ANxVoltageToolTip.AutoPopDelay = 20000;
            this.ANxVoltageToolTip.InitialDelay = 15;
            this.ANxVoltageToolTip.ReshowDelay = 15;
            // 
            // ToggleLEDToolTip
            // 
            this.ToggleLEDToolTip.AutomaticDelay = 2000;
            this.ToggleLEDToolTip.AutoPopDelay = 20000;
            this.ToggleLEDToolTip.InitialDelay = 15;
            this.ToggleLEDToolTip.ReshowDelay = 15;
            // 
            // PushbuttonStateTooltip
            // 
            this.PushbuttonStateTooltip.AutomaticDelay = 20;
            this.PushbuttonStateTooltip.AutoPopDelay = 20000;
            this.PushbuttonStateTooltip.InitialDelay = 15;
            this.PushbuttonStateTooltip.ReshowDelay = 15;
            // 
            // toolTip1
            // 
            this.toolTip1.AutomaticDelay = 2000;
            this.toolTip1.AutoPopDelay = 20000;
            this.toolTip1.InitialDelay = 15;
            this.toolTip1.ReshowDelay = 15;
            // 
            // toolTip2
            // 
            this.toolTip2.AutomaticDelay = 20;
            this.toolTip2.AutoPopDelay = 20000;
            this.toolTip2.InitialDelay = 15;
            this.toolTip2.ReshowDelay = 15;
            // 
            // RTCC_dtpkr
            // 
            this.RTCC_dtpkr.CustomFormat = "dddd, MMMM dd, yyyy        h:mm:ss tt";
            this.RTCC_dtpkr.Enabled = false;
            this.RTCC_dtpkr.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.RTCC_dtpkr.Location = new System.Drawing.Point(12, 263);
            this.RTCC_dtpkr.MaxDate = new System.DateTime(2099, 12, 31, 0, 0, 0, 0);
            this.RTCC_dtpkr.MinDate = new System.DateTime(2000, 1, 1, 0, 0, 0, 0);
            this.RTCC_dtpkr.Name = "RTCC_dtpkr";
            this.RTCC_dtpkr.Size = new System.Drawing.Size(298, 20);
            this.RTCC_dtpkr.TabIndex = 26;
            this.RTCC_dtpkr.Leave += new System.EventHandler(this.RTCC_dtpkr_Leave);
            this.RTCC_dtpkr.Enter += new System.EventHandler(this.RTCC_dtpkr_Enter);
            // 
            // LCD_Banner
            // 
            this.LCD_Banner.Enabled = false;
            this.LCD_Banner.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.LCD_Banner.Location = new System.Drawing.Point(12, 170);
            this.LCD_Banner.MaxLength = 40;
            this.LCD_Banner.Multiline = true;
            this.LCD_Banner.Name = "LCD_Banner";
            this.LCD_Banner.Size = new System.Drawing.Size(298, 41);
            this.LCD_Banner.TabIndex = 27;
            this.LCD_Banner.TextChanged += new System.EventHandler(this.LCD_Banner_TextChanged);
            // 
            // LCD_lbl
            // 
            this.LCD_lbl.AutoSize = true;
            this.LCD_lbl.Enabled = false;
            this.LCD_lbl.Location = new System.Drawing.Point(122, 154);
            this.LCD_lbl.Name = "LCD_lbl";
            this.LCD_lbl.Size = new System.Drawing.Size(65, 13);
            this.LCD_lbl.TabIndex = 28;
            this.LCD_lbl.Text = "LCD Banner";
            // 
            // RTCC_lbl
            // 
            this.RTCC_lbl.AutoSize = true;
            this.RTCC_lbl.Enabled = false;
            this.RTCC_lbl.Location = new System.Drawing.Point(122, 247);
            this.RTCC_lbl.Name = "RTCC_lbl";
            this.RTCC_lbl.Size = new System.Drawing.Size(36, 13);
            this.RTCC_lbl.TabIndex = 29;
            this.RTCC_lbl.Text = "RTCC";
            // 
            // TogglePixels_btn
            // 
            this.TogglePixels_btn.Enabled = false;
            this.TogglePixels_btn.Location = new System.Drawing.Point(12, 217);
            this.TogglePixels_btn.Name = "TogglePixels_btn";
            this.TogglePixels_btn.Size = new System.Drawing.Size(134, 23);
            this.TogglePixels_btn.TabIndex = 30;
            this.TogglePixels_btn.Text = "Toggle Special Pixels";
            this.TogglePixels_btn.UseVisualStyleBackColor = true;
            this.TogglePixels_btn.Click += new System.EventHandler(this.TogglePixels_btn_Click);
            // 
            // DisplayDate_chbx
            // 
            this.DisplayDate_chbx.AutoSize = true;
            this.DisplayDate_chbx.Checked = true;
            this.DisplayDate_chbx.CheckState = System.Windows.Forms.CheckState.Checked;
            this.DisplayDate_chbx.Enabled = false;
            this.DisplayDate_chbx.Location = new System.Drawing.Point(172, 217);
            this.DisplayDate_chbx.Name = "DisplayDate_chbx";
            this.DisplayDate_chbx.Size = new System.Drawing.Size(133, 17);
            this.DisplayDate_chbx.TabIndex = 31;
            this.DisplayDate_chbx.Text = "Display Date and Time";
            this.DisplayDate_chbx.UseVisualStyleBackColor = true;
            this.DisplayDate_chbx.CheckedChanged += new System.EventHandler(this.DisplayDate_chbx_CheckedChanged);
            // 
            // RTCC_Sync_btn
            // 
            this.RTCC_Sync_btn.Enabled = false;
            this.RTCC_Sync_btn.Location = new System.Drawing.Point(12, 289);
            this.RTCC_Sync_btn.Name = "RTCC_Sync_btn";
            this.RTCC_Sync_btn.Size = new System.Drawing.Size(134, 23);
            this.RTCC_Sync_btn.TabIndex = 32;
            this.RTCC_Sync_btn.Text = "RTCC Sync to PC";
            this.RTCC_Sync_btn.UseVisualStyleBackColor = true;
            this.RTCC_Sync_btn.Click += new System.EventHandler(this.RTCC_Sync_btn_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(359, 328);
            this.Controls.Add(this.RTCC_Sync_btn);
            this.Controls.Add(this.DisplayDate_chbx);
            this.Controls.Add(this.TogglePixels_btn);
            this.Controls.Add(this.RTCC_lbl);
            this.Controls.Add(this.LCD_lbl);
            this.Controls.Add(this.LCD_Banner);
            this.Controls.Add(this.RTCC_dtpkr);
            this.Controls.Add(this.PushbuttonState_lbl);
            this.Controls.Add(this.ToggleLEDs_btn);
            this.Controls.Add(this.ANxVoltage_lbl);
            this.Controls.Add(this.StatusBox_lbl);
            this.Controls.Add(this.StatusBox_txtbx);
            this.Controls.Add(this.progressBar1);
            this.Name = "Form1";
            this.Text = "PIC18F97J94 Family HID PnP Demo";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label PushbuttonState_lbl;
        private System.Windows.Forms.Button ToggleLEDs_btn;
        private System.Windows.Forms.Label ANxVoltage_lbl;
        private System.Windows.Forms.Label StatusBox_lbl;
        private System.Windows.Forms.TextBox StatusBox_txtbx;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.ComponentModel.BackgroundWorker ReadWriteThread;
        private System.Windows.Forms.Timer FormUpdateTimer;
        private System.Windows.Forms.ToolTip ANxVoltageToolTip;
        private System.Windows.Forms.ToolTip ToggleLEDToolTip;
        private System.Windows.Forms.ToolTip PushbuttonStateTooltip;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.ToolTip toolTip2;
        private System.Windows.Forms.DateTimePicker RTCC_dtpkr;
        private System.Windows.Forms.TextBox LCD_Banner;
        private System.Windows.Forms.Label LCD_lbl;
        private System.Windows.Forms.Label RTCC_lbl;
        private System.Windows.Forms.Button TogglePixels_btn;
        private System.Windows.Forms.CheckBox DisplayDate_chbx;
        private System.Windows.Forms.Button RTCC_Sync_btn;
    }
}

