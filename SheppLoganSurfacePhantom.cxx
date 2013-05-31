
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

class HeadPart
{
public:
  HeadPart(std::string name,
           double centerX,
           double centerY,
           double centerZ,
           double radiusX,
           double radiusY,
           double radiusZ,
           double m11,
           double m12,
           double m13,
           double m21,
           double m22,
           double m23,
           double m31,
           double m32,
           double m33,
           double value)
  {
    m_Transform = vtkSmartPointer<vtkMatrix4x4>::New();
    m_Name = name;
    m_Center[0] = centerX; m_Center[1] = centerY; m_Center[2] = centerZ;
    m_Radius[0] = radiusX; m_Radius[1] = radiusY; m_Radius[2] = radiusZ;

    m_Transform->Identity();

    m_Transform->SetElement(0,0,m11);
    m_Transform->SetElement(1,0,m12);
    m_Transform->SetElement(2,0,m13);
    m_Transform->SetElement(0,1,m21);
    m_Transform->SetElement(1,1,m22);
    m_Transform->SetElement(2,1,m23);
    m_Transform->SetElement(0,2,m31);
    m_Transform->SetElement(1,2,m32);
    m_Transform->SetElement(2,2,m33);

    m_Value = value;
  }

  std::string                   m_Name;
  double                        m_Center[3];
  double                        m_Radius[3];
  vtkSmartPointer<vtkMatrix4x4> m_Transform;
  double                        m_Value;
};

int main (int, char *[])
{
  HeadPart outerSkull(
    "OuterSkull",
    0, 0, 0,
    0.7233, 0.9644, 1.27,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    2.0);
  HeadPart innerSkull(
    "InnerSkull",
    0, -0.0184, -0.0185,
    0.7008, 0.9246, 1.2241,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    -0.9800);
  HeadPart leftEye(
    "LeftEye",
    0.2583, 0.7534, 0.0,
    0.1270, 0.1270, 0.1270,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    -1.0);
  HeadPart rightEye(
    "RightEye",
    -0.2583, 0.7534, 0.0,
    0.1270, 0.1270, 0.1270,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    -1.0);
  HeadPart nose(
    "Nose",
    0.0, 1.1398, -0.1957,
    0.1270, 0.3400, 0.1700,
    1, 0, 0, 0, 0.5446, -0.8387, 0, 0.8387, 0.5446,
    1.5);
  HeadPart mouth(
    "Mouth",
    0.0, 0.0, -0.7620,
    0.4575, 0.6099, 0.5080,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    -1.0);
  HeadPart leftEar(
    "LeftEar",
    0.7076, -0.1378, -.1905,
    0.0635, 0.3175, 0.3175,
    0.9903, -0.1085, -0.0865, 0.1089, 0.9941, 0.0, 0.0860, -0.0094, 0.9963,
    1.0);
  HeadPart rightEar(
    "RightEar",
    -0.7076, -0.1378, -.1905,
    0.0635, 0.3175, 0.3175,
    -0.9903, -0.1085, -0.0865, -0.1089, 0.9941, 0.0, -0.0860, -0.0094, 0.9963,
    1.0);
  HeadPart leftSmallTumor(
    "LeftSmallTumor",
    -0.0800, -0.6050, 0.3810,
    0.0460, 0.0230, 0.0230,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    0.01);
  HeadPart centerSmallTumor(
    "CenterSmallTumor",
    0.0, -0.6050,0.3810,
    0.0230, 0.0230, 0.0460,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    0.01);
  HeadPart rightSmallTumor(
    "RightSmallTumor",
    0.0600, -0.6050,0.3810,
    0.0230, 0.0460, 0.0230,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    0.01);
  HeadPart rightVentricle(
    "RightVentricle",
    0.2200, 0.0, 0.3810,
    0.1100, 0.3100, 0.2540,
    0.9511, -0.3090, 0, 0.3090, 0.9511, 0.0, 0.0, 0.0, 1.0,
    -0.02);
  HeadPart leftVentricle(
    "LeftVentricle",
    -0.2200, 0.0, 0.3810,
    0.1600, 0.4100, 0.3810,
    -0.9511, -0.3090, 0, -0.3090, 0.9511, 0.0, 0.0, 0.0, 1.0,
    -0.02);
  HeadPart bloodClot(
    "BloodClot",
    0.5600, -0.4000, 0.3810,
    0.0300, 0.2000, 0.2000,
    0.9192, -0.3381, 0.2020, 0.3452, 0.9385, 0, 0.1896, -0.0697, -0.9794,
    0.0300);
  std::vector<HeadPart> parts;
  parts.push_back(outerSkull);
  parts.push_back(innerSkull);
  parts.push_back(leftEye);
  parts.push_back(rightEye);
  parts.push_back(nose);
  parts.push_back(mouth);
  parts.push_back(leftEar);
  parts.push_back(rightEar);
  parts.push_back(leftSmallTumor);
  parts.push_back(centerSmallTumor);
  parts.push_back(rightSmallTumor);
  parts.push_back(leftVentricle);
  parts.push_back(rightVentricle);
  parts.push_back(bloodClot);
  vtkSmartPointer<vtkAppendPolyData> phantom =
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
    phantom->AddInputConnection(transformPart->GetOutputPort());
    }

  vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter =
    vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
  connectivityFilter->SetInputConnection(phantom->GetOutputPort());
  connectivityFilter->SetExtractionModeToAllRegions();
  connectivityFilter->ColorRegionsOn();
  connectivityFilter->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(phantom->GetOutputPort());
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
