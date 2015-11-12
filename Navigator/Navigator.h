/*=========================================================================

  Program:				Surgical Navigation System
  Organization:			RC-MIC (CUHK)
  File Name:			Navigator.h
  Language:				C++
  Lasted Updated:       2015/11/12
  Version:				2.0
  CopyRight    Research Center of Medical Imaging Center 
				(Chinese University of HongKong)   All Rights Reserved
		Please be noted that: this program is knowledge property of 
		RC-MIC(CUHK). You can contact the author or our office to 
		access more information. This program is for research purpose 
		and we take no responsibility for its consequences. 

=========================================================================*/
#ifndef __Navigator_h
#define __Navigator_h

#include "NavigatorGUI.h"

#include "igstkConfigure.h"

#include "igstkStateMachine.h"
#include "igstkMacros.h"
#include "igstkObject.h"
#include "igstkEvents.h"

/**#include "igstkMRImageReader.h"
#include "igstkMRImageSpatialObject.h"
#include "igstkMRImageSpatialObjectRepresentation.h"**/
#include "igstkCTImageReader.h"
#include "igstkCTImageSpatialObject.h"
#include "igstkCTImageSpatialObjectRepresentation.h"

#include "igstkMRImageReader.h"   //qinshuo add
#include "igstkMRImageSpatialObject.h"    // qinshuo add
#include "igstkMRImageSpatialObjectRepresentation.h" // qinshuo add

#include "igstkImageSpatialObjectVolumeRepresentation.h"  //Sun adds
#include "igstkImageSpatialObjectSurfaceRepresentation.h"  //Sun adds

#include "igstkReslicerPlaneSpatialObject.h"
#include "igstkImageSliceObjectRepresentation.h"
#include "igstkImageSliceObjectRepresentationPlus.h"
#include "igstkMeshResliceObjectRepresentation.h"

#include "igstkTracker.h"
#include "igstkTrackerTool.h"

#include "igstkLandmark3DRegistration.h"

#include "igstkEllipsoidObject.h"
#include "igstkEllipsoidObjectRepresentation.h"

#include "igstkMeshReader.h"
#include "igstkMeshObjectRepresentation.h"

#include "igstkCylinderObject.h"
#include "igstkCylinderObjectRepresentation.h"

#include "igstkAxesObject.h"

#include "igstkToolProjectionSpatialObject.h"
#include "igstkToolProjectionObjectRepresentation.h"

#include "igstkCrossHairSpatialObject.h"
#include "igstkCrossHairObjectRepresentation.h"

#include "igstkAnnotation2D.h"

#include "itkStdStreamLogOutput.h"

#include "igstkFiducialsPlanIO.h"
#include "igstkPathPlanIO.h"

#include "igstkTrackerController.h"
#include "igstkTrackerConfiguration.h"
#include "igstkCoordinateSystemTransformToResult.h"

#include "igstkTransformFileReader.h"
#include "igstkRigidTransformXMLFileReader.h"
#include "igstkTrackerConfigurationFileReader.h"
#include "igstkTrackerConfigurationXMLFileReaderBase.h"
#include "igstkTransform.h"
#include "igstkNiftiImageReader.h"


//qinshuo add: file path
#define ImagePath_DEF         "D:/QIN/DATA"
#define OverlayPath_DEF       "D:/QIN/DATA"
#define MeshPath_DEF          "D:/QIN/DATA"
#define SecondImagePath_DEF   "D:/QIN/DATA"
#define VicraConfigurationFile_DEF			"D:/QIN/DATA/vicraConfiguration.xml"
#define ToolSpatial_DEF						"D:/QIN/DATA/TrackerToolRepresentationMeshes/needletip.msh"
#define Target_DEF							""




class vtkImageMapToColors;
class vtkPlaneSource;
class vtkPlane;
class vtkImageSlice;
class vtkImageShiftScale;
class vtkImageResliceMapper;
class vtkImageProperty;
class vtkLookupTable;
class my_MRImageReader;
class com_DicomImageSpatialObject;
class com_DicomImageReader;

namespace igstk
{
/***   .nii file reader   **/
	class my_MRImageReader:  public NiftiImageReader< MRImageSpatialObject >
	{

	public:

	  /** Macro with standard traits declarations. */
	  igstkStandardClassTraitsMacro( my_MRImageReader, 
									 NiftiImageReader< MRImageSpatialObject > )

	 /** Event type */
     //	igstkLoadedTemplatedObjectEventMacro( ImageModifiedEvent, IGSTKEvent, MRImageSpatialObject);

	  /** Declarations needed for the Logger */
	  igstkLoggerMacro();

	protected:

