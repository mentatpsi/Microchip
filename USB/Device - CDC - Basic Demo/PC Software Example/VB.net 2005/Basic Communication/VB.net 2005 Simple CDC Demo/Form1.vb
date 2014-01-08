'Here are some useful articles when creating new PC applications for COM ports:
'(links valid as of June 3, 2008)
'
'"SerialPort Class"
'http://msdn.microsoft.com/en-us/library/system.io.ports.serialport.aspx
'
'"SerialPort Members"
'http://msdn.microsoft.com/en-us/library/system.io.ports.serialport_members.aspx
'
'"SerialPort.DataReceived Event"
'http://msdn.microsoft.com/en-us/library/system.io.ports.serialport.datareceived.aspx
'
'"How to: Make Thread-Safe Calls to Windows Forms Controls"
'http://msdn.microsoft.com/en-us/library/ms171728(VS.80).aspx
'
'"Visual Basic Developer Center"
'http://msdn.microsoft.com/en-us/vbasic/default.aspx?pull=/library/en-us/vbcon/html/vboriintroductiontovisualbasic70forvisualbasicveterans.asp

Imports System.IO.Ports

Public Class Form1

    'Create a delegate function for this thread that will take
    '  in a string and will write it to the txtDataReceived textbox
    Delegate Sub SetTextCallback(ByVal [text] As String)

    '****************************************************************************
    '   Function:
    '       private void UpdateCOMPortList()
    '
    '   Summary:
    '       This function updates the COM ports listbox.
    '
    '   Description:
    '       This function updates the COM ports listbox.  This function is launched 
    '       periodically based on its Interval attribute (set in the form editor under
    '       the properties window).
    '
    '   Precondition:
    '       None
    '
    '   Parameters:
    '       None
    '
    '   Return Values
    '       None
    '
    '   Remarks:
    '       None
    '***************************************************************************
    Private Sub UpdateCOMPortList()
        Dim s As String
        Dim i As Integer
        Dim foundDifference As Boolean

        i = 0
        foundDifference = False
        'If the number of COM ports is different than the last time we
        '  checked, then we know that the COM ports have changed and we
        '  don't need to verify each entry.

        If lstCOMPorts.Items.Count = SerialPort.GetPortNames().Length Then
            'Search the entire SerialPort object.  Look at COM port name
            '  returned and see if it already exists in the list.
            For Each s In SerialPort.GetPortNames()
                'If any of the names have changed then we need to update 
                '  the list
                If lstCOMPorts.Items(i).Equals(s) = False Then
                    foundDifference = True
                End If
                i = i + 1
            Next s
        Else
            foundDifference = True
        End If

        'If nothing has changed, exit the function.
        If foundDifference = False Then
            Exit Sub
        End If

        'If something has changed, then clear the list
        lstCOMPorts.Items.Clear()

        'Add all of the current COM ports to the list
        For Each s In SerialPort.GetPortNames()
            lstCOMPorts.Items.Add(s)
        Next s
        'Set the listbox to point to the first entry in the list
        lstCOMPorts.SelectedIndex = 0
    End Sub


    '****************************************************************************
    '   Function:
    '       private void timer1_Tick(object sender, EventArgs e)
    '
    '   Summary:
    '       This function updates the COM ports listbox.
    '
    '   Description:
    '       This function updates the COM ports listbox.  This function is launched 
    '       periodically based on its Interval attribute (set in the form editor under
    '       the properties window).
    '
    '   Precondition:
    '       None
    '
    '   Parameters:
    '       object sender     - Sender of the event (this form)
    '       EventArgs e       - The event arguments
    '
    '   Return Values
    '       None
    '
    '   Remarks:
    '       None
    '***************************************************************************/

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        'Update the COM ports list so that we can detect
        '  new COM ports that have been added.
        UpdateCOMPortList()
    End Sub


    '****************************************************************************
    '   Function:
    '       private void btnConnect_Click(object sender, EventArgs e)
    '
    '   Summary:
    '       This function opens the COM port.
    '
    '   Description:
    '       This function opens the COM port.  This function is launched when the 
    '       btnConnect button is clicked.  In addition to opening the COM port, this 
    '       function will also change the Enable attribute of several of the form
    '       objects to disable the user from opening a new COM port.
    '
    '   Precondition:
    '       None
    '
    '   Parameters:
    '       object sender     - Sender of the event (this form)
    '       EventArgs e       - The event arguments
    '
    '   Return Values
    '       None
    '
    '   Remarks:
    '       None
    '***************************************************************************/
    Private Sub btnConnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnConnect.Click
        'This section of code will try to open the COM port.
        '  Please note that it is important to use a try/catch
        '  statement when opening the COM port.  If a USB virtual
        '  COM port is removed and the PC software tries to open
        '  the COM port before it detects its removal then
        '  an exeception is thrown.  If the execption is not in a
        '  try/catch statement this could result in the application
        '  crashing.
        Try
            'Get the port name from the application list box.
            '  the PortName attribute is a string name of the COM
            '  port (e.g. - "COM1").
            SerialPort1.PortName = lstCOMPorts.Items(lstCOMPorts.SelectedIndex).ToString()

            'Open the COM port.
            SerialPort1.Open()

            'Change the state of the application objects
            btnConnect.Enabled = False
            lstCOMPorts.Enabled = False
            btnClose.Enabled = True

            'Clear the textbox and print that we are connected.
            txtDataReceived.Clear()
            txtDataReceived.AppendText("Connected." + vbCrLf)
        Catch ex As Exception
            'If there was an exception, then close the handle to 
            '  the device and assume that the device was removed
            btnClose_Click(Me, e)

        End Try
    End Sub


    '****************************************************************************
    '   Function:
    '       private void btnClose_Click(object sender, EventArgs e)
    '
    '   Summary:
    '       This function closes the COM port.
    '
    '   Description:
    '       This function closes the COM port.  This function is launched when the 
    '       btnClose button is clicked.  This function can also be called directly
    '       from other functions.  In addition to closing the COM port, this 
    '       function will also change the Enable attribute of several of the form
    '       objects to enable the user to open a new COM port.
    '
    '   Precondition:
    '       None
    '
    '   Parameters:
    '       object sender     - Sender of the event (this form)
    '       EventArgs e       - The event arguments
    '
    '   Return Values
    '       None
    '
    '   Remarks:
    '       None
    '***************************************************************************/
    Private Sub btnClose_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnClose.Click
        'Reset the state of the application objects
        btnClose.Enabled = False
        btnConnect.Enabled = True
        lstCOMPorts.Enabled = True

        'This section of code will try to close the COM port.
        '  Please note that it is important to use a try/catch
        '  statement when closing the COM port.  If a USB virtual
        '  COM port is removed and the PC software tries to close
        '  the COM port before it detects its removal then
        '  an exeception is thrown.  If the execption is not in a
        '  try/catch statement this could result in the application
        '  crashing.
        Try

            'Dispose the In and Out buffers;
            SerialPort1.DiscardInBuffer()
            SerialPort1.DiscardOutBuffer()
            'Close the COM port
            SerialPort1.Close()
            'If there was an exeception then there isn't much we can
            '  do.  The port is no longer available.
        Catch ex As Exception

        End Try
    End Sub


    '****************************************************************************
    '   Function:
    '       private void serialPort1_DataReceived(  object sender, 
    '                                               SerialDataReceivedEventArgs e)
    '
    '   Summary:
    '       This function prints any data received on the COM port.
    '
    '   Description:
    '       This function is called when the data is received on the COM port.  This
    '       function attempts to write that data to the txtDataReceived textbox.  If
    '       an exception occurs the btnClose_Click() function is called in order to
    '       close the COM port that caused the exception.
    '   
    '   Precondition:
    '       None
    '
    '   Parameters:
    '       object sender     - Sender of the event (this form)
    '       SerialDataReceivedEventArgs e       - The event arguments
    '
    '   Return Values
    '       None
    '
    '   Remarks:
    '       None
    '***************************************************************************/
    Private Sub SerialPort1_DataReceived(ByVal sender As System.Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived
        'The ReadExisting() function will read all of the data that
        '  is currently available in the COM port buffer.  In this 
        '  example we are sending all of the available COM port data
        '  to the SetText() function.
        '
        '  NOTE: the <SerialPort>_DataReceived() function is launched
        '  in a seperate thread from the rest of the application.  A
        '  delegate function is required in order to properly access
        '  any managed objects inside of the other thread.  Since we
        '  will be writing to a textBox (a managed object) the delegate
        '  function is required.  Please see the SetText() function for 
        '  more information about delegate functions and how to use them.
        Try
            SetText(SerialPort1.ReadExisting())
        Catch ex As Exception
            'If there was an exception, then close the handle to 
            '  the device and assume that the device was removed
            btnClose_Click(Me, e)
        End Try

    End Sub


    '****************************************************************************
    '   Function:
    '       private void SetText(string text)
    '
    '   Summary:
    '       This function prints the input text to the txtDataReceived textbox.
    '
    '   Description:
    '       This function prints the input text to the txtDataReceived textbox.  If
    '       the calling thread is the same as the thread that owns the textbox, then
    '       the AppendText() method is called directly.  If a thread other than the
    '       main thread calls this function, then an instance of the delegate function
    '       is created so that the function runs again in the main thread.
    '
    '   Precondition:
    '       None
    '
    '   Parameters:
    '       string text     - Text that needs to be printed to the textbox
    '
    '   Return Values
    '       None
    '
    '   Remarks:
    '       None
    '***************************************************************************/
    Private Sub SetText(ByVal [text] As String)
        'InvokeRequired required compares the thread ID of the
        '  calling thread to the thread ID of the creating thread.
        '  If these threads are different, it returns true.  We can
        '  use this attribute to determine if we can append text
        '  directly to the textbox or if we must launch an a delegate
        '  function instance to write to the textbox.
        If txtDataReceived.InvokeRequired Then
            'InvokeRequired returned TRUE meaning that this function
            '  was called from a thread different than the current
            '  thread.  We must launch a deleage function.

            'Create an instance of the SetTextCallback delegate and
            '  assign the delegate function to be this function.  This
            '  effectively causes this same SetText() function to be
            '  called within the main thread instead of the second
            '  thread.
            Dim d As New SetTextCallback(AddressOf SetText)

            'Invoke the new delegate sending the same text to the
            '  delegate that was passed into this function from the
            '  other thread.
            Invoke(d, New Object() {[text]})
        Else
            'If this function was called from the same thread that 
            '  holds the required objects then just add the text.
            txtDataReceived.AppendText(text)
        End If
    End Sub


    '****************************************************************************
    '   Function:
    '       private void btnSendData_Click(object sender, EventArgs e)
    '
    '   Summary:
    '       This function will attempt to send the contents of txtData over the COM port
    '
    '   Description:
    '       This function is called when the btnSendData button is clicked.  It will 
    '       attempt to send the contents of txtData over the COM port.  If the attempt
    '       is unsuccessful this function will call the btnClose_Click() in order to
    '       close the COM port that just failed.
    '
    '   Precondition:
    '       None
    '
    '   Parameters:
    '       object sender     - Sender of the event (this form)
    '       EventArgs e       - The event arguments
    '
    '   Return Values
    '       None
    '
    '   Remarks:
    '       None
    '***************************************************************************/
    Private Sub btnSendData_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSendData.Click
        'This section of code will try to write to the COM port.
        '  Please note that it is important to use a try/catch
        '  statement when writing to the COM port.  If a USB virtual
        '  COM port is removed and the PC software tries to write
        '  to the COM port before it detects its removal then
        '  an exeception is thrown.  If the execption is not in a
        '  try/catch statement this could result in the application
        '  crashing.
        Try
            'Write the data in the text box to the open serial port
            SerialPort1.Write(txtData.Text)
        Catch ex As Exception
            'If there was an exception, then close the handle to 
            '  the device and assume that the device was removed
            btnClose_Click(Me, e)
        End Try
    End Sub
End Class
