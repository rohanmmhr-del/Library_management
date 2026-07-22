#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define OPEN_LINK(url) { char cmd[256]; sprintf(cmd, "start %s", url); system(cmd); }
#elif __APPLE__
    #define OPEN_LINK(url) { char cmd[256]; sprintf(cmd, "open %s", url); system(cmd); }
#else
    #define OPEN_LINK(url) { char cmd[256]; sprintf(cmd, "xdg-open %s", url); system(cmd); }
#endif

typedef struct {
    int id;
    char title[100];
    char author[50];
    int isAvailable;
    int renewalCount;
} Book;

Book library[5] = {
    {101, "C Programming Language", "Dennis Ritchie", 1, 0},
    {102, "Data Structures and Algorithms", "Mark Allen", 1, 0},
    {103, "Software Engineering Principles", "Roger S. Pressman", 1, 0},
    {104, "Introduction to Algorithms", "Thomas H. Cormen", 1, 0},
    {105, "Artificial Intelligence: A Modern Approach", "Stuart Russell", 1, 0}
};

int registeredClasses = 0;

void displayHeader();
int selectUserRole();
int authenticateUser(int role);
void mainMenu(int userRole);
void resourcesMenu();
void servicesMenu();
void activityCalendar();
void coreLibraryOperations();

void elibrary();                                            
void databaseAZ();                                         
void instRepository();                                     
void voiceLibrary();                                       
void enewspaper();                                         
void remoteAccess();                                       

void itemRenew();                                           
void literacyClasses();                                    
void internshipPortal();                                   
void refManagement();                                      
void turnitinManual();                                     
void businessInABox();                                     

void searchBook();                                         
void issueBook();
void returnBook();

void skillJobsBooks();                                     
void researchPapers();                                     
void ieltsPreparation();                                   
void calculateOverdueFines();

int main() {
    int userRole;
    displayHeader();
    userRole = selectUserRole();
    
    if (userRole == 0) {
        printf("\nExiting program. Thank you for using DIU Library System!\n");
        return 0;
    }
    
    mainMenu(userRole);
    return 0;
}

void displayHeader() {
    printf("===============================================================\n");
    printf("        DAFFODIL INTERNATIONAL UNIVERSITY LIBRARY SYSTEM       \n");
    printf("           Established to Foster Education & Research          \n");
    printf("===============================================================\n");
}

int authenticateUser(int role) {
    int id, pin, attempts = 3;
    int validId, validPin;

    if (role == 1) {
        validId = 201; validPin = 1111;
    } else if (role == 2) {
        validId = 301; validPin = 2222;
    } else if (role == 3) {
        validId = 101; validPin = 1234;
    } else if (role == 4) {
        validId = 401; validPin = 3333;
    } else if (role == 5) {
        validId = 501; validPin = 4444;
    } else {
        return 0;
    }

    printf("\n===============================================================\n");
    printf("                   USER AUTHENTICATION                         \n");
    printf("===============================================================\n");
    
    while (attempts > 0) {
        printf("Enter ID [Default for selected role: %d]: ", validId);
        scanf("%d", &id);
        printf("Enter PIN [Default for selected role: %d]: ", validPin);
        scanf("%d", &pin);
        
        if (id == validId && pin == validPin) {
            printf("\n[ACCESS GRANTED]: Welcome to DIU Library Control Panel!\n");
            return 1;
        } else {
            attempts--;
            printf("\n[AUTHENTICATION FAILED]: Invalid ID or PIN! Attempts remaining: %d\n\n", attempts);
        }
    }
    printf("[ACCESS DENIED]: Authentication failed.\n");
    return 0;
}

int selectUserRole() {
    int choice;
    while (1) {
        printf("\nPlease select your user profile to customize your experience:\n");
        printf("1. DIU Student (ID: 201, PIN: 1111)\n");
        printf("2. Faculty Member / Teacher (ID: 301, PIN: 2222)\n");
        printf("3. DIU Staff (ID: 101, PIN: 1234)\n");
        printf("4. Skill Jobs Participant (ID: 401, PIN: 3333)\n");
        printf("5. International Scholarship Holder (ID: 501, PIN: 4444)\n");
        printf("0. Exit\n");
        printf("Enter option: ");
        scanf("%d", &choice);
        
        if (choice == 0) {
            return 0;
        } else if (choice >= 1 && choice <= 5) {
            if (authenticateUser(choice)) {
                return choice;
            } else {
                continue;
            }
        } else {
            printf("Invalid selection. Try again!\n");
        }
    }
}

