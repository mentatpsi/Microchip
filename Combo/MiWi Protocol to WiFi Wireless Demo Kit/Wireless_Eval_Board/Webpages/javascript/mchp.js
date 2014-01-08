/*********************************************************************
 * Microchip TCP/IP Stack Javascript Library
 **********************************************************************
 *
 * Software License Agreement
 *
 * Copyright © 2002-2010 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product (“Device”) which is 
 *      integrated into Licensee’s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT 
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
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Chris Smith			10/26/11	Modified for MiWi to WiFi Bridge
 ********************************************************************/

// Determines when a request is considered "timed out"
var timeOutMS = 5000; //ms
 
// Stores a queue of AJAX events to process
var ajaxList = new Array();

/**
 * Stores the number of dots current being shown in the rescan button
 */
var scanDots = 0;

/**
 * Current iteration of the bss info read from WiFi module.
 */
var currBss = 0;

/**
 * Current iteration of the Node info read from MiWi module.
 */
var currMiWiNode = 0;

/**
 * Whether to destroy or build other networks table
 */
var otherNetworkExpanded = 1;


var xmlhttp;


//  Initiates a new AJAX command
//	url: the url to access
//	cfunc: the function to call with response XML (optional)
//	data: an URL encoded string to be submitted as POST data (optional)
function loadXMLDoc(url, cfunc, data)
{
    // Create and send the request for appropriate browser
    if (window.XMLHttpRequest)
    {   // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
        xmlhttp.open((data==null)?"GET":"POST", url, true);
        xmlhttp.send(data);
    }
    else
    {   // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        xmlhttp.open((data==null)?"GET":"POST", url, true);
        xmlhttp.send(data);
    }
    xmlhttp.onreadystatechange = cfunc;
}


// Initiates a new AJAX command
//	url: the url to access
//	container: the document ID to fill, or a function to call with response XML (optional)
//	repeat: true to repeat this call indefinitely (optional)
//	data: an URL encoded string to be submitted as POST data (optional)
function newAJAXCommand(url, container, repeat, data)
{
	// Set up our object
	var newAjax = new Object();
	var theTimer = new Date();
	newAjax.url = url;
	newAjax.container = container;
	newAjax.repeat = repeat;
	newAjax.ajaxReq = null;
	
	// Create and send the request for appropriate browser
	if(window.XMLHttpRequest) 
	{   // Code for IE7+, Firefox, Chrome, Safari, Opera
        newAjax.ajaxReq = new XMLHttpRequest();
        newAjax.ajaxReq.open((data==null)?"GET":"POST", newAjax.url, true);
        newAjax.ajaxReq.send(data);
    } 
    else if(window.ActiveXObject) 
    {   // Code for IE6, IE5
        newAjax.ajaxReq = new ActiveXObject("Microsoft.XMLHTTP");
        if(newAjax.ajaxReq) 
        {
            newAjax.ajaxReq.open((data==null)?"GET":"POST", newAjax.url, true);
            newAjax.ajaxReq.send(data);
        }
    }
    
    newAjax.lastCalled = theTimer.getTime();
    
    // Store in our array
    ajaxList.push(newAjax);
}




// Loops over all pending AJAX events to determine if any action is required
function pollAJAX() 
{	
	var curAjax = new Object();
	var theTimer = new Date();
	var elapsed;
	
	// Read off the ajaxList objects one by one
	for(i = ajaxList.length; i > 0; i--)
	{
		curAjax = ajaxList.shift();
		if(!curAjax)
			continue;
		elapsed = theTimer.getTime() - curAjax.lastCalled;
				
		// If we succeeded
		if(curAjax.ajaxReq.readyState == 4 && curAjax.ajaxReq.status == 200) 
		{
			// If it has a container, write the result
			if(typeof(curAjax.container) == 'function')
			{
				curAjax.container(curAjax.ajaxReq.responseXML.documentElement);
			} 
			else if(typeof(curAjax.container) == 'string') 
			{
				document.getElementById(curAjax.container).innerHTML = curAjax.ajaxReq.responseText;
			} // (otherwise do nothing for null values)
			
	    	curAjax.ajaxReq.abort();
	    	curAjax.ajaxReq = null;

			// If it's a repeatable request, then do so
			if(curAjax.repeat)
				newAJAXCommand(curAjax.url, curAjax.container, curAjax.repeat);
			continue;
		}
		
		// If we've waited over 1 second, then we timed out
		if(elapsed > timeOutMS) 
		{
			// Invoke the user function with null input
			if(typeof(curAjax.container) == 'function')
			{
				curAjax.container(null);
			} 
			else 
			{
				// Alert the user
				alert("Command failed.\nConnection to development board was lost.");
			}

	    	curAjax.ajaxReq.abort();
	    	curAjax.ajaxReq = null;
			
			// If it's a repeatable request, then do so
			if(curAjax.repeat)
				newAJAXCommand(curAjax.url, curAjax.container, curAjax.repeat);
			continue;
		}
		
		// Otherwise, just keep waiting
		ajaxList.push(curAjax);
	}
	
	// Call ourselves again in 10ms
	setTimeout("pollAJAX()",10);
	
}

			
// Parses the xmlResponse returned by an XMLHTTPRequest object
//	xmlData: the xmlData returned
//  field: the field to search for
function getXMLValue(xmlData, field, child) 
{
	child = typeof(child) == "undefined" ? 0 : child;
	
	try 
	{
		if(xmlData.getElementsByTagName(field)[child].firstChild.nodeValue)
			return xmlData.getElementsByTagName(field)[child].firstChild.nodeValue;
		else
			return null;
	} 
	catch(err) { return null; }
}

