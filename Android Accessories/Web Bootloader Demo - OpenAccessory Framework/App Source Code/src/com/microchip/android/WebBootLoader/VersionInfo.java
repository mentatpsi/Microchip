package com.microchip.android.WebBootLoader;

import com.android.future.usb.UsbAccessory;

/** This class holds information for an accessory
 *  
 * @author Microchip Technology Inc.
 *
 */
public class VersionInfo {
	// Private version variables for accessory
	private int major = -1;
	private int minor = -1;
	private int dot = -1;
	
	// Private variables to hold model, description, url strings
	private String model = null;
	private String description = null;
	private String url = null;
	
	// Constant string for the bootloader's model name
	protected static final String bootloader_model_name = "Web Bootloader Accessory Demo";
	
	/** Creates an empty VersionInfo object
	 * 
	 */
	public VersionInfo() {
		
	}
	
	/** Creates a VersionInfo object from a UsbAccessory object
	 * 
	 * @param accessory A UsbAccessory object with information about an attached accessory
	 */
	public VersionInfo(UsbAccessory accessory) {
		// Split the version of the accessory up into individual strings with '.' as the delimiter
		String delim = "[.]";
		String[] tokens = accessory.getVersion().split(delim);
		
		// Save the split up strings into the dot, minor, and major versions
		switch(tokens.length)
		{
		case 3:
			dot = Integer.parseInt(tokens[2]);
		case 2:
			minor = Integer.parseInt(tokens[1]);
		case 1:
			major = Integer.parseInt(tokens[0]);
		}
		
		// Save the model and description
		model = accessory.getModel();
		description = accessory.getDescription();
	}
	
	/** Changes the major version
	 * 
	 * @param major The new major version
	 */
	public void setMajorVersion(int major) {
		this.major = major;
	}
	
	/** Changes the minor version
	 * 
	 * @param minor The new minor version
	 */
	public void setMinorVersion(int minor) {
		this.minor = minor;
	}
	
	/** Changes the dot version
	 * 
	 * @param dot The new dot version
	 */
	public void setDotVersion(int dot) {
		this.dot = dot;
	}
	
	/** Changes the model name
	 * 
	 * @param model The new model name
	 */
	public void setModel(String model) {
		this.model = model;
	}
	
	/** Changes the description name
	 * 
	 * @param description The new description name
	 */
	public void setDescription(String description) {
		this.description = description;
	}
	
	/** Changes the URL string
	 * 
	 * @param url The new URL string
	 */
	public void setURL(String url) {
		this.url = url;
	}
	
	/** Returns the major version
	 * 
	 * @return int The major version value
	 */
	public int getMajorVersion() {
		return major;
	}
	
	/** Returns the minor version
	 * 
	 * @return int The minor version value
	 */
	public int getMinorVersion() {
		return minor;
	}
	
	/** Returns the dot version
	 * 
	 * @return int The dot version value
	 */
	public int getDotVersion() {
		return dot;
	}
	
	/** Returns the full string for version
	 * 
	 * @return String The full version in string form
	 */
	public String getVersionString() {
		// If the major version hasn't been defined, then return null
		if(major == -1) {
			return null;
		} else {
			// Otherwise return the major, minor, and dot versions (if they're defined)
			if(minor == -1) {
				return new Integer(major).toString();
			} else {
				if(dot == -1) {
					return new Integer(major).toString() + "."
					+ new Integer(minor).toString();
				} else {
					return new Integer(major).toString() + "."
					+ new Integer(minor).toString() + "."
					+ new Integer(dot).toString();
				}
			}
		}

	}
	  
	/** Returns the model string
	 * 
	 * @return String The model string
	 */
	public String getModel() {
		return model;
	}
	
	/** Returns the description string
	 * 
	 * @return String The description string
	 */
	public String getDescription() {
		return description;
	}
	
	/** Returns the URL string
	 * 
	 * @return String The URL string
	 */
	public String getURL() {
		return url;
	}
	
	/** Returns the filename from the URL in string form
	 * 
	 * @return String The filename string
	 */
	public String getFileName() {
		// Return null if the URL hasn't been defined
		if(url == null) {
			return null;
		} else {
			// Otherwise delimit the URL by '/' and return the last token
			String delim = "[/]";
			String[] tokens = url.split(delim);
			return tokens[tokens.length - 1];
		}
	}
	
	/** Compares versions to see which is newer
	 * 
	 * @param current_major The current major version of the accessory
	 * @param current_minor The current minor version of the accessory
	 * @param current_dot The current dot version of the accessory
	 * @return boolean True if this version is newer than the current, false otherwise
	 */
	public boolean isNewerVersionThan(int current_major, int current_minor, int current_dot) {
		// Return true if major is newer
		if( current_major < this.major) {
			return true;
		} else if( current_major == this.major) {
			// Otherwise return true if minor is newer if major is the same
			if( current_minor < this.minor) {
				return true;
			} else if( current_minor == this.minor) {
				// Otherwise return true if dot is newer if minor is also the same
				if( current_dot < this.dot) {
					return true;
				}
			}
		}
		// Otherwise it isn't newer, return false
		return false;
	}
	
	/** Compares versions to see which is newer
	 * 
	 * @param current_version The current version VersionInfo object to compare to
	 * @return boolean True if this version is newer than the current, false otherwise
	 */
	public boolean isNewerVersionThan(VersionInfo current_version) {
		return isNewerVersionThan(current_version.getMajorVersion(), current_version.getMinorVersion(), current_version.getDotVersion());
	}

	/** Shows whether this version is the bootloader or not
	 * 
	 * @return boolean True if this accessory is in bootloader mode, false otherwise
	 */
	public boolean isInBootloadMode() {
		// The bootloader has a specific model name that we're looking for
		return model.equals(bootloader_model_name);
	}
}
