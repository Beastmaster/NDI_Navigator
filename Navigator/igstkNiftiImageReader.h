/*=========================================================================

  Program:   Surgical Navigation System
  Module:    igstkNiftiImageReader.h
  Language:  C++
  Date:      2015/8/6
  Version:   1.0
  Description: This module is for Analyze image reader, modified by Qin Shuo


  CopyRight    Research Center of Medical Imaging Center 
	(Chinese University of HongKong)   All Rights Reserved
	Please be noted that: this program is knowledge property of 
	RC-MIC(CUHK). You can contact the author or our office to 
	access more information. This program is for research purpose 
	and we take no responsibility for its consequences. 

=========================================================================*/
#ifndef __igstkNiftiImageReader_h
#define __igstkNiftiImageReader_h

#include "igstkImageReader.h"

#include "igstkEvents.h"

#include "itkImageSeriesReader.h"
#include "itkImageFileReader.h"
#include "itkEventObject.h"
//#include "itkGDCMImageIO.h"
//#include "itkGDCMSeriesFileNames.h"
#include "itkNiftiImageIO.h"
#include "igstkImageSpatialObject.h"
#include "itkOrientImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

namespace igstk
{

#define igstkUnsafeGetMacro(name,type) \
	virtual const type & Get##name () const \
	{ \
	igstkLogMacro( CRITICAL, \
	"igstk::DICOMImageReader::Get" #name " unsafe method called...\n"); \
	return this->m_##name; \
}

igstkEventMacro( NiftiModalityEvent,                             StringEvent);
igstkEventMacro( NiftiPatientNameEvent,                          StringEvent);
igstkEventMacro( NiftiImageReaderEvent,                          StringEvent);

// Invalid request error event 
igstkEventMacro( NiftiImageReaderErrorEvent, IGSTKErrorWithStringEvent );
igstkEventMacro( NiftiInvalidRequestErrorEvent, NiftiImageReaderErrorEvent );

//Image reading error
igstkEventMacro(NiftiImageReadingErrorEvent, NiftiImageReaderErrorEvent );
  

/** \class AnalyzeImageReader
 * 
 * \brief This class reads nifti files. 
 */
template <class TImageSpatialObject>
class NiftiImageReader : public ImageReader< TImageSpatialObject >
{

public:

  /** Macro with standard traits declarations. */
  igstkStandardTemplatedAbstractClassTraitsMacro( NiftiImageReader, \
                                           ImageReader< TImageSpatialObject> )

public:

  /** Type for representing the string of the directory 
   *  from which the DICOM files will be read. */
  typedef std::string    DirectoryNameType;
  typedef std::string    FileNameType;
  
  /** Method to pass the directory name containing the DICOM image data */
  void RequestSetFileName( const FileNameType & filename );

  void RequestSetProgressCallback(itk::Command *progressCallback)
    {
    m_ImageSeriesReader->AddObserver(itk::ProgressEvent(),progressCallback);
    }

  void RequestSetAbortCallback(itk::Command *abortCallback)
    {
    m_ImageSeriesReader->AddObserver(itk::AbortEvent(),abortCallback);
    }

  /** This method request image read */
  void RequestReadImage();

  /** This function should be used to request modality info */
  void RequestGetModalityInformation();

  /** This function will be used to request patient name info */
  void RequestGetPatientNameInformation(); 

  /** Type used for returning string values from the DICOM header */
  typedef std::string    NiftiInformationType;

  /** Precondition that should be invoked and verified before attempting to
   *  use the values of the methods GetPatientName(), GetPatientID() and
   *  GetModality().
   */
  bool FileSuccessfullyRead() const { return m_FileSuccessfullyRead; }
  
  /** Unsafe Get Macro for having access to the Patient Name.  This method is
   * considered unsafe because it is not subject to the control of the internal
   * state machine.  The method GetPatientName() should only be invoked if the
   * precondition method FileSuccessfullyRead() has already been called and it
   * has returned true. Calling GetPatientName() in any other situation will 
   * lead to unpredictable behavior. */
  igstkUnsafeGetMacro( PatientName, NiftiInformationType );

  /** Unsafe Get Macro for having access to the Patient unique Identifier.
   * This method is considered unsafe because it is not subject to the control
   * of the internal state machine.  The method GetPatientID() should only be
   * invoked if the precondition method FileSuccessfullyRead() has already been
   * called and it has returned true. Calling GetPatientID() in any other
   * situation will lead to unpredictable behavior. */
  igstkUnsafeGetMacro( PatientID, NiftiInformationType );

  /** Unsafe Get Macro for having access to the image Modality.  This method is
   * considered unsafe because it is not subject to the control of the internal
   * state machine.  The method GetModality() should only be invoked if the
   * precondition method FileSuccessfullyRead() has already been called and it
   * has returned true. Calling GetModality() in any other situation will lead
   * to unpredictable behavior. */
  igstkUnsafeGetMacro( Modality, NiftiInformationType );

  /** Request to get the output image as an event */
  void RequestGetImage();
  
  /** Event type */
  igstkLoadedTemplatedObjectEventMacro( ImageModifiedEvent, IGSTKEvent, 
                                        TImageSpatialObject);

  /** Declarations needed for the Logger */
  igstkLoggerMacro();

protected:

  NiftiImageReader( void );
  ~NiftiImageReader( void );

  /** Helper classes for the image series reader */
  FileNameType							m_FileNameToBeSet;
  FileNameType                          m_FileName;
  itk::NiftiImageIO::Pointer            m_ImageIO;


  typedef typename Superclass::ImageType         ImageType;
  typedef typename itk::Image< float, 3 >        ImageType_I;

  typedef itk::ImageSeriesReader< ImageType >    ImageSeriesReaderType;
  typedef itk::ImageFileReader< ImageType_I >      ImageReaderType;
  typedef itk::CastImageFilter< ImageType_I, ImageType >  ImgaeCasterType;

  /** Internal itkImageSeriesReader */
  typename ImageSeriesReaderType::Pointer        m_ImageSeriesReader;
  typename ImageReaderType::Pointer              m_ImageFileReader;
  typename itk::OrientImageFilter< ImageType,ImageType >::Pointer m_orientor;
  typename ImgaeCasterType::Pointer				 m_Caster;

  /** Print the object information in a stream. */
  void PrintSelf( std::ostream& os, itk::Indent indent ) const; 

  /** This method MUST be protected n order to prevent 
   *  unsafe access to the ITK image level */
  virtual const ImageType * GetITKImage() const;

private:

  /** These two methods must be declared and note be implemented
   *  in order to enforce the protocol of smart pointers. */
  NiftiImageReader(const Self&);    //purposely not implemented
  void operator=(const Self&);      //purposely not implemented


  /** List of States */
  igstkDeclareStateMacro( Idle );
  igstkDeclareStateMacro( ImageFileNameRead );
  igstkDeclareStateMacro( AttemptingToReadImage );
  igstkDeclareStateMacro( ImageRead );

  /** List of State Inputs */
  igstkDeclareInputMacro( ReadImage );
  igstkDeclareInputMacro( ImageFileNameValid ); 
  igstkDeclareInputMacro( ImageReadingSuccess );
  igstkDeclareInputMacro( ResetReader );
  igstkDeclareInputMacro( GetImage );
  
  /** Error related state inputs */
  igstkDeclareInputMacro( ImageReadingError );
  igstkDeclareInputMacro( ImageFileNameDoesNotExist );
  
  /** Set the name of the directory. To be invoked ONLY by the StateMachine */
  void SetFileNameProcessing();

  /** a useless function to pass filename.  To be invoked ONLY by the StateMachine**/
  void ReadFileNameProcessing();

  /** This method request image read. To be invoked ONLY by the StateMachine. */
  void AttemptReadImageProcessing();

  /** The "ReportInvalidRequest" method throws InvalidRequestErrorEvent
     when invalid requests are made */
  void ReportInvalidRequestProcessing();

  /** This function reports an error while image reading */
  void ReportImageReadingErrorProcessing();

  /** This function reports success in image reading */
  void ReportImageReadingSuccessProcessing();

  /** This function reports the image */
  void ReportImageProcessing();

  /** This function resets the reader */
  void ResetReaderProcessing();

  /** This function throws a string loaded event. The string is loaded
   *  with patient name */
  void GetPatientNameInformationProcessing();

  /** Flag that indicates whether the file has been read successfully */
  bool                    m_FileSuccessfullyRead;

  /** Internal variables for holding patient and image specific information. */
  NiftiInformationType    m_PatientID;
  NiftiInformationType    m_PatientName;
  NiftiInformationType    m_Modality;
  NiftiInformationType    m_GantryTilt;

  /** Variable to hold image reading error information */
  std::string             m_ImageReadingErrorInformation;
};

} // end namespace igstk

#ifndef IGSTK_MANUAL_INSTANTIATION
#include "igstkNiftiImageReader.hxx"
#endif

#endif // 