function getStrength(str)
{	
	if (str == 5)
		str = 4;
	
	return "images/bar-" + str + ".png";
}

function removeCheckmark() 
{
    var table = document.getElementById('scanTable').getElementsByTagName("tbody")[0]; 
    var rows = table.rows;

    for (i = 1; i < rows.length; i++) 
    {
    	rows[i].lastChild.removeChild(rows[i].lastChild.lastChild);
    	rows[i].lastChild.appendChild(document.createTextNode(""));
    }
}

function printButtonName(buttonID)
{
	var textLabel = "Scanning";
	
	for (i = 0; i < scanDots % 4; i++) 
	{
		textLabel += ".";
	}
	
	scanDots++;
	
	document.getElementById(buttonID).value = textLabel;
}

// =======================================================================
// = This routine updates the dynamic variables on each of the webpages. =
// =======================================================================
function updateStatus(xmlData) 
{	
    // Get the active Page URL
	var urlPath = window.location.pathname;
	var pageName = urlPath.substring(urlPath.lastIndexOf('/') + 1);
	
	
	// miwi_configure.htm
	if (pageName == 'miwi_configure.htm')
	{
	    // Check if Command complete
	    var status = getXMLValue(xmlData, 'miwistatus', 0);
	    
	    switch(status)
	    {
	        case 2:
    		    // Check if currMiWiNode is at last scan entry
    			if (currMiWiNode != getXMLValue(xmlData, 'nodecnt', 0)) 
    			{
    			    // Add the Nodes Information to the table
    				addMiWiScanRow(getXMLValue(xmlData, 'macadd', 0), getXMLValue(xmlData, 'chnl', 0), getXMLValue(xmlData, 'panid', 0));
				
    				// Increment to next Node in Scan list	
    				currMiWiNode++;
					
    				// Kick off request to get next scan entry
    				if (currMiWiNode < 16) 
    				{
    					// Pad 0 before sending to host
    					newAJAXCommand('miwi.cgi?miwicmd=2&getNodeIdx=0' + currMiWiNode.toString(16));
    				}
    				else 
    				{
    					newAJAXCommand('miwi.cgi?miwicmd=2&getNodeIdx=' + currMiWiNode.toString(16));
    				}
				
    				setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 50);
    			}
    			else
    			{
    				// We're done here, all scan results posted, so reenable scan button
    				document.getElementById("miwiscan").disabled = false;
    				document.getElementById("miwiscan").value = "Scan for MiWi Nodes";
    			}
    			break;
    			
            default:
                printButtonName("miwiscan");
			    setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 25);
                break;
    			        
	    }
	}
/*
	else if (pageName == 'wifi_configure.htm') {
                                if (getXMLValue(xmlData, 'scan', 0) === '0') {
                                                if (currBss != getXMLValue(xmlData, 'count', 0)) {
                                                                if (getXMLValue(xmlData, 'valid', 0) === '0') {
                                                                                // current bss returned isn't valid, so issue a request to chip
                                                                                if (currBss < 16) {
                                                                                                // pad 0 before sending to host
                                                                                                newAJAXCommand('scan.cgi?getBss=0' + currBss.toString(16));
                                                                                }
                                                                                else {
                                                                                                newAJAXCommand('scan.cgi?getBss=' + currBss.toString(16));
                                                                                }
                                                                               
                                                                                setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 50);
                                                                }
                                                                else {
                                                                                if ((getXMLValue(xmlData, 'name', 0) != null) &&
                                                                                (getXMLValue(xmlData, 'name', 0) != (getXMLValue(xmlData, 'ssid', 0)))) {
                                                                                                // don't display hidden networks or the network you are currently
                                                                                                // connected to
                                                                                                addScanRow(getXMLValue(xmlData, 'name', 0), getXMLValue(xmlData, 'privacy', 0), getXMLValue(xmlData, 'strength', 0), getXMLValue(xmlData, 'wlan', 0));
                                                                                }
                                                                               
                                                                                currBss++;
                                                                               
                                                                                // kick off request for next scan entry
                                                                                if (currBss < 16) {
                                                                                                // pad 0 before sending to host
                                                                                                newAJAXCommand('scan.cgi?getBss=0' + currBss.toString(16));
                                                                                }
                                                                                else {
                                                                                                newAJAXCommand('scan.cgi?getBss=' + currBss.toString(16));
                                                                                }
                                                                                setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 50);
                                                                }
                                                }
                                               
                                                if (currBss == getXMLValue(xmlData, 'count', 0)) {
                                                                // we're done here, all scan results posted, so reenable scan button
                                                                document.getElementById("rescan").disabled = false;
                                                                document.getElementById("rescan").value = "Display Scan Results";
                                                }
                                }
                                else {
                                                printButtonName();
                                                setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 25);
                                }
                }
*/
	else
	{
	    // Do nothing for other pages.
	}
}

