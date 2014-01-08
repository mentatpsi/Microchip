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
        Me.btnSendData = New System.Windows.Forms.Button
        Me.btnConnect = New System.Windows.Forms.Button
        Me.btnClose = New System.Windows.Forms.Button
        Me.lstCOMPorts = New System.Windows.Forms.ComboBox
        Me.txtDataReceived = New System.Windows.Forms.TextBox
        Me.txtData = New System.Windows.Forms.TextBox
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.SuspendLayout()
        '
        'btnSendData
        '
        Me.btnSendData.Location = New System.Drawing.Point(12, 41)
        Me.btnSendData.Name = "btnSendData"
        Me.btnSendData.Size = New System.Drawing.Size(75, 23)
        Me.btnSendData.TabIndex = 0
        Me.btnSendData.Text = "Send Data"
        Me.btnSendData.UseVisualStyleBackColor = True
        '
        'btnConnect
        '
        Me.btnConnect.Location = New System.Drawing.Point(12, 12)
        Me.btnConnect.Name = "btnConnect"
        Me.btnConnect.Size = New System.Drawing.Size(75, 23)
        Me.btnConnect.TabIndex = 1
        Me.btnConnect.Text = "Connect"
        Me.btnConnect.UseVisualStyleBackColor = True
        '
        'btnClose
        '
        Me.btnClose.Enabled = False
        Me.btnClose.Location = New System.Drawing.Point(220, 12)
        Me.btnClose.Name = "btnClose"
        Me.btnClose.Size = New System.Drawing.Size(75, 23)
        Me.btnClose.TabIndex = 2
        Me.btnClose.Text = "Close"
        Me.btnClose.UseVisualStyleBackColor = True
        '
        'lstCOMPorts
        '
        Me.lstCOMPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.lstCOMPorts.FormattingEnabled = True
        Me.lstCOMPorts.Location = New System.Drawing.Point(93, 14)
        Me.lstCOMPorts.Name = "lstCOMPorts"
        Me.lstCOMPorts.Size = New System.Drawing.Size(121, 21)
        Me.lstCOMPorts.TabIndex = 3
        '
        'txtDataReceived
        '
        Me.txtDataReceived.Location = New System.Drawing.Point(12, 70)
        Me.txtDataReceived.Multiline = True
        Me.txtDataReceived.Name = "txtDataReceived"
        Me.txtDataReceived.Size = New System.Drawing.Size(470, 184)
        Me.txtDataReceived.TabIndex = 4
        '
        'txtData
        '
        Me.txtData.Location = New System.Drawing.Point(93, 44)
        Me.txtData.Name = "txtData"
        Me.txtData.Size = New System.Drawing.Size(389, 20)
        Me.txtData.TabIndex = 5
        '
        'SerialPort1
        '
        '
        'Timer1
        '
        Me.Timer1.Enabled = True
        Me.Timer1.Interval = 1
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(494, 266)
        Me.Controls.Add(Me.txtData)
        Me.Controls.Add(Me.txtDataReceived)
        Me.Controls.Add(Me.lstCOMPorts)
        Me.Controls.Add(Me.btnClose)
        Me.Controls.Add(Me.btnConnect)
        Me.Controls.Add(Me.btnSendData)
        Me.Name = "Form1"
        Me.Text = "VB.net COM port example"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents btnSendData As System.Windows.Forms.Button
    Friend WithEvents btnConnect As System.Windows.Forms.Button
    Friend WithEvents btnClose As System.Windows.Forms.Button
    Friend WithEvents lstCOMPorts As System.Windows.Forms.ComboBox
    Friend WithEvents txtDataReceived As System.Windows.Forms.TextBox
    Friend WithEvents txtData As System.Windows.Forms.TextBox
    Friend WithEvents SerialPort1 As System.IO.Ports.SerialPort
    Friend WithEvents Timer1 As System.Windows.Forms.Timer

End Class
