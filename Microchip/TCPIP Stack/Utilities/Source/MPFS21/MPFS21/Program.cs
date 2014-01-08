/*********************************************************************
 *
 *    Application Launch Point and CLI for MPFS21
 *
 *********************************************************************
 * FileName:        Program.cs
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
using System.Windows.Forms;
using System.Drawing;
using Microchip;

namespace MPFS21
{
	
    static class Program
    {
        
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(String[] args)
        {
        	// If no arguments, show the GUI
            if (args.Length == 0)
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MPFS21Form());
            }
            // Operate in command line mode
            else
            {

                // Make sure we got at least 3 parameters
                if (args.Length < 3)
                {
                    MessageBox.Show(
                        "Usage: MPFS2 [options] <SourceDir> <ProjectDir> <OutputFile>\n" +
                        "    /BIN\t\t(/b)\t: Output a BIN image (Default)\n" +
                        "    /C18\t\t(/c)\t: Output a C18 image\n" +
                        "    /ASM30\t(/s)\t: Output an ASM30 image\n" +
                        "    /C32\t\t(/x)\t: Output a C32 image\n" +
                        "    /classic\t\t(/1)\t: MPFSClassic format\n" +
                        "    /mpfs2\t\t(/2)\t: MPFS2 format (Default)\n" +
                        "    /reserve #\t(/r #)\t: Reserved space for Classic BINs (Default 64)\n" +
                        "    /html \"...\"\t(/h)\t: Dynamic file types (\"*.htm, *.html, *.xml, *.cgi\")\n" +
                        "    /xgzip \"...\"\t(/z)\t: Non-compressible types (\"snmp.bib, *.inc\")\n\n" +
                        "SourceDir, ProjectDir, and OutputFile are required and should be enclosed in quotes.\n" +
                        "OutputFile is placed relative to ProjectDir and *CANNOT* be a full path name.",
                        "MPFS2 Console Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
                    return;
                }

                // Locate the parameters
                String sourceDir = args[args.Length - 3];
                String projectDir = args[args.Length - 2];
                String outputFile = args[args.Length - 1];

                // Set up some defaults
                MPFSOutputFormat fmt = MPFSOutputFormat.BIN;
                byte version = 2;
                int reserveBlock = 64;
                String htmlTypes = "*.htm, *.html, *.xml, *.cgi";
                String noGZipTypes = "*.inc, snmp.bib";

                // Process each command line argument
                for(int i =0; i < args.Length - 3; i++)
                {
                    String arg = args[i].ToLower();

			        // Check for output format parameters
			        if(arg == "/bin" || arg == "/b")
				        fmt = MPFSOutputFormat.BIN;
			        else if(arg == "/c18" || arg == "/c")
				        fmt = MPFSOutputFormat.C18;
			        else if(arg == "/asm30" || arg == "/s")
				        fmt = MPFSOutputFormat.ASM30;
			        else if(arg == "/c32" || arg == "/x")
				        fmt = MPFSOutputFormat.C32;
			        else if(arg == "/classic" || arg == "/1")
				        version = 1;
			        else if(arg == "/mpfs2" || arg == "/2")
				        version = 2;

                    // Check for string parameters
			        else if(arg == "/reserve" || arg == "/r")
				        reserveBlock = Convert.ToInt32(args[++i]);
			        else if(arg == "/html" || arg == "/h")
				        htmlTypes = args[++i];
			        else if(arg == "/xgzip" || arg == "/z")
				        noGZipTypes = args[++i];

                    else
                    {
                        MessageBox.Show("The command-line option \""+arg+"\" was not recognized.",
					        "MPFS2 Console Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
				        return;
                    }
                }

                // Set up an appropriate builder
                MPFSBuilder builder;
                // This is a dummy string , will be initialized when MDD is supported from command line
                String dummy = "Dummy";      
                if (version == 2)
                {
                    builder = new MPFS2Builder(projectDir, outputFile);
                    ((MPFS2Builder)builder).DynamicTypes = htmlTypes;
                    ((MPFS2Builder)builder).NonGZipTypes = noGZipTypes;
                }
                else
                {
                    builder = new MPFSClassicBuilder(projectDir, outputFile);
                    ((MPFSClassicBuilder)builder).ReserveBlock = (UInt32)reserveBlock;
                }

                // Add the files to the image and generate the image
                builder.AddDirectory(sourceDir, "");

                // Generate the image and trap errors
                if (!builder.Generate(fmt))
                {
                    LogWindow dlg = new LogWindow();
                    dlg.Image = SystemIcons.Error;
                    dlg.Message = "An error was encountered during generation.";
                    dlg.Log = builder.Log;
                    dlg.ShowDialog();
                    return;
                }
            }
        }
    }
}
