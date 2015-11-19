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

#find_package(GDCM REQUIRED)

######-----------add by qinshuo----------######

SUBDIRS(
 TransformReaders
 TrackerConfiguration
 Tools
)

#
# Examples that require FLTK
set(IGSTK_USE_FLTK 1)

IF(IGSTK_USE_FLTK)
  SUBDIRS(
    Navigator
	Navigator2
	Navigator3
   )
ENDIF(IGSTK_USE_FLTK)


