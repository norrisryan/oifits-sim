
#ifndef TRIPLET_H
#define TRIPLET_H

#include <string>
#include <complex>
// Include headers for the unordered map.  Note, this may need to be just <unordered_map> if compiled in MSVS.
#include <tr1/unordered_map>

#include "Baseline.h"
#include "Station.h"
#include "UVPoint.h"

using namespace std;

typedef std::tr1::unordered_map<std::string, complex<double> > BisHash;
typedef std::tr1::unordered_map<std::string, double> BisErrHash;

class Station;
class Array;
class Target;

/// \todo The hash lookup code in this class is in common with the Baseline code, factor this.

class Triplet
{
  private:
    Station     *   mStations[3];
    Baseline    *   mBaselines[3]; 
    string name;
    
    BisHash     mBisValues; // Stores computed bispectrum values
    BisErrHash  mBisErrors; // Stores computed/stored bispectrum error values.

  public:
    Triplet();
    Triplet(Array * array, Station * station1, Station * station2, Station * station3);

  private:    
//    complex<double> ComputeBisError(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);
    complex<double> ComputeBis(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);
    string  GetHashKey(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);
    
  public:
    string  GetName(void);
    bool    ContainsBaseline(string bl_name);
    
    complex<double> GetBispectra(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);
    complex<double> GetBispectra(Target & target, double hour_angle, double wavenumber);
    
    double  GetBisAmp(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);
    double  GetBisAmpErr(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);
    double  GetBisPhi(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);
    double  GetBisPhiErr(Target & target, UVPoint uv_ab, UVPoint uv_bc, UVPoint uv_ac);

    // Wrapper functions for the UV-coordinate taking versions.
    double  GetBisAmp(Target & target, double hour_angle, double wavenumber);
    double  GetBisAmpErr(Target & target, double hour_angle, double wavenumber);
    double  GetBisPhi(Target & target, double hour_angle, double wavenumber);
    double  GetBisPhiErr(Target & target, double hour_angle, double wavenumber);

        
    int     GetStationID(int station_num);
    
    Baseline * GetBaseline(int baseline_num);
};

// Hash data type
typedef std::tr1::unordered_map<std::string, Triplet*> TripletHash;

vector<Triplet> ComputeTriplets(Array * array, vector<Station> & stations);
TripletHash ComputeTripletHash(vector<Triplet> & triplets);

#endif // TRIPLET_H