	  my_MRImageReader( void ):m_StateMachine(this){};
	  virtual ~my_MRImageReader( void ) {};

	  /** check if the dicom data is from a "CT" modality */
	  bool CheckModalityType(){};

	  /** Print the object information in a stream. */
	  void PrintSelf( std::ostream& os, itk::Indent indent ) const
	  {
		    Superclass::PrintSelf(os, indent);
	  }; 

	private:
  
	  /** These two methods must be declared and note be implemented
	   *  in order to enforce the protocol of smart pointers. */
		my_MRImageReader(const Self&);         //purposely not implemented
		void operator=(const Self&);        //purposely not implemented
	};


/**   common spatial image object regardless of modiality  **/
	class com_DicomImageSpatialObject : 
		  public ImageSpatialObject< signed short, 3 >
	{
	public:
	  /** Type of the superclass. 
	   *  This must be declared first because the StandardClassTraitsMacro
	   *  will otherwise get confused with the commas of the template */
	  typedef ImageSpatialObject< signed short, 3>     SuperclassType;
  
	  /** Macro with standard traits declarations. */
	  igstkStandardClassTraitsMacro( com_DicomImageSpatialObject, SuperclassType )

	protected:

	  com_DicomImageSpatialObject():m_StateMachine((Self *)this){}
	  virtual ~com_DicomImageSpatialObject(){};

	  /** Print the object information in a stream. */
	  void PrintSelf( std::ostream& os, itk::Indent indent ) const {;} 

	private:
 
	  /** These two methods must be declared and note be implemented
	   *  in order to enforce the protocol of smart pointers. */
	  com_DicomImageSpatialObject(const Self&);     //purposely not implemented
	  void operator=(const Self&);           //purposely not implemented
	};


/**   common dicom image reader regardless of modiality  **/
	class com_DicomImageReader:  public DICOMImageReader< CTImageSpatialObject >
	{

	public:

	  /** Macro with standard traits declarations. */
	  igstkStandardClassTraitsMacro( com_DicomImageReader, 
									 DICOMImageReader< CTImageSpatialObject > )

	protected:

	  com_DicomImageReader( void ) : m_StateMachine(this){}
	  virtual ~com_DicomImageReader( void ) {};
	  /** Check if MRI dicom is being read */
	  // the modification here is: no matter what the modality is, just read the image!
	  bool CheckModalityType( DICOMInformationType modaltiy )
	  {
		  if( modaltiy != "MR" )			  return true;
		  else								  return true;
	  }
	  /** Print the object information in a stream. */
	  void PrintSelf( std::ostream& os, itk::Indent indent ) const {Superclass::PrintSelf(os, indent);}

	private:
  
	  /** These two methods must be declared and note be implemented
	   *  in order to enforce the protocol of smart pointers. */
	  com_DicomImageReader(const Self&);         //purposely not implemented
	  void operator =(const Self&);        //purposely not implemented
	};
}



/** \class Navigator
* 
* \brief Implementation class for NavigatorGUI.
*
* This class implements the main application.
*
*/
class Navigator : public NavigatorGUI
{
public:

  /** Typedefs */
  igstkStandardClassBasicTraitsMacro( Navigator, NavigatorGUI );
  
  igstkLoggerMacro();

  /** Declarations needed for the State Machine */
  igstkStateMachineMacro();  

  /** typedef for image reader */
  typedef igstk::CTImageReader                        ImageReaderType;
  typedef igstk::com_DicomImageReader				  DicomReaderType;  //qinshuo add: read dicom images regardless of it modilaty
  typedef igstk::my_MRImageReader      				  MRImageReaderType;

  /** typedef for mesh readers */
  typedef igstk::MeshReader                           MeshReaderType;
  /** typedef for image spatial objects */
//  typedef ImageReaderType::ImageSpatialObjectType     ImageSpatialObjectType

  typedef igstk::MRImageSpatialObject                 MRI_ImageSpatialObjectType;
  typedef igstk::CTImageSpatialObject                 CT_ImageSpatialObjectType;
  typedef igstk::CTImageSpatialObject				  Dicom_ImageSpatialObjectType; //qinshuo add 


  typedef igstk::MeshObject                           MeshObjectType;  //Sun adds

  /** typedef for mesh spatial objects */
  typedef MeshReaderType::MeshObjectType              MeshType;

  typedef igstk::Transform                               TransformType;   //Sun adds For Reg

  /** typedef for a cylinder spatial object and representation
  * used to represent the tracked tool */
  typedef igstk::CylinderObject                      CylinderType;
  typedef igstk::CylinderObjectRepresentation        CylinderRepresentationType;

