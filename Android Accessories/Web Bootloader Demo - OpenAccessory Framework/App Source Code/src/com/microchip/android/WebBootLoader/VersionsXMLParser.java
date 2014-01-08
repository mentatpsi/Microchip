package com.microchip.android.WebBootLoader;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.Vector;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

/** This class parses the XML file that holds all of the update info and can 
 *  filter out the updates and return updates for specific models and descriptions
 *  
 * @author Microchip Technology Inc.
 *
 */
public class VersionsXMLParser implements Runnable {
	// Private variables
	private Vector<VersionInfo> Versions = new Vector<VersionInfo>(0);
	private Document doc = null;
	private Handler handler = null;
	private int what = 0;
	private InputStream stream;
	
	// Public enum to keep track of what type of filtering is requested
	public enum VersionFilterMode {
		EQUAL_TO, EQUAL_TO_OR_GREATER_THAN, GREATER_THAN
	}

	/** Creates a new VersionsXMLParser object that saves the handler and message code
	 * 
	 *  @param handler The handler to send messages to
	 *  @param what    The message code to send to the handler
	 */
	public VersionsXMLParser(Handler handler, int what) {
		this.handler = handler;
		this.what = what;
	}

	/** Starts the thread and parses the xml file
	 * 
	 *  @param file The file to parse
	 */
	public void ParseThreaded(File file) {
		// Create a stream and save it from the file
		try {
			stream = new BufferedInputStream(new FileInputStream(file));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		// Start parsing in new thread
		new Thread(this).start();
	}
	
	/** Starts the thread and parses the xml file
	 * 
	 *  @param stream The stream to parse
	 */
	public void ParseThreaded(InputStream stream) {
		// Save stream and start parsing in new thread
		this.stream = stream;
		new Thread(this).start();		
	}
	
	/** Returns the results of parsing the xml file
	 * 
	 *  @return Vector<VersionInfo> A vector of VersionInfo objects
	 */
	public Vector<VersionInfo> GetResults() {
		return Versions;
	}
	
	/** Filters the results of the xml file by description. All other descriptions
	 *  will be filtered out.
	 * 
	 *  @param String The string for the description that you want to filter by
	 */
	public void filterResults(String description) {
		// Iterate through Versions and remove those that do not have the same description
		for(int i=0; i<Versions.size(); i++) {	
			if( Versions.elementAt(i).getDescription().equals(description) == false) {
				Versions.remove(i--);
			}
		}
	}

	/** Filters the results of the xml file by description and model. All other
	 *  descriptions and models will be filtered out.
	 * 
	 *  @param model       The string for the model that you want to filter by
	 *  @param description The string for the description that you want to filter by
	 */
	public void filterResults(String model, String description) {
		// First filter by description
		filterResults(description);
		
		// Then iterate through Versions and remove those that do not have the same model
		for(int i=0; i<Versions.size(); i++) {				
			if( Versions.elementAt(i).getModel().equals(model) == false) {
				Versions.remove(i--);
			}
		}
	}
	
	/** Filters the results of the xml file by major version number and by filter mode
	 * 
	 *  @param major The major version to compare to
	 *  @param mode  The mode in which you want to compare versions
	 */
	public void filterResults(int major, VersionFilterMode mode) {
		for(int i=0; i<Versions.size(); i++) {
			switch(mode) {
			case EQUAL_TO:
				// Check to see if major version is equal, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() != major) {
					Versions.remove(i--);
				}
				break;
			case EQUAL_TO_OR_GREATER_THAN:
				// Check to see if major version is equal or greater, if not, remove from vector	
				if( Versions.elementAt(i).getMajorVersion() < major) {
					Versions.remove(i--);
				}
				break;
			case GREATER_THAN:
				// Check to see if major version is greater, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() <= major) {
					Versions.remove(i--);
				}
				break;
			}
		}
	}
	
	/** Filters the results of the xml file by major and minor version number and by filter mode
	 * 
	 *  @param major The major version to compare to
	 *  @param minor The minor version to compare to
	 *  @param mode  The mode in which you want to compare versions
	 */
	public void filterResults(int major, int minor, VersionFilterMode mode) {
		for(int i=0; i<Versions.size(); i++) {
			switch(mode) {
			case EQUAL_TO:
				// Check to see if major and minor version is equal, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() != major) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getMinorVersion() != minor) {
					Versions.remove(i--);
				}
				break;
			case EQUAL_TO_OR_GREATER_THAN:
				// Check to see if major and minor version is equal or greater, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() < major) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getMajorVersion() == major) {
					if( Versions.elementAt(i).getMinorVersion() < minor) {
						Versions.remove(i--);
					}
				}
				break;
			case GREATER_THAN:
				// Check to see if major and minor version are greater, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() < major) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getMajorVersion() == major) {
					if( Versions.elementAt(i).getMinorVersion() <= minor) {
						Versions.remove(i--);
					}
				}
				break;
			}
		}
	}
	
	/** Filters the results of the xml file by major and minor version number and by filter mode
	 * 
	 *  @param model       The string for the model that you want to filter by
	 *  @param description The string for the description that you want to filter by
	 */
	public void filterResults(int major, int minor, int dot, VersionFilterMode mode) {
		for(int i=0; i<Versions.size(); i++) {
			switch(mode) {
			case EQUAL_TO:
				// Check to see if full version is equal, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() != major) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getMinorVersion() != minor) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getDotVersion() != dot) {
					Versions.remove(i--);
				}
				break;
			case EQUAL_TO_OR_GREATER_THAN:
				// Check to see if full version is equal or greater, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() < major) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getMajorVersion() == major) {
					if( Versions.elementAt(i).getMinorVersion() < minor) {
						Versions.remove(i--);
					} else if( Versions.elementAt(i).getMinorVersion() == minor) {
						if( Versions.elementAt(i).getDotVersion() < dot) {
							Versions.remove(i--);
						}
					}
				}
				break;
			case GREATER_THAN:
				// Check to see if full version is greater, if not, remove from vector
				if( Versions.elementAt(i).getMajorVersion() < major) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getMajorVersion() == major) {
					if( Versions.elementAt(i).getMinorVersion() < minor) {
						Versions.remove(i--);
					} else if( Versions.elementAt(i).getMinorVersion() == minor) {
						if( Versions.elementAt(i).getDotVersion() <= dot) {
							Versions.remove(i--);
						}
					}
				}
				break;
			}
		}
	}
	
	/** Filters the results of the xml file and removes all but the newest versions.
	 *  It's best to call this after filtering by description and/or model.
	 * 
	 */
	public void filterResultsBestVersionOnly() {
		int bestMajor = -1;
		int bestMinor = -1;
		int bestDot = -1;
		
		//Loop to find the best version, and remove versions lesser than the best
		for(int i=0; i<Versions.size(); i++) {
			if( Versions.elementAt(i).getMajorVersion() < bestMajor) {
				Versions.remove(i--);
			} else if( Versions.elementAt(i).getMajorVersion() == bestMajor) {
				if( Versions.elementAt(i).getMinorVersion() < bestMinor) {
					Versions.remove(i--);
				} else if( Versions.elementAt(i).getMinorVersion() == bestMinor) {
					if( Versions.elementAt(i).getDotVersion() <= bestDot) {
						Versions.remove(i--);
					} else {
						bestMajor = Versions.elementAt(i).getMajorVersion();
						bestMinor = Versions.elementAt(i).getMinorVersion();
						bestDot = Versions.elementAt(i).getDotVersion();
					}
				} else {
					bestMajor = Versions.elementAt(i).getMajorVersion();
					bestMinor = Versions.elementAt(i).getMinorVersion();
					bestDot = Versions.elementAt(i).getDotVersion();
				}
			} else {
				bestMajor = Versions.elementAt(i).getMajorVersion();
				bestMinor = Versions.elementAt(i).getMinorVersion();
				bestDot = Versions.elementAt(i).getDotVersion();
			}
		}
		
		//Now that we know what the best version is, remove the rest that might be remaining
		filterResults(bestMajor, bestMinor, bestDot, VersionsXMLParser.VersionFilterMode.EQUAL_TO);
	}	
	
	/** This is the main working function that parses the xml file for the device, version,
	 *  model, description, and individual version tags and creates a new VersionInfo object
	 *  for each entry and adds it to the vector.
	 * 
	 */
	private void ParseMain() {
		// Separate into devices
		NodeList devices = doc.getElementsByTagName("device");
		
		// Iterate through each device
		for(int i=0;i<devices.getLength();i++) {
			// Get the current device
			Node device = devices.item(i);
			
			// Get the versions, models, and descriptions for this device
			NodeList versions = ((Element) device).getElementsByTagName("version");
			NodeList model = ((Element) device).getElementsByTagName("model");
			NodeList description = ((Element) device).getElementsByTagName("description");
			
			// Iterate through each version of this device
			for(int j=0; j < versions.getLength(); j++) {
				// Create an empty VersionInfo object
				VersionInfo info = new VersionInfo();
				
				// Separate the current version into major, minor, dot, and url values
				Node version = versions.item(j);
				NodeList major = ((Element) version).getElementsByTagName("major");
				NodeList minor = ((Element) version).getElementsByTagName("minor");
				NodeList dot = ((Element) version).getElementsByTagName("dot");
				NodeList url = ((Element) version).getElementsByTagName("url");
				
				// If there is a model tag (there should only be one)
				if(model.getLength() > 0) {
					// Then just take the first one and add it to the VersionInfo object's model value
					Node item = model.item(0).getFirstChild();
					info.setModel(item.getNodeValue());
				} else {
					// Otherwise go to the next device, since there needs to be a model
					break;
				}
				
				// If there is a description tag (there should only be one)
				if(description.getLength() > 0) {
					// Then just take the first one and add it to the VersionInfo object's description value
					Node item = description.item(0).getFirstChild();
					info.setDescription(item.getNodeValue());
				} else {
					// Otherwise go to the next device, since there needs to be a description
					break;
				}
				
				// If there is a URL (there should only be one)
				if(url.getLength() > 0) {
					// Then just take the first on and add it to the VersionInfo object's URL value
					Node item = url.item(0).getFirstChild();
					info.setURL(item.getNodeValue());
				} else {
					// Otherwise go to the next version, since there needs to be a URL
					continue;
				}
				
				// If there is a major version (there should only be one)
				if(major.getLength() > 0) {
					// Then just take the first one and add it to the VersionInfo object's major value
					Node item = major.item(0).getFirstChild();
					info.setMajorVersion(Integer.parseInt(item.getNodeValue()));
				} else {
					// Otherwise go to the next version, since there needs to be a major version
					continue;
				}
				
				// If there is a minor version (there should only be one or none)
				if(minor.getLength() > 0) {
					// Then just take the first one and add it tot he VersionInfo object's minor value
					Node item = minor.item(0).getFirstChild();
					info.setMinorVersion(Integer.parseInt(item.getNodeValue()));
				}
				
				// If there is a dot version (there should only be one or none)
				if(dot.getLength() > 0) {
					// Then just take the first one and add it to the VersionInfo object's dot value
					Node item = dot.item(0).getFirstChild();
					info.setDotVersion(Integer.parseInt(item.getNodeValue()));
				}
				
				// Add the VersionInfo object to the vector list
				Versions.add(info);
			} //per version loop
		} //per device loop
	} //ParseMain function

	/** The running function of the thread. Parses through the xml file and sends message to handler when done.
	 * 
	 **/
	@Override
	public void run() {
		// Remove all the elements within the vector
		Versions.removeAllElements();
		
		// Obtain new instance of DocumentBuilderFactory
		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		
		try {
			// From the DocumentBuilderFactory instance, create a new DocumentBuilder
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			
			// Create a Document file by parsing the xml stream with the DoucmentBuilder object
			doc = dBuilder.parse(stream);
			
			// Parse through this Document to populate the vector
			ParseMain();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		// When finished parsing, check to see if handler is defined
		if(handler != null) {
			// If so, then send message to handler with this instance as the attached object
			try {
				Message XMLResults = Message.obtain(handler, what);
				XMLResults.obj = this;
				handler.sendMessage(XMLResults);
			} catch (Exception e) {
				Log.e("XMLParser",e.toString());
			}
		}
	}
} //VersionsXMLParser class
