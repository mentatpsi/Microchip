#pragma once
/********************************************************************
 FileName:		Form1.h
 Dependencies:	Windows SDK during development.  When compiled, needs
				.NET framework 3.5 redistributable to run
 Processor:		x86 
 Hardware:		Need a free USB port to connect USB peripheral device
 Compiler:  	Microsoft Visual C++ 2008 Express Edition (or better)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively with Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   06/11/2009   Initial release

This demonstrates the HID USB DLL interface.

The DLL has four functions:
void USBHIDClassInit (VID, PID, Size) - Sets the Vendor and Product Id used in the class.
                                      Also specifies the Report Size.  This must match
									  the size specified in the devices report descriptor.

bool USBHIDWriteReport (buffer, len)  - Sends a buffer to the device.  len specifies the
                                      number of valid bytes in the buffer.  The data is 
									  padded out to the report size with 0xFF.

bool USBHIDReadReport (buffer)        - Reads a packet from the device and returns it the
                                      buffer.  buffer[0] contains the first byte from the device.

bool IsConnected()                    - Checks with the OS to see if the device is connected
									  (VID & PID specified by USBHIDClassInit).  Returns
									  true if the device is enumerated.  This is an OS
									  inquiry only and does not generate any traffic on the bus.

ReadReport and WriteReport functions use non-blocking IO calls.  That is the function
will return after one second even if the call fails.  The functions return a boolean indicating 
success or failure.  This allows the application to gracefully recover when the device is 
disconnected rather than hang.

*********************************************************************************************/

namespace GenericHIDDLLSimpleDemo {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace HIDClass;



	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			MCHPHIDClass::USBHIDClassInit (0x4D8, 0x003F, 64);
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  ToggleLED_btn;
	protected: 

	private: System::Windows::Forms::Button^  GetPushbuttonState_btn;
	private: System::Windows::Forms::Label^  StateLabel;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  lblDeviceState;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->ToggleLED_btn = (gcnew System::Windows::Forms::Button());
			this->GetPushbuttonState_btn = (gcnew System::Windows::Forms::Button());
			this->StateLabel = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->lblDeviceState = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// ToggleLED_btn
			// 
			this->ToggleLED_btn->Location = System::Drawing::Point(13, 47);
			this->ToggleLED_btn->Name = L"ToggleLED_btn";
			this->ToggleLED_btn->Size = System::Drawing::Size(125, 23);
			this->ToggleLED_btn->TabIndex = 1;
			this->ToggleLED_btn->Text = L"Toggle LED(s)";
			this->ToggleLED_btn->UseVisualStyleBackColor = true;
			this->ToggleLED_btn->Click += gcnew System::EventHandler(this, &Form1::ToggleLED_btn_Click);
			// 
			// GetPushbuttonState_btn
			// 
			this->GetPushbuttonState_btn->Location = System::Drawing::Point(13, 77);
			this->GetPushbuttonState_btn->Name = L"GetPushbuttonState_btn";
			this->GetPushbuttonState_btn->Size = System::Drawing::Size(125, 23);
			this->GetPushbuttonState_btn->TabIndex = 2;
			this->GetPushbuttonState_btn->Text = L"Get Pushbutton State";
			this->GetPushbuttonState_btn->UseVisualStyleBackColor = true;
			this->GetPushbuttonState_btn->Click += gcnew System::EventHandler(this, &Form1::GetPushbuttonState_btn_Click);
			// 
			// StateLabel
			// 
			this->StateLabel->AutoSize = true;
			this->StateLabel->Location = System::Drawing::Point(144, 82);
			this->StateLabel->Name = L"StateLabel";
			this->StateLabel->Size = System::Drawing::Size(84, 13);
			this->StateLabel->TabIndex = 3;
			this->StateLabel->Text = L"State: Unknown";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->lblDeviceState);
			this->groupBox1->Location = System::Drawing::Point(13, 10);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(125, 31);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L" Device State ";
			// 
			// lblDeviceState
			// 
			this->lblDeviceState->AutoSize = true;
			this->lblDeviceState->Location = System::Drawing::Point(14, 16);
			this->lblDeviceState->Name = L"lblDeviceState";
			this->lblDeviceState->Size = System::Drawing::Size(73, 13);
			this->lblDeviceState->TabIndex = 0;
			this->lblDeviceState->Text = L"Disconnected";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(346, 117);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->StateLabel);
			this->Controls->Add(this->GetPushbuttonState_btn);
			this->Controls->Add(this->ToggleLED_btn);
			this->Name = L"Form1";
			this->Text = L"Generic HID Class DLL Simple Demo";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 // OS Inquiry to see if the device is connected
			 if (MCHPHIDClass::USBHIDIsConnected () == true)
			 {
				 lblDeviceState->Text = "Connected";
			 }
			 else
			 {
				 lblDeviceState->Text = "Disconnected";	
			 }
		 }

private: System::Void ToggleLED_btn_Click(System::Object^  sender, System::EventArgs^  e) {
		 unsigned char OutputPacketBuffer[64];	//Allocate a memory buffer equal to our report size

		 OutputPacketBuffer[0] = 0x80;		//0x80 is the "Toggle LED(s)" command in the firmware
		 if (MCHPHIDClass::USBHIDWriteReport (OutputPacketBuffer, 1) == true)
		 {
			 lblDeviceState->Text = "Connected";
		 }
		 else
		 {
			 lblDeviceState->Text = "Disconnected";	
		 }
		 }


private: System::Void GetPushbuttonState_btn_Click(System::Object^  sender, System::EventArgs^  e) {
		 unsigned char OutputPacketBuffer[64];	//Allocate a memory buffer equal to our report size
		 unsigned char InputPacketBuffer[64];	//Allocate a memory buffer equal to our report size

		 OutputPacketBuffer[0] = 0x81;			//0x81 is the "Get Pushbutton State" command in the firmware

		 if (MCHPHIDClass::USBHIDWriteReport (OutputPacketBuffer, 1) == true)
		 {
			 lblDeviceState->Text = "Connected";	
			 MCHPHIDClass::USBHIDReadReport (InputPacketBuffer);
			 //InputPacketBuffer[0] is an echo back of the command.
			 //InputPacketBuffer[1] contains the I/O port pin value for the pushbutton.  
			 if(InputPacketBuffer[1] == 0x01)	
			 {
				 StateLabel->Text = "State: Not Pressed";		//Update the pushbutton state text label on the form, so the user can see the result 
			 }
			 else //InputPacketBuffer[1] must have been == 0x00 instead
			 {
				 StateLabel->Text = "State: Pressed";			//Update the pushbutton state text label on the form, so the user can see the result 
			 }
		 }
		 else
		 {
			 lblDeviceState->Text = "Disconnected";	
		 }

		 }
};
}