  /** typedef for an image voxel index and world coordinates */
  typedef CT_ImageSpatialObjectType::IndexType           IndexType;
  typedef CT_ImageSpatialObjectType::PointType           PointType;
  typedef MeshObjectType::PointType                   MeshPointType;  //sun adds
 

  /** Cross hair spatial object and representation */
  typedef igstk::CrossHairSpatialObject            CrossHairType;
  typedef igstk::CrossHairObjectRepresentation     CrossHairRepresentationType;

  /** tool projection spatial object and representation */
  typedef igstk::ToolProjectionSpatialObject             ToolProjectionType;
  typedef igstk::ToolProjectionObjectRepresentation      
                                               ToolProjectionRepresentationType;

  /** reslicer plane spatial object */
  typedef igstk::ReslicerPlaneSpatialObject              ReslicerPlaneType;

  /** tool representation */
  typedef igstk::MeshObjectRepresentation                MeshRepresentationType;

  /** mesh reslice representation */
  typedef igstk::MeshResliceObjectRepresentation  MeshResliceRepresentationType;

  /** image reslice representation */
  typedef igstk::ImageSliceObjectRepresentationPlus< Dicom_ImageSpatialObjectType >
                                                         ImageRepresentationTypePlus;
  typedef igstk::ImageSliceObjectRepresentation< CT_ImageSpatialObjectType >
                                                         ImageRepresentationType;
  typedef igstk::ImageSliceObjectRepresentation< MRI_ImageSpatialObjectType >
														 ImageRepresentationType2;

 // typedef vtkSmartPointer<vtkImageActor>               ImageRepresentationType;  //New add

  /** typedef for ImageRepresentationVolumeType */
  typedef igstk::ImageSpatialObjectVolumeRepresentation< CT_ImageSpatialObjectType >  VolumeRepresentationType; //Sun adds
  typedef igstk::ImageSpatialObjectSurfaceRepresentation< CT_ImageSpatialObjectType >  SurfaceRepresentationType; //Sun adds
  
  /** typedef for landmark registration types */
  typedef igstk::Landmark3DRegistration                        RegistrationType;

  typedef TransformType::VersorType                            VersorType;  //Sun adds

  typedef RegistrationType::LandmarkPointContainerType  
                                                     LandmarkPointContainerType;

  typedef std::map< unsigned int, bool >     AcceptedLandmarkPointContainerType;

  typedef itk::MemberCommand<Navigator>                     ProgressCommandType;



  /** Public request methods from the GUI */
  virtual void RequestLoadImage();
  virtual void RequestCancelImageLoad();
  virtual void RequestAcceptImageLoad();
  virtual void RequestLoadSecondImage();   //qinshuo add
  virtual void RequestLoadMesh();
  virtual void RequestLoadOverlay();       //New adds
  virtual void RequestLoadToolSpatialObject();
  virtual void RequestToggleSetImageFiducials();
  virtual void RequestStartSetTrackerFiducials();
  virtual void RequestEndSetTrackerFiducials();
  virtual void RequestAcceptTrackerFiducial();
  virtual void RequestPrepareToQuit();
  virtual void RequestScreenShot();  // Sun adds
  virtual void RequestLoadTarget();  // Sun adds
  virtual void RequestLoadMoving();  // Sun adds
  virtual void ChangeSelectedSurface(int); //Sun adds
  virtual void ChangeSelectedTpoint(); //Sun adds
  virtual void SetOpacityOfImplant();
  virtual void SetOpacityOfSkull();

  /** Public request methods from the GUI to help operation workflow */

  /**
   * Open the relevant tracker settings dialog (Polaris/Aurora/Micron/Ascension)
   * and get the users input.
   */
  virtual void RequestConfigureTracker();
  virtual void RequestInitializeTracker();
  virtual void RequestTrackerRegistration();
  virtual void RequestStartTracking();
  virtual void RequestStopTracking();
  virtual void RequestAcceptRegistration();
  virtual void RequestRejectRegistration();
  virtual void RequestChangeSelectedFiducial();
  virtual void RequestChangeSelectedViewMode();
  virtual void RequestDisconnectTracker();


   /**
   * Observer for the event generated by 
   * TrackerConfigurationFileReader->RequestGetData() method.
   */
  igstkObserverObjectMacro( TrackerConfiguration, 
         igstk::TrackerConfigurationFileReader::TrackerConfigurationDataEvent,
         igstk::TrackerConfiguration)

  igstkObserverMacro( TransformData, 
                      igstk::TransformFileReader::TransformDataEvent, 
                      igstk::PrecomputedTransformData::Pointer )

  igstkObserverMacro( CoordinateSystemTransform, 
                      igstk::CoordinateSystemTransformToEvent, 
                      igstk::CoordinateSystemTransformToResult )

