#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    static time_t start = std::time(NULL);
    
    while(1){
        time_t now = time(NULL);
        cout << "now:" << now << endl;
        if( now - start > 10){
            usleep(500000);
            cout << "sleep 500ms." << endl;
            start  = now;
        }
        sleep(1);
    }
    return 0;
}