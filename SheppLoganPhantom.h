#ifndef __itkSheppLoganPhantom_h
#define __itkSheppLoganPhantom_h
#include <vtkSmartPointer.h>
#include <string>
#include <vector>

#include <vtkMatrix4x4.h>

#include "SheppLoganPhantomPart.h"

class SheppLoganPhantom
{
public:
  typedef std::vector<SheppLoganPhantomPart> PartContainerType;
  SheppLoganPhantom();

  const PartContainerType &GetParts() const;
private:
  PartContainerType  m_Parts;
};

#endif
