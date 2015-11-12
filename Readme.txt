Navigator Project
---------------------------

1. Derive from "IGSTKSandbox/Examples"

2. Device: NDI tracker, P6-01480

3. Re-implemented module: read NiFTI(.nii) file module
   File name: igstkNiftiImageReader.h igstkNiftiImageReader.hxx




Usage:
   1. Compatible devece: NDI Polaris Vicra
   2. Specify vicra Configuration first, filename: vicraConfiguration.xml
   3. Modify the followings in the configuration file:
		a. refresh rate
		b. serial communication port
		c. reference model (.rom file, provided by NDI)
		d. tracker tool model (.rom file, provided by NDI)
		e. tracker calibrate file (Calibrate1.xml)
	4. You should install NDI tracker softwares to use the device driver
	5. The configuration file path is defined and compiled to binary file