  igstkObserverMacro( TransformationDescription, 
                      igstk::StringEvent, 
                      std::string )

  igstkObserverMacro( TransformationDate, 
                      igstk::PrecomputedTransformData::TransformDateTypeEvent, 
                      std::string )

  typedef igstk::TrackerConfigurationFileReader::
  ReadTrackerConfigurationFailSuccessObserver
                                ReadTrackerConfigurationFailSuccessObserverType;

  igstkObserverMacro( DICOMImageReaderInvalidDirectoryNameError, 
                      igstk::DICOMImageDirectoryIsNotDirectoryErrorEvent, 
                      std::string )

  igstkObserverMacro( DICOMImageReaderNonExistingDirectoryError, 
                      igstk::DICOMImageDirectoryDoesNotExistErrorEvent, 
                      igstk::EventHelperType::StringType )

  igstkObserverMacro( DICOMImageReaderEmptyDirectoryError, 
                      igstk::DICOMImageDirectoryEmptyErrorEvent, 
                      igstk::EventHelperType::StringType )

  igstkObserverMacro( DICOMImageDirectoryNameDoesNotHaveEnoughFilesError, 
                     igstk::DICOMImageDirectoryDoesNotHaveEnoughFilesErrorEvent,
                     igstk::EventHelperType::StringType )

  igstkObserverMacro( DICOMImageDirectoryDoesNotContainValidDICOMSeriesError, 
                      igstk::DICOMImageSeriesFileNamesGeneratingErrorEvent, 
                      igstk::EventHelperType::StringType )

  igstkObserverMacro( DICOMImageInvalidError, 
                      igstk::DICOMImageReadingErrorEvent, 
                      igstk::EventHelperType::StringType )

  /** Define observers for event communication */
 // igstkObserverObjectMacro( Image, igstk::MRImageReader::ImageModifiedEvent,
 //                                  igstk::MRImageSpatialObject );
  igstkObserverObjectMacro( Image, igstk::CTImageReader::ImageModifiedEvent,
                                   igstk::CTImageSpatialObject );

  igstkObserverObjectMacro( MeshObject, igstk::MeshReader::MeshModifiedEvent,
                                        igstk::MeshObject );
  
  igstkObserverObjectMacro( MRImage, igstk::my_MRImageReader::ImageModifiedEvent,   //qinshuo add
										igstk::MRImageSpatialObject );

  igstkObserverObjectMacro( DicomImage, igstk::com_DicomImageReader::ImageModifiedEvent,                //qinshuo add
										igstk::CTImageSpatialObject );    // make the comm_dicom image to CT Image

  igstkObserverMacro( Registration, igstk::CoordinateSystemTransformToEvent,
                                    igstk::CoordinateSystemTransformToResult );

  igstkObserverMacro( RegistrationError, igstk::DoubleTypeEvent, double );

  igstkObserverMacro( ImageExtent, igstk::ImageExtentEvent, 
                      igstk::EventHelperType::ImageExtentType );

  igstkObserverMacro( VTKImage, igstk::VTKImageModifiedEvent,
                      igstk::EventHelperType::VTKImagePointerType);   //vtkImage

  igstkEventMacro( InvalidRequestErrorEvent, igstk::IGSTKErrorEvent );

  Navigator();
  virtual ~Navigator();

private:

  Navigator(const Self&); // purposely not implemented
  void operator=(const Self&); // purposely not implemented
  
   /** States for the State Machine */
  
  igstkDeclareStateMacro( Initial );
  igstkDeclareStateMacro( LoadingImage );
  igstkDeclareStateMacro( LoadingSecondImage );// qinshuo add
  igstkDeclareStateMacro( ConfirmingImagePatientName );
  igstkDeclareStateMacro( ImageReady );
  igstkDeclareStateMacro( LoadingToolSpatialObject );
  igstkDeclareStateMacro( LoadingMesh );
  igstkDeclareStateMacro( LoadingOverlay );   //New adds
  igstkDeclareStateMacro( SettingImageFiducials );
  igstkDeclareStateMacro( SettingTrackerFiducials );
  igstkDeclareStateMacro( EndingSetTrackerFiducials );
  igstkDeclareStateMacro( TrackerFiducialsReady );
  igstkDeclareStateMacro( ConfiguringTracker );
  igstkDeclareStateMacro( TrackerConfigurationReady );
  igstkDeclareStateMacro( InitializingTracker );
  igstkDeclareStateMacro( TrackerInitializationReady );
  igstkDeclareStateMacro( RegisteringTracker );
  igstkDeclareStateMacro( AcceptingRegistration );
  igstkDeclareStateMacro( RegistrationReady );
  igstkDeclareStateMacro( StartingTracker );
  igstkDeclareStateMacro( StoppingTracker );
  igstkDeclareStateMacro( DisconnectingTracker );
  igstkDeclareStateMacro( Tracking );
  
