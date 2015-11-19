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
	6. Put flolder 'DATA' to the executable directory. There are some
	   important configure files.

	   
	   
	   
Modifications:
    1. igstkImageSpatialObjectVolumeRepresentation.h/.hxx
	       vtkSTLWriter path re-direct to ./
	2. Navigator::AcceptTrackerFiducialProcessing()
	       disable transform valid check (problem occur in laptop)
	
	3. disable Polaris Vicra bump detection. Or the tracker will not show.
	
	
	
	
	
	
	
	