//*********************************************************************************//
//  WiFi Specific Routines
//*********************************************************************************//

function getCurBssInfo()
{
    // Generate request to hardware to send current BSS Information
    loadXMLDoc('miwi.cgi?miwicmd=1&scanchnl=' + num.toString(2),function()
    {
        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            alert("Got here: 1");
        }
    });
    
	//newAJAXCommand('miwi.cgi?miwicmd=1&scanchnl=' + num.toString(2));

 	// Delete old table, replace with new table after scan is finished
    deleteMiWiScanTable();
    
    // Reset the current MiWi Node pointer
    currMiWiNode = 0;
    
    //setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 2000);	
}


function getSecurity(sec) 
{
	var security;
	
	switch (sec)
	{
	case 0:
		security = "no";
		break;
	case 1:
		security = "wep";
		break;
	case 5:
	case 9:
	case 13:
		security = "wpa";
		break;
	default:
	   // impossible to get here!
		break;
	}
	
	return security;
}


function securityKeyPrompt(secCode) 
{
	var key;
	
	var wep64RE = new RegExp("^[0-9a-fA-F]{10}$"); // check for hex key, 10 digits long
	var wep64REA = new RegExp("^.{5}$"); // check for ASCII key, 5 digits long
	var wep128RE = new RegExp("^[0-9a-fA-F]{26}$"); // check for hex key, 26 digits long
	var wep128REA = new RegExp("^.{13}$"); // check for ASCII key, 13 digits long
	var wpaREA = new RegExp("^.{8,63}$"); // check for ASCII passphrase, 8-63 characters long
	
	var errMsg;
	var keyInvalid = 0;
	if (secCode == 1) {
		do {
			msg = "Please enter your WEP key";
			if (keyInvalid) {
				msg = errMsg + "\r\n" + msg;
			}
			
			key = prompt(msg);
			
			if (key == null) {
				// user hit cancel, so need to go back to main screen and not submit
				// remove check mark also
				
				return "__UFU__";
			} else if (key == "") {
				// user hit enter only, so modify error message to notify user
				keyInvalid = 1;
				errMsg = "No key entered!\r\n";
			} else {
				// key could be valid, check it
				if ((wep64RE.test(key) == false) && (wep64REA.test(key) == false) &&
					(wep128RE.test(key) == false) && (wep128REA.test(key) == false)) {
					keyInvalid = 1;
					errMsg = "WEP key is not the correct length!\r\n";
					errMsg += "Keys must be either:\r\n";
					errMsg += "(a) 10 or 26 hexadecimal digits or\r\n";
					errMsg += "(b) 5 or 13 ASCII characters\r\n";
				} else {
					if ((key.length == 5) || (key.length == 10)) {
						document.getElementById("sec").value += "40";
					} else if ((key.length == 13 || (key.length == 26))) {
						document.getElementById("sec").value += "104";
					}
					
					keyInvalid = 0;
				}
			}
		} while (keyInvalid);
	} else if (secCode > 1) {
		do {
			msg = "Please enter your passphrase";
			if (keyInvalid) {
				msg = errMsg + "\r\n" + msg;
			}
			
			key = prompt(msg);
			
			if (key == null) {
				// user hit cancel, so need to go back to main screen and not submit
				// remove check mark also
				
				return "__UFU__";
			} else if (key == "") {
				// user hit enter only, so modify error message to notify user
				keyInvalid = 1;
				errMsg = "No passphrase entered!\r\n";
			} else {
				// passphrase could be valid, check it
				if (wpaREA.test(key) == false) {
					keyInvalid = 1;
					errMsg = "WPA passphrase is not the correct length!\r\n";
					errMsg += "Passphrase must be 8-63 characters long.\r\n";
				} else {
					keyInvalid = 0;
				}
			}
		} while (keyInvalid);
	}
	
	return key;
}