   /** Inputs to the state machine and it's designed transitions */

  igstkDeclareInputMacro( Success );
  igstkDeclareInputMacro( Failure );
  igstkDeclareInputMacro( LoadImage );
  igstkDeclareInputMacro( LoadSecondImage ); //qinshuo add
  igstkDeclareInputMacro( ConfirmImagePatientName );
  igstkDeclareInputMacro( LoadMesh );
  igstkDeclareInputMacro( LoadOverlay );//New adds
  igstkDeclareInputMacro( LoadToolSpatialObject );
  igstkDeclareInputMacro( ConfigureTracker );
  igstkDeclareInputMacro( StartSetImageFiducials );
  igstkDeclareInputMacro( SetPickingPosition );
  igstkDeclareInputMacro( EndSetImageFiducials );
  igstkDeclareInputMacro( InitializeTracker );
  igstkDeclareInputMacro( StartSetTrackerFiducials );
  igstkDeclareInputMacro( AcceptTrackerFiducial );
  igstkDeclareInputMacro( EndSetTrackerFiducials );
  igstkDeclareInputMacro( RegisterTracker );
  igstkDeclareInputMacro( StartTracking );
  igstkDeclareInputMacro( StopTracking );
  igstkDeclareInputMacro( DisconnectTracker );



  /** DICOM image reader */
  ImageReaderType::Pointer                              m_ImageReader;
  DicomReaderType::Pointer                              m_ImageReader_overlay;  //modification: ImageReadType  ImageReaderType-->DicomReaderType


  /** Nifti Image reader -- qinshuo add*/
  MRImageReaderType::Pointer                             m_ImageReader2;
  bool													State_Observer2;
  MRImageObserver::Pointer								m_ImageObserver2;
  
  /** DICOM image observers */
  ImageObserver::Pointer                                m_ImageObserver;
  bool													State_Observer;
  ImageObserver::Pointer                                m_ImageObserver_overlay;
  VTKImageObserver::Pointer                             m_VTKImageObserver;   //Sun adds
  vtkImageData                                          *m_ImageData;          //Sun adds

  LandmarkPointContainerType                            m_CandidateLandmarks;

  std::string                                           m_ImageDir;
  std::string                                           m_PlanFilename;
  std::string                                           m_PlanFilenamePath;
  
  igstk::FiducialsPlan                                * m_Plan;

  CT_ImageSpatialObjectType::PointType                     m_ImageCenter;

  bool													m_flagImage;  //main image flag bool
  bool													m_flagOverlay; //overlay image flag
  bool													m_flagSecondImage; // qinshuo add: check status of second image
  bool													m_flagMesh;

  double                                                m_WindowLevel;
  double                                                m_WindowWidth;

  double                                              m_TrackerRMS;  
  bool                                                m_AxialViewInitialized;
  bool                                                m_SagittalViewInitialized;
  bool                                                m_CoronalViewInitialized;

  bool                                            m_ImagePlanesIn3DViewEnabled;
  bool                                            m_ModifyImageFiducialsEnabled;

  /* Command used for progress tracking */
  itk::SmartPointer<ProgressCommandType>                m_ProgressCommand;  
  
  /** image spatial object */
  CT_ImageSpatialObjectType::Pointer                       m_ImageSpatialObject;
  Dicom_ImageSpatialObjectType::Pointer                       m_OverlaySpatialObject;   //qinshuo add: modification: CT_ImageSpatialObjectType --> DicomImageSpatialObjectType
  MRI_ImageSpatialObjectType::Pointer					   m_ImageSpatialObject2;   //qinshuo add: second image object


  /** image spatial object */
  MeshObjectType::Pointer                       meshObject;  //Sun pick

  /** tool projection spatial object */
  ToolProjectionType::Pointer                           m_ToolProjection;

  /** cross hair spatial object */
  CrossHairType::Pointer                                m_CrossHair;

  /** tool projection representations */
  ToolProjectionRepresentationType::Pointer             
                                            m_AxialToolProjectionRepresentation;
  ToolProjectionRepresentationType::Pointer  
                                         m_SagittalToolProjectionRepresentation;
  ToolProjectionRepresentationType::Pointer             
                                          m_CoronalToolProjectionRepresentation;

  /** cross hair representation */
  CrossHairRepresentationType::Pointer        m_AxialCrossHairRepresentation;
  CrossHairRepresentationType::Pointer        m_SagittalCrossHairRepresentation;
  CrossHairRepresentationType::Pointer        m_CoronalCrossHairRepresentation;
  CrossHairRepresentationType::Pointer        m_3DViewCrossHairRepresentation;