void mainMenu(int userRole) {
    int choice;
    char roles[6][40] = {"Guest", "DIU Student", "Teacher", "Staff", "Skill Jobs Participant", "Scholarship Holder"};
    
    while(1) {
        printf("\n\n===============================================================\n");
        printf("  DASHBOARD | Profile: %s\n", roles[userRole]);
        printf("===============================================================\n");
        printf("1. DIU Library Resources (E-Library, Databases, DSpace...)\n");
        printf("2. DIU Library Services (Renew, Literacy Class, Turnitin...)\n");
        printf("3. View Activity Calendar\n");
        printf("4. Core Library Book Inventory (Search / Issue / Return)\n");
        
        if (userRole == 4) {
            printf("5. [SPECIAL] Access Skill Jobs Books Portal\n");
        } else if (userRole == 2) {
            printf("5. [SPECIAL] Search Academic Research Papers\n");
        } else if (userRole == 5) {
            printf("5. [SPECIAL] Open IELTS Preparation Portal\n");
        } else if (userRole == 3) {
            printf("5. [DIU STAFF] Calculate Overdue Fines\n");
        } else {
            printf("5. Switch User Profile\n");
        }
        
        if (userRole >= 1 && userRole <= 5) {
            printf("6. Switch User Profile\n");
            printf("0. Exit System\n");
        } else {
            printf("0. Exit System\n");
        }
        
        printf("---------------------------------------------------------------\n");
        printf("Choose action: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            resourcesMenu();
        } else if (choice == 2) {
            servicesMenu();
        } else if (choice == 3) {
            activityCalendar();
        } else if (choice == 4) {
            coreLibraryOperations();
        } else if (choice == 5) {
            if (userRole == 4) skillJobsBooks();
            else if (userRole == 2) researchPapers();
            else if (userRole == 5) ieltsPreparation();
            else if (userRole == 3) calculateOverdueFines();
            else userRole = selectUserRole();
        } else if (choice == 6 && (userRole >= 1 && userRole <= 5)) {
            userRole = selectUserRole();
            if (userRole == 0) {
                printf("\nExiting system... Thank you for using DIU Library Services.\n");
                exit(0);
            }
        } else if (choice == 0) {
            printf("\nExiting system... Thank you for using DIU Library Services.\n");
            exit(0);
        } else {
            printf("\nInvalid selection. Try again!\n");
        }
    }
}

void resourcesMenu() {
    int option;
    while(1) {
        printf("\n--- DIU RESOURCES PORTAL ---\n");
        printf("1. Access E-Library (Trending Books Online)\n");
        printf("2. Browse Database A-Z\n");
        printf("3. Access Institutional Repository (DSpace)\n");
        printf("4. Access Voice Library (with Voice Search)\n");
        printf("5. Read E-Newspapers (Live Link)\n");
        printf("6. Configure Remote Access\n");
        printf("7. Return to Main Menu\n");
        printf("Select choice (1-7): ");
        scanf("%d", &option);
        
        if (option == 7) break;
        
        switch(option) {
            case 1: elibrary(); break;
            case 2: databaseAZ(); break;
            case 3: instRepository(); break;
            case 4: voiceLibrary(); break;
            case 5: enewspaper(); break;
            case 6: remoteAccess(); break;
            default: printf("Invalid option!\n");
        }
    }
}

void servicesMenu() {
    int option;
    while(1) {
        printf("\n--- DIU SERVICES PORTAL ---\n");
        printf("1. Item Renew Online\n");
        printf("2. Book Information Literacy Classes\n");
        printf("3. Internship Portal & DSpace Guidelines\n");
        printf("4. Reference Management Help\n");
        printf("5. View Turnitin Manual\n");
        printf("6. DIU Business in a Box Initiator\n");
        printf("7. Return to Main Menu\n");
        printf("Select choice (1-7): ");
        scanf("%d", &option);
        
        if (option == 7) break;
        
        switch(option) {
            case 1: itemRenew(); break;
            case 2: literacyClasses(); break;
            case 3: internshipPortal(); break;
            case 4: refManagement(); break;
            case 5: turnitinManual(); break;
            case 6: businessInABox(); break;
            default: printf("Invalid option!\n");
        }
    }
}