function switchNetwork(id) 
{
	if (document.getElementById("rescan").disabled == true) 
	{
		// don't allow users to click any of the network names
		// if we are currently doing a scan
		return;
	}
	
	var networkParam = new Array();
	networkParam = id.split("\020", 3);
	
	var name = networkParam[0];
	var secCode = networkParam[1];
	var wlanCode = networkParam[2];
	
	if (name == "")
	{
		alert('SSID cannot be left blank!');
		return;
	}
	else if (wlanCode == "undefined")
	{
		alert('Please choose either adhoc or infrastructure');
		return;
	}
	
	removeCheckmark();
	var row = document.getElementById(id);
	if (row != null) 
	{
		// null means that the row doesn't exist
		// only happens when we are entering a SSID manually
		// from the Other Network... dropdown
		row.lastChild.removeChild(row.lastChild.lastChild);
	
		var checkImg = document.createElement("img");
		checkImg.src = "images/checkmark.png";
		row.lastChild.appendChild(checkImg);
	}

	document.getElementById("wlan").value = (wlanCode == 1) ? "infra" : "adhoc";
	document.getElementById("ssid").value = name;
	
	document.getElementById("sec").value = getSecurity(parseInt(secCode));
	
	var key;
	if (parseInt(secCode)) 
	{
		key = securityKeyPrompt(secCode);
		document.getElementById("key").value = key;
	}

	if (key == "__UFU__") 
	{
		// user hit cancel on the prompt box for the network
		// so assume they didn't want to set the network
		removeCheckmark();
		document.getElementById("key").value = null;
	} 
	else 
	{
		document.kickoff.submit();
	}
}

function joinNetwork() 
{
	// copy elements from hidden form and submit them through
	// the normal form
	var id;
	
	id = document.getElementById('ssid1').value + "\020";
	id += document.getElementById('sec1').value + "\020";
	
	var wlanVal;
	var wlanRadio = document.getElementsByName('wlan1');
	for (i = 0; i < wlanRadio.length; i++) 
	{
		if (wlanRadio[i].checked) 
		{
			wlanVal = 2 - i;
			break;
		}
	}
	
	id += wlanVal;

	switchNetwork(id);
}


function userSelectNetwork() 
{
	// add rows for adhoc/infra selection, text input of ssid and 
	// selector box for security type
	// if security type other than none is chosen, append additional
	// text input field for key/passphrase
	
	otherNetworkExpanded = (otherNetworkExpanded == 1) ? 0 : 1;
	
	if (otherNetworkExpanded) 
	{
   	// need to destroy table back to just button
				
		var table = document.getElementById('scanTable').getElementsByTagName('tfoot')[0];
		var rows = table.rows;
	
		while (rows.length - 1) // length=1 -> stop 
			table.deleteRow(rows.length - 1);
	}
	else 
	{			
		var tfoot = document.getElementById('scanTable').getElementsByTagName("tfoot")[0];
		
		var row1 = document.createElement("tr");
		row1.setAttribute('style', 'width:9em');
		
		var data1 = document.createElement("td");
		data1.setAttribute('colspan', 3);
		data1.appendChild(document.createTextNode('Adhoc'));
		
		var data2 = document.createElement("td");
		var adhocInput = document.createElement("input");
		adhocInput.setAttribute('type', 'radio');
		adhocInput.setAttribute('name', 'wlan1');
		adhocInput.setAttribute('value', '2');
		adhocInput.setAttribute('onclick', 'adhocSel();');
		data2.appendChild(adhocInput);
		
		row1.appendChild(data1);
		row1.appendChild(data2);
		
		var row2 = document.createElement("tr");
		row2.setAttribute('style', 'width:9em');
		
		var data3 = document.createElement("td");
		data3.setAttribute('colspan', 3);
		data3.appendChild(document.createTextNode('Infrastructure'));
		
		var data4 = document.createElement("td");
		var infraInput = document.createElement("input");
		infraInput.setAttribute('type', 'radio');
		infraInput.setAttribute('name', 'wlan1');
		infraInput.setAttribute('value', '1');
		infraInput.setAttribute('onclick', 'infraSel();');
		data4.appendChild(infraInput);
		
		row2.appendChild(data3);
		row2.appendChild(data4);
		
		var row3 = document.createElement("tr");
		row3.setAttribute('style', 'width:9em');
		var data5 = document.createElement("td");
		data5.setAttribute('colspan', '4');
		data5.appendChild(document.createTextNode("Network Name"));
		row3.appendChild(data5);
		
		var row4 = document.createElement("tr");
		row4.setAttribute('style', 'width:9em');
		var data6 = document.createElement("td");
		data6.setAttribute('colspan', '4');
		var ssidInput = document.createElement("input");
		ssidInput.setAttribute('type', 'text');
		ssidInput.setAttribute('id', 'ssid1');
		ssidInput.setAttribute('name', 'ssid1');
		ssidInput.setAttribute('maxlength', '32');
		data6.appendChild(ssidInput);
		
		row4.appendChild(data6);
		
		var row5 = document.createElement("tr");
		row5.setAttribute('style', 'width:9em');
		var data7 = document.createElement("td");
		data7.setAttribute('colspan', '4');
		var secSel = document.createElement("select");
		secSel.setAttribute('name', 'sec1');
		secSel.setAttribute('id', 'sec1');
		secSel.options[0] = new Option('None', '0');
		secSel.options[1] = new Option('WEP', '1');
		secSel.options[2] = new Option('WPA/WPA2 Passphrase', '5');
		data7.appendChild(secSel);
		row5.appendChild(data7);
		
		var row6 = document.createElement("tr");
		row6.setAttribute('style', 'width:9em');
		var data8 = document.createElement("td");
		data8.setAttribute('colspan', '3');
		var joinButton = document.createElement("input");
		joinButton.setAttribute('id', 'joinButton');
		joinButton.setAttribute('type', 'button');
		joinButton.setAttribute('value', 'Join');
		joinButton.setAttribute('onclick', 'joinNetwork();');
		data8.appendChild(joinButton);
		row6.appendChild(data8);
		
		tfoot.appendChild(row1);
		tfoot.appendChild(row2);
		tfoot.appendChild(row3);
		tfoot.appendChild(row4);
		tfoot.appendChild(row5);
		tfoot.appendChild(row6);
	}
}


