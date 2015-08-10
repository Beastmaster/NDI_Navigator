#ifndef __igstkImageSpatialObjectSurfaceRepresentation_txx
#define __igstkImageSpatialObjectSurfaceRepresentation_txx

#include "igstkImageSpatialObjectSurfaceRepresentation.h"
#include <vtkLookupTable.h>
#include <StockColorMap.h>

namespace igstk
{

/** Constructor */
template < class TImageSpatialObject >
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::ImageSpatialObjectSurfaceRepresentation():m_StateMachine(this)
{
  // We create the image spatial object
  m_ImageSpatialObject = NULL;

  this->RequestSetSpatialObject( m_ImageSpatialObject );

  // Create classes for displaying images
  m_BoneActor = vtkActor::New();
  this->AddActor( m_BoneActor );
  m_SkinActor = vtkActor::New();//F
  this->AddActor( m_SkinActor );//F
  m_ImageData  = NULL;

  // Create the observer to VTK image events 
  m_VTKImageObserver = VTKImageObserver::New();

  m_ShiftBy = 0;

  m_ShiftScale = vtkImageShiftScale::New();

  //Bone rendering 
 // m_BoneExtractor  = vtkContourFilter::New();  //F
  m_BoneExtractor  = vtkDiscreteMarchingCubes::New();
 // m_BoneNormals  = vtkPolyDataNormals::New();  //F
 // m_BoneStripper = vtkStripper::New();   //F
  m_BoneMapper = vtkPolyDataMapper::New();//F

  //Skin Rendering
  m_SkinExtractor  = vtkContourFilter::New();  //F
  m_SkinNormals  = vtkPolyDataNormals::New();  //F
  m_SkinStripper = vtkStripper::New();   //F
  m_SkinMapper = vtkPolyDataMapper::New();//F

  igstkAddInputMacro( ValidImageSpatialObject );
  igstkAddInputMacro( NullImageSpatialObject  );
  igstkAddInputMacro( EmptyImageSpatialObject  );
  igstkAddInputMacro( ConnectVTKPipeline );

  igstkAddStateMacro( NullImageSpatialObject );
  igstkAddStateMacro( ValidImageSpatialObject );
 
  igstkAddTransitionMacro( NullImageSpatialObject, NullImageSpatialObject,
                           NullImageSpatialObject,  No );
  igstkAddTransitionMacro( NullImageSpatialObject, EmptyImageSpatialObject,
                           NullImageSpatialObject,  No );
  igstkAddTransitionMacro( NullImageSpatialObject, ValidImageSpatialObject,
                           ValidImageSpatialObject,  SetImageSpatialObject );
  
  igstkAddTransitionMacro( ValidImageSpatialObject, NullImageSpatialObject,
                           NullImageSpatialObject,  No ); 
  igstkAddTransitionMacro( ValidImageSpatialObject, EmptyImageSpatialObject,
                           NullImageSpatialObject,  No ); 
  igstkAddTransitionMacro( ValidImageSpatialObject, ValidImageSpatialObject,
                           ValidImageSpatialObject,  No ); 
  
  igstkAddTransitionMacro( NullImageSpatialObject, ConnectVTKPipeline,
                           NullImageSpatialObject, No );
  igstkAddTransitionMacro( ValidImageSpatialObject, ConnectVTKPipeline,
                           ValidImageSpatialObject, ConnectVTKPipeline );

  igstkSetInitialStateMacro( NullImageSpatialObject );

  m_StateMachine.SetReadyToRun();
} 

/** Destructor */
template < class TImageSpatialObject >
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::~ImageSpatialObjectSurfaceRepresentation()  
{


  // This deletes also the Actors
  this->DeleteActors();

  if(m_BoneExtractor )
    {
    m_BoneExtractor->Delete();
    }
  if(m_BoneNormals) 
    {
    m_BoneNormals->Delete();
    }
  if(m_BoneStripper)
    {
	m_BoneStripper ->Delete();
	}
  if(m_BoneMapper)
    {
	m_BoneMapper->Delete();
	}
  if(m_SkinExtractor )
    {
    m_SkinExtractor->Delete();
    }
  if(m_SkinNormals) 
    {
    m_SkinNormals->Delete();
    }
  if(m_SkinStripper)
    {
	m_SkinStripper ->Delete();
	}
  if(m_SkinMapper)
    {
	m_SkinMapper->Delete();
	}
}

/** Overloaded DeleteActor function*/
template < class TImageSpatialObject >
void 
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::DeleteActors( )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::DeleteActors called...\n");
  
  this->Superclass::DeleteActors();
  
  m_BoneActor = NULL;
  
  m_SkinActor = NULL; 

}
 
