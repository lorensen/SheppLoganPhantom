#include "SheppLoganPhantom.h"

SheppLoganPhantom::SheppLoganPhantom()
{
  SheppLoganPhantomPart outerSkull(
    "OuterSkull",
    0, 0, 0,
    0.7233, 0.9644, 1.27,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    2.0);
  SheppLoganPhantomPart innerSkull(
    "InnerSkull",
    0, -0.0184, -0.0185,
    0.7008, 0.9246, 1.2241,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    -0.9800);
  SheppLoganPhantomPart leftEye(
    "LeftEye",
    0.2583, 0.7534, 0.0,
    0.1270, 0.1270, 0.1270,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    -1.0);
  SheppLoganPhantomPart rightEye(
    "RightEye",
    -0.2583, 0.7534, 0.0,
    0.1270, 0.1270, 0.1270,
    1, 0, 0, 0, 1, 0, 0, 0, 1,
    -1.0);
  SheppLoganPhantomPart nose(
    "Nose",
    0.0, 1.1398, -0.1957,
    0.1270, 0.3400, 0.1700,
    1, 0, 0, 0, 0.5446, -0.8387, 0, 0.8387, 0.5446,
    1.5);
  SheppLoganPhantomPart mouth(
    "Mouth",
    0.0, 0.0, -0.7620,
    0.4575, 0.6099, 0.5080,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    -1.0);
  SheppLoganPhantomPart leftEar(
    "LeftEar",
    0.7076, -0.1378, -.1905,
    0.0635, 0.3175, 0.3175,
    0.9903, -0.1085, -0.0865, 0.1089, 0.9941, 0.0, 0.0860, -0.0094, 0.9963,
    1.0);
  SheppLoganPhantomPart rightEar(
    "RightEar",
    -0.7076, -0.1378, -.1905,
    0.0635, 0.3175, 0.3175,
    -0.9903, -0.1085, -0.0865, -0.1089, 0.9941, 0.0, -0.0860, -0.0094, 0.9963,
    1.0);
  SheppLoganPhantomPart leftSmallTumor(
    "LeftSmallTumor",
    -0.0800, -0.6050, 0.3810,
    0.0460, 0.0230, 0.0230,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    0.1);
  SheppLoganPhantomPart centerSmallTumor(
    "CenterSmallTumor",
    0.0, -0.6050,0.3810,
    0.0230, 0.0230, 0.0460,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    0.1);
  SheppLoganPhantomPart rightSmallTumor(
    "RightSmallTumor",
    0.0600, -0.6050,0.3810,
    0.0230, 0.0460, 0.0230,
    1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
    0.1);
  SheppLoganPhantomPart rightVentricle(
    "RightVentricle",
    0.2200, 0.0, 0.3810,
    0.1100, 0.3100, 0.2540,
    0.9511, -0.3090, 0, 0.3090, 0.9511, 0.0, 0.0, 0.0, 1.0,
    -0.2);
  SheppLoganPhantomPart leftVentricle(
    "LeftVentricle",
    -0.2200, 0.0, 0.3810,
    0.1600, 0.4100, 0.3810,
    -0.9511, -0.3090, 0, -0.3090, 0.9511, 0.0, 0.0, 0.0, 1.0,
    -0.2);
  SheppLoganPhantomPart bloodClot(
    "BloodClot",
    0.5600, -0.4000, 0.3810,
    0.0300, 0.2000, 0.2000,
    0.9192, -0.3381, 0.2020, 0.3452, 0.9385, 0, 0.1896, -0.0697, -0.9794,
    0.300);

  m_Parts.push_back(outerSkull);
  m_Parts.push_back(innerSkull);
  m_Parts.push_back(leftEye);
  m_Parts.push_back(rightEye);
  m_Parts.push_back(nose);
  m_Parts.push_back(mouth);
  m_Parts.push_back(leftEar);
  m_Parts.push_back(rightEar);
  m_Parts.push_back(leftSmallTumor);
  m_Parts.push_back(centerSmallTumor);
  m_Parts.push_back(rightSmallTumor);
  m_Parts.push_back(leftVentricle);
  m_Parts.push_back(rightVentricle);
  m_Parts.push_back(bloodClot);
}

const SheppLoganPhantom::PartContainerType &
SheppLoganPhantom::GetParts() const
{
  return m_Parts;
}