function adhocSel() {
	if (document.getElementById('sec1').options.length == 3) {
		document.getElementById('sec1').remove(2);
	}
}


function infraSel() {
	if (document.getElementById('sec1').options.length == 2) {
		document.getElementById('sec1').options[2] = new Option('WPA/WPA2 Passphrase', '5');	
	}
}

// ===============================================
// = WiFi add Network Information to Scan Table. =
// ===============================================
function addScanRow(ssid, sec, str, wlan) {
	var tbody = document.getElementById('scanTable').getElementsByTagName("tbody")[0];
	var row = document.createElement("tr");
	
	var blankImg = document.createElement("img");
	blankImg.src = "images/blank.png";
	
	row.setAttribute('id', ssid + "\020" + sec + "\020" + wlan);
	row.setAttribute('onmouseover', "this.style.cursor='pointer'");
	row.setAttribute('onclick', 'switchNetwork(id);');
	
	var data1 = document.createElement("td");
	data1.setAttribute('style', 'width:10em');
	data1.appendChild(document.createTextNode(ssid));
	
	var data2 = document.createElement("td");
	var secImg = document.createElement("img");
	secImg.src = "images/lock.png";
	if (sec > 0) {
		data2.appendChild(secImg);
	} else {
		data2.appendChild(blankImg);
	}
	
	var data3 = document.createElement("td");
	var pwrImg = document.createElement("img");
	pwrImg.src = getStrength(str);
	data3.appendChild(pwrImg);
	
	var data4 = document.createElement("td");
	data4.appendChild(blankImg);
	
	row.appendChild(data1);
	row.appendChild(data2);
	row.appendChild(data3);
	row.appendChild(data4);
	
	tbody.appendChild(row);
}


// ==========================
// = WiFi delete scan table =
// ==========================
function deleteWiFiScanTable() 
{
    var table = document.getElementById('scanTable').getElementsByTagName('tbody')[0]; 
    var rows = table.rows;

    while (rows.length - 1) // length=1 -> stop 
        table.deleteRow(rows.length - 1); 
}

// ==========================
// = WiFi Scan for Networks =
// ==========================
function scanWiFiNetwork()
{
	scanDots = 0;
	printButtonName("rescan");
	document.getElementById("rescan").disabled = true;
	
	// delete old table, replace with new table after scan is finished
    deleteWiFiScanTable();
	
    // Get the Scan Results
    loadXMLDoc('wifi.xml?scan=1',function()
    {
        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            // Display the Scan Results
            var i,x;

            xmlData=xmlhttp.responseXML;
            
            x=xmlhttp.responseXML.documentElement.getElementsByTagName("bss");

            for(i=0; i < x.length; i++)
            {
                // Grab BSS Info
				if ((getXMLValue(xmlData, 'name', i) != null) &&
				(getXMLValue(xmlData, 'name', i) != (getXMLValue(xmlData, 'ssid', 0))))
				{
					// don't display hidden networks or the network you are currently
					// connected to
					addScanRow(getXMLValue(xmlData, 'name', i), getXMLValue(xmlData, 'privacy', i), getXMLValue(xmlData, 'strength', i), getXMLValue(xmlData, 'wlan', i));
				}
            }
            // we're done here, all scan results posted, so reenable scan button
			document.getElementById("rescan").disabled = false;
			document.getElementById("rescan").value = "Display Scan Results";
        }
    });
}

/*
function scanWiFinetwork()
{

	scanDots = 0;

	//printButtonName();
	document.getElementById("rescan").disabled = true;

	new AJAXCommand('scan.cgi?scan=1');

	deleteWiFiScanTable();

	curBss = 0;

	setTimeout("newAJAXCommand('status.xml, updateStatus, false)", 50);

}
*/
//*********************************************************************************//
//  MiWi Specific Routines
//*********************************************************************************//

