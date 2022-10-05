#include "G4MagneticField.hh"

#include <fstream>
#include <vector>
#include <cmath>

class MagLensTabulatedField3D : public G4MagneticField
{
  std::vector< std::vector< std::vector< double > > > xField;
  std::vector< std::vector< std::vector< double > > > yField;
  std::vector< std::vector< std::vector< double > > > zField;

  int nx,ny,nz; 
  // The physical limits of the defined region
  double minx, maxx, miny, maxy, minz, maxz;
  // The physical extent of the defined region
  double dx, dy, dz;

public:

  MagLensTabulatedField3D(const char* filename );
  void  GetFieldValue( const  double Point[4],
		                double *Bfield ) const;
};