/** Set the Image Spatial Object */
template < class TImageSpatialObject >
void 
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::RequestSetImageSpatialObject( const ImageSpatialObjectType * image )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::RequestSetImageSpatialObject called...\n");
  
  m_ImageSpatialObjectToAdd = image;

  if( !m_ImageSpatialObjectToAdd )
    {
    m_StateMachine.PushInput( m_NullImageSpatialObjectInput );
    }
  else 
    {
    if( m_ImageSpatialObjectToAdd->IsEmpty() )
      {
      m_StateMachine.PushInput( m_EmptyImageSpatialObjectInput );
      }
    else
      {
      m_StateMachine.PushInput( m_ValidImageSpatialObjectInput );
      }
    }
  
  m_StateMachine.ProcessInputs();
}


/** Null Operation for a State Machine Transition */
template < class TImageSpatialObject >
void 
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::NoProcessing()
{
}

/** Set the Image Spatial Object */
template < class TImageSpatialObject >
void 
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::SetImageSpatialObjectProcessing()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::SetImageSpatialObjectProcessing called...\n");

  // We create the image spatial object
  m_ImageSpatialObject = m_ImageSpatialObjectToAdd;

  m_ImageSpatialObject->AddObserver( VTKImageModifiedEvent(), 
                                     m_VTKImageObserver );

  this->RequestSetSpatialObject( m_ImageSpatialObject );
  
  // This method gets a VTK image data from the private method of the
  // ImageSpatialObject and stores it in the representation by invoking the
  // private SetImage method.
  
  this->m_ImageSpatialObject->RequestGetVTKImage();

  if( this->m_VTKImageObserver->GotVTKImage() ) 
    {
    m_ImageData = this->m_VTKImageObserver->GetVTKImage();
    if( m_ImageData )
    {
    m_ImageData->Update();
     }
    }

  vtkImageResample *resample = vtkImageResample::New();
  resample->SetInput( m_ImageData );
  resample->SetDimensionality(3); 
  resample->SetAxisMagnificationFactor(0,0.4); 
  resample->SetAxisMagnificationFactor(1,0.4); 
  resample->SetAxisMagnificationFactor(2,0.4);

  m_ShiftScale->SetInput( resample->GetOutput() );

  m_BoneExtractor->SetInput( m_ShiftScale->GetOutput() );
 // m_BoneNormals->SetInput( m_BoneExtractor->GetOutput() );  //F
 // m_BoneStripper->SetInput( m_BoneNormals->GetOutput() );   //F
//  m_BoneMapper->SetInput( m_BoneStripper->GetOutput() );//F
  m_BoneMapper->SetInput( m_BoneExtractor->GetOutput() );
  m_BoneActor->SetMapper(m_BoneMapper);//F
//  m_BoneMapper->ScalarVisibilityOn();//F

  m_SkinExtractor->SetInput( m_ShiftScale->GetOutput() );  //F
  m_SkinNormals->SetInput( m_SkinExtractor->GetOutput() );  //F
  m_SkinStripper->SetInput( m_SkinNormals->GetOutput() );   //F
  m_SkinMapper->SetInput( m_SkinStripper->GetOutput() );//F
  m_SkinActor->SetMapper(m_SkinMapper);//F
  m_SkinMapper->ScalarVisibilityOff();//F

}


/** Print Self function */
template < class TImageSpatialObject >
void
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}


/** Update the visual representation in response to changes in the geometric
 * object */
template < class TImageSpatialObject >
void
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::UpdateRepresentationProcessing()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                       ::UpdateRepresentationProcessing called...\n");
  if( m_ImageData )
    {
    m_ShiftScale->SetInput( m_ImageData );

    m_BoneExtractor->SetInput( m_ShiftScale->GetOutput() );  //F
  //  m_BoneNormals->SetInput( m_BoneExtractor->GetOutput() );  //F
   // m_BoneStripper->SetInput( m_BoneNormals->GetOutput() );   //F
   // m_BoneMapper->SetInput( m_BoneStripper->GetOutput() );//F
   m_BoneMapper->SetInput( m_BoneExtractor->GetOutput() );//F
	

	m_SkinExtractor->SetInput( m_ShiftScale->GetOutput() );  //F
    m_SkinNormals->SetInput( m_SkinExtractor->GetOutput() );  //F
    m_SkinStripper->SetInput( m_SkinNormals->GetOutput() );   //F
    m_SkinMapper->SetInput( m_SkinStripper->GetOutput() );//F

    }
}