// =========================
// = MiWi Scan for Nodes   =
// =========================
function scanMiWiNodes()
{
    // Dis-able the Scan Button
    scanDots = 0;
    printButtonName("miwiScanBtn");
    document.getElementById("miwiScanBtn").disabled = true;
    
    // Get Channel to scan
    var e = document.getElementById("scanchnl");
    var num = e.options[e.selectedIndex].value;
    
    // Generate request to hardware to issue a scan
    loadXMLDoc('miwi.xml?miwicmd=1&scanchnl=' + num.toString(2),function()
    {
        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            // Display the Scan Results
            var i,x,xx;

            x=xmlhttp.responseXML.documentElement.getElementsByTagName("Node");

            // Display the Scan Table Results
            document.getElementById("scanTableMiWi").style.display="inline";
            
            for(i=0; i < x.length; i++)
            {
                // Grab Node Info
                xx = x[i].getElementsByTagName("macaddr");
                var macaddr = xx[0].firstChild.nodeValue;
                xx = x[i].getElementsByTagName("panid");
                var panid = xx[0].firstChild.nodeValue;
                xx = x[i].getElementsByTagName("index");
                var scanIndex = xx[0].firstChild.nodeValue;

                // Add Nodes Info to Scan Table
                addMiWiScanRow(macaddr, panid, scanIndex);
            }
            
            // Display the Node Configuration Form
            document.getElementById("nodeConfigFrm").style.display="inline";
            
            // Re-Enable the Scan Button
            document.getElementById("miwiScanBtn").disabled = false;
			document.getElementById("miwiScanBtn").value = "Scan for MiWi Nodes";
			
			// Populate the Room Option List
			populateRoomOpt();
        }
    });
    
 	// Delete old table, replace with new table after scan is finished
    deleteMiWiScanTable();

}

// =========================================
// = MiWi delete MiWi scan table from page =
// =========================================
function deleteMiWiScanTable() 
{
    var table = document.getElementById('scanTableMiWi').getElementsByTagName('tbody')[0]; 
    var rows = table.rows;

    // Hide the Scan Table
    document.getElementById("scanTableMiWi").style.display="none";
    // Hide the Node Configuration Form
    document.getElementById("nodeConfigFrm").style.display="none";
      
    while (rows.length - 1) // length=1 -> stop 
        table.deleteRow(rows.length - 1); 
}


// ===========================================
// = MiWi add Node information to Scan Table =
// ===========================================
function addMiWiScanRow(macadd, panid, scanIndex)
{
    var table = document.getElementById('scanTableMiWi');
    var rowCount = table.rows.length;
	var row = table.insertRow(rowCount);
	row.id='row' + rowCount;
	
	var cell0 = row.insertCell(0);
	var element0 = document.createElement("input");
	element0.type = "checkbox";
	cell0.setAttribute('align', 'center');
	cell0.appendChild(element0);
	
	var cell1 = row.insertCell(1);
	var element1 = document.createElement("input");
	element1.type = "checkbox";
	cell1.setAttribute('align', 'center');
	element1.setAttribute('onclick','identifyNode(row.id);'); // for FF
    element1.onclick = function() {identifyNode(row.id);}; // for IE
	cell1.appendChild(element1);

	var cell2 = row.insertCell(2);
	cell2.setAttribute('style', 'width:10em');
	cell2.setAttribute('onclick','getNodeInfo(this);'); // for FF
    cell2.onclick = function() {getNodeInfo(row.id);}; // for IE
	cell2.appendChild(document.createTextNode(macadd));

	var cell3 = row.insertCell(3);
	cell3.setAttribute('align', 'center');
	cell3.setAttribute('style', 'width:10em');
	cell3.appendChild(document.createTextNode(panid));

	var cell4 = row.insertCell(4);
	cell4.setAttribute('style', 'display:none');
	cell4.appendChild(document.createTextNode(scanIndex));	
				
}


// ==============================
// = MiWi Identify Node Routine =
// ==============================
function identifyNode(node)
{
    var elTableRow = document.getElementById(node);
    var elTableCells = elTableRow.getElementsByTagName("td");
    var scanIndex = elTableCells[4].innerText;
    
	 // Get Channel to scan
    var e = document.getElementById("scanchnl");
    var num = e.options[e.selectedIndex].value;
    var channel = num.toString(2);

	// Generate request to hardware to issue a scan
	newAJAXCommand('cmd.cgi?miwicmd=2&nodeIdx=' + scanIndex+'&roomChan='+channel);
	
}

function getNodeInfo(node)
{
    var x, xx;
    var rowindex = document.getElementById(node).rowIndex;
    
    x = xmlhttp.responseXML.documentElement.getElementsByTagName("Node");
    // Grab Node Info
    xx = x[i].gcreateTextNodeetElementsByTagName("macaddr");
    var address = xx[0].firstChild.nodeValue;
    xx = x[i].getElementsByTagName("panid");
    var panid = xx[0].firstChild.nodeValue;
    xx = x[i].getElementsByTagName("channel");
    var channel = xx[0].firstChild.nodeValue;
    xx = x[i].getElementsByTagName("rssi");
    var rssi = xx[0].firstChild.nodeValue;   
    xx = x[i].getElementsByTagName("lqi");
    var lqi = xx[0].firstChild.nodeValue;
    xx = x[i].getElementsByTagName("capability");
    var capability = xx[0].firstChild.nodeValue;
    
    alert("MAC Address: "+address+"\n"+
            "Channel: "+channel+"\n"+
            "PANID: "+panid+"\n"+
            "RSSI Value: "+rssi+"\n"+
            "LQI Value: "+lqi+"\n"+
            "Capability: "+capability);
}


