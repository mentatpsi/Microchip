<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.PushbuttonState_lbl = New System.Windows.Forms.Label
        Me.ToggleLEDs_btn = New System.Windows.Forms.Button
        Me.ANxVoltage_lbl = New System.Windows.Forms.Label
        Me.StatusBox_lbl = New System.Windows.Forms.Label
        Me.StatusBox_txtbx = New System.Windows.Forms.TextBox
        Me.progressBar1 = New System.Windows.Forms.ProgressBar
        Me.FormUpdateTimer = New System.Windows.Forms.Timer(Me.components)
        Me.ANxVoltageToolTip = New System.Windows.Forms.ToolTip(Me.components)
        Me.ToggleLEDToolTip = New System.Windows.Forms.ToolTip(Me.components)
        Me.PushbuttonStateTooltip = New System.Windows.Forms.ToolTip(Me.components)
        Me.ReadWriteThread = New System.ComponentModel.BackgroundWorker
        Me.SuspendLayout()
        '
        'PushbuttonState_lbl
        '
        Me.PushbuttonState_lbl.AutoSize = True
        Me.PushbuttonState_lbl.Enabled = False
        Me.PushbuttonState_lbl.Location = New System.Drawing.Point(131, 65)
        Me.PushbuttonState_lbl.Name = "PushbuttonState_lbl"
        Me.PushbuttonState_lbl.Size = New System.Drawing.Size(141, 13)
        Me.PushbuttonState_lbl.TabIndex = 25
        Me.PushbuttonState_lbl.Text = "Pushbutton State: Unknown"
        '
        'ToggleLEDs_btn
        '
        Me.ToggleLEDs_btn.Enabled = False
        Me.ToggleLEDs_btn.Location = New System.Drawing.Point(12, 60)
        Me.ToggleLEDs_btn.Name = "ToggleLEDs_btn"
        Me.ToggleLEDs_btn.Size = New System.Drawing.Size(96, 23)
        Me.ToggleLEDs_btn.TabIndex = 24
        Me.ToggleLEDs_btn.Text = "ToggleLED(s)"
        Me.ToggleLEDs_btn.UseVisualStyleBackColor = True
        '
        'ANxVoltage_lbl
        '
        Me.ANxVoltage_lbl.AutoSize = True
        Me.ANxVoltage_lbl.Enabled = False
        Me.ANxVoltage_lbl.Location = New System.Drawing.Point(122, 105)
        Me.ANxVoltage_lbl.Name = "ANxVoltage_lbl"
        Me.ANxVoltage_lbl.Size = New System.Drawing.Size(93, 13)
        Me.ANxVoltage_lbl.TabIndex = 23
        Me.ANxVoltage_lbl.Text = "ANx/POT Voltage"
        '
        'StatusBox_lbl
        '
        Me.StatusBox_lbl.AutoSize = True
        Me.StatusBox_lbl.Location = New System.Drawing.Point(316, 15)
        Me.StatusBox_lbl.Name = "StatusBox_lbl"
        Me.StatusBox_lbl.Size = New System.Drawing.Size(37, 13)
        Me.StatusBox_lbl.TabIndex = 22
        Me.StatusBox_lbl.Text = "Status"
        '
        'StatusBox_txtbx
        '
        Me.StatusBox_txtbx.BackColor = System.Drawing.SystemColors.Window
        Me.StatusBox_txtbx.Location = New System.Drawing.Point(12, 12)
        Me.StatusBox_txtbx.Name = "StatusBox_txtbx"
        Me.StatusBox_txtbx.ReadOnly = True
        Me.StatusBox_txtbx.Size = New System.Drawing.Size(298, 20)
        Me.StatusBox_txtbx.TabIndex = 21
        '
        'progressBar1
        '
        Me.progressBar1.BackColor = System.Drawing.Color.White
        Me.progressBar1.ForeColor = System.Drawing.Color.Red
        Me.progressBar1.Location = New System.Drawing.Point(12, 121)
        Me.progressBar1.Maximum = 1024
        Me.progressBar1.Name = "progressBar1"
        Me.progressBar1.Size = New System.Drawing.Size(298, 23)
        Me.progressBar1.Step = 1
        Me.progressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous
        Me.progressBar1.TabIndex = 20
        '
        'FormUpdateTimer
        '
        Me.FormUpdateTimer.Enabled = True
        Me.FormUpdateTimer.Interval = 6
        '
        'ANxVoltageToolTip
        '
        Me.ANxVoltageToolTip.AutomaticDelay = 20
        Me.ANxVoltageToolTip.AutoPopDelay = 20000
        Me.ANxVoltageToolTip.InitialDelay = 15
        Me.ANxVoltageToolTip.ReshowDelay = 15
        '
        'ToggleLEDToolTip
        '
        Me.ToggleLEDToolTip.AutomaticDelay = 2000
        Me.ToggleLEDToolTip.AutoPopDelay = 20000
        Me.ToggleLEDToolTip.InitialDelay = 15
        Me.ToggleLEDToolTip.ReshowDelay = 15
        '
        'PushbuttonStateTooltip
        '
        Me.PushbuttonStateTooltip.AutomaticDelay = 20
        Me.PushbuttonStateTooltip.AutoPopDelay = 20000
        Me.PushbuttonStateTooltip.InitialDelay = 15
        Me.PushbuttonStateTooltip.ReshowDelay = 15
        '
        'ReadWriteThread
        '
        Me.ReadWriteThread.WorkerReportsProgress = True
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(356, 158)
        Me.Controls.Add(Me.PushbuttonState_lbl)
        Me.Controls.Add(Me.ToggleLEDs_btn)
        Me.Controls.Add(Me.ANxVoltage_lbl)
        Me.Controls.Add(Me.StatusBox_lbl)
        Me.Controls.Add(Me.StatusBox_txtbx)
        Me.Controls.Add(Me.progressBar1)
        Me.Name = "Form1"
        Me.Text = "Generic HID PnP Demo"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents PushbuttonState_lbl As System.Windows.Forms.Label
    Private WithEvents ToggleLEDs_btn As System.Windows.Forms.Button
    Private WithEvents ANxVoltage_lbl As System.Windows.Forms.Label
    Private WithEvents StatusBox_lbl As System.Windows.Forms.Label
    Private WithEvents StatusBox_txtbx As System.Windows.Forms.TextBox
    Private WithEvents progressBar1 As System.Windows.Forms.ProgressBar
    Private WithEvents FormUpdateTimer As System.Windows.Forms.Timer
    Private WithEvents ToggleLEDToolTip As System.Windows.Forms.ToolTip
    Private WithEvents PushbuttonStateTooltip As System.Windows.Forms.ToolTip
    Private WithEvents ReadWriteThread As System.ComponentModel.BackgroundWorker
    Private WithEvents ANxVoltageToolTip As System.Windows.Forms.ToolTip

End Class
