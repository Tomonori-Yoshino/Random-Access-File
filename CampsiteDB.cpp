#include "CampsiteDB.h"


/**
 * Construct a CampsiteDB given a filename and open
 * the file with bainary mode.
 *
 * @param filename  file's name
 */
CampsiteDB::CampsiteDB( std::string filename ){
    _filename = filename;
    if ( !_open_file() ){  //if the file doesn't exist...
        _create_file();
        if ( !_open_file() )  //still couldn't open the file
            throw std::runtime_error{"Unable to create the database."};
    }
}


/**
 * Opens a file with output mode. Since its default
 * is trancate mode, a file will be created if the
 * corresponding file does not exist.
 */
void CampsiteDB::_create_file( ){
    _file.open( _filename, std::ios::out);
    _file.close();
}


/**
 * Opens a file of under lined filename with binary mode.
 *
 * @return true if the file opens, or false otherwise
 */
bool CampsiteDB::_open_file( ){
    _file.open( _filename, std::ios::out | std::ios::in | std::ios::binary );
    _file.seekp(0, std::ios::end);
    return _file.good();
}




/**
 * Writes the given record in the file at the current location.
 *
 * @param site  a record to be written in the file
 */
void CampsiteDB::write_next_sequential( const Campsite& site ){
    if ( get_current_index(true) > get_record_count() )
        throw std::length_error{"Index out of bounds."};

    CampsiteRecord record = site.get_record();
    _file.write( reinterpret_cast<char*>(&record), sizeof(CampsiteRecord));
}


/**
 * Reads a record in the file at the current location.
 *
 * @return the record to be read
 */
Campsite CampsiteDB::get_next_sequential( ){
    if ( get_current_index() >= get_record_count() )
        throw std::length_error{"Index out of bounds."};

    CampsiteRecord record;
    _file.read( reinterpret_cast<char*>(&record), sizeof(CampsiteRecord));
    Campsite site{record};
    return site;
}


/**
 * Counts the number of records in the file.
 *
 * @return  the number of records in the file
 */
int CampsiteDB::get_record_count( ){
    int current = _file.tellg();    //save current location
    _file.seekg(0, std::ios::end);  //move the marker at the end
    int size_file = _file.tellg();  //save the size of the file
    _file.seekg(current, std::ios::beg);  //move back the marker
    return size_file / sizeof(CampsiteRecord);
}


/**
 * Gets the current index of either write or read marker, based
 * on the which one is needed.
 *
 * @param   write  boolean variable indicating if
 *                 it deals with write or not, default to false.
 *
 * @return  the current index of either write or read marker
 */
int CampsiteDB::get_current_index( bool write ){
    int index;
    if ( write )
        index = _file.tellp() / sizeof(CampsiteRecord);
    else
        index = _file.tellg() / sizeof(CampsiteRecord);
    return index;
}



/**
 * Sends each record in the file to the given output stream.
 *
 * @param[out]   strm    the output stream object where the records
 *                       are sent.
 */
void CampsiteDB::list_records( std::ostream& strm ){
    int count = 0, num_of_elem = get_record_count();
    _file.clear();
    _file.seekg(0, std::ios::beg);
    while ( _file.good() && count < num_of_elem ){
        count++;
        Campsite site = get_next_sequential();
        if ( _file.good() )
            site.write(strm);
        cout << endl;
    }
    _file.clear();
    _file.seekg(0, std::ios::beg);
}



/**
 * Gets a record at the given index in the file.
 *
 * @param   index  a index of a value to be read
 *
 * @return  a record at the given index
 */
Campsite CampsiteDB::get_at_index( int index ){
    if ( !bounds_check(index) )
        throw std::length_error{"Index out of bounds"};

    _file.clear();
    int offset = index * sizeof(CampsiteRecord);
    _file.seekg(offset, std::ios::beg);
    return get_next_sequential();
}




/**
 * Writes the given record in the file at the gicen index.
 *
 * @param        index   a index where a record is written
 * @param[out]   site    a record to be written
 */
void CampsiteDB::write_at_index( int index, const Campsite& site ){
    if ( !bounds_check(index, true) )
        throw std::length_error{"Index out of bounds."};

    _file.clear();
    int offset = index * sizeof(CampsiteRecord);
    _file.seekp(offset, std::ios::beg);
    write_next_sequential(site);
}




