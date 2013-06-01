
#include <vtkVersion.h>
#include <vtkSmartPointer.h>

#include <vtkAppendPolyData.h>

#include <vtkParametricFunctionSource.h>
#include <vtkParametricEllipsoid.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkPointData.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <string>

#include "SheppLoganPhantom.h"

int main (int, char *[])
{

  SheppLoganPhantom phantom;
  SheppLoganPhantom::PartContainerType parts = phantom.GetParts();

  vtkSmartPointer<vtkAppendPolyData> phantomData =
    vtkSmartPointer<vtkAppendPolyData>::New();

  for (size_t i = 0; i < parts.size(); ++i)
    {
    vtkSmartPointer<vtkParametricEllipsoid> part =
      vtkSmartPointer<vtkParametricEllipsoid>::New();
    part->SetXRadius(parts[i].m_Radius[0]);
    part->SetYRadius(parts[i].m_Radius[1]);
    part->SetZRadius(parts[i].m_Radius[2]);
    vtkSmartPointer<vtkParametricFunctionSource> partSource =
      vtkSmartPointer<vtkParametricFunctionSource>::New();
    partSource->SetParametricFunction(part);
    vtkSmartPointer<vtkTransform> transform =
      vtkSmartPointer<vtkTransform>::New();
    transform->Identity();
    transform->Translate(parts[i].m_Center[0],
                         parts[i].m_Center[1],
                         parts[i].m_Center[2]);
    transform->Concatenate(parts[i].m_Transform);
    if (transform->GetMatrix()->Determinant() < 0.0)
      {
      transform->Scale(-1.0, 1.0, 1.0);
      }

    vtkSmartPointer<vtkTransformPolyDataFilter> transformPart =
      vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformPart->SetTransform(transform);
    transformPart->SetInputConnection(partSource->GetOutputPort());
    phantomData->AddInputConnection(transformPart->GetOutputPort());
    }

  vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
    vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
  connectivityFilter->SetInputConnection(phantomData->GetOutputPort());
  connectivityFilter->SetExtractionModeToAllRegions();
  connectivityFilter->ColorRegionsOn();
  connectivityFilter->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(connectivityFilter->GetOutputPort());
  mapper->SetScalarRange(connectivityFilter->GetOutput()->GetPointData()->GetArray("RegionId")->GetRange());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetOpacity(.3);

  // Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->UseDepthPeelingOn();
  renderer->AddActor(actor);
  renderer->SetBackground(.2, .3, .4);

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
  
  return EXIT_SUCCESS;
}