// ================================
// = MiWi Configure Node Routine =
// ================================
function configMiWiNodes()
{
    var table = document.getElementById('scanTableMiWi');
    var rowCount = table.rows.length;
    var i,x,xx;

    x=xmlhttp.responseXML.documentElement.getElementsByTagName("Room");     
    var select = document.getElementById("setRoom");
      
    for(i=0; i < x.length; i++)
    {
        // Grab Info
        xx = x[i].getElementsByTagName("name");
        var name = xx[0].firstChild.nodeValue;
        if(name == select.value)
        {
            xx = x[i].getElementsByTagName("panid");
            var panId = xx[0].firstChild.nodeValue;
            xx = x[i].getElementsByTagName("channel");
            var channel = xx[0].firstChild.nodeValue;
            break;
        }
    }
  
    for (var i=0; i < rowCount; i++)
    {
        var row = table.rows[i];
        var chkbox = row.cells[0].childNodes[0];
        
        var x=document.getElementById('scanTableMiWi').rows[i].cells;
        scanIndex = x[4].innerHTML;

        if(null != chkbox && chkbox.checked)
        {
            // Generate request to hardware to Configure Node
        	newAJAXCommand('cmd.cgi?miwicmd=3&nodeIdx='+scanIndex+'&chnl='+channel+'&panid='+panId);
        }
    }
    for (var i=0; i < rowCount; i++)
    {
        var row = table.rows[i];
        var chkbox = row.cells[0].childNodes[0];
        if(null != chkbox && chkbox.checked)
        {
         	table.deleteRow(i);
			rowCount--;
			i--;
        }
    }
    
    // Reset the form
    document.getElementById("nodeConfigFrm").reset();
}


// =============================================
// = This routine populate the room list Table =
// =============================================
function getRoomList()
{
    // Generate request to hardware to issue a scan
    loadXMLDoc('facility.xml',function()
    {
        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            // Display the Scan Results
            var i,x,xx;

            x=xmlhttp.responseXML.documentElement.getElementsByTagName("Room");

            // Display the Scan Table Results
            document.getElementById("roomTable").style.display="inline";
            
            for(i=0; i < x.length; i++)
            {
                // Grab Info
                xx = x[i].getElementsByTagName("name");
                var name = xx[0].firstChild.nodeValue;
                xx = x[i].getElementsByTagName("panid");
                var panid = xx[0].firstChild.nodeValue;
                xx = x[i].getElementsByTagName("channel");
                var channel = xx[0].firstChild.nodeValue;
            
                // Add Nodes Info to Scan Table
                addRoomRow(name, panid, channel);
                
            }
        }
    });    
}


function deleteRoomInfo()
{
	try 
	{
    	var table = document.getElementById('roomTable');
    	var rowCount = table.rows.length;

    	for(var i=0; i<rowCount; i++) 
    	{
    		var row = table.rows[i];
    		var rowindex = row.rowIndex;
    		var chkbox = row.cells[0].childNodes[0];
    		var index = row.cells[4].childNodes[0];
    		if(null != chkbox && true == chkbox.checked) 
    		{
    		    // Generate request to hardware
            	newAJAXCommand('cmd.cgi?miwicmd=5&index='+index);
    			table.deleteRow(i);
    			rowCount--;
    			i--;
    		}
    	}
	}
	catch(e) 
	{
		alert(e);
	}

}


// ======================================
// = This routine adds a room to memory =
// ======================================
function addRoom()
{
    var name = document.getElementById('roomName').value;
    var panId = document.getElementById('roomPanId').value;
    var channel = document.getElementById('roomChan').value;

    newAJAXCommand('cmd.cgi?miwicmd=4&name='+name+'&panId='+panId+'&roomChan='+channel);
    
    addRoomRow(name, panId, channel);
  
    // Reset the form
    document.getElementById("addRoomFrm").reset();
}

// =========================================
// = This routine adds a room to the table =
// =========================================
function addRoomRow(name, panid, channel)
{
    var table = document.getElementById('roomTable').getElementsByTagName("tbody")[0];
    //var table = document.getElementById('roomTable');
    var rowCount = table.rows.length;
	var row = table.insertRow(rowCount);
	
	var cell0 = row.insertCell(0);
	var element0 = document.createElement("input");
	element0.type = "checkbox";
	cell0.setAttribute('align', 'center');
	cell0.setAttribute('style', 'display:none');
	cell0.appendChild(element0);
	
	var cell1 = row.insertCell(1);
	cell1.setAttribute('align', 'center');
	cell1.setAttribute('style', 'width:10em');
	cell1.appendChild(document.createTextNode(channel));


	var cell2 = row.insertCell(2);
	cell2.setAttribute('style', 'width:10em');
	cell2.appendChild(document.createTextNode(name));

	var cell3 = row.insertCell(3);
	cell3.setAttribute('align', 'center');
	cell3.setAttribute('style', 'width:10em');
	cell3.appendChild(document.createTextNode(panid));
	
}


