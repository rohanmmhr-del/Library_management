#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "md5.h"

#define MAX_BOOKS 5
#define MAX_USERS 50
#define BORROW_PERIOD_DAYS 7

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

extern Book library[MAX_BOOKS];
extern UserProfile users[MAX_USERS];
extern int userCount;
extern UserProfile *currentUser;
extern int registeredClasses;

void clearTerminal();

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

#endif