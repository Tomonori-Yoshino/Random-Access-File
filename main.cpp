#include "CampsiteRecord.h"
#include "Campsite.h"
#include "CampsiteDB.h"
#include <ctime>



int main(){
   
    
    CampsiteDB db{"campsites.db"};
    cout << "File currently contains " << db.get_record_count() << " records.\n\n";

    std::ifstream fin("sites.txt");
    while(fin.good()){
        Campsite site;
        if(site.from_ascii_file(fin)){
            cout << "Adding " << site << "\n";
            db.write_next_sequential(site);
        }
    }
    cout << "\n\n";
    fin.close();


    cout << "Read index is "  << db.get_current_index() << "\n";
    cout << "Write index is " << db.get_current_index(true) << "\n";
    cout << "File now contains " << db.get_record_count() << " records.\n\n";

    db.list_records();
    cout << "\n";



    // Now query a few directly:
    Campsite result;

    result = db.get_at_index(9); // should be "cabin, riverfront"
    cout << result << "\n";
    result = db.get_at_index(3); // should be "RV site, covered table"
    cout << result << "\n";
    result = db.get_at_index(6); // should be "tent site, large, riverfront"
    cout << result << "\n";
    db.print_record(9);




/*
    // Write different records in two places:
    db.write_at_index(8, Campsite{18, "Pavillion", true, 200.00});
    db.write_at_index(2, Campsite{12, "Treehouse", false, 250.00});
    db.write_at_index(0, Campsite{1000, "Treehouse", false, 250.00});
    db.write_at_index(10, Campsite{1000, "Treehouse", false, 250.00});
*/

    result = db.get_random();
    cout << "Random: " << result << "\n\n";

    cout << "Read index is "  << db.get_current_index() << "\n";
    cout << "Write index is " << db.get_current_index(true) << "\n";
    cout << "They are moved to index 10..." << "\n\n";

    db.move_to_index(10);

    cout << "Now Read index is "  << db.get_current_index() << "\n";
    cout << "Now Write index is " << db.get_current_index(true) << "\n\n";



    cout << "Index 4 and 4 are swapped...\n\n";
    db.swap_records(4, 4);
    db.list_records();

    cout << "\n\nTest get range method\n";
    std::vector<Campsite> sites = db.get_range(0, 10);
    cout << "0 to 10: \n";
    for (auto v : sites){
        cout << v << endl;
    }

    /*
    // See if it worked:
    result = db.get_at_index(8); // should be "Pavillion"
    cout << result << "\n";
    result = db.get_at_index(2); // should be "Treehouse"
    cout << result << "\n";
    */
    


    return 0;
}
