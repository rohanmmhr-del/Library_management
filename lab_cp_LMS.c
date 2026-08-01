#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define MAX_BOOKS 5
#define MAX_USERS 50
#define BORROW_PERIOD_DAYS 7

void clearTerminal() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
// MD5 ALGORITHM :
typedef struct {
    uint32_t state[4];
    uint32_t count[2];
    uint8_t buffer[64];
} MD5_CTX;

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static void MD5Transform(uint32_t state[4], const uint8_t block[64]);
static void MD5Encode(uint8_t *output, const uint32_t *input, unsigned int len);
static void MD5Decode(uint32_t *output, const uint8_t *input, unsigned int len);

static uint8_t PADDING[64] = { 0x80 };

void MD5Init(MD5_CTX *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}
void MD5Update(MD5_CTX *context, const uint8_t *input, unsigned int inputLen) {
    unsigned int i, index, partLen;
    index = (unsigned int)((context->count[0] >> 3) & 0x3F);
    if ((context->count[0] += ((uint32_t)inputLen << 3)) < ((uint32_t)inputLen << 3))
        context->count[1]++;
    context->count[1] += ((uint32_t)inputLen >> 29);
    partLen = 64 - index;

    if (inputLen >= partLen) {
        memcpy(&context->buffer[index], input, partLen);
        MD5Transform(context->state, context->buffer);
        for (i = partLen; i + 63 < inputLen; i += 64)
            MD5Transform(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }
    memcpy(&context->buffer[index], &input[i], inputLen - i);
}
void MD5Final(uint8_t digest[16], MD5_CTX *context) {
    uint8_t bits[8];
    unsigned int index, padLen;
    MD5Encode(bits, context->count, 8);
    index = (unsigned int)((context->count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5Update(context, PADDING, padLen);
    MD5Update(context, bits, 8);
    MD5Encode(digest, context->state, 16);
    memset(context, 0, sizeof(*context));
}
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (uint32_t)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (uint32_t)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (uint32_t)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (uint32_t)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
}
static void MD5Transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    MD5Decode(x, block, 64);

    /* Round 1 */
    FF (a, b, c, d, x[ 0], S11, 0xd76aa478); FF (d, a, b, c, x[ 1], S12, 0xe8c7b756);
    FF (c, d, a, b, x[ 2], S13, 0x242070db); FF (b, c, d, a, x[ 3], S14, 0xc1bdceee);
    FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); FF (d, a, b, c, x[ 5], S12, 0x4787c62a);
    FF (c, d, a, b, x[ 6], S13, 0xa8304613); FF (b, c, d, a, x[ 7], S14, 0xfd469501);
    FF (a, b, c, d, x[ 8], S11, 0x698098d8); FF (d, a, b, c, x[ 9], S12, 0x8b44f7af);
    FF (c, d, a, b, x[10], S13, 0xffff5bb1); FF (b, c, d, a, x[11], S14, 0x895cd7be);
    FF (a, b, c, d, x[12], S11, 0x6b901122); FF (d, a, b, c, x[13], S12, 0xfd987193);
    FF (c, d, a, b, x[14], S13, 0xa679438e); FF (b, c, d, a, x[15], S14, 0x49b40821);

    /* Round 2 */
    GG (a, b, c, d, x[ 1], S21, 0xf61e2562); GG (d, a, b, c, x[ 6], S22, 0xc040b340);
    GG (c, d, a, b, x[11], S23, 0x265e5a51); GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa);
    GG (a, b, c, d, x[ 5], S21, 0xd62f105d); GG (d, a, b, c, x[10], S22,  0x2441453);
    GG (c, d, a, b, x[15], S23, 0xd8a1e681); GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8);
    GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); GG (d, a, b, c, x[14], S22, 0xc33707d6);
    GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); GG (b, c, d, a, x[ 8], S24, 0x455a14ed);
    GG (a, b, c, d, x[13], S21, 0xa9e3e905); GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8);
    GG (c, d, a, b, x[ 7], S23, 0x676f02d9); GG (b, c, d, a, x[12], S24, 0x8d2a4c8a);

    /* Round 3 */
    HH (a, b, c, d, x[ 5], S31, 0xfffa3942); HH (d, a, b, c, x[ 8], S32, 0x8771f681);
    HH (c, d, a, b, x[11], S33, 0x6d9d6122); HH (b, c, d, a, x[14], S34, 0xfde5380c);
    HH (a, b, c, d, x[ 1], S31, 0xa4beea44); HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9);
    HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); HH (b, c, d, a, x[10], S34, 0xbebfbc70);
    HH (a, b, c, d, x[13], S31, 0x289b7ec6); HH (d, a, b, c, x[0], S32, 0xeaa127fa);
    HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); HH (b, c, d, a, x[ 6], S34,  0x4881d05);
    HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); HH (d, a, b, c, x[12], S32, 0xe6db99e5);
    HH (c, d, a, b, x[15], S33, 0x1fa27cf8); HH (b, c, d, a, x[ 2], S34, 0xc4ac5665);

    /* Round 4 */
    II (a, b, c, d, x[ 0], S41, 0xf4292244); II (d, a, b, c, x[ 7], S42, 0x432aff97);
    II (c, d, a, b, x[14], S43, 0xab9423a7); II (b, c, d, a, x[ 5], S44, 0xfc93a039);
    II (a, b, c, d, x[12], S41, 0x655b59c3); II (d, a, b, c, x[ 3], S42, 0x8f0ccc92);
    II (c, d, a, b, x[10], S43, 0xffeff47d); II (b, c, d, a, x[ 1], S44, 0x85845dd1);
    II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); II (d, a, b, c, x[15], S42, 0xfe2ce6e0);
    II (c, d, a, b, x[ 6], S43, 0xa3014314); II (b, c, d, a, x[13], S44, 0x4e0811a1);
    II (a, b, c, d, x[ 4], S41, 0xf7537e82); II (d, a, b, c, x[11], S42, 0xbd3af235);
    II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); II (b, c, d, a, x[ 9], S44, 0xeb86d391);

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    memset(x, 0, sizeof(x));
}

