#ifndef CampsiteDB_h
#define CampsiteDB_h


#include "Campsite.h"

class CampsiteDB {
public:
    CampsiteDB( std::string filename );

    int get_record_count( );
    int get_current_index( bool write = false );

    Campsite get_next_sequential( );
    Campsite get_at_index( int index );

    void write_next_sequential( const Campsite& site );
    void write_at_index( int index, const Campsite& site );

    void print_record( int index, std::ostream& strm = std::cout );
    void list_records( std::ostream& strm = std::cout );

    void move_to_index( int index );
    void swap_records( int index_1, int index_2 );
    std::vector<Campsite> get_range( int first_index, int last_index );
    Campsite get_random() { return get_at_index( rand()%100 ); }


    bool bounds_check( int index, bool write = false );

    // This object is non-copyable
    CampsiteDB(const CampsiteDB&)            = delete;
    CampsiteDB& operator=(const CampsiteDB&) = delete;

private:
    // private methods:
    void _create_file( );
    bool _open_file( );
    // attributes
    std::string  _filename;
    std::fstream _file;
};



#endif
