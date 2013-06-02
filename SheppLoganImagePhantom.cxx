
#include <vtkVersion.h>
#include <vtkSmartPointer.h>

#include <vtkParametricFunctionSource.h>
#include <vtkParametricEllipsoid.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkImplicitVolume.h>
#include <vtkMetaImageWriter.h>
#include <string>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include "SheppLoganPhantom.h"

double Ellipsoid(double a, double b, double c, double x, double y, double z);

int main (int, char *[])
{
  // Allocate the volume
  double phantomBounds[6] =
    {-1.0, 1.0, -1.0, 1.0, -1.5, 1.5};
  int phantomDimension = 256;
  double phantomSpacing[3];
  phantomSpacing[0] = (phantomBounds[1] - phantomBounds[0]) / (phantomDimension - 1);
  phantomSpacing[1] = (phantomBounds[3] - phantomBounds[2]) / (phantomDimension - 1);
  phantomSpacing[2] = (phantomBounds[5] - phantomBounds[4]) / (phantomDimension - 1);

  vtkSmartPointer<vtkImageData> phantomVolume =
    vtkSmartPointer<vtkImageData>::New();
  phantomVolume->SetDimensions(phantomDimension, phantomDimension, phantomDimension);
  phantomVolume->AllocateScalars(VTK_FLOAT,1);
  phantomVolume->SetSpacing(phantomSpacing);
  phantomVolume->SetOrigin(phantomBounds[0], phantomBounds[2], phantomBounds[4]);

  // Initialize the volume
  phantomVolume->GetPointData()->GetScalars()->FillComponent(0, 0.0);

  // Create the phantom parts
  SheppLoganPhantom phantom;
  SheppLoganPhantom::PartContainerType parts = phantom.GetParts();

  // For each part, create an ellipsoid
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
    transformPart->Update();
    double *bounds = transformPart->GetOutput()->GetBounds();
    std::cout << parts[i].m_Name << ": "
             << "(" << bounds[0] << ", " << bounds[1] << "), "
             << "(" << bounds[2] << ", " << bounds[3] << "), "
             << "(" << bounds[4] << ", " << bounds[5] << ")"
             << std::endl;
    
    int partDimensions[3];
    double partBounds[6];
    partBounds[0] = bounds[0] - 4.0 * phantomSpacing[0];
    partBounds[1] = bounds[1] + 4.0 * phantomSpacing[0];
    partBounds[2] = bounds[2] - 4.0 * phantomSpacing[1];
    partBounds[3] = bounds[3] + 4.0 * phantomSpacing[1];
    partBounds[4] = bounds[4] - 4.0 * phantomSpacing[2];
    partBounds[5] = bounds[5] + 4.0 * phantomSpacing[2];
    partDimensions[0] = (partBounds[1] - partBounds[0]) / phantomSpacing[0];
    partDimensions[1] = (partBounds[3] - partBounds[2]) / phantomSpacing[1];
    partDimensions[2] = (partBounds[5] - partBounds[4]) / phantomSpacing[2];
    std::cout << parts[i].m_Name << ": "
             << "(" << partBounds[0] << ", " << partBounds[1] << "), "
             << "(" << partBounds[2] << ", " << partBounds[3] << "), "
             << "(" << partBounds[4] << ", " << partBounds[5] << ")"
             << std::endl;
    std::cout << parts[i].m_Name << ": "
             << partDimensions[0] << ", "
             << partDimensions[1] << ", "
             << partDimensions[2]
             << std::endl;

    double partOrigin[3];
    partOrigin[0] = partBounds[1] - partDimensions[0] * phantomSpacing[0];
    partOrigin[1] = partBounds[3] - partDimensions[1] * phantomSpacing[1];
    partOrigin[2] = partBounds[5] - partDimensions[2] * phantomSpacing[2];

    vtkSmartPointer<vtkTransform> inverseTransform =    
      vtkSmartPointer<vtkTransform>::New();
    inverseTransform->DeepCopy(transform);
    inverseTransform->Inverse();
    
    // Walk the part ellipsoid and evaluate
    double x, y, z;
    int ii, jj, kk;

    for (kk = 0, z = partOrigin[2]; kk < partDimensions[2]; ++kk)
      {
      z += kk * phantomSpacing[2];
      for (jj = 0, y = partOrigin[1]; jj < partDimensions[1]; ++jj)
        {
        y += jj * phantomSpacing[1];
        for (ii = 0, x = partOrigin[0]; ii < partDimensions[0]; ++ii)
          {
          x += ii * phantomSpacing[0];
          double value = Ellipsoid(
            parts[i].m_Radius[0], parts[i].m_Radius[1], parts[i].m_Radius[2],
            x, y, z);
          if (value <= 0.0)
            {
            std::cout << x << ", " << y << ", " << z << ": " << value << std::endl;
            }
          }
        }
      }
    }
  return EXIT_SUCCESS;
}

double Ellipsoid(double a, double b, double c, double x, double y, double z)
{
  return x*x/a*a + y*y/b*b + z*z/c*c - 1.0;
}
