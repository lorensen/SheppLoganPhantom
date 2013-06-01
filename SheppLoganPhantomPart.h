#ifndef __itkSheppLoganPhantomPart_h
#define __itkSheppLoganPhantomPart_h
#include <vtkSmartPointer.h>
#include <string>
#include <vector>

#include <vtkMatrix4x4.h>

class SheppLoganPhantomPart
{
public:
  SheppLoganPhantomPart(std::string name,
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

#endif
