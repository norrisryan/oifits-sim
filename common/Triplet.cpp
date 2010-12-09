#include "Triplet.h"

#include "Array.h"
#include "Baseline.h"
#include "Station.h"

#include <cstdio>
using namespace std;

Triplet::Triplet()
{
    // Just initialize one variable.
    this->name = "INVALID_TRIPLET";
}

Triplet::Triplet(Array * array, Station & station1, Station & station2, Station & station3)
{
    // Query the array for the baselines involved
    mStations[0] = station1;
    mStations[1] = station2;
    mStations[2] = station3;
    
    string baseline;
    baseline = station1.GetName() + '-' + station2.GetName();    // ab
    mBaselines[0] = array->GetBaseline(baseline);
    baseline = station2.GetName() + '-' + station3.GetName();    // bc
    mBaselines[1] = array->GetBaseline(baseline);
    baseline = station1.GetName() + '-' + station3.GetName();    // ac, note we do this backward to keep names ordered
    mBaselines[2] = array->GetBaseline(baseline);
    
    this->name = station1.GetName() + "-" + station2.GetName() + "-" + station3.GetName();
    
    // A useful comment if you wish to see all triplets created.
//    printf("Constructed %s from %s %s %s\n", this->name.c_str(), 
//        mBaselines[0].GetName().c_str(),  mBaselines[1].GetName().c_str(),  mBaselines[2].GetName().c_str());
}

/// Returns the name of this triplet as the names of the stations separated by hyphens, e.g.:
///     S1-E1-E2
string Triplet::GetName(void)
{
    return this->name;
}

/// Returns a boolean to indicate if the baseline specified by bl_name is involved in this triplet.
bool    Triplet::ContainsBaseline(string bl_name)
{
    /// \todo We could probably implment this as a hash lookup instead of a string operation
    /// this should make the lookup slightly faster.
    for(int i = 0; i < 3; i++)
    {
        if(this->mBaselines[i].GetName() == bl_name)
            return true;
    }

    return false;
}

// Computes the bispectra from the three baselines in this triplet.
complex<double> Triplet::GetBispectra(Source & source, double hour_angle, double wavenumber)
{
    // Get the visibilities on the baselines AB, BC, and CA.
    complex<double> AB = mBaselines[0].GetVisibility(source, hour_angle, wavenumber);
    complex<double> BC = mBaselines[1].GetVisibility(source, hour_angle, wavenumber);
    complex<double> AC = mBaselines[2].GetVisibility(source, hour_angle, wavenumber);   // Note, this is AC NOT CA
    
    // Now compute the bispectrum.  We take the conjugate of AC to form CA.
    return AB * BC * conj(AC);
}

////////////////////////////////////////////////////////////////////
// Non Class Functions Below
////////////////////////////////////////////////////////////////////

/// Computes all possible triplets for the stations provided.
vector<Triplet> ComputeTriplets(Array * array, vector<Station> stations)
{
    // init some local variables
    vector<Triplet> triplets;
    int num_stations = stations.size();

    // Compute all possible triplets.  This should total to N choose 3.
    for(int i = 0; i < num_stations - 2; i++)
    {
        for(int j = i + 1; j < num_stations - 1; j++)
        {
            for(int k = j + 1; k < num_stations; k++)
            {
                triplets.push_back( Triplet(array, stations[i], stations[j], stations[k]) );
            }
        }
    }
    
    return triplets;
}

TripletHash ComputeTripletHash(vector<Triplet> triplets)
{
    TripletHash hash;
    
    for(unsigned int i = 0; i < triplets.size(); i++)
    {
        hash.insert(TripletHash::value_type(triplets[i].GetName(), triplets[i]) );
    }
    
    return hash;
}