static void MD5Encode(uint8_t *output, const uint32_t *input, unsigned int len) {
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (uint8_t)(input[i] & 0xff);
        output[j+1] = (uint8_t)((input[i] >> 8) & 0xff);
        output[j+2] = (uint8_t)((input[i] >> 16) & 0xff);
        output[j+3] = (uint8_t)((input[i] >> 24) & 0xff);
    }
}

static void MD5Decode(uint32_t *output, const uint8_t *input, unsigned int len) {
    unsigned int i, j;
    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = ((uint32_t)input[j]) | (((uint32_t)input[j+1]) << 8) |
                    (((uint32_t)input[j+2]) << 16) | (((uint32_t)input[j+3]) << 24);
}

void hashPassword(const char *plainPin, char *outputHex) {
    MD5_CTX ctx;
    uint8_t digest[16];
    MD5Init(&ctx);
    MD5Update(&ctx, (uint8_t*)plainPin, strlen(plainPin));
    MD5Final(digest, &ctx);

    for (int i = 0; i < 16; i++) {
        sprintf(outputHex + (i * 2), "%02x", digest[i]);
    }
    outputHex[32] = '\0';
}
typedef struct {
    int id;
    char title[100];
    char author[50];
    int isAvailable;
    int renewalCount;
    int issuedToUserId;
    int daysBorrowed; 
} Book;

typedef struct {
    int id;
    char name[100];
    char department[50];
    int role; 
    char pinHash[33]; 
} UserProfile;

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

void initializeFiles();
void loadUsersFromFile();
void saveUserToFile(UserProfile u);
void displayHeader();
int userAuthenticationFlow();
void registerNewUser();
int loginUser();
void viewUserProfile();
void checkNotifications();

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

void mainMenu();
void resourcesMenu();
void servicesMenu();
void activityCalendar();
void coreLibraryOperations();

int main() {
    initializeFiles();
    loadUsersFromFile();
    
    if (userAuthenticationFlow()) {
        mainMenu();
    }
    
    printf("\nExiting program. Thank you for using DIU Library System!\n");
    return 0;
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
    FILE *fp = fopen("content_data.txt", "r");
    if (!fp) return;

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
                    library[i].daysBorrowed = 0; // Reset borrow tracker on renew
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
    FILE *fp = fopen("content_data.txt", "r");
    if (!fp) return;

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
    FILE *fp = fopen("content_data.txt", "r");
    if (!fp) return;

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
    FILE *fp = fopen("content_data.txt", "r");
    if (!fp) return;

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