void activityCalendar() {
    printf("\n===============================================================\n");
    printf("                DIU LIBRARY ACTIVITY CALENDAR                  \n");
    printf("===============================================================\n");
    printf(" [Jan 15] - Welcome Session & Virtual Library Tour for Freshers\n");
    printf(" [Feb 21] - Special Language Movement History Book Display\n");
    printf(" [Mar 10] - Workshop: How to Avoid Plagiarism with Turnitin\n");
    printf(" [May 05] - Seminar on Advanced Citation Management (Mendeley)\n");
    printf(" [Jul 18] - E-Journal Publishing Guidance for Teachers/Students\n");
    printf(" [Oct 22] - National Library Day: Special DIU Innovation Hub Event\n");
    printf("===============================================================\n");
    printf("Press any number and hit Enter to return to dashboard: ");
    int temp;
    scanf("%d", &temp);
}

void coreLibraryOperations() {
    int option;
    while(1) {
        printf("\n--- CORE PHYSICAL BOOK TRANSACTIONS ---\n");
        printf("1. Search & View Book Catalog\n");
        printf("2. Borrow / Issue Book\n");
        printf("3. Return Borrowed Book\n");
        printf("4. Return to Main Menu\n");
        printf("Select choice (1-4): ");
        scanf("%d", &option);
        
        if (option == 4) break;
        
        switch(option) {
            case 1: searchBook(); break;
            case 2: issueBook(); break;
            case 3: returnBook(); break;
            default: printf("Invalid choice!\n");
        }
    }
}

void elibrary() {
    printf("\n>>> Launching DIU E-Library Portal (OpenLibrary Platform) <<<\n");
    printf("Redirecting to: https://openlibrary.org/trending/now\n");
    OPEN_LINK("https://openlibrary.org/trending/now");
}

void databaseAZ() {
    printf("\n>>> Database A-Z Portal <<<\n");
    printf("Currently subscribed resources for academic research:\n");
    printf(" - [1] IEEE Xplore Digital Library\n");
    printf(" - [2] Scopus Citations database\n");
    printf(" - [3] Emerald Insight Journals\n");
    printf(" - [4] SpringerLink Journals\n");
    printf("All databases are currently ACTIVE and authenticated via DIU IP.\n");
}

void instRepository() {
    printf("\n>>> DIU DSpace Institutional Repository <<<\n");
    printf("Displaying recent submissions:\n");
    printf(" - 'Machine Learning Models on Bengali OCR' (CSE Thesis)\n");
    printf(" - 'Sustainable Fashion Supply Chain' (Textile Eng.)\n");
    printf("Direct Link: http://dspace.daffodilvarsity.edu.bd:8080\n");
}

void voiceLibrary() {
    int option;
    char spokenPhrase[100];
    
    printf("\n===================================================\n");
    printf("             DIU ACCESSIBLE VOICE LIBRARY           \n");
    printf("===================================================\n");
    printf("1. Trigger Voice Activation & Search\n");
    printf("2. List all accessible Audiobooks\n");
    printf("3. Exit Voice Module\n");
    printf("Choose voice option: ");
    scanf("%d", &option);
    
    if (option == 1) {
        printf("\n[Voice Assistant]: \"Listening... Speak now.\"\n");
        printf("(Type what you want to 'say' to the library system): ");
        getchar(); 
        fgets(spokenPhrase, sizeof(spokenPhrase), stdin);
        spokenPhrase[strcspn(spokenPhrase, "\n")] = 0; 
        
        printf("\n[Voice Assistant Processor]: Re-analyzing speech wave pattern...\n");
        printf(">> Voice Tag Processed: \"%s\"\n", spokenPhrase);
        printf(">> Result: Successfully found 2 matches in Audio Archives relating to \"%s\"!\n", spokenPhrase);
    } else if (option == 2) {
        printf("\nReady Audio Streams:\n");
        printf(" - 'C Programming Audio Class' [Chapter 1-5]\n");
        printf(" - 'Business Ethics for Entrepreneurs' [Full Audiobook]\n");
    } else {
        printf("Returning to Resources Portal.\n");
    }
}

