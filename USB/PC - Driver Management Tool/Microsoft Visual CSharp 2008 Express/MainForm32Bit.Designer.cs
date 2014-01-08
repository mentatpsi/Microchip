namespace DriverManagementTool
{
    partial class MainForm32Bit
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm32Bit));
            this.Install_btn = new System.Windows.Forms.Button();
            this.Remove_btn = new System.Windows.Forms.Button();
            this.OutputStatus_lstbx = new System.Windows.Forms.ListBox();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.SuspendLayout();
            // 
            // Install_btn
            // 
            this.Install_btn.Location = new System.Drawing.Point(12, 12);
            this.Install_btn.Name = "Install_btn";
            this.Install_btn.Size = new System.Drawing.Size(98, 28);
            this.Install_btn.TabIndex = 0;
            this.Install_btn.Text = "Install Drivers";
            this.Install_btn.UseVisualStyleBackColor = true;
            this.Install_btn.Click += new System.EventHandler(this.Install_btn_Click);
            // 
            // Remove_btn
            // 
            this.Remove_btn.Location = new System.Drawing.Point(126, 12);
            this.Remove_btn.Name = "Remove_btn";
            this.Remove_btn.Size = new System.Drawing.Size(98, 28);
            this.Remove_btn.TabIndex = 1;
            this.Remove_btn.Text = "Remove Drivers";
            this.Remove_btn.UseVisualStyleBackColor = true;
            this.Remove_btn.Click += new System.EventHandler(this.Remove_btn_Click);
            // 
            // OutputStatus_lstbx
            // 
            this.OutputStatus_lstbx.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.OutputStatus_lstbx.FormattingEnabled = true;
            this.OutputStatus_lstbx.Location = new System.Drawing.Point(13, 67);
            this.OutputStatus_lstbx.Name = "OutputStatus_lstbx";
            this.OutputStatus_lstbx.Size = new System.Drawing.Size(369, 69);
            this.OutputStatus_lstbx.TabIndex = 2;
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            this.backgroundWorker1.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorker1_RunWorkerCompleted);
            // 
            // MainForm32Bit
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(394, 148);
            this.Controls.Add(this.OutputStatus_lstbx);
            this.Controls.Add(this.Remove_btn);
            this.Controls.Add(this.Install_btn);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm32Bit";
            this.Text = "USB Driver Management Tool";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Install_btn;
        private System.Windows.Forms.Button Remove_btn;
        private System.Windows.Forms.ListBox OutputStatus_lstbx;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
    }
}