  CrossHairRepresentationType::Pointer        new_Axial_Hair_Representation;//qinshuo add
  CrossHairRepresentationType::Pointer        new_Sagital_Hair_Representation;//qinshuo add

  /** a vector of mesh spatial objects */
  std::vector< MeshType::Pointer >                      m_MeshVector;

  /** tool spatial object and representation */
  MeshType::Pointer                                     m_ToolSpatialObject;
  MeshRepresentationType::Pointer                       m_ToolRepresentation;
   // CylinderType::Pointer                                     m_ToolSpatialObject;
   // CylinderRepresentationType::Pointer                       m_ToolRepresentation;

  ReslicerPlaneType::Pointer                     m_AxialPlaneSpatialObject;
  ReslicerPlaneType::Pointer                     m_SagittalPlaneSpatialObject;
  ReslicerPlaneType::Pointer                     m_CoronalPlaneSpatialObject;

  ReslicerPlaneType::Pointer                     m_AxialPlaneSpatialObject2;     //qinshuo add
  ReslicerPlaneType::Pointer                     m_SagittalPlaneSpatialObject2;  //qinshuo add
  ImageRepresentationType2::Pointer               new_AxialPlaneRepresentation;     //qinshuo add
  ImageRepresentationType2::Pointer               new_SagittalPlaneRepresentation;  //qinshuo add
  
  ImageRepresentationType::Pointer               m_AxialPlaneRepresentation;
  ImageRepresentationType::Pointer               m_SagittalPlaneRepresentation;
  ImageRepresentationType::Pointer               m_CoronalPlaneRepresentation;

  ImageRepresentationType::Pointer               m_AxialPlaneRepresentation2;
  ImageRepresentationType::Pointer               m_SagittalPlaneRepresentation2;
  ImageRepresentationType::Pointer               m_CoronalPlaneRepresentation2;

  ImageRepresentationTypePlus::Pointer           m_AxialOverlayPlaneRepresentation;
  ImageRepresentationTypePlus::Pointer           m_SagittalOverlayPlaneRepresentation;
  ImageRepresentationTypePlus::Pointer           m_CoronalOverlayPlaneRepresentation;

  VolumeRepresentationType::Pointer              m_ImageRepresentation3D;    //Sun adds
  SurfaceRepresentationType::Pointer             m_ImageSurface3D;

  
  std::vector< MeshRepresentationType::Pointer >     m_MeshRepresentationVector;

  std::vector< MeshResliceRepresentationType::Pointer > 
                                         m_AxialMeshResliceRepresentationVector;
  std::vector< MeshResliceRepresentationType::Pointer > 
                                      m_SagittalMeshResliceRepresentationVector;
  std::vector< MeshResliceRepresentationType::Pointer > 
                                       m_CoronalMeshResliceRepresentationVector;
  

  /** Landmark registration points containers */
  LandmarkPointContainerType                       m_LandmarksContainer;
  AcceptedLandmarkPointContainerType               m_AcceptedLandmarksContainer;

  typedef igstk::EllipsoidObject                  EllipsoidType;
  typedef igstk::EllipsoidObjectRepresentation    EllipsoidRepresentationType;
  EllipsoidType::Pointer                          m_FiducialPoint;
  std::vector<EllipsoidRepresentationType::Pointer>    m_FiducialRepresentation;  //Sun pick

  /** Class that observes the events generated by the tracker controller. */
  class TrackerControllerObserver : public itk::Command
  {
  public:
    typedef TrackerControllerObserver            Self;
    typedef ::itk::Command                       Superclass;
    typedef ::itk::SmartPointer<Self>            Pointer;
    typedef ::itk::SmartPointer<const Self>      ConstPointer;

    igstkNewMacro(Self)
    igstkTypeMacro(TrackerControllerObserver, itk::Command)

    void SetParent(Navigator *p); 

      /** Invoke this method when an initialization event occurs. */
    void Execute(itk::Object *caller, const itk::EventObject & event); 
    void Execute(const itk::Object *caller, const itk::EventObject & event);
  
    /**
     * Clear the current error.
     */
    void ClearError() 
    {
      this->m_ErrorOccured = false; 
      this->m_ErrorMessage.clear();
    }
    /**
     * If an error occurs in one of the observed IGSTK components this method 
     * will return true.
     */
    bool Error() 
    {
      return this->m_ErrorOccured;
    }

    void GetErrorMessage(std::string &errorMessage) 
    {
      errorMessage = this->m_ErrorMessage;
    }


