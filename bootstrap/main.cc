/* Copyright (c) 2015, Alister Chan. All rights reserved.
 * See the README file for license conditions and contact info.
 */
/* Main entry for bootstrap compiler */

#include "main.hh"

using namespace std;

int main(int argc, char* argv[]) {
    
    int result = 1;
    driver drv;
    
    for(int i = 1; i < argc; i++) {
        if(argv[i] == string("-p")) {
            drv.trace_parsing = true;
        } else if(argv[i] == string("-s")) {
            drv.trace_scanning = true;
        } else if(argv[i] == string("-v1")) {
            LOGGER_SET_LEVEL(logger::L1);
        } else if(argv[i] == string("-v2")) {
            LOGGER_SET_LEVEL(logger::L2);
        } else if(argv[i] == string("-v3")) {
            LOGGER_SET_LEVEL(logger::L3);
        } else if(argv[i] == string("-V") ||
                  argv[i] == string("--version")) {
            cout << "lc (Leo Compiler)" << LC_VERSION << "\n"
                 << "Written by Alister Chan.\n\n"
                 << "Copyright (c) 2015, Madcoz. All rights reserved.\n"
                 << "See the README file for license conditions and contact info."
                 << endl;   
            break;
        } else {
            result = drv.parse(argv[i]);
        }
    }
    return result;
}