// =========================================================================
// = This routine populate the Room Option List from the facility.xml file =
// =========================================================================
function populateRoomOpt()
{
    // Generate request to hardware
    loadXMLDoc('facility.xml',function()
    {
        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            // Display Results
            var i,x,xx;

            x=xmlhttp.responseXML.documentElement.getElementsByTagName("Room");

            // Display the Scan Table Results
            var select = document.getElementById("setRoom");
            
            // Remove any previous options
            for(i = select.options.length-1;i>=0;i--)
            {
                select.remove(i);
            }
            
            for(i=0; i < x.length; i++)
            {
                // Grab Info
                xx = x[i].getElementsByTagName("name");
                var name = xx[0].firstChild.nodeValue;

                // Add to Option List
                select.options[select.options.length] = new Option(name, name);
                
            }
        }
    });
}


function ctrlRoomList()
{
    // Generate request to hardware to issue a scan
    loadXMLDoc('facility.xml',function()
    {
        if (xmlhttp.readyState==4 && xmlhttp.status==200)
        {
            // Display the Scan Results
            var i,x,xx;

            x=xmlhttp.responseXML.documentElement.getElementsByTagName("Room");

            for(i=0; i < x.length; i++)
            {
                // Grab Info
                xx = x[i].getElementsByTagName("name");
                var name = xx[0].firstChild.nodeValue;
                xx = x[i].getElementsByTagName("panid");
                var panid = xx[0].firstChild.nodeValue;
                xx = x[i].getElementsByTagName("channel");
                var channel = xx[0].firstChild.nodeValue;
            
                // Add Nodes Info to Scan Table
                addCtrlRoom(name, panid, channel);
                
            }
        }
    });    
}


function addCtrlRoom(name, panid, channel)
{
    var table = document.getElementById('ctrlTable').getElementsByTagName("tbody")[0];


	//var table = document.getElementById('roomTable');
    var rowCount = table.rows.length;
	var row = table.insertRow(rowCount);
    row.id='row' + rowCount;

	var cell0 = row.insertCell(0);
	var element0 = document.createElement("input");
	element0.type = "button";
	element0.setAttribute("value", "OFF");
	element0.style.color = 'red';
	element0.style.fontSize = "20px";
	element0.setAttribute('onclick','lightsOFF(row.id);'); // for FF
    element0.onclick = function() {lightsOFF(row.id);}; // for IE
	cell0.setAttribute('align', 'center');
	cell0.setAttribute('style', 'width:10em');
	cell0.appendChild(element0);

	var cell1 = row.insertCell(1);
	cell1.setAttribute('align', 'center');
	cell1.setAttribute('style', 'width:15em');
	cell1.appendChild(document.createTextNode(name));

	var cell2 = row.insertCell(2);
	var element2 = document.createElement("input");
	element2.type = "button";
	element2.setAttribute("value", "ON");
	element2.style.color = 'green';
	element2.style.fontSize = "20px";
	element2.setAttribute('onclick','lightsON(row.id);'); // for FF
    element2.onclick = function() {lightsON(row.id);}; // for IE
	cell2.setAttribute('align', 'center');
	cell2.setAttribute('style', 'width:10em');
	cell2.appendChild(element2);
				
	var cell3 = row.insertCell(3);
	cell3.setAttribute('style', 'display:none;');
	cell3.appendChild(document.createTextNode(channel));

	var cell4 = row.insertCell(4);
	cell4.setAttribute('style', 'display:none;');
	cell4.appendChild(document.createTextNode(panid));

		
}


// ========================================================
// = This Routione turns OFF the lights in specified room =
// ========================================================
function lightsOFF(id)
{
    var elTableRow = document.getElementById(id);
    var elTableCells = elTableRow.getElementsByTagName("td");
    var channel = elTableCells[3].innerText;
    var panId = elTableCells[4].innerText;
    var hasInnerText =(channel != undefined) ? true : false;

    if(!hasInnerText){
   	channel = elTableCells[3].textContent;
	panId = elTableCells[4].textContent;
    }    
    newAJAXCommand('cmd.cgi?miwicmd=6&roomChan='+channel+'&panId='+panId+'&code=0');
    
    //alert("Turn lights OFF:\nPAN ID: "+panId+"\nChannel: "+channel);
}


// ======================================================
// = This Routine turns ON the lights in specified room =
// ======================================================
function lightsON(id)
{
    var elTableRow = document.getElementById(id);
    var elTableCells = elTableRow.getElementsByTagName('td');
    var panId = elTableCells[4].innerText;
    var channel = elTableCells[3].innerText;
    var hasInnerText =(channel != undefined) ? true : false;

    if(!hasInnerText){
   	channel = elTableCells[3].textContent;
	panId = elTableCells[4].textContent;
    }    

    newAJAXCommand('cmd.cgi?miwicmd=6&roomChan='+channel+'&panId='+panId+'&code=1');
    
    //alert("Turn lights ON:\nPAN ID: "+panId+"\nChannel: "+channel);
}
