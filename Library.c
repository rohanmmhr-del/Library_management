#include "library.h"

Book library[MAX_BOOKS] = {
    {101, "C Programming Language", "Dennis Ritchie", 1, 0, 0, 0},
    {102, "Data Structures and Algorithms", "Mark Allen", 1, 0, 0, 0},
    {103, "Software Engineering Principles", "Roger S. Pressman", 1, 0, 0, 0},
    {104, "Introduction to Algorithms", "Thomas H. Cormen", 1, 0, 0, 0},
    {105, "Artificial Intelligence: A Modern Approach", "Stuart Russell", 1, 0, 0, 0}
};

UserProfile users[MAX_USERS];
int userCount = 0;
UserProfile *currentUser = NULL;
int registeredClasses = 0;

void clearTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void initializeFiles() {
    FILE *fp = fopen("content_data2.txt", "r");
    if (!fp) {
        fp = fopen("content_data2.txt", "w");
        if (fp) {
            fprintf(fp, "[IELTS_LESSON]\n");
            fprintf(fp, "=== DIU IELTS PREPARATION MODULE ===\n");
            fprintf(fp, "Reading: Focus on skimming topic sentences in academic passages to improve time management.\n");
            fprintf(fp, "Writing: Structure Task 2 with a clear thesis statement, two main body paragraphs, and a summary conclusion.\n");
            fprintf(fp, "Speaking: Expand your responses using the PREP framework (Point, Reason, Example, Point).\n\n");
            
            fprintf(fp, "[SKILL_JOBS]\n");
            fprintf(fp, "=== SKILL JOBS CAREER DEVELOPMENT CORNER ===\n");
            fprintf(fp, "Paragraph 1: In today's digital economy, technical skills must be coupled with adaptive soft skills.\n");
            fprintf(fp, "Paragraph 2: DIU Skill Jobs recommends mastering basic Git, technical writing, and teamwork fundamentals.\n\n");
            
            fprintf(fp, "[RESEARCH_PAPERS]\n");
            fprintf(fp, "=== DAFFODIL INTERNATIONAL UNIVERSITY RESEARCH PUBLICATIONS ===\n");
            fprintf(fp, "1. 'Bengali Handwritten Character Recognition using Deep CNNs' - Dept. of CSE, DIU.\n");
            fprintf(fp, "2. 'Optimization of Supply Chain in Bangladesh Garment Sector' - Dept. of Textile Eng., DIU.\n\n");

            fprintf(fp, "[ENEWSPAPER]\n");
            fprintf(fp, "=== THE DAILY STAR - FRONT PAGE HEADLINES ===\n");
            fprintf(fp, "DIU Innovation Hub Launches Tech Incubator for Student Startups in Smart Campus.\n");
            fprintf(fp, "National Seminar on Higher Education Highlights Research and AI Integration.\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }
}

void loadUsersFromFile() {
    FILE *fp = fopen("users2.txt", "r");
    if (!fp) return;

    userCount = 0;
    while (fscanf(fp, "%d|%99[^|]|%49[^|]|%d|%32s\n", 
                  &users[userCount].id, 
                  users[userCount].name, 
                  users[userCount].department, 
                  &users[userCount].role, 
                  users[userCount].pinHash) == 5) {
        userCount++;
        if (userCount >= MAX_USERS) break;
    }
    fclose(fp);
}

void saveUserToFile(UserProfile u) {
    FILE *fp = fopen("users2.txt", "a");
    if (fp) {
        fprintf(fp, "%d|%s|%s|%d|%s\n", u.id, u.name, u.department, u.role, u.pinHash);
        fclose(fp);
    }
}

void displayHeader() {
    printf("===============================================================\n");
    printf("        DAFFODIL INTERNATIONAL UNIVERSITY LIBRARY SYSTEM       \n");
    printf("           Established to Foster Education & Research          \n");
    printf("===============================================================\n");
}

int userAuthenticationFlow() {
    int choice;
    while (1) {
        clearTerminal();
        displayHeader();
        printf("\n1. Login to Existing Account\n");
        printf("2. Register New User Account\n");
        printf("0. Exit System\n");
        printf("Select option: ");
        if (scanf("%d", &choice) != 1) return 0;

        if (choice == 1) {
            if (loginUser()) return 1;
        } else if (choice == 2) {
            registerNewUser();
        } else if (choice == 0) {
            return 0;
        } else {
            printf("Invalid selection!\n");
        }
    }
}

void registerNewUser() {
    if (userCount >= MAX_USERS) {
        printf("User database limit reached.\n");
        return;
    }

    UserProfile u;
    char rawPin[20];

    printf("\n=== NEW USER REGISTRATION ===\n");
    printf("Enter User ID (Numbers only): ");
    scanf("%d", &u.id);

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == u.id) {
            printf("Error: User ID already registered!\n");
            return;
        }
    }

    printf("Enter Full Name: ");
    getchar(); 
    fgets(u.name, sizeof(u.name), stdin);
    u.name[strcspn(u.name, "\n")] = 0;

    printf("Enter Department: ");
    fgets(u.department, sizeof(u.department), stdin);
    u.department[strcspn(u.department, "\n")] = 0;

    printf("Select Profile Role:\n");
    printf(" 1. DIU Student\n 2. Faculty / Teacher\n 3. DIU Staff\n 4. Skill Jobs Participant\n 5. Intl Scholarship Holder\nRole (1-5): ");
    scanf("%d", &u.role);

    printf("Create Secret Passcode / PIN: ");
    scanf("%19s", rawPin);

    hashPassword(rawPin, u.pinHash);

    users[userCount] = u;
    userCount++;
    saveUserToFile(u);

    printf("\n[REGISTRATION SUCCESSFUL]: Profile created securely with MD5 Encryption!\n");
    printf("\nPress Enter to continue...");
    getchar(); getchar();
}

int loginUser() {
    int id;
    char rawPin[20], inputHash[33];

    printf("\n=== USER LOGIN ===\n");
    printf("Enter User ID: ");
    scanf("%d", &id);
    printf("Enter PIN: ");
    scanf("%19s", rawPin);

    hashPassword(rawPin, inputHash);

    for (int i = 0; i < userCount; i++) {
        if (users[i].id == id) {
            if (strcmp(users[i].pinHash, inputHash) == 0) {
                currentUser = &users[i];
                printf("\n[ACCESS GRANTED]: Welcome back, %s!\n", currentUser->name);
                printf("\nPress Enter to continue...");
                getchar(); getchar();
                return 1;
            } else {
                printf("\n[AUTHENTICATION FAILED]: Invalid PIN!\n");
                printf("\nPress Enter to continue...");
                getchar(); getchar();
                return 0;
            }
        }
    }
    printf("\n[AUTHENTICATION FAILED]: User ID not found. Please register.\n");
    printf("\nPress Enter to continue...");
    getchar(); getchar();
    return 0;
}

void viewUserProfile() {
    if (!currentUser) return;

    char roles[6][40] = {"Guest", "DIU Student", "Teacher", "Staff", "Skill Jobs Participant", "Scholarship Holder"};

    printf("\n===============================================================\n");
    printf("                     USER PROFILE DASHBOARD                    \n");
    printf("===============================================================\n");
    printf(" User Name   : %s\n", currentUser->name);
    printf(" User ID     : %d\n", currentUser->id);
    printf(" Department  : %s\n", currentUser->department);
    printf(" Profile Role: %s\n", roles[currentUser->role]);
    printf(" Security    : PIN Hashed via MD5 (%s)\n", currentUser->pinHash);
    printf("---------------------------------------------------------------\n");
    printf(" CURRENTLY BORROWED BOOKS:\n");

    int count = 0;
    for (int i = 0; i < MAX_BOOKS; i++) {
        if (library[i].issuedToUserId == currentUser->id && !library[i].isAvailable) {
            count++;
            printf("  - [%d] %s (Borrowed for: %d days | Renewals: %d/2)\n", 
                   library[i].id, library[i].title, library[i].daysBorrowed, library[i].renewalCount);
        }
    }
    if (count == 0) printf("  None\n");

    checkNotifications();
    printf("===============================================================\n");
}

void checkNotifications() {
    printf("\n [NOTIFICATION PANEL]:\n");
    int activeNotifs = 0;

    for (int i = 0; i < MAX_BOOKS; i++) {
        if (library[i].issuedToUserId == currentUser->id && !library[i].isAvailable) {
            if (library[i].daysBorrowed > BORROW_PERIOD_DAYS) {
                activeNotifs++;
                int overdue = library[i].daysBorrowed - BORROW_PERIOD_DAYS;
                printf("  ! [OVERDUE ALERT] Book '%s' (ID: %d) is overdue by %d days! Fine applies.\n",
                       library[i].title, library[i].id, overdue);
            }
        }
    }

    if (activeNotifs == 0) {
        printf("  No pending notifications or overdue warnings.\n");
    }
}

void mainMenu() {
    int choice;
    char roles[6][40] = {"Guest", "DIU Student", "Teacher", "Staff", "Skill Jobs Participant", "Scholarship Holder"};

    while (1) {
        clearTerminal();
        printf("\n===============================================================\n");
        printf("  DASHBOARD | Logged in as: %s (%s)\n", currentUser->name, roles[currentUser->role]);
        printf("===============================================================\n");
        printf("1. My User Profile & Notifications Panel\n");
        printf("2. DIU Library Resources (E-Library, Databases, DSpace...)\n");
        printf("3. DIU Library Services (Renew, Literacy Class, Turnitin...)\n");
        printf("4. View Activity Calendar\n");
        printf("5. Core Library Book Inventory (Search / Issue / Return)\n");

        if (currentUser->role == 4) {
            printf("6. [SPECIAL] Access Skill Jobs Books Portal\n");
        } else if (currentUser->role == 2) {
            printf("6. [SPECIAL] Search Academic Research Papers\n");
        } else if (currentUser->role == 5) {
            printf("6. [SPECIAL] Open IELTS Preparation Portal\n");
        } else if (currentUser->role == 3) {
            printf("6. [DIU STAFF] Calculate Overdue Fines\n");
        } else {
            printf("6. [SPECIAL] Read Live E-Newspapers\n");
        }

        printf("7. Switch User Account / Logout\n");
        printf("0. Exit System\n");
        printf("---------------------------------------------------------------\n");
        printf("Choose action: ");
        scanf("%d", &choice);

        if (choice == 1) {
            clearTerminal();
            viewUserProfile();
            printf("\nPress Enter to return to main menu...");
            getchar(); getchar();
        }
        else if (choice == 2) resourcesMenu();
        else if (choice == 3) servicesMenu();
        else if (choice == 4) {
            clearTerminal();
            activityCalendar();
            printf("\nPress Enter to return to main menu...");
            getchar(); getchar();
        }
        else if (choice == 5) coreLibraryOperations();
        else if (choice == 6) {
            clearTerminal();
            if (currentUser->role == 4) skillJobsBooks();
            else if (currentUser->role == 2) researchPapers();
            else if (currentUser->role == 5) ieltsPreparation();
            else if (currentUser->role == 3) calculateOverdueFines();
            else enewspaper();
            printf("\nPress Enter to return to main menu...");
            getchar(); getchar();
        } else if (choice == 7) {
            currentUser = NULL;
            if (userAuthenticationFlow()) continue;
            else break;
        } else if (choice == 0) {
            break;
        } else {
            printf("\nInvalid selection. Try again!\n");
        }
    }
}

void resourcesMenu() {
    int option;
    while (1) {
        clearTerminal();
        printf("\n--- DIU RESOURCES PORTAL ---\n");
        printf("1. Access E-Library (Trending Books Online)\n");
        printf("2. Browse Database A-Z\n");
        printf("3. Access Institutional Repository (DSpace)\n");
        printf("4. Access Voice Library\n");
        printf("5. Read E-Newspapers\n");
        printf("6. Configure Remote Access\n");
        printf("7. Return to Main Menu\n");
        printf("Select choice (1-7): ");
        scanf("%d", &option);

        if (option == 7) break;

        clearTerminal();
        switch (option) {
            case 1: elibrary(); break;
            case 2: databaseAZ(); break;
            case 3: instRepository(); break;
            case 4: voiceLibrary(); break;
            case 5: enewspaper(); break;
            case 6: remoteAccess(); break;
            default: printf("Invalid option!\n");
        }
        printf("\nPress Enter to continue...");
        getchar(); getchar();
    }
}

void servicesMenu() {
    int option;
    while (1) {
        clearTerminal();
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

        clearTerminal();
        switch (option) {
            case 1: itemRenew(); break;
            case 2: literacyClasses(); break;
            case 3: internshipPortal(); break;
            case 4: refManagement(); break;
            case 5: turnitinManual(); break;
            case 6: businessInABox(); break;
            default: printf("Invalid option!\n");
        }
        printf("\nPress Enter to continue...");
        getchar(); getchar();
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
}

void coreLibraryOperations() {
    int option;
    while (1) {
        clearTerminal();
        printf("\n--- CORE PHYSICAL BOOK TRANSACTIONS ---\n");
        printf("1. Search & View Book Catalog\n");
        printf("2. Borrow / Issue Book\n");
        printf("3. Return Borrowed Book\n");
        printf("4. Return to Main Menu\n");
        printf("Select choice (1-4): ");
        scanf("%d", &option);

        if (option == 4) break;

        clearTerminal();
        switch (option) {
            case 1: searchBook(); break;
            case 2: issueBook(); break;
            case 3: returnBook(); break;
            default: printf("Invalid choice!\n");
        }
        printf("\nPress Enter to continue...");
        getchar(); getchar();
    }
}

void elibrary() {
    printf("\n>>> DIU E-Library Portal <<<\n");
    printf("Reading digital repository metadata offline...\n");
    printf(" - C Programming Essentials (Digital Ed. 2024)\n");
    printf(" - Advanced System Architecture and Cloud Frameworks\n");
}

void databaseAZ() {
    printf("\n>>> Database A-Z Portal <<<\n");
    printf(" - [1] IEEE Xplore Digital Library\n");
    printf(" - [2] Scopus Citations Database\n");
    printf(" - [3] Emerald Insight Journals\n");
    printf(" - [4] SpringerLink Journals\n");
}

void instRepository() {
    printf("\n>>> DIU DSpace Institutional Repository <<<\n");
    printf("Recent Submissions:\n");
    printf(" - 'Machine Learning Models on Bengali OCR' (Dept. of CSE)\n");
    printf(" - 'Sustainable Fashion Supply Chain' (Textile Eng.)\n");
}

void voiceLibrary() {
    printf("\n>>> DIU ACCESSIBLE VOICE LIBRARY <<<\n");
    printf("Audio Streams Available:\n");
    printf(" 1. 'C Programming Audio Class' [Chapter 1-5]\n");
    printf(" 2. 'Business Ethics for Entrepreneurs' [Full Audiobook]\n");
}

void enewspaper() {
    FILE *fp = fopen("content_data2.txt", "r");
    if (!fp) {
        printf("\nError: Could not open content_data2.txt!\n");
        return;
    }

    char line[256];
    int printFlag = 0;
    printf("\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "[ENEWSPAPER]")) { printFlag = 1; continue; }
        if (printFlag && line[0] == '[') break;
        if (printFlag) printf("%s", line);
    }
    fclose(fp);
}

void remoteAccess() {
    char email[50];
    printf("\n>>> Remote Access Configuration <<<\n");
    printf("Enter your official @diu.edu.bd email: ");
    scanf("%s", email);
    if (strstr(email, "@diu.edu.bd") || strstr(email, "@daffodilvarsity.edu.bd")) {
        printf("Verification token issued to %s. Remote terminal linked!\n", email);
    } else {
        printf("Error: Access restricted to DIU domain emails!\n");
    }
}

void itemRenew() {
    int bookId, found = 0;
    printf("\n>>> Item Renew Service <<<\n");
    printf("Enter Book ID to renew: ");
    scanf("%d", &bookId);

    for (int i = 0; i < MAX_BOOKS; i++) {
        if (library[i].id == bookId) {
            found = 1;
            if (library[i].issuedToUserId == currentUser->id) {
                if (library[i].renewalCount < 2) {
                    library[i].renewalCount++;
                    library[i].daysBorrowed = 0;
                    printf("Success! Extended borrowing period for '%s'. Renewals used: %d/2.\n", 
                           library[i].title, library[i].renewalCount);
                } else {
                    printf("Renewal Denied! Maximum renewals reached.\n");
                }
            } else {
                printf("Error: This book is not issued under your User ID.\n");
            }
            break;
        }
    }
    if (!found) printf("Error: Book ID %d not found.\n", bookId);
}

void literacyClasses() {
    int option;
    printf("\n>>> Information Literacy & Research Seminars <<<\n");
    printf("1. Join 'Advanced Searching & Google Scholar Hacks'\n");
    printf("2. Join 'How to Format Academic Thesis'\n");
    printf("Select class (1-2) or 0 to cancel: ");
    scanf("%d", &option);
    if (option == 1 || option == 2) {
        registeredClasses++;
        printf("Registration successful! Class added to your profile records.\n");
    }
}

void internshipPortal() {
    printf("\n>>> Internship Portal & DSpace Upload Guidelines <<<\n");
    printf("1. PDF must contain your signature page.\n");
    printf("2. Keep file size under 15MB.\n");
    printf("3. Format naming: ID_Department_Semester.pdf\n");
}

void refManagement() {
    printf("\n>>> Reference Management Assistant <<<\n");
    printf("Mendeley: Use APA 7th edition as default citation format.\n");
    printf("Zotero: Sync library with Zotero Cloud for backup.\n");
}

void turnitinManual() {
    printf("\n>>> Turnitin Plagiarism Portal Guidelines <<<\n");
    printf("1. Allowed similarity index for DIU thesis: LESS than 20%%.\n");
    printf("2. Reach supervisor for submission credentials.\n");
}

void businessInABox() {
    printf("\n>>> DIU 'Business in a Box' Startup Hub <<<\n");
    printf("Includes Pitch Deck Templates and Trade License Registration guides.\n");
}

void searchBook() {
    printf("\n--- DIU Library Catalog ---\n");
    printf("%-8s %-40s %-20s %-12s %-10s\n", "ID", "Title", "Author", "Status", "Issued To");
    printf("-----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < MAX_BOOKS; i++) {
        printf("%-8d %-40s %-20s %-12s %-10d\n", 
               library[i].id, 
               library[i].title, 
               library[i].author, 
               library[i].isAvailable ? "Available" : "Issued Out",
               library[i].issuedToUserId);
    }
}

void issueBook() {
    int bookId, daysSim, found = 0;
    printf("\nEnter Book ID to Issue: ");
    scanf("%d", &bookId);

    for (int i = 0; i < MAX_BOOKS; i++) {
        if (library[i].id == bookId) {
            found = 1;
            if (library[i].isAvailable) {
                library[i].isAvailable = 0;
                library[i].renewalCount = 0;
                library[i].issuedToUserId = currentUser->id;

                printf("Simulate days already borrowed (e.g., 2 for normal, 10 for overdue testing): ");
                scanf("%d", &daysSim);
                library[i].daysBorrowed = daysSim;

                printf("Success! Book '%s' assigned to User ID %d.\n", library[i].title, currentUser->id);
            } else {
                printf("Sorry, this book is currently borrowed.\n");
            }
            break;
        }
    }
    if (!found) printf("Book ID %d not found.\n", bookId);
}

void returnBook() {
    int bookId, found = 0;
    printf("\nEnter Book ID to Return: ");
    scanf("%d", &bookId);

    for (int i = 0; i < MAX_BOOKS; i++) {
        if (library[i].id == bookId) {
            found = 1;
            if (!library[i].isAvailable) {
                library[i].isAvailable = 1;
                library[i].renewalCount = 0;
                library[i].issuedToUserId = 0;
                library[i].daysBorrowed = 0;
                printf("Success! '%s' has been returned.\n", library[i].title);
            } else {
                printf("This book is already sitting in the catalog.\n");
            }
            break;
        }
    }
    if (!found) printf("Book ID %d not found.\n", bookId);
}

void skillJobsBooks() {
    FILE *fp = fopen("content_data2.txt", "r");
    if (!fp) {
        printf("\nError: Could not open content_data2.txt!\n");
        return;
    }

    char line[256];
    int printFlag = 0;
    printf("\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "[SKILL_JOBS]")) { printFlag = 1; continue; }
        if (printFlag && line[0] == '[') break;
        if (printFlag) printf("%s", line);
    }
    fclose(fp);
}

void researchPapers() {
    FILE *fp = fopen("content_data2.txt", "r");
    if (!fp) {
        printf("\nError: Could not open content_data2.txt!\n");
        return;
    }

    char line[256];
    int printFlag = 0;
    printf("\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "[RESEARCH_PAPERS]")) { printFlag = 1; continue; }
        if (printFlag && line[0] == '[') break;
        if (printFlag) printf("%s", line);
    }
    fclose(fp);
}

void ieltsPreparation() {
    FILE *fp = fopen("content_data2.txt", "r");
    if (!fp) {
        printf("\nError: Could not open content_data2.txt!\n");
        return;
    }

    char line[256];
    int printFlag = 0;
    printf("\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "[IELTS_LESSON]")) { printFlag = 1; continue; }
        if (printFlag && line[0] == '[') break;
        if (printFlag) printf("%s", line);
    }
    fclose(fp);
}

void calculateOverdueFines() {
    int targetId, daysOverdue;
    float finePerDay = 10.0, totalFine = 0.0;

    printf("\n=== DIU STAFF - OVERDUE FINE CALCULATOR ===\n");
    printf("Enter Student ID: ");
    scanf("%d", &targetId);
    printf("Enter Total Days Overdue: ");
    scanf("%d", &daysOverdue);

    if (daysOverdue <= 0) {
        printf("\n[RECORD]: Student ID %d has NO overdue fine.\n", targetId);
    } else if (daysOverdue <= 7) {
        totalFine = daysOverdue * finePerDay;
        printf("\nStudent ID     : %d\nDays Overdue   : %d days\nTotal Fine Due : %.2f BDT\n", targetId, daysOverdue, totalFine);
    } else {
        totalFine = (7 * finePerDay) + ((daysOverdue - 7) * 20.0);
        printf("\nStudent ID     : %d\nDays Overdue   : %d days\nTotal Fine Due : %.2f BDT (Includes late penalty)\n", targetId, daysOverdue, totalFine);
    }
}