  protected:
    TrackerControllerObserver() : m_Parent(NULL), m_ErrorOccured(false) {}
    virtual ~TrackerControllerObserver(){}


  private:
    Navigator *m_Parent;
    bool m_ErrorOccured;
    std::string m_ErrorMessage;

    //purposely not implemented
    TrackerControllerObserver(const Self&);
    void operator=(const Self&); 
  };

  igstk::TrackerController::Pointer                 m_TrackerController;
  TrackerControllerObserver::Pointer                m_TrackerControllerObserver;

  /** tracker configuration object */
  const igstk::TrackerConfiguration *                   m_TrackerConfiguration;

  /** typedef for the vector of tracker tools */
  typedef std::vector < igstk::TrackerTool::Pointer >    ToolVectorType;

  igstk::TrackerTool::Pointer                           m_TrackerTool;
  igstk::TrackerTool::Pointer                           m_ReferenceTool;
  
  /** Action methods to be invoked only by the state machine */
  void NoProcessing();
  void ReportInvalidRequestProcessing();
  void ReportSuccessImageLoadedProcessing();
  void ReportFailuredImageLoadedProcessing();
  void ReportSuccessSecondImageLoadedProcessing();   //qinshuo adds
  void ReportFailuredSecondImageLoadedProcessing();   //qinshuo add
  void ReportSuccessToolSpatialObjectLoadedProcessing();
  void ReportFailuredToolSpatialObjectLoadedProcessing();
  void ReportSuccessMeshLoadedProcessing();
  void ReportFailuredMeshLoadedProcessing();
  void ReportSuccessOverlayLoadedProcessing();   //New adds
  void ReportFailuredOverlayLoadedProcessing();   //New adds
  void ReportSuccessStartSetImageFiducialsProcessing();
  void ReportSuccessEndSetImageFiducialsProcessing();
  void ReportSuccessStartSetTrackerFiducialsProcessing();
  void ReportSuccessEndSetTrackerFiducialsProcessing();
  void ReportFailureEndSetTrackerFiducialsProcessing();
  void ReportSuccessPinpointingTrackerFiducialProcessing();
  void ReportFailurePinpointingTrackerFiducialProcessing();
  void ReportSuccessTrackerConfigurationProcessing();
  void ReportFailureTrackerConfigurationProcessing();
  void ReportSuccessTrackerInitializationProcessing();
  void ReportFailureTrackerInitializationProcessing();  
  void ReportSuccessTrackerRegistrationProcessing();
  void ReportFailureTrackerRegistrationProcessing();
  void ReportSuccessAcceptingRegistrationProcessing();
  void ReportFailureAcceptingRegistrationProcessing();
  void ReportSuccessTrackerDisconnectionProcessing();
  void ReportFailureTrackerDisconnectionProcessing();  
  void ReportSuccessStopTrackingProcessing();
  void ReportFailureStopTrackingProcessing();
  void ReportSuccessStartTrackingProcessing();
  void ReportFailureStartTrackingProcessing();
  void LoadImageProcessing();
  void LoadSecondImageProcessing();         //qinshuo add
  void ConfirmPatientNameProcessing();  
  void LoadMeshProcessing();
  void LoadOverlayProcessing();             //New adds
  void LoadToolSpatialObjectProcessing();
  void SetImageFiducialProcessing();
  void SetImagePickingProcessing();
  void StartSetTrackerFiducialsProcessing();
  void AcceptTrackerFiducialProcessing();
  void EndSetTrackerFiducialsProcessing();  
  void ConfigureTrackerProcessing();
  void InitializeTrackerProcessing();
  void TrackerRegistrationProcessing();
  void StartTrackingProcessing();
  void StopTrackingProcessing();
  void DisconnectTrackerProcessing();

  /** Observer type for loaded event, 
   *  the callback can be set to a member function. */
  typedef itk::ReceptorMemberCommand < Self > LoadedObserverType;
  typedef itk::SimpleMemberCommand < Self > CancelObserverType;
  
  LoadedObserverType::Pointer               m_AxialViewPickerObserver;
  LoadedObserverType::Pointer               m_SagittalViewPickerObserver;
  LoadedObserverType::Pointer               m_CoronalViewPickerObserver;
  LoadedObserverType::Pointer               m_3DViewPickerObserver;//Sun pick
  LoadedObserverType::Pointer               m_View3DPickerObserver;//Sun pick
  LoadedObserverType::Pointer               m_View3D2PickerObserver;//Sun pick