/** Create the vtk Actors */
template < class TImageSpatialObject >
void
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::CreateActors()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::CreateActors called...\n");

  // to avoid duplicates we clean the previous actors
  this->DeleteActors();

  m_BoneActor = vtkActor::New();
  this->AddActor( m_BoneActor );

  m_SkinActor = vtkActor::New();//F
  this->AddActor(m_SkinActor);//F

  m_ShiftScale = vtkImageShiftScale::New();
  m_ShiftScale->SetInput( m_ImageData );
  m_ShiftScale->SetShift( m_ShiftBy );
  m_ShiftScale->SetOutputScalarTypeToShort ();

  //Bone Rendering  
 // m_BoneExtractor->SetValue(0,-1023);
  const int numContours = 2; 
  m_BoneExtractor->SetNumberOfContours(numContours); 
  m_BoneExtractor->GenerateValues(256, 1.0, 256.0);
  //m_BoneExtractor->Update();
 // m_BoneNormals->SetFeatureAngle(120.0);
 // m_BoneNormals->Update();
  //m_BoneStripper->Update();

   //Skin Rendering  
 // m_SkinExtractor->SetValue(0,-900);
//  m_SkinExtractor->Update();
 // m_SkinNormals->SetFeatureAngle(120.0);
//  m_SkinNormals->Update();

  igstkPushInputMacro( ConnectVTKPipeline );
  m_StateMachine.ProcessInputs(); 

}

/** Create a copy of the current object representation */
template < class TImageSpatialObject >
typename ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >::Pointer
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::Copy() const
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::Copy called...\n");

  Pointer newOR = 
         ImageSpatialObjectSurfaceRepresentation<TImageSpatialObject>::New();
  newOR->SetColor(this->GetRed(),this->GetGreen(),this->GetBlue()); 
  newOR->SetOpacity(this->GetOpacity());
  newOR->RequestSetImageSpatialObject(m_ImageSpatialObject);

  return newOR;
}

template < class TImageSpatialObject >
void
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::SetImage( const vtkImageData * image )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::SetImage called...\n");

  // This const_cast<> is needed here due to the lack of 
  // const-correctness in VTK 
  m_ImageData = const_cast< vtkImageData *>( image );
}


template < class TImageSpatialObject >
void
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::ConnectVTKPipelineProcessing() 
{
   m_LookupTable = vtkLookupTable::New();
   m_LookupTable->SetNumberOfColors(256); 
 for (int c = 1; c < 256; c++)
  {
      float red,green,blue;
      red = (rand() % 256)/255.0;
      green = (rand() % 256)/255.0;
      blue = (rand() % 256)/255.0;
      m_LookupTable->SetTableValue (c,red,green,blue,1.0);
  }
  // for(unsigned int i = 1; i < 256; i++){
//         m_LookupTable->SetTableValue(i,Jet[i][0],Jet[i][1],Jet[i][2],1.0); 
	//      m_LookupTable->SetTableValue(i,stock_nih[i][0],stock_nih[i][1],stock_nih[i][2],1.0);  
		//  m_LookupTable->SetTableValue(i,stock_ge_color[i][0],stock_ge_color[i][1],stock_ge_color[i][2],1.0);
	//} 
	m_LookupTable->SetTableRange(1.0, 256.0);
  m_LookupTable->SetHueRange(0.0, 0.667);
  m_LookupTable->Build();
  m_BoneMapper->SetLookupTable(m_LookupTable);
  m_BoneMapper->SetScalarRange(0.0,256.0);
//m_BoneMapper->SetScalarModeToUsePointData();
  m_BoneMapper->SetScalarModeToUseCellData();
//m_BoneMapper->ScalarVisibilityOn();
//m_BoneMapper->SetScalarRange(-1,1);
// m_BoneMapper->InterpolateScalarsBeforeMapping =1;
m_BoneActor->GetProperty()->SetColor(1,1,1);
m_BoneActor->SetMapper(m_BoneMapper);
//m_BoneActor->GetProperty()->SetOpacity(1.0);
//m_BoneActor->GetProperty()->SetDiffuseColor(1.000000, 0.937033, 0.954531);
//m_BoneActor->GetProperty()->SetAmbient(0.04);
//m_BoneActor->GetProperty()->SetDiffuse(0.9);
//m_BoneActor->GetProperty()->SetSpecular(0.5);
//m_BoneActor->GetProperty()->SetSpecularPower(60.0);

m_BoneActor->SetVisibility( 1 );
m_BoneActor->SetPickable( 0 );

//m_SkinActor->SetMapper(m_SkinMapper);//F
//m_SkinActor->GetProperty()->SetRepresentationToSurface();//F
//m_SkinActor->GetProperty()->SetOpacity(1.0);
//m_SkinActor->GetProperty()->SetDiffuseColor(1.0,0.49,0.25);
//m_SkinActor->SetVisibility( 1 );
//m_SkinActor->SetPickable( 0 );

}

template < class TImageSpatialObject >
void 
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::updateOpacityTransferFunction( double value )
{
	m_BoneActor->GetProperty()->SetOpacity(value);
}

template < class TImageSpatialObject >
void 
ImageSpatialObjectSurfaceRepresentation< TImageSpatialObject >
::updateSkinOpacityTransferFunction( double value )
{ 
	m_SkinActor->GetProperty()->SetOpacity(value);

}

} // end namespace igstk

#endif
