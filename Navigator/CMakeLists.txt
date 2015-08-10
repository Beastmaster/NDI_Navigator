cmake_minimum_required(VERSION 2.8)


PROJECT(Navigator)

Find_package(FLTK REQUIRED)
INCLUDE(${FLTK_USE_FILE})

Find_package(IGSTK REQUIRED)
INCLUDE(${IGSTK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

INCLUDE_DIRECTORIES (
#  ${IGSTK_BINARY_DIR}
  ../TrackerConfiguration
  ../TransformReaders
  ../TrackerConfiguration/configurationFileReaders
  ../Tools
) 

 
SET(Navigator_GUI_SRCS
    NavigatorGUI.fl
   )
  
SET(Navigator_SRCS
  main.cxx
  MyView.h
  MyView.cxx
  MyView2D.h
  MyView2D.cxx
  MyView3D.h
  MyView3D.cxx
  Navigator.h
  Navigator.cxx
  NavigatorQuadrantViews.h
  NavigatorQuadrantViews.cxx
  igstkImageSpatialObjectSurfaceRepresentation.h
  igstkImageSpatialObjectSurfaceRepresentation.txx
  igstkImageSpatialObjectVolumeRepresentation.h
  igstkImageSpatialObjectVolumeRepresentation.txx
  igstkImageSliceObjectRepresentation.txx
  igstkImageSliceObjectRepresentation.h 
  igstkImageSliceObjectRepresentationPlus.txx
  igstkImageSliceObjectRepresentationPlus.h
  StockColorMap.h  
  ICPTransform.cxx
  ICPTransform.h
  NavigatorRegistrationBase.cxx
  NavigatorRegistrationBase.h
  igstkNiftiImageReader.h
  igstkNiftiImageReader.hxx
)
  
IF(IGSTK_USE_MicronTracker)
  SET(Navigator_SRCS ${Navigator_SRCS}
  # MicronTracker related classes
  )
ENDIF(IGSTK_USE_MicronTracker)

INCLUDE_DIRECTORIES(
  ${Navigator_SOURCE_DIR}
  ${Navigator_BINARY_DIR}
  )

FLTK_WRAP_UI(Navigator ${Navigator_GUI_SRCS})
ADD_EXECUTABLE(Navigator ${Navigator_FLTK_UI_SRCS} ${Navigator_SRCS})
TARGET_LINK_LIBRARIES(Navigator TrackerConfiguration TransformReaders Tools IGSTK )


 target_link_libraries(Navigator ${VTK_LIBRARIES} ${ITK_LIBRARIES} ${ITK_VERSION_MAJOR})