  LoadedObserverType::Pointer               m_ManualReslicingObserver;
  LoadedObserverType::Pointer               m_KeyPressedObserver;
  LoadedObserverType::Pointer               m_MousePressedObserver;
  LoadedObserverType::Pointer               m_TrackerToolUpdateObserver;
  LoadedObserverType::Pointer               m_TrackerToolNotAvailableObserver;
  LoadedObserverType::Pointer               m_TrackerToolAvailableObserver;
  LoadedObserverType::Pointer               m_ReferenceNotAvailableObserver;
  LoadedObserverType::Pointer               m_ReferenceAvailableObserver;

  /** Ellipsoid spatial object, used to represent the fiducial points*/
  typedef igstk::EllipsoidObject                  EllipsoidType;
  typedef igstk::EllipsoidObjectRepresentation    EllipsoidRepresentationType;  

  /** Objects for path planning and fiducial selection */
  std::vector< EllipsoidType::Pointer >                m_FiducialPointVector;
  std::vector< EllipsoidRepresentationType::Pointer >  
                                            m_AxialFiducialRepresentationVector;
  std::vector< EllipsoidRepresentationType::Pointer >  
                                         m_SagittalFiducialRepresentationVector;
  std::vector< EllipsoidRepresentationType::Pointer >  
                                          m_CoronalFiducialRepresentationVector;
  std::vector< EllipsoidRepresentationType::Pointer >  
                                           m_3DViewFiducialRepresentationVector;

  /** Define a initial world coordinate system */
  typedef igstk::AxesObject                       AxesObjectType;
  AxesObjectType::Pointer                         m_WorldReference;

  igstk::Transform                                m_ImageToTrackerTransform;
  igstk::Transform                                m_ICPTransform;
  igstk::Transform                                m_RegistrationTransform;   //registration transform  Sun emphasizes
  igstk::Transform                                m_PickingTransform;
  igstk::Transform                                m_MR2MRTransform;

  /** Log file */
  std::ofstream                                   m_LogFile; 
  std::ifstream                                   m_MappingFile;

  /** Utility functions, conversion between points and transform */
  inline 
  igstk::Transform 
  PointToTransform( PointType point)
  {
    igstk::Transform transform;
    igstk::Transform::VectorType translation;
    for (unsigned int i=0; i<3; i++)
      {
      translation[i] = point[i];
      }
    transform.SetTranslation( translation, 0.1, 
                                igstk::TimeStamp::GetLongestPossibleTime() );
    return transform;
  }

  inline 
  CT_ImageSpatialObjectType::PointType 
  TransformToPoint( igstk::Transform transform)
  {
    CT_ImageSpatialObjectType::PointType point;
    for (unsigned int i=0; i<3; i++)
      {
      point[i] = transform.GetTranslation()[i];
      }
    return point;
  }

  /** Internal functions */
  void ReadFiducials();
  void WriteFiducials(); 
  void ReadLabelMapping();
  void ConnectImageRepresentation();
  void UpdateFiducialPoint();
  void RequestToggleOrthogonalPlanes();
  void ResliceImage( IndexType index );
  void HandleKeyPressed ( igstk::NavigatorQuadrantViews::KeyboardCommandType 
                                                                   keyCommand );
  void HandleMousePressed ( igstk::NavigatorQuadrantViews::MouseCommandType 
                                                                 mouseCommand );
  void EnableOrthogonalPlanes();
  void DisableOrthogonalPlanes();

  /** Callback functions for picking and reslicing image events. */  
  void AxialViewPickingCallback( const itk::EventObject & event );
  void SagittalViewPickingCallback( const itk::EventObject & event );
  void CoronalViewPickingCallback( const itk::EventObject & event );
  void _3DViewPickingCallback( const itk::EventObject & event );  //Sun pick
  void Picking( const itk::EventObject & event ); //Sun adds
  void ResliceImageCallback( const itk::EventObject & event );
  void HandleKeyPressedCallback( const itk::EventObject & event );
  void HandleMousePressedCallback( const itk::EventObject & event ); 
  void ToolNotAvailableCallback( const itk::EventObject & event ); 
  void ToolAvailableCallback( const itk::EventObject & event ); 
  void ReferenceNotAvailableCallback( const itk::EventObject & event ); 
  void ReferenceAvailableCallback( const itk::EventObject & event ); 
  void OffOrthogonalResliceCallback( const itk::EventObject & event ); 
  void OrthogonalResliceCallback( const itk::EventObject & event ); 
  void NullActionCallback(const itk::EventObject & itkNotUsed(event) ){};
  void OnITKProgressEvent(itk::Object *source, const itk::EventObject &);


  //Mesh(Implant) Representation
  MeshRepresentationType::Pointer meshRepresentation;
  MeshResliceRepresentationType::Pointer axialContour;
  MeshResliceRepresentationType::Pointer sagittalContour;
  MeshResliceRepresentationType::Pointer coronalContour;

};



#endif
