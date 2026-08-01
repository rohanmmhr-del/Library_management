/* terminal codes :
cd Library_management

dir

gcc main.c library.c md5.c -o library_system 

.\library_system.exe
*/

#include "library.h"

int main() {
    initializeFiles();
    loadUsersFromFile();
    
    if (userAuthenticationFlow()) {
        mainMenu();
    }
    
    printf("\nExiting program. Thank you for using DIU Library System!\n");
    return 0;
}