void enewspaper() {
    printf("\n>>> Launching Live E-Newspaper Platform <<<\n");
    printf("Redirecting to: https://www.24livenewspaper.com/bangla-newspaper\n");
    OPEN_LINK("https://www.24livenewspaper.com/bangla-newspaper");
}

void remoteAccess() {
    char email[50];
    printf("\n>>> Remote Access Configuration <<<\n");
    printf("To access DIU paid resources off-campus, enter your official @diu.edu.bd email: ");
    scanf("%s", email);
    if (strstr(email, "@diu.edu.bd") != NULL || strstr(email, "@daffodilvarsity.edu.bd") != NULL) {
        printf("Authentication request sent to %s. Please login to MyLOFT application to verify.\n", email);
    } else {
        printf("Error: This service is strictly restricted to valid DIU domain emails!\n");
    }
}

void itemRenew() {
    int bookId, found = 0;
    printf("\n>>> Item Renew Service <<<\n");
    printf("Enter Book ID you wish to renew: ");
    scanf("%d", &bookId);
    
    for(int i = 0; i < 5; i++) {
        if(library[i].id == bookId) {
            found = 1;
            if(library[i].isAvailable == 0) {
                if(library[i].renewalCount < 2) {
                    library[i].renewalCount++;
                    printf("Success! Renewed '%s'. New return due date extended by 7 days.\n", library[i].title);
                    printf("Total times renewed: %d/2\n", library[i].renewalCount);
                } else {
                    printf("Renewal Denied! Maximum renewal limit (2 times) exceeded for this item.\n");
                }
            } else {
                printf("Error: This book is not issued out yet, no renewal needed.\n");
            }
            break;
        }
    }
    if(!found) printf("Error: Book ID %d not found in records.\n", bookId);
}

void literacyClasses() {
    int option;
    printf("\n>>> Information Literacy & Research Seminars <<<\n");
    printf("1. Join 'Advanced Searching & Google Scholar Hacks' (Saturday 3:00 PM)\n");
    printf("2. Join 'How to Format Academic Thesis' (Monday 11:00 AM)\n");
    printf("Currently booked classes: %d\n", registeredClasses);
    printf("Select a class to register (1-2) or 0 to cancel: ");
    scanf("%d", &option);
    if(option == 1 || option == 2) {
        registeredClasses++;
        printf("Registration successful! Joining links have been dispatched to your portal.\n");
    }
}

void internshipPortal() {
    printf("\n>>> Internship Portal & DSpace Upload Guidelines <<<\n");
    printf("ATTENTION GRADUATING STUDENTS:\n");
    printf("1. Final PDF must contain your signature page.\n");
    printf("2. File size must remain under 15MB.\n");
    printf("3. Format naming: ID_Department_Semester.pdf (e.g., 221-15-XXX_CSE_Fall2025.pdf).\n");
    printf("Download link to Official Template: https://diu.edu.bd/dspace-upload-guide\n");
}

void refManagement() {
    int choice;
    printf("\n>>> Reference Management Assistant <<<\n");
    printf("Generate standard academic citation guide for:\n");
    printf("1. Mendeley\n");
    printf("2. Zotero\n");
    printf("Choose (1-2): ");
    scanf("%d", &choice);
    if(choice == 1) {
        printf("Download Mendeley Desktop plugin for Word. Use APA 7th edition as default citation format.\n");
    } else if(choice == 2) {
        printf("Install Zotero Connector for Chrome. Sync your library with Zotero Cloud for safe recovery.\n");
    } else {
        printf("Invalid input.\n");
    }
}

void turnitinManual() {
    printf("\n>>> Turnitin Plagiarism Portal Manual <<<\n");
    printf("1. Do not use random AI paraphrasers (Turnitin flags AI-generated structures).\n");
    printf("2. Allowed similarity index for DIU standard thesis: LESS than 20%%.\n");
    printf("3. To receive credentials, reach out to your respective supervisor or visit the DIU main library building (3rd floor).\n");
}

void businessInABox() {
    printf("\n>>> DIU 'Business in a Box' - Startup Resource Hub <<<\n");
    printf("Fostering student entrepreneurship in Daffodil Varsity!\n");
    printf("Available Packages to unlock:\n");
    printf(" - Pitch Deck Templates\n");
    printf(" - Trade License Registration guide in Bangladesh\n");
    printf(" - Access to DIU Venture Capital contact directory\n");
    printf("Ask a librarian physically to claim your enterprise access pack!\n");
}

