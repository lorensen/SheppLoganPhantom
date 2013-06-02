
#include <vtkVersion.h>
#include <vtkSmartPointer.h>

#include <vtkParametricFunctionSource.h>
#include <vtkParametricEllipsoid.h>
#include <vtkImageEllipsoidSource.h>
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

    // create an image ellipsoid
    vtkSmartPointer<vtkImageEllipsoidSource> partEllipsoid =
      vtkSmartPointer<vtkImageEllipsoidSource>::New();
    partEllipsoid->SetWholeExtent(
      0, partDimensions[0] - 1,
      0, partDimensions[1] - 1,
      0, partDimensions[2] - 1);
    double partCenter[3] = {0.0, 0.0, 0.0};
    partEllipsoid->SetCenter(partCenter);
    partEllipsoid->SetRadius(parts[i].m_Radius);
    partEllipsoid->SetOutputScalarTypeToFloat();
    partEllipsoid->SetOutValue(0.0);
    partEllipsoid->SetInValue(parts[i].m_Value * 100.0);
    partEllipsoid->Update();

    double partOrigin[3];
    partOrigin[0] = partBounds[1] - partDimensions[0] * phantomSpacing[0];
    partOrigin[1] = partBounds[3] - partDimensions[1] * phantomSpacing[1];
    partOrigin[2] = partBounds[5] - partDimensions[2] * phantomSpacing[2];
    partEllipsoid->GetOutput()->SetOrigin(partOrigin);
    partEllipsoid->GetOutput()->SetSpacing(phantomSpacing);

    vtkSmartPointer<vtkTransform> inverseTransform =    
      vtkSmartPointer<vtkTransform>::New();
    inverseTransform->DeepCopy(transform);
    inverseTransform->Inverse();
    
    vtkSmartPointer<vtkImplicitVolume> implicitVolume =
      vtkSmartPointer<vtkImplicitVolume>::New();
    implicitVolume->SetOutValue(0.0);
    implicitVolume->SetVolume(partEllipsoid->GetOutput());
    implicitVolume->SetTransform(inverseTransform);
    
#if 0
    // Walk the part ellipsoid and evaluate
    double x, y, z;
    int i, j, k;
    for (k = 0, z = partOrigin[2]; k < partDimensions[2]; ++k)
      {
      z += k * phantomSpacing[2];
      for (j = 0, y = partOrigin[1]; j < partDimensions[1]; ++j)
        {
        y += j * phantomSpacing[1];
        for (i = 0, x = partOrigin[0]; i < partDimensions[0]; ++i)
          {
          x += i * phantomSpacing[0];
          double value = implicitVolume->FunctionValue(x, y, z);
          if (value != 0.0)
            {
            std::cout << x << ", " << y << ", " << z << ": " << value << std::endl;
            }
          }
        }
      }
#endif
    vtkSmartPointer<vtkMetaImageWriter> writer =
      vtkSmartPointer<vtkMetaImageWriter>::New();
    std::string fileName = parts[i].m_Name + ".mhd";
    writer->SetFileName(fileName.c_str());
    writer->SetInputConnection(partEllipsoid->GetOutputPort());
    writer->Write();
    }
  return EXIT_SUCCESS;
}
