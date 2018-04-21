/**
 * @file Campsite.h
 *
 * Implementation for the Campsite class
 *
 * @remarks
 *     Provided as a resource for the "Random Access Files"
 *     laboratory (CS2124).
 */
#ifndef CAMPSITERECORD_H
#define CAMPSITERECORD_H

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <fstream>
using std::fstream;
#include <cstring>
#include <iomanip>
#include <exception>
#include <vector>
#include <cstdlib>



/**
 * A fixed-size record representing a campsite in a
 * recreational area.
 */
struct CampsiteRecord {
    CampsiteRecord() { memset(this, 0, sizeof(CampsiteRecord)); }
    CampsiteRecord(int number, const char description[], bool has_electric, double rate);
    static const int desc_size = 128;         /// max storage size of description
    int              number;                  /// site number
    char             description[desc_size];  /// a short description
    bool             has_electric;            /// is power available?
    double           rate;                    /// per-night rate


};

std::istream& from_ascii_file( std::istream& strm, CampsiteRecord& site );
std::ostream& operator<<( std::ostream& strm, const CampsiteRecord& site );


#endif