void searchBook() {
    printf("\n--- DIU Library Catalog ---\n");
    printf("%-8s %-42s %-25s %-12s\n", "Book ID", "Title", "Author", "Status");
    printf("-----------------------------------------------------------------------------------------\n");
    for(int i = 0; i < 5; i++) {
        printf("%-8d %-42s %-25s %-12s\n", 
               library[i].id, 
               library[i].title, 
               library[i].author, 
               library[i].isAvailable ? "Available" : "Issued Out");
    }
}

void issueBook() {
    int bookId, found = 0;
    printf("\nEnter Book ID to Issue: ");
    scanf("%d", &bookId);
    
    for(int i = 0; i < 5; i++) {
        if(library[i].id == bookId) {
            found = 1;
            if(library[i].isAvailable == 1) {
                library[i].isAvailable = 0;
                library[i].renewalCount = 0;
                printf("Success! Book '%s' has been checked out to you.\n", library[i].title);
            } else {
                printf("Sorry, this book is currently borrowed by someone else.\n");
            }
            break;
        }
    }
    if(!found) printf("Book ID %d not found in the DIU Library system.\n", bookId);
}

void returnBook() {
    int bookId, found = 0;
    printf("\nEnter Book ID to Return: ");
    scanf("%d", &bookId);
    
    for(int i = 0; i < 5; i++) {
        if(library[i].id == bookId) {
            found = 1;
            if(library[i].isAvailable == 0) {
                library[i].isAvailable = 1;
                library[i].renewalCount = 0;
                printf("Success! Book '%s' has been safely returned to the shelf.\n", library[i].title);
            } else {
                printf("This book is already sitting in the library catalog.\n");
            }
            break;
        }
    }
    if(!found) printf("Book ID %d not found in the DIU Library system.\n", bookId);
}

void skillJobsBooks() {
    printf("\n>>> Accessing Skill Jobs Career Books & Preparation Catalog <<<\n");
    printf("Opening free career e-books portal in web browser...\n");
    OPEN_LINK("https://www.free-ebooks.net/career");
}

void researchPapers() {
    printf("\n>>> Launching Google Scholar Research Directory <<<\n");
    printf("Opening Search Console for Teachers & Researchers...\n");
    OPEN_LINK("https://scholar.google.com/");
}

void ieltsPreparation() {
    printf("\n>>> Launching IELTS Liz Prep Materials Portal <<<\n");
    printf("Opening scholarship preparation tracks...\n");
    OPEN_LINK("https://ieltsliz.com/");
}

void calculateOverdueFines() {
    char studentID[20];
    int daysOverdue;
    float finePerDay = 10.0; 
    float totalFine = 0.0;
    
    printf("\n===============================================================\n");
    printf("          DIU STAFF - OVERDUE FINE CALCULATOR                  \n");
    printf("===============================================================\n");
    printf("Enter Student ID (e.g., 221-15-XXX): ");
    scanf("%s", studentID);
    printf("Enter Total Days Overdue: ");
    scanf("%d", &daysOverdue);
    
    if (daysOverdue <= 0) {
        printf("\n[RECORD]: Student ID %s has NO overdue fine. Book returned on time!\n", studentID);
    } else if (daysOverdue <= 7) {
        totalFine = daysOverdue * finePerDay;
        printf("\n--- DIU FINE RECEIPT ---");
        printf("\nStudent ID      : %s", studentID);
        printf("\nDays Overdue    : %d days", daysOverdue);
        printf("\nFine Rate       : %.2f BDT/day", finePerDay);
        printf("\nTotal Fine Due  : %.2f BDT\n", totalFine);
    } else {
        totalFine = (7 * finePerDay) + ((daysOverdue - 7) * 20.0);
        printf("\n--- DIU FINE RECEIPT (WITH LATE PENALTY) ---");
        printf("\nStudent ID      : %s", studentID);
        printf("\nDays Overdue    : %d days", daysOverdue);
        printf("\nStandard Fine   : 70.00 BDT (First 7 days)");
        printf("\nLate Penalty    : %.2f BDT (%d days @ 20 BDT/day)", (daysOverdue - 7) * 20.0, daysOverdue - 7);
        printf("\nTotal Fine Due  : %.2f BDT\n", totalFine);
    }
}