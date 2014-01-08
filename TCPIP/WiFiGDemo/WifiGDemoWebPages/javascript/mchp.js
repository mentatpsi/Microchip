/*********************************************************************
 * Microchip TCP/IP Stack Javascript Library
 **********************************************************************
 *
 * Software License Agreement
 *
 * Copyright © 2002-2013 Microchip Technology Inc.  All rights 
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
 * Amy Ong                  10/29/12       Modified for WiFi G Demo
 *
 ********************************************************************/

// Determines when a request is considered "timed out"
var timeOutMS = 5000; //ms
 
// Stores a queue of AJAX events to process
var ajaxList = new Array();

/**
 * stores the number of dots current being shown in the rescan button
 */
var scanDots = 0;

/**
 * current iteration of the bss info read from WiFi module.
 */
var currBss = 0;

/**
 * whether to destroy or build other networks table
 */
var otherNetworkExpanded = 1;

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
	
	// Create and send the request
	if(window.XMLHttpRequest) 
	{
        newAjax.ajaxReq = new XMLHttpRequest();
        newAjax.ajaxReq.open((data==null)?"GET":"POST", newAjax.url, true);
        newAjax.ajaxReq.send(data);
    // If we're using IE6 style (maybe 5.5 compatible too)
    } 
    else if(window.ActiveXObject) 
    {
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
function pollAJAX() {	
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
			if(typeof(curAjax.container) == 'function'){
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
// xmlData: the xmlData returned
//  field: the field to search for
function getXMLValue(xmlData, field, child) 
{
	child = typeof(child) == "undefined" ? 0 : child;
	
	try {
		if(xmlData.getElementsByTagName(field)[child].firstChild.nodeValue)
			return xmlData.getElementsByTagName(field)[child].firstChild.nodeValue;
		else
			return null;
	} catch(err) { return null; }
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
	
	if (secCode == 1) 
	{
		do {
			msg = "Please enter your WEP key";
			if (keyInvalid) 
			{
				msg = errMsg + "\r\n" + msg;
			}
			
			key = prompt(msg);
			
			if (key == null) 
			{
				// user hit cancel, so need to go back to main screen and not submit
				// remove check mark also
				
				return "__UFU__";
			} 
			else if (key == "") 
			{
				// user hit enter only, so modify error message to notify user
				keyInvalid = 1;
				errMsg = "No key entered!\r\n";
			} 
			else 
			{
				// key could be valid, check it
				if ((wep64RE.test(key) == false) && (wep64REA.test(key) == false) &&
					(wep128RE.test(key) == false) && (wep128REA.test(key) == false)) 
				{
					keyInvalid = 1;
					errMsg = "WEP key is not the correct length!\r\n";
					errMsg += "Keys must be either:\r\n";
					errMsg += "(a) 10 or 26 hexadecimal digits or\r\n";
					errMsg += "(b) 5 or 13 ASCII characters\r\n";
				} 
				else 
				{
					if ((key.length == 5) || (key.length == 10)) 
					{
						document.getElementById("sec").value += "40";
					} 
					else if ((key.length == 13 || (key.length == 26))) 
					{
						document.getElementById("sec").value += "104";
					}
					
					keyInvalid = 0;
				}
			}
		} while (keyInvalid);
	} 
	else if (secCode > 1) 
	{
		do {
			msg = "Please enter your passphrase";
			if (keyInvalid) 
			{
				msg = errMsg + "\r\n" + msg;
			}
			
			key = prompt(msg);
			
			if (key == null) 
			{
				// user hit cancel, so need to go back to main screen and not submit
				// remove check mark also
				
				return "__UFU__";
			} 
			else if (key == "") 
			{
				// user hit enter only, so modify error message to notify user
				keyInvalid = 1;
				errMsg = "No passphrase entered!\r\n";
			} 
			else 
			{
				// passphrase could be valid, check it
				if (wpaREA.test(key) == false) 
				{
					keyInvalid = 1;
					errMsg = "WPA passphrase is not the correct length!\r\n";
					errMsg += "Passphrase must be 8-63 characters long.\r\n";
				} 
				else 
				{
					keyInvalid = 0;
				}
			}
		} while (keyInvalid);
	}
	
	return key;
}


// ======================================
// = Connect to the passed Network SSID =
// ======================================
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

    //if (wlanCode == 1)
		//document.getElementById("wlan").value = "infra" 
    //else if (wlanCode == 2)
		//document.getElementById("wlan").value = "adhoc" 
    //else if (wlanCode == 3)
		//document.getElementById("wlan").value = "wfp2p" 
    //else if (wlanCode == 4)
    //else 
		//document.getElementById("wlan").value = "sofap" 

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


// ==============================
// = Joins the Selected Network =
// ==============================
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
    
    // now switch to the network
	switchNetwork(id);
}

// ================================
// = User Entered Network Routine =
// = configure.html "Other Network..."
// ================================
function userSelectNetwork() {
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

// ======================================
// = WiFi Scan for Networks =
// = configure.htm "WPS Push Button (WPS-PBC)"
// ======================================
function userSelectWPSButton()
{
if (1)
	alert("This will re-start as client in infrastructure network in WPS PBC security mode: To be implemented later ........");
else
	var sel = confirm("Push the WPS button on the Access Point then click \"OK\"");
	
	if (sel == true) 
	{
	    // Set SSID to ""
	    
	    // user hit ok
		alert("WPS PBC : Initiating connection with Access Point ........");
	} 
	else
	{
		// user hit cancel, so need to go back to main screen and not submit
		return "__UFU__";
	} 
}

// ======================================
// = WiFi Scan for Networks =
// = configure.htm "WPS PIN (WPS-PIN)"
// ======================================
function userSelectWPSPin()
{
if (1)
	alert("This will re-start as client in infrastructure network in WPS PIN security mode: To be implemented later ........");
else
    {
    var ssid = prompt("1. Please register the following Pin Number on the AP: 12390212\n2. Enter the SSID of AP below.\n3. Click \"OK\" to connect.","");

	if (ssid != null && ssid != "")
	{
	    // user hit ok
		alert("WPS PIN : Initiating connection with Access Point ........");
	}
	else
	{
	 	// user hit cancel
	    alert("Please enter SSID of AP to join.");
		return;
	}
    }
}

// ======================================
// = WiFi Scan for Networks =
// = configure.html "Ad-Hoc"
// ======================================
function userSelectStartUpAdHoc()
{
	alert("This will re-start as adhoc network: To be implemented later ........");
}

// ======================================
// = WiFi Scan for Networks =
// = configure.htm "Soft AP"
// ======================================
function userSelectStartUpSoftAP()
{
	alert("If network is currently in non-softAP mode, this will re-start as SoftAP : To be implemented later ........");
}

// ======================================
// = WiFi Scan for Networks =
// = configure.htm "WiFi Direct (group client role)"
// ======================================
function userSelectStartUpWiFiDirect()
{
	alert("This will re-start as a Wifi Direct group client : To be implemented later ........");
}

function adhocSel()
{
	if (document.getElementById('sec1').options.length == 3) 
	{
		document.getElementById('sec1').remove(2);
	}
}

function infraSel() 
{
	if (document.getElementById('sec1').options.length == 2) 
	{
		document.getElementById('sec1').options[2] = new Option('WPA/WPA2 Passphrase', '5');	
	}
}


// ===============================================
// = WiFi add Network Information to Scan Table. =
// ===============================================
function addScanRow(ssid, sec, str, wlan) 
{
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
	if (sec > 0) 
	{
		data2.appendChild(secImg);
	} 
	else 
	{
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

function deleteScanTable() 
{
    var table = document.getElementById('scanTable').getElementsByTagName('tbody')[0]; 
    var rows = table.rows;

    while (rows.length - 1) // length=1 -> stop 
        table.deleteRow(rows.length - 1); 
}


function printButtonName()
{
	var textLabel = "Scanning";
	
	for (i = 0; i < scanDots % 4; i++) 
	{
		textLabel += ".";
	}
	
	scanDots++;
	
	document.getElementById("rescan").value = textLabel;
}

function updateStatus(xmlData) 
{	
	var urlPath = window.location.pathname;
	var pageName = urlPath.substring(urlPath.lastIndexOf('/') + 1);
	
	if (pageName == 'configure.htm') {
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
	else // either index.htm or nothing (xxx.xxx.xxx.xxx gives no page name) 
	{
		// Check if a timeout occurred
		if(!xmlData)
		{
			document.getElementById('display').style.display = 'none';
			document.getElementById('loading').style.display = 'inline';
			return;
		}

		// Make sure we're displaying the status display
		document.getElementById('loading').style.display = 'none';
		document.getElementById('display').style.display = 'inline';

		// LED0
		if(getXMLValue(xmlData, 'led0') == '1')
			document.getElementById('led0').style.color = "green";
		else
			document.getElementById('led0').style.color = '#ddd';

		// LED1
		if(getXMLValue(xmlData, 'led1') == '1')
			document.getElementById('led1').style.color = "orange";
		else
			document.getElementById('led1').style.color = '#ddd';
		
		// LED2
		if(getXMLValue(xmlData, 'led2') == '1')
			document.getElementById('led2').style.color = "red";
		else
			document.getElementById('led2').style.color = '#ddd';

		// Button0
		if(getXMLValue(xmlData, 'btn0') == 'up')
			document.getElementById('btn0').innerHTML = '&Lambda;';
		else
			document.getElementById('btn0').innerHTML = 'V';

	}
}

// ======================================
// = WiFi Scan for Networks =
// = configure.htm "Scan for Wireless Networks"
// ======================================
function rescanNetwork()
{
    if (1)
    {
	alert("This will invoke scanning. To be implemented later ........");
    }
    else
    {
	scanDots = 0;
	printButtonName();
	document.getElementById("rescan").disabled = true;
	
	// generate a request to hardware to issue a rescan
	newAJAXCommand('scan.cgi?scan=1');
	
	// delete old table, replace with new table after scan is finished
        deleteScanTable();
	
	currBss = 0; // reset the current bss pointer
	
	setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 50);
    }
}

// ======================================
// = WiFi Display Scan Results  =
// = configure.htm "Display Scan Results"
// ======================================
function displayScanResults()
{
        var textLabel = "This is scan results before starting up in softAP ...  ";

	scanDots = 0;
	//printButtonName();
	document.getElementById("rescan").disabled = true;
        
        document.getElementById("rescan").value = textLabel;
        // alert("This is scan results before starting up in softAP ... ");

	// generate a request to hardware to issue a rescan
	newAJAXCommand('scan.cgi?scan=1');
	
	// delete old table, replace with new table after scan is finished
        deleteScanTable();
	
	currBss = 0; // reset the current bss pointer
	
	setTimeout("newAJAXCommand('status.xml', updateStatus, false)", 50);
}


//kick off the AJAX Updater
setTimeout("pollAJAX()", 10);
