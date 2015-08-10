/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkDICOMImageReader.txx
  Language:  C++
  Date:      2015/8/6
  Version:   1.0
  Description: This module is for Analyze image reader, modified by Qin Shuo


  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __igstkNiftiImageReader_txx
#define __igstkNiftiImageReader_txx

#include "igstkNiftiImageReader.h"

#include "itksys/SystemTools.hxx"
#include "itksys/Directory.hxx"

namespace igstk
{ 

/** Constructor */
template <class TPixelType>
NiftiImageReader<TPixelType>::NiftiImageReader() : m_StateMachine(this)
{ 
  m_Logger = NULL;
 
  //Set the state descriptors
  igstkAddStateMacro( Idle );
  igstkAddStateMacro( ImageFileNameRead ); 
  igstkAddStateMacro( ImageRead ); 
  igstkAddStateMacro( AttemptingToReadImage ); 

  /** List of  Inputs */
  igstkAddInputMacro( ReadImage );
  igstkAddInputMacro( ResetReader );
  igstkAddInputMacro( GetImage );
  igstkAddInputMacro( ImageReadingError );
  igstkAddInputMacro( ImageReadingSuccess );
  igstkAddInputMacro( ImageFileNameValid );
  igstkAddInputMacro( ImageFileNameDoesNotExist );


  /**--  Transitions for image reading -- **/

  // Transitions for image reading
  igstkAddTransitionMacro( AttemptingToReadImage,
                           ImageReadingSuccess,
                           ImageRead,
                           ReportImageReadingSuccess );

  //Transition for invalid inputs to Idle state
  igstkAddTransitionMacro( Idle,
                           ReadImage,
                           Idle,
                           ReportInvalidRequest );

  igstkAddTransitionMacro(Idle,
							ImageFileNameValid,
							ImageFileNameRead,
							ReadFileName);

  igstkAddTransitionMacro( Idle,
                           ImageReadingError,
                           Idle,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( Idle,
                           ImageReadingSuccess,
                           Idle,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( Idle,
                           GetImage,
                           Idle,
                           ReportInvalidRequest );

  // Transitions for Invalid inputs to  ImageFileNameRead state 
  igstkAddTransitionMacro( ImageFileNameRead,
                           ImageReadingSuccess,
                           ImageFileNameRead,
                           ReportInvalidRequest );


  igstkAddTransitionMacro( ImageFileNameRead,
                           ImageReadingError,
                           ImageFileNameRead,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( ImageFileNameRead,
                           GetImage,
                           ImageFileNameRead,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( ImageFileNameRead,
                           ImageFileNameValid,
                           ImageFileNameRead,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( ImageFileNameRead,
						  ReadImage,
						  AttemptingToReadImage,
						  AttemptReadImage );

  // Transitions for invalid inputs to ImageRead state
  igstkAddTransitionMacro( ImageRead,
                           ReadImage,
                           ImageRead,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( ImageRead,
                           ImageReadingSuccess,
                           ImageRead,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( ImageRead,
                           ImageReadingError,
                           ImageRead,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( ImageRead,
                           ImageFileNameValid,
                           ImageRead,
                           ReportInvalidRequest );


  igstkAddTransitionMacro( ImageRead,
                           ImageFileNameDoesNotExist,
                           ImageRead,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( ImageRead,
							  GetImage,
							  ImageRead,
							  ReportImage );


  // Transitions for invalid inputs to AttemptingToReadImage state
  igstkAddTransitionMacro( AttemptingToReadImage,
                           ReadImage,
                           AttemptingToReadImage,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( AttemptingToReadImage,
                           ImageFileNameValid,
                           AttemptingToReadImage,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( AttemptingToReadImage,
                           ImageFileNameDoesNotExist,
                           AttemptingToReadImage,
                           ReportInvalidRequest );

  igstkAddTransitionMacro( AttemptingToReadImage,
                           GetImage,
                           AttemptingToReadImage,
                           ReportInvalidRequest );


  //Transitions for reset reader input 

  igstkAddTransitionMacro( Idle,
                           ResetReader,
                           Idle,
                           ResetReader );

  igstkAddTransitionMacro( ImageRead,
                           ResetReader,
                           Idle,
                           ResetReader );

  igstkAddTransitionMacro( ImageFileNameRead,
                           ResetReader,
                           Idle,
                           ResetReader );

  igstkAddTransitionMacro( AttemptingToReadImage,
                           ResetReader,
                           Idle,
                           ResetReader );


  //Errors related to  image reading 
  igstkAddTransitionMacro( AttemptingToReadImage,
                           ImageReadingError,
                           Idle,
                           ReportImageReadingError );


  igstkAddTransitionMacro( ImageRead,
                           GetImage,
                           ImageRead,
                           ReportImage );


  // Select the initial state of the state machine
  igstkSetInitialStateMacro( Idle );

  // Finish the programming and get ready to run
  m_StateMachine.SetReadyToRun();

  // Initialize the booleas for the preconditions of the unsafe Get macros 
  m_FileSuccessfullyRead = false;

  // Create the Nifti file reader
  m_ImageIO = itk::NiftiImageIO::New();
  m_ImageIO->SetGlobalWarningDisplay(this->GetGlobalWarningDisplay());
  m_ImageFileReader = ImageReaderType::New();
  m_ImageFileReader->SetImageIO( m_ImageIO );

} 

/** Destructor */
template <class TPixelType>
NiftiImageReader<TPixelType>::~NiftiImageReader()  
{

}

template <class TImageSpatialObject>
void NiftiImageReader<TImageSpatialObject>
::RequestSetFileName( const FileNameType & filename )
{
  igstkLogMacro( DEBUG, 
                 "igstk::DICOMImageReader::RequestSetDirectory called...\n");

  // Reset the reader 
  this->m_StateMachine.PushInput( this->m_ResetReaderInput );
  this->m_StateMachine.ProcessInputs();

  m_FileNameToBeSet = filename;
 
  if( filename.empty() )
    {
    this->m_StateMachine.PushInput( this->m_ImageFileNameDoesNotExistInput );
    this->m_StateMachine.ProcessInputs();
    return;
    }

  if( !itksys::SystemTools::FileExists( filename.c_str() ) )
    {
    this->m_StateMachine.PushInput( 
                               this->m_ImageFileNameDoesNotExistInput );
    this->m_StateMachine.ProcessInputs();
    return;
    }

  // input file name valid
  this->m_StateMachine.PushInput( this->m_ImageFileNameValidInput );
  this->m_StateMachine.ProcessInputs();

}

/** This function reports the image */
template <class TImageSpatialObject>
void 
NiftiImageReader<TImageSpatialObject>
::ReportImageProcessing()
{
  ImageModifiedEvent  event;
  event.Set( this->m_ImageSpatialObject );
  this->InvokeEvent( event );
}

template <class TPixelType>
void NiftiImageReader<TPixelType>::RequestGetImage()
{
  igstkLogMacro( DEBUG, 
                 "igstk::NiftiImageReader::RequestGetImage called...\n" );
  this->m_StateMachine.PushInput( this->m_GetImageInput);
  this->m_StateMachine.ProcessInputs();
}

template <class TImageSpatialObject>
void 
NiftiImageReader<TImageSpatialObject>
::SetFileNameProcessing()
{
  igstkLogMacro( DEBUG, 
                "igstk::DICOMImageReader::SetDirectoryName called...\n" );
  m_FileName = m_FileNameToBeSet;
  this->ReadFileNameProcessing();//-----------------------------------------
}


template <class TPixelType>
void NiftiImageReader<TPixelType>::RequestReadImage()
{
  igstkLogMacro( DEBUG, 
                 "igstk::NiftiImageReader::RequestReadImage called...\n" );
  this->m_StateMachine.PushInput( this->m_ReadImageInput);
  this->m_StateMachine.ProcessInputs();
}

/** Read in the DICOM series image */
template <class TPixelType>
void NiftiImageReader<TPixelType>::ReadFileNameProcessing()
{
  m_FileName = m_FileNameToBeSet;
	
	igstkLogMacro( DEBUG, 
              "igstk::NiftiImageReader::Read Image File Name called...\n" );
  
  igstkLogMacro( DEBUG, "igstk::NiftiImageReader will open image: " 
                                          << m_FileName << "\n");

  m_ImageFileReader->SetFileName(m_FileName);
  this->m_StateMachine.PushInput( 
                   this->m_ImageFileNameValidInput );
  this->m_StateMachine.ProcessInputs();
}


/** Read in the Nifti image */
template <class TPixelType>
void NiftiImageReader<TPixelType>::AttemptReadImageProcessing()
{
  igstkLogMacro( DEBUG, 
                 "igstk::NiftiImageReader::AttemptReadImage called...\n" );
  try
    {
    m_ImageFileReader->UpdateLargestPossibleRegion();
    m_ImageFileReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    igstkLogMacro( DEBUG, 
    "igstk:NiftiImageReader - Failed to update image reader.\n" );
    this->m_ImageReadingErrorInformation = excp.GetDescription();
    igstkLogMacro( DEBUG,"ITK Exception:"+ m_ImageReadingErrorInformation );
    this->m_StateMachine.PushInput( this->m_ImageReadingErrorInput );
    this->m_StateMachine.ProcessInputs();
    return;
    }

  this->m_StateMachine.PushInput( this->m_ImageReadingSuccessInput );
  this->m_StateMachine.ProcessInputs();

  this->Superclass::ConnectImage();
  
  m_FileSuccessfullyRead = true;
}

/* This function reports invalid requests */
template <class TPixelType>
void
NiftiImageReader<TPixelType>::ReportInvalidRequestProcessing()
{
  igstkLogMacro( DEBUG, 
      "igstk::DICOMImageReader::ReportInvalidRequestProcessing called...\n" );

  this->InvokeEvent( NiftiInvalidRequestErrorEvent() );
}

/* This function resets the reader */
template <class TPixelType>
void
NiftiImageReader<TPixelType>::ResetReaderProcessing()
{
  igstkLogMacro( DEBUG, "igstk::NiftiImageReader::ResetReader called...\n" );

  m_FileSuccessfullyRead = false;
}



template <class TPixelType>
void
NiftiImageReader<TPixelType>::ReportImageReadingErrorProcessing()
{
  igstkLogMacro( DEBUG, 
            "igstk::DICOMImageReader::ReportImageReadingError: called...\n");

  NiftiImageReadingErrorEvent event;
  event.Set ( this->m_ImageReadingErrorInformation );
  this->InvokeEvent( event );
}

template <class TPixelType>
void
NiftiImageReader<TPixelType>::ReportImageReadingSuccessProcessing()
{
  igstkLogMacro( DEBUG, 
  "igstk::NiftiImageReader::ReportImageReadingSuccessProcessing: called...\n");
}


/** Get the image. This function MUST be private in order to  prevent unsafe
 * access to the ITK image level. */
template <class TPixelType>
const typename NiftiImageReader< TPixelType >::ImageType *
NiftiImageReader<TPixelType>::GetITKImage() const
{
  return m_ImageFileReader->GetOutput();
}

/** Print Self function */
template <class TPixelType>
void NiftiImageReader<TPixelType>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}

} // end namespace igstk

#endif