/**
 * Sends a record of the given index to the output stream.
 *
 * @param        index   a value of index to be printed
 * @param[out]   strm    output stream where the value is sent
 */
void CampsiteDB::print_record( int index, std::ostream& strm ){
    Campsite site = get_at_index( index );
    site.write( strm );
}



/**
 * Moves both the “get” and “put” markers on the underlying
 * file stream to the logical index supplied in the index parameter.
 *
 * @param        index   a position where the markers are moved to
 */
void CampsiteDB::move_to_index( int index ){
    if ( !bounds_check(index, true) )
        throw std::length_error{"Index out of bounds."};

    int offset = index * sizeof(CampsiteRecord);
    _file.seekg( offset, std::ios::beg );
    _file.seekp( offset, std::ios::beg );
}



/**
 * Swaps the record at index_1 with the record at index_2 in the file.
 *
 * @param        index_1     a index of value to be swapped
 * @param        index_2     another index of value to be swapped
 */
void CampsiteDB::swap_records( int index_1, int index_2 ){
    //get each site
    Campsite site1 = get_at_index( index_1 );
    Campsite site2 = get_at_index( index_2 );
    //swap the position
    write_at_index( index_1, site2 );
    write_at_index( index_2, site1 );
}




/**
 * Reads the value ranged between given indices and makes a vector.
 *
 * @param        first_index     a index to bgin reading
 * @param        last_index      a index to stop reading
 *
 * @return  a vector conataing read value
 */
std::vector<Campsite> CampsiteDB::get_range( int first_index, int last_index ){
    last_index--;  //a user is expected to enter the physical number of records
    //check index
    if ( first_index >= last_index
            && !bounds_check(first_index) && !bounds_check(last_index) )
        throw std::length_error{"Index out of bounds."};

    std::vector<Campsite> sites;
    //calculate the offset
    int offset = first_index * sizeof(CampsiteRecord);
    //set the get marker at the start point
    _file.seekg( offset, std::ios::beg );
    while ( first_index <= last_index ){
        //put the value into vector
        sites.push_back( get_next_sequential() );
        first_index++;
        offset = first_index * sizeof(CampsiteRecord);
        //set the marker at the next point
        _file.seekg( offset, std::ios::beg );
    }

    return sites;
}



/**
 * @brief   returns a pseudo-random integer in the interval [low, high]
 * @details Sets up an mt19937 Mersenne Twister random number generator
 *          using a random seed obtained from the best entropy source
 *          available to the <random> library.
 *          Notice:  first call incurs additional setup time.
 *
 * @remarks This is a stand alone function and cited from Dr. Jason L Causey's
 *          slides used in Object Oriented Programming in Spring 2018.
 *          URL https://gitpitch.com/jcausey-astate/CS2124_lecture_notes?p=easy_rand#/
 *
 * @param low  lower-bound of interval for random value (included in interval)
 * @param high upper-bound of interval for random value (included in interval)
 *
 * @return pseudo-random integer in the range [low, high]
 */
int rand_between(int low, int high){
    // used to seed with system entropy
    static std::random_device            seeder;
    // set up (and seed) the PRNG
    static std::mt19937                  generator{seeder()};
    // set up distribution for requested range
    std::uniform_int_distribution<int>   distribution{low, high};
    // generate and return result
    return distribution(generator);
}



/**
 * Gets a record randomly in the file.
 *
 * @return  a record gotten randomly
 */
Campsite CampsiteDB::get_random(){
    //gets a random index betwenn [0, N-1]
    int random_index = rand_between(0, get_record_count()-1);
    return get_at_index( random_index );
}



/**
 * Checks if the given index is out of bounds or not, based on the it is write or read.
 *
 * @param        index      index to be checked
 * @param        write      boolean variable indicating write or read
 *
 * @return  true if the index is in the bounds, and otherwise false.
 */
bool CampsiteDB::bounds_check( int index, bool write ) {
    bool result;
    if ( write ){  //domain of index for write: [0, N]
        result = ( index >= 0 && index <= get_record_count() );
    }
    else{  //domain of index for read: [0, N)
        result = ( index >= 0 && index < get_record_count() );
    }
    return result;
}




