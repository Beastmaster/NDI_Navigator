#
# Examples that do not require GUI
#


######----------add by qinshuo-----------######

cmake_minimum_required(VERSION 2.8)


Find_package(FLTK REQUIRED)
INCLUDE(${FLTK_USE_FILE})

Find_package(IGSTK REQUIRED)
INCLUDE(${IGSTK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(GDCM REQUIRED)

######-----------add by qinshuo----------######

SUBDIRS(
 TransformReaders
 TransformWriters
 TrackerConfiguration
 Tools
 TrackerDataLogger
 MultiTrackerLogger
)



#
# Examples that require FLTK
#
IF(IGSTK_USE_FLTK)
  SUBDIRS(
    HelloWorld
    OneViewAndTracking
    FourViewsAndTracking
    TrackingVolumeViewer 
    NeedleBiopsy
    Navigator
	Navigator2
    PivotCalibrationFLTKWidget
   )

   IF(IGSTK_USE_MicronTracker)
     SUBDIRS(OneViewAndTrackingUsingMicronTracker)
   ENDIF(IGSTK_USE_MicronTracker)
ENDIF(IGSTK_USE_FLTK)

IF(IGSTK_USE_Qt)
   IF(IGSTK_USE_MicronTracker)
     SUBDIRS(OneViewAndTrackingUsingQTWidgetAndMicronTracker)
   ENDIF(IGSTK_USE_MicronTracker)
ENDIF(IGSTK_USE_Qt)

IF (IGSTK_USE_OpenIGTLink)
    SUBDIRS(  OpenIGTLinkTrackerBroadcasting )
    SUBDIRS(  OpenIGTLinkConfigurationReaders )
ENDIF (IGSTK_USE_OpenIGTLink)

IF(IGSTK_USE_VideoImager)
 IF( WIN32 )
   IF (IGSTK_USE_OpenCV)
      SUBDIRS( VideoFrameGrabberAndViewerWebcam )
   ENDIF (IGSTK_USE_OpenCV)
 ENDIF( WIN32 )

 IF(IGSTK_USE_OpenIGTLink)
  SUBDIRS( VideoFrameGrabberAndViewerOpenIGTLink )
 ENDIF(IGSTK_USE_OpenIGTLink)

ENDIF(IGSTK_USE_VideoImager)
 


