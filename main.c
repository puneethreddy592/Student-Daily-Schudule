#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_ITEMS_PER_MEAL 10
#define MAX_SUBJECT_LENGTH 50   
#define MAX_SUBJECTS_PER_DAY 11
#define CLASS_IN_WEEK 5

// Define ANSI color codes
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"  // Reset to default color

typedef struct  {
    char breakfast[7][10][50];   // Breakfast menu for each day
    char lunch[7][10][50];       // Lunch menu for each day
    char snacks[7][10][50];      // Snacks menu for each day
    char dinner[7][10][50];      // Dinner menu for each day
} Menu;
typedef struct  {
    char food[7][40][50];   // Subjects for each time slot
} MenuF;
typedef struct  {
    char breakfast[10][50];   // Breakfast menu for each day
    char lunch[10][50];       // Lunch menu for each day
    char snacks[10][50];       // Snacks menu for each day
    char dinner[10][50];      // Dinner menu for each day
} DailyMenu;

struct Admin {
    char name[25];
    char password[64];
};

typedef struct  {
    char subject[CLASS_IN_WEEK][MAX_SUBJECTS_PER_DAY][MAX_SUBJECT_LENGTH];   // Subjects for each time slot
} Timetable;

typedef struct Student {
    char name[50];
    char username[50];
    char rollNumber[10];
    int year;
    char gender[10];
    char course[50];
    char hostel[50];
    int roomNumber;
    int group;
    char password[64];
}Student;

bool addStudent();//payal
Student getStudentData();//payal
void displayStudentData(const char *username);//hemanth
bool addAdmin();//hemanth
bool verifyAdmin();//hemanth
void editTimetable(char* filename);//raqeeb
void inputTimetable(char* filename);//raqeeb
void printTimetable(Timetable timetable, char* day);
void readFileAdmin(char* filename, char* day);
void printTimetableStudent(Timetable timetable, char* day);//deva
void readFileStudent(char* filename, char* day);//deva
void inputMenu(const char *filename);//shobhit
void editMenu(char* filename);//puneeth
DailyMenu readMenu(char* filename, char* day);//shobhit
void printMenuAdmin(DailyMenu menu, char* day);//sanoj
void printCurrentMenu(DailyMenu menu);//aryan
void getCurrentTime(char *timeString);//puneeth
void getCurrentDate(char *dateString);//puneeth
void getCurrentDay(char *dayString);//puneeth
char* encrypt(const char *plaintext);//sanoj

bool addStudent() {
    FILE *file = fopen("studentdata.csv", "a");
    if (file == NULL) {
        printf("Error opening file for appending.\n");
        return false;
    }

    struct Student student;
    printf("Enter student details:\n");
    printf("Name: ");
    scanf("%s", student.name);
    printf("Username: ");
    scanf("%s", student.username);
    printf("Roll Number: ");
    scanf("%s", student.rollNumber);
    printf("Year: ");
    scanf("%d", &student.year);
    printf("Gender: ");
    scanf("%s", student.gender);
    printf("Course: ");
    scanf("%s", student.course);
    printf("Hostel: ");
    scanf("%s", student.hostel);
    printf("Room Number: ");
    scanf("%d", &student.roomNumber);
     printf("Group Number: ");
    scanf("%d", &student.group);
    printf("Password: ");
    scanf("%s", student.password);

    fprintf(file, "%s,%s,%s,%d,%s,%s,%s,%d,%d,%s\n", student.name, student.username, student.rollNumber,
            student.year, student.gender, student.course, student.hostel, student.roomNumber, student.group, encrypt(student.password));

    fclose(file);

    file = fopen("studentlogindata.csv", "a");
    if (file == NULL) {
        printf("Error opening file for appending.\n");
        return false;
    }

    fprintf(file, "%s,%s\n", student.username,  encrypt(student.password));

    fclose(file);

    return true;
}

Student getStudentData() {
    Student student; // Declare a Student struct to store the data
    FILE *file = fopen("studentdata.csv", "r"); // Open file for reading

    if (file == NULL) {
        printf("Error opening file for reading.\n");
        exit(1); // Exit program if file cannot be opened
    }

    char username[50], password[64],ppassword[64];
    printf("Enter student username: ");
    scanf("%s", username);
    printf("Enter student password: ");
    scanf("%s", ppassword);
    strcpy(password, encrypt(ppassword));

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[50], storedPassword[64];
        if (sscanf(line, "%*[^,],%49[^,],%*[^,],%*d,%*[^,],%*[^,],%*[^,],%*d,%*d,%63s", storedUsername, storedPassword) != 2) {
            continue; // Skip if unable to read username and password
        }
        if (strcmp(storedUsername, username) == 0 && strcmp(storedPassword, password) == 0) {
            // Found the student and verified the password, populate the Student struct
            sscanf(line, "%49[^,],%*[^,],%9[^,],%d,%9[^,],%49[^,],%49[^,],%d,%d,%*s", 
                   student.name, student.rollNumber, &student.year, 
                   student.gender, student.course, student.hostel, 
                   &student.roomNumber, &student.group);
            strcpy(student.username, username);
            strcpy(student.password, password);
            fclose(file);
            return student;
        }
    }

    fclose(file);
    // If student not found or password incorrect, initialize all fields to empty/zero
    strcpy(student.name, "");
    strcpy(student.username, "");
    strcpy(student.rollNumber, "");
    student.year = 0;
    strcpy(student.gender, "");
    strcpy(student.course, "");
    strcpy(student.hostel, "");
    student.roomNumber = 0;
    student.group = 0;
    strcpy(student.password, "");
    return student;
}

// Function to display student data
void displayStudentData(const char *username) {
    // Open file for reading
    FILE *file = fopen("studentdata.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    // Search for the student with the given username
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char storedUsername[50];
        if (sscanf(line, "%*[^,],%49[^,],%*[^,],%*d,%*[^,],%*[^,],%*[^,],%*d,%*d,%*s", storedUsername) != 1) {
            continue; // Skip if unable to read username
        }
        if (strcmp(storedUsername, username) == 0) {
            // Found the student, print details
            char name[50], rollNumber[10], gender[10], course[50], hostel[50];
            int year, roomNumber,group;
            sscanf(line, "%49[^,],%*[^,],%9[^,],%d,%9[^,],%49[^,],%49[^,],%d,%d,%*s", name, rollNumber, &year, gender, course, hostel, &roomNumber,&group);
            printf("Name: %s\n", name);
            printf("Roll Number: %s\n", rollNumber);
            printf("Year: %d\n", year);
            printf("Gender: %s\n", gender);
            printf("Course: %s\n", course);
            printf("Hostel: %s\n", hostel);
            printf("Room Number: %d\n", roomNumber);
            printf("Group Number: %d\n", group);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Student with username %s not found.\n", username);
}

// Function to add a new admin and append admin data to the file
bool addAdmin() {
    // Open file for reading
    FILE *file = fopen("admindata.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return false;
    }

    // Input admin name
    char name[25];
    printf("Enter admin name:\n");
    if (scanf("%s", name) != 1) {
        printf("Invalid input.\n");
        fclose(file);
        return false;
    }

    // Check if username already exists
    char existingName[25], password[64];
    while (fscanf(file, "%[^,],%s\n", existingName, password) != EOF) {
        if (strcmp(existingName, name) == 0) {
            printf("Username already exists. Please choose a different username:\n");
            fclose(file);
            return false;
        }
    }

    fclose(file);

    // Open file for appending
    file = fopen("admindata.csv", "a");
    if (file == NULL) {
        printf("Error opening file for appending.\n");
        return false;
    }

    // Input admin password
    char passwordInput[64],ppassword[64];
    printf("Enter admin password:\n");
    if (scanf("%s", ppassword) != 1) {
        printf("Invalid input.\n");
        fclose(file);
        return false;
    }
    strcpy(passwordInput, encrypt(ppassword));
    // Append admin data to file
    fprintf(file, "%s,%s\n", name, passwordInput);

    fclose(file);

    return true;
}

// Function to verify admin credentials
bool verifyAdmin() {
    // Open file for reading
    FILE *file = fopen("admindata.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return false;
    }

    // Input admin name
    char name[25];
    printf("Enter admin name:\n");
    if (scanf("%s", name) != 1) {
        printf("Invalid input.\n");
        fclose(file);
        return false;
    }

    // Check if username exists
    char existingName[25], password[64];
    bool isAdmin = false;
    while (fscanf(file, "%[^,],%s\n", existingName, password) != EOF) {
        if (strcmp(existingName, name) == 0) {
            isAdmin = true;
            break;
        }
    }

    fclose(file);

    if (!isAdmin) {
        printf("Admin username not found. Please provide a valid username.\n");
        return false;
    }

    // Input admin password
    char passwordInput[64],ppassword[64];

    printf("Enter admin password:\n");
    if (scanf("%s", ppassword) != 1) {
        printf("Invalid input.\n");
        return false;
    }
    strcpy(passwordInput, encrypt(ppassword));

    // Verify password
    if (strcmp(password, passwordInput) == 0) {
        return true;
    } else {
        return false;
    }
}

void editTimetable(char* filename) {
    FILE *file = fopen(filename, "r+"); // Open file for reading and writing
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Initialize the timetable struct
    Timetable timetable;

    // Determine which day to edit
    char day[10];
    printf("Enter the day you want to edit (e.g., Monday, Tuesday, etc.): ");
    scanf("%s", day);

    // Convert the input day to lowercase
    for (int i = 0; day[i]; i++) {
        day[i] = tolower(day[i]);
    }
    
    int line_to_edit = -1;
    if (strcmp(day, "monday") == 0)
        line_to_edit = 0;
    else if (strcmp(day, "tuesday") == 0)
        line_to_edit = 1;
    else if (strcmp(day, "wednesday") == 0)
        line_to_edit = 2;
    else if (strcmp(day, "thursday") == 0)
        line_to_edit = 3;
    else if (strcmp(day, "friday") == 0)
        line_to_edit = 4;
    else {
        printf("Invalid day.\n");
        fclose(file);
        return;
    }

    // Read existing timetable from file
    for (int line = 0; line < 5; line++) {
        for (int i = 0; i < 11; i++) {
            fscanf(file, "%[^,],", timetable.subject[line][i]);
        }
        fscanf(file, "\n"); // Consume newline
    }

    // Take input for the line to edit
    printf("Enter 11 subjects for %s:\n", day);
    for (int i = 0; i < 11; i++) {
        scanf("%s", timetable.subject[line_to_edit][i]);
    }

    // Close the file
    fclose(file);
    file = fopen(filename, "w");


    // Rewrite the entire timetable to the file with the updated line
    for (int line = 0; line < 5; line++) {
        for (int i = 0; i < 11; i++) {
            fprintf(file, "%s,", timetable.subject[line][i]);
        }
        fprintf(file, "\n");// Newline after each line
    }

    // Close the file
    fclose(file);

    printf("Timetable for %s updated.\n", day);
}

// Function to input timetable line by line and write to file
void inputTimetable(char* filename) {
    FILE *file = fopen(filename, "w"); // Open file for writing
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Initialize the timetable struct
    Timetable timetable;

    // Take input for 5 lines of 11 subjects each
    for (int line = 0; line < 5; line++) {
        printf("Enter 11 subjects for line %d:\n", line + 1);
        for (int i = 0; i < 11; i++) {
            scanf("%s", timetable.subject[line][i]);
        }
    }

    // Write subjects to the file
    for (int line = 0; line < 5; line++) {
        for (int i = 0; i < 11; i++) {
            fprintf(file, "%s,", timetable.subject[line][i]);
        }
        fprintf(file, "\n"); // Newline after each line
    }

    // Close the file
    fclose(file);

    printf("Timetable written to file.\n");
}

void printTimetable(Timetable timetable, char* day) {
    // Convert the input day to lowercase
    for (int i = 0; day[i]; i++) {
        day[i] = tolower(day[i]);
    }

    int line_to_print = -1;
    if (strcmp(day, "monday") == 0)
        line_to_print = 0;
    else if (strcmp(day, "tuesday") == 0)
        line_to_print = 1;
    else if (strcmp(day, "wednesday") == 0)
        line_to_print = 2;
    else if (strcmp(day, "thursday") == 0)
        line_to_print = 3;
    else if (strcmp(day, "friday") == 0)
        line_to_print = 4;
    else if (strcmp(day, "saturday") == 0)
        line_to_print = 5;
    else if (strcmp(day, "sunday") == 0)
        line_to_print = 6;
    else {
        printf("Invalid day.\n");
        return;
    }

    // Print the subjects for the specified day in a box-like format
    printf("+--------------+----------------------------------+\n");
    printf("|    Time      |               Subject            |\n");
    printf("+--------------+----------------------------------+\n");
    for (int i = 0; i < MAX_SUBJECTS_PER_DAY; i++) {
        printf("| %02d:00:00     | %-32s |\n", i + 8, timetable.subject[line_to_print][i]);
        printf("+--------------+----------------------------------+\n");
    }
   
}

// Function to read timetable for a specific day from file 
void readFileAdmin(char* filename, char* day) {
    FILE *file = fopen(filename, "r");
    Timetable timetable;
    memset(timetable.subject, 0, sizeof(timetable.subject)); // Initialize all subjects to empty strings

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Convert the input day to lowercase
    for (int i = 0; day[i]; i++) {
        day[i] = tolower(day[i]);
    }

    int line_to_read = -1;
    if (strcmp(day, "monday") == 0)
        line_to_read = 0;
    else if (strcmp(day, "tuesday") == 0)
        line_to_read = 1;
    else if (strcmp(day, "wednesday") == 0)
        line_to_read = 2;
    else if (strcmp(day, "thursday") == 0)
        line_to_read = 3;
    else if (strcmp(day, "friday") == 0)
        line_to_read = 4;
    else if (strcmp(day, "saturday") == 0)
        line_to_read = 5;
    else if (strcmp(day, "sunday") == 0)
        line_to_read = 6;
    else {
        printf("Invalid day.\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_SET);

    // Skip lines until the one to read
    for (int line = 0; line < line_to_read; line++) {
        for (int i = 0; i < 11; i++) {
            fscanf(file, "%*[^,],");
        }
        fscanf(file, "\n"); // Consume newline
    }

    // Read subjects for the specified day
    for (int i = 0; i < 11; i++) {
        fscanf(file, "%[^,],", timetable.subject[line_to_read][i]);
    }

    fclose(file);

    // Print the timetable
    printTimetable(timetable, day);
}

void printTimetableStudent(Timetable timetable, char* day) {
    // Convert the input day to lowercase
    for (int i = 0; day[i]; i++) {
        day[i] = tolower(day[i]);
    }

    int line_to_print = -1;
    if (strcmp(day, "monday") == 0)
        line_to_print = 0;
    else if (strcmp(day, "tuesday") == 0)
        line_to_print = 1;
    else if (strcmp(day, "wednesday") == 0)
        line_to_print = 2;
    else if (strcmp(day, "thursday") == 0)
        line_to_print = 3;
    else if (strcmp(day, "friday") == 0)
        line_to_print = 4;
    else if (strcmp(day, "saturday") == 0)
        line_to_print = 5;
    else if (strcmp(day, "sunday") == 0)
        line_to_print = 6;
    else {
        printf("Invalid day.\n");
        return;
    }

    // Get current time
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    int currentHour = localTime->tm_hour;

    // Print the timetable in box-like format
    printf("Timetable for %s (from %02d:00:00):\n", day, currentHour);
    printf("+-----------+----------------------------------+\n");
    printf("|   Time    |               Class              |\n");
    printf("+-----------+----------------------------------+\n");
    for (int i = currentHour - 8; i < MAX_SUBJECTS_PER_DAY; i++) {
        if (i < 0) continue; // Skip if before 8:00:00
        if (strcmp("null", timetable.subject[line_to_print][i]) == 0) continue; // Skip if subject is empty
        printf("| %02d:00:00  | %-32s |\n", i + 8, timetable.subject[line_to_print][i]);
        printf("+-----------+----------------------------------+\n");
    }
    
}
// Function to read timetable for a specific day from file 
void readFileStudent(char* filename, char* day) {
    FILE *file = fopen(filename, "r");
    Timetable timetable;
    memset(timetable.subject, 0, sizeof(timetable.subject)); // Initialize all subjects to empty strings

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Convert the input day to lowercase
    for (int i = 0; day[i]; i++) {
        day[i] = tolower(day[i]);
    }

    int line_to_read = -1;
    if (strcmp(day, "monday") == 0)
        line_to_read = 0;
    else if (strcmp(day, "tuesday") == 0)
        line_to_read = 1;
    else if (strcmp(day, "wednesday") == 0)
        line_to_read = 2;
    else if (strcmp(day, "thursday") == 0)
        line_to_read = 3;
    else if (strcmp(day, "friday") == 0)
        line_to_read = 4;
    else if (strcmp(day, "saturday") == 0)
        line_to_read = 5;
    else if (strcmp(day, "sunday") == 0)
        line_to_read = 6;
    else {
        printf("Invalid day.\n");
        fclose(file);
        return;
    }

    fseek(file, 0, SEEK_SET);

    // Skip lines until the one to read
    for (int line = 0; line < line_to_read; line++) {
        for (int i = 0; i < 11; i++) {
            fscanf(file, "%*[^,],");
        }
        fscanf(file, "\n"); // Consume newline
    }

    // Read subjects for the specified day
    for (int i = 0; i < 11; i++) {
        fscanf(file, "%[^,],", timetable.subject[line_to_read][i]);
    }

    fclose(file);

    // Print the timetable
    printTimetableStudent(timetable, day);
}

void inputMenu(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    MenuF menu;
    char buffer[10000]; // Increased buffer size to accommodate 40 items per day

    for (int day = 0; day < 7; day++) {
        printf("Day %d\n", day + 1);
        for (int i = 0; i < 4; i++) {
            const char *meal;
            switch (i) {
                case 0:
                    meal = "breakfast";
                    break;
                case 1:
                    meal = "lunch";
                    break;
                case 2:
                    meal = "snacks";
                    break;
                case 3:
                    meal = "dinner";
                    break;
                default:
                    meal = "";
                    break;
            }
            printf("%s\n", meal);
            for (int j = 0; j < 10; j++) {
                printf("Enter %s %d: ", meal, j + 1);
                fgets(menu.food[day][i * 10 + j], sizeof(menu.food[day][i * 10 + j]), stdin);
                // Remove trailing newline character if present
                menu.food[day][i * 10 + j][strcspn(menu.food[day][i * 10 + j], "\n")] = '\0';
            }
        }
        // Write the day's menu to the file
        sprintf(buffer, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
                menu.food[day][0], menu.food[day][1], menu.food[day][2], menu.food[day][3], menu.food[day][4], 
                menu.food[day][5], menu.food[day][6], menu.food[day][7], menu.food[day][8], menu.food[day][9],
                menu.food[day][10], menu.food[day][11], menu.food[day][12], menu.food[day][13], menu.food[day][14], 
                menu.food[day][15], menu.food[day][16], menu.food[day][17], menu.food[day][18], menu.food[day][19],
                menu.food[day][20], menu.food[day][21], menu.food[day][22], menu.food[day][23], menu.food[day][24], 
                menu.food[day][25], menu.food[day][26], menu.food[day][27], menu.food[day][28], menu.food[day][29],
                menu.food[day][30], menu.food[day][31], menu.food[day][32], menu.food[day][33], menu.food[day][34], 
                menu.food[day][35], menu.food[day][36], menu.food[day][37], menu.food[day][38], menu.food[day][39]);
        fputs(buffer, file);
    }

    fclose(file);
    printf("Menu has been saved to %s.\n", filename);
}

void editMenu(char* filename) {
    FILE *file = fopen(filename, "r"); // Open file for reading and writing
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Initialize the timetable struct
    Menu menu;

    // Determine which day to edit
    char day[10];
    printf("Enter the day you want to edit (e.g., Monday, Tuesday, etc.): ");
    scanf("%s", day);

    int line_to_edit = -1;
    if (strcmp(day, "Monday") == 0)
        line_to_edit = 0;
    else if (strcmp(day, "Tuesday") == 0)
        line_to_edit = 1;
    else if (strcmp(day, "Wednesday") == 0)
        line_to_edit = 2;
    else if (strcmp(day, "Thursday") == 0)
        line_to_edit = 3;
    else if (strcmp(day, "Friday") == 0)
        line_to_edit = 4;
    else if (strcmp(day, "Saturday") == 0)
        line_to_edit = 5;
    else if (strcmp(day, "Sumday") == 0)
        line_to_edit = 6;
    else {
        printf("Invalid day.\n");
        fclose(file);
        return;
    }

    // Read existing timetable from file
    for (int line = 0; line < 28; line++) {
        for (int i = 0; i < 10; i++) {
            if(line%4==0){
                fscanf(file, "%[^,],", menu.breakfast[line/4][i]);
            }
            if(line%4==1){
                fscanf(file, "%[^,],", menu.lunch[line/4][i]);
            }
            if(line%4==2){
                fscanf(file, "%[^,],", menu.snacks[line/4][i]);
            }
            if(line%4==3){
                fscanf(file, "%[^,],", menu.dinner[line/4][i]);
            }
        }
        fscanf(file, "\n"); // Consume newline
    }
    printf("Enter the breakfast items for %s:\n", day);
    for (int i = 0; i < 10; i++) {
        scanf("%s", menu.breakfast[line_to_edit][i]);
    }
    printf("Enter the lunch items for %s:\n", day);
    for (int i = 0; i < 10; i++) {
        scanf("%s", menu.lunch[line_to_edit][i]);
    }
    printf("Enter the snacks items for %s:\n", day);
    for (int i = 0; i < 10; i++) {
        scanf("%s", menu.snacks[line_to_edit][i]);
    }
    printf("Enter the dinner items for %s:\n", day);
    for (int i = 0; i < 10; i++) {
        scanf("%s", menu.dinner[line_to_edit][i]);
    }
    fclose(file);
    file = fopen(filename, "w");
    char buffer[10000];
    // Rewrite the entire timetable to the file with the updated line
    for (int day = 0; day < 7; day++) {
        // Write the day's menu to the file
        sprintf(buffer, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", 
                menu.breakfast[day][0], menu.breakfast[day][1], menu.breakfast[day][2], menu.breakfast[day][3], menu.breakfast[day][4], 
                menu.breakfast[day][5], menu.breakfast[day][6], menu.breakfast[day][7], menu.breakfast[day][8], menu.breakfast[day][9],
                menu.lunch[day][0], menu.lunch[day][1], menu.lunch[day][2], menu.lunch[day][3], menu.lunch[day][4], 
                menu.lunch[day][5], menu.lunch[day][6], menu.lunch[day][7], menu.lunch[day][8], menu.lunch[day][9],
                menu.snacks[day][0], menu.snacks[day][1], menu.snacks[day][2], menu.snacks[day][3], menu.snacks[day][4], 
                menu.snacks[day][5], menu.snacks[day][6], menu.snacks[day][7], menu.snacks[day][8], menu.snacks[day][9],
                menu.dinner[day][0], menu.dinner[day][1], menu.dinner[day][2], menu.dinner[day][3], menu.dinner[day][4], 
                menu.dinner[day][5], menu.dinner[day][6], menu.dinner[day][7], menu.dinner[day][8], menu.dinner[day][9]);
        fputs(buffer, file);
    }

    // Close the file
    fclose(file);

    printf("Menu for %s updated.\n", day);
}

void printCurrentMenu(DailyMenu menu) {
    // Get the current time
    char currentTime[9];
    getCurrentTime(currentTime);

    // Determine the meal based on the current time
    const char* meal;
    int currentHour = atoi(currentTime);
    if (currentHour < 10) {
        meal = "Breakfast";
    } else if (currentHour < 14) {
        meal = "Lunch";
    } else if (currentHour < 18 || (currentHour == 18 && atoi(currentTime + 3) < 30)) {
        meal = "Snacks";
    } else if (currentHour < 22 || (currentHour == 22 && atoi(currentTime + 3) < 0)) {
        meal = "Dinner";
    } else {
        printf("No menu available at this time.\n");
        return;
    }

    // Print the ongoing meal
    printf(BLUE "Ongoing:\n");
    printf("%s%s%s (Time: %s)\n", RED, meal, RESET, currentTime);
    char (*menuItems)[50]; // Pointer to array of chars
    int itemsPerMeal;

    if (strcmp(meal, "Breakfast") == 0) {
        menuItems = menu.breakfast;
        itemsPerMeal = MAX_ITEMS_PER_MEAL;
    } else if (strcmp(meal, "Lunch") == 0) {
        menuItems = menu.lunch;
        itemsPerMeal = MAX_ITEMS_PER_MEAL;
    } else if (strcmp(meal, "Snacks") == 0) {
        menuItems = menu.snacks;
        itemsPerMeal = MAX_ITEMS_PER_MEAL;
    } else if (strcmp(meal, "Dinner") == 0) {
        menuItems = menu.dinner;
        itemsPerMeal = MAX_ITEMS_PER_MEAL;
    }

    if (menuItems != NULL) {
        for (int i = 0; i < itemsPerMeal; i++) {
            if (strcmp(menuItems[i], "null") != 0 && strlen(menuItems[i]) > 0) {
                printf("%s%s%s\n", BLUE, menuItems[i], RESET);
            }
        }
    }

    // Print the upcoming meals and their items
    printf(GREEN "\nUpcoming:\n");
    if (strcmp(meal, "Breakfast") == 0) {
        printf("%s%s%s (12:30:00 to 14:00:00)\n", RED, "Lunch", RESET);
        for (int i = 0; i < MAX_ITEMS_PER_MEAL; i++) {
            if (strlen(menu.lunch[i]) > 0 && strcmp(menu.lunch[i], "null") != 0) {
                printf("%s%s%s\n", GREEN, menu.lunch[i], RESET);
            } else {
                break;
            }
        }
        printf("%s%s%s (17:15:00 to 18:30:00)\n", RED, "Snacks", RESET);
        for (int i = 0; i < MAX_ITEMS_PER_MEAL; i++) {
            if (strlen(menu.snacks[i]) > 0 && strcmp(menu.snacks[i], "null") != 0) {
                printf("%s%s%s\n", GREEN, menu.snacks[i], RESET);
            } else {
                break;
            }
        }
        printf("%s%s%s (20:00:00 to 22:00:00)\n", RED, "Dinner", RESET);
        for (int i = 0; i < MAX_ITEMS_PER_MEAL; i++) {
            if (strlen(menu.dinner[i]) > 0 && strcmp(menu.dinner[i], "null") != 0) {
                printf("%s%s%s\n", GREEN, menu.dinner[i], RESET);
            } else {
                break;
            }
        }
    } else if (strcmp(meal, "Lunch") == 0) {
        printf("%s%s%s (17:15:00 to 18:30:00)\n", RED, "Snacks", RESET);
        for (int i = 0; i < MAX_ITEMS_PER_MEAL; i++) {
            if (strlen(menu.snacks[i]) > 0 && strcmp(menu.snacks[i], "null") != 0) {
                printf("%s%s%s\n", GREEN, menu.snacks[i], RESET);
            } else {
                break;
            }
        }
        printf("%s%s%s (20:00:00 to 22:00:00)\n", RED, "Dinner", RESET);
        for (int i = 0; i < MAX_ITEMS_PER_MEAL; i++) {
            if (strlen(menu.dinner[i]) > 0 && strcmp(menu.dinner[i], "null") != 0) {
                printf("%s%s%s\n", GREEN, menu.dinner[i], RESET);
            } else {
                break;
            }
        }
    } else if (strcmp(meal, "Snacks") == 0) {
        printf("%s%s%s (20:00:00 to 22:00:00)\n", RED, "Dinner", RESET);
        for (int i = 0; i < MAX_ITEMS_PER_MEAL; i++) {
            if (strlen(menu.dinner[i]) > 0 && strcmp(menu.dinner[i], "null") != 0) {
                printf("%s%s%s\n", GREEN, menu.dinner[i], RESET);
            } else {
                break;
            }
        }
    } else if (strcmp(meal, "Dinner") == 0) {
        printf("%s%s%s (07:30:00 to 10:00:00)\n", RED, "Breakfast", RESET);
        for (int i = 0; i < MAX_ITEMS_PER_MEAL; i++) {
            if (strlen(menu.breakfast[i]) > 0 && strcmp(menu.breakfast[i], "null") != 0) {
                printf("%s%s%s\n", GREEN, menu.breakfast[i], RESET);
            } else {
                break;
            }
        }
    }
}

DailyMenu readMenu(char* filename, char* day) {
    FILE *file = fopen(filename, "r");
    DailyMenu menu;
    if (file == NULL) {
        printf("Error opening file.\n");
        return menu; // Return an empty menu
    }

    // Convert the input day to lowercase
    for (int i = 0; day[i]; i++) {
        day[i] = tolower(day[i]);
    }

    int line_to_read = -1;
    if (strcmp(day, "monday") == 0)
        line_to_read = 0;
    else if (strcmp(day, "tuesday") == 0)
        line_to_read = 1;
    else if (strcmp(day, "wednesday") == 0)
        line_to_read = 2;
    else if (strcmp(day, "thursday") == 0)
        line_to_read = 3;
    else if (strcmp(day, "friday") == 0)
        line_to_read = 4;
    else if (strcmp(day, "saturday") == 0)
        line_to_read = 5;
    else if (strcmp(day, "sunday") == 0)
        line_to_read = 6;
    else {
        printf("Invalid day.\n");
        fclose(file);
        return menu; // Return an empty menu
    }

    // Read and discard header line
    char header[1024];
    fgets(header, sizeof(header), file);

    // Read the specified line
    char line[1024];
    for (int i = 0; i <= line_to_read; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            printf("Day not found.\n");
            fclose(file);
            return menu; // Return an empty menu
        }
    }

    // Parse the line and store it in the DailyMenu structure
    char *token = strtok(line, ",");
    strcpy(menu.breakfast[0], token);
    for (int i = 1; i < 10; i++) {
        token = strtok(NULL, ",");
        strcpy(menu.breakfast[i], token);
    }

    for (int i = 0; i < 10; i++) {
        token = strtok(NULL, ",");
        strcpy(menu.lunch[i], token);
    }

    for (int i = 0; i < 10; i++) {
        token = strtok(NULL, ",");
        strcpy(menu.snacks[i], token);
    }

    for (int i = 0; i < 10; i++) {
        token = strtok(NULL, ",");
        strcpy(menu.dinner[i], token);
    }

    // Close the file
    fclose(file);

    return menu;
}

void printMenuAdmin(DailyMenu menu, char* day){
    // Print the menu
    printf("%sMenu for %s:%s\n", RED, day, RESET);
    printf("%sBreakfast:%s ", RED, RESET);
    for (int i = 0; i < 10; i++) {
        if (strcmp(menu.breakfast[i], "null") == 0) 
            continue ;
        else {
            printf("%s%s%s ", GREEN, menu.breakfast[i], RESET);
        }
    }
    printf("\n%sLunch:%s ", RED, RESET);
    for (int i = 0; i < 10; i++) {
        if (strcmp(menu.lunch[i], "null") == 0) 
        continue;
        else {
            printf("%s%s%s ", GREEN, menu.lunch[i], RESET);
        }
    }
    printf("\n%sSnacks:%s ", RED, RESET);
    for (int i = 0; i < 10; i++) {
        if (strcmp(menu.snacks[i], "null") == 0) 
        continue;
        else {
            printf("%s%s%s ", GREEN, menu.snacks[i], RESET);
        }
    }
    printf("\n%sDinner:%s ", RED, RESET);
    for (int i = 0; i < 10; i++) {
        if (strcmp(menu.dinner[i], "null") == 0) 
        continue;
        else {
            printf("%s%s%s ", GREEN, menu.dinner[i], RESET);
        }
    }
    printf("\n");
}

// Function to get current time as a string
void getCurrentTime(char *timeString) {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    strftime(timeString, 9, "%H:%M:%S", localTime);
}

// Function to get current date as a string in the format "dd-mm-yyyy"
void getCurrentDate(char *dateString) {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    strftime(dateString, 11, "%d-%m-%Y", localTime);
}

// Function to get current day of the week as a string
void getCurrentDay(char *dayString) {
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    switch(localTime->tm_wday) {
        case 0:
            strcpy(dayString, "Sunday");
            break;
        case 1:
            strcpy(dayString, "Monday");
            break;
        case 2:
            strcpy(dayString, "Tuesday");
            break;
        case 3:
            strcpy(dayString, "Wednesday");
            break;
        case 4:
            strcpy(dayString, "Thursday");
            break;
        case 5:
            strcpy(dayString, "Friday");
            break;
        case 6:
            strcpy(dayString, "Saturday");
            break;
        default:
            strcpy(dayString, "Invalid Day");
    }
}

int main() {
    printf("//WELCOME TO THE LOGIN PAGE//\n");
    char c,n1;
    int n,group,admingroup;
    char a,day[10];
    Student student;
    printf("Do you want to login as an Admin?(y/n)\n");
    scanf(" %c",&c);
    if(c=='y') {
        printf("Do you have an existing account?(y/n)\n");
        scanf(" %c",&a);
        if(a=='y') {
            bool real;
            real=verifyAdmin();
            while(!real) {
                int choice;
                printf("Options:\n 1.Try Again\n 2.Register\n");
                scanf("%d",&choice);
                if(choice==1) {
                    real=verifyAdmin();
                }
                else if(choice==2){
                    addAdmin();
                    real=true;
                }
            }
            }
        else addAdmin();
        printf("Now What do you want to do?\n");
        char ch='y';
        while(ch=='y') {
            printf("Choose any:\n");
            printf("1.Add the timetable for students.\n");
            printf("2.Edit the timetable.\n");
            printf("3.Read the Timetable.\n");
            printf("4.Add the menu.\n");
            printf("5.Edit the menu.\n"); 
            printf("6.Read the menu.\n");
            printf("7.EXIT\n");
            bool t=true;
            while(t){
            scanf(" %c",&n1);
            if(!(n1>='1' && n1<='7')) {
                printf("Not a vaild choice. Try Again\n");
            }
            else t=false;
            }
            n=n1-'0';
            switch(n) {
                case 1: {
                        printf("For which group you want to add the timetable?\n");
                        scanf("%d",&admingroup);
                        char filename[10];
                        switch (admingroup) {
                            case 1:
                            case 2:
                                strcpy(filename, "g1.csv");
                                break;
                            case 3:
                            case 4:
                                strcpy(filename, "g3.csv");
                                break;
                            case 5:
                            case 6:
                                strcpy(filename, "g5.csv");
                                break;
                            case 7:
                            case 8:
                                strcpy(filename, "g7.csv");
                                break;
                            case 9:
                            case 10:
                                strcpy(filename, "g9.csv");
                                break;
                            case 11:
                            case 12:
                                strcpy(filename, "g11.csv");
                                break;
                            case 13:
                            case 14:
                                strcpy(filename, "g13.csv");
                                break;
                            case 15:
                            case 16:
                                strcpy(filename, "g15.csv");
                                break;
                            case 17:
                            case 18:
                                strcpy(filename, "g17.csv");
                                break;
                            case 19:
                            case 20:
                                strcpy(filename, "g19.csv");
                                break;
                            default:
                                printf("Invalid group number.\n");
                                break;
                        }
                        inputTimetable(filename);
                        break;}
                case 2: {printf("On which day you want to edit the timetable?\n");
                        scanf("%s",day);
                        printf("For which group do you want to edit the timetable?\n");
                        scanf("%d", &admingroup);
                        char filename[10];
                        switch (admingroup) {
                            case 1:
                            case 2:
                                strcpy(filename, "g1.csv");
                                break;
                            case 3:
                            case 4:
                                strcpy(filename, "g3.csv");
                                break;
                            case 5:
                            case 6:
                                strcpy(filename, "g5.csv");
                                break;
                            case 7:
                            case 8:
                                strcpy(filename, "g7.csv");
                                break;
                            case 9:
                            case 10:
                                strcpy(filename, "g9.csv");
                                break;
                            case 11:
                            case 12:
                                strcpy(filename, "g11.csv");
                                break;
                            case 13:
                            case 14:
                                strcpy(filename, "g13.csv");
                                break;
                            case 15:
                            case 16:
                                strcpy(filename, "g15.csv");
                                break;
                            case 17:
                            case 18:
                                strcpy(filename, "g17.csv");
                                break;
                            case 19:
                            case 20:
                                strcpy(filename, "g19.csv");
                                break;
                            default:
                                printf("Invalid group number.\n");
                                break;
                        }
                        editTimetable(filename);
                        break;
                        }
                case 3: {printf("On which day you want to read the timetable?\n");
                        scanf("%s",day);
                        printf("For which group do you want to read the timetable?\n");
                        scanf("%d", &admingroup);
                        char filename[10];
                        switch (admingroup) {
                            case 1:
                            case 2:
                                strcpy(filename, "g1.csv");
                                break;
                            case 3:
                            case 4:
                                strcpy(filename, "g3.csv");
                                break;
                            case 5:
                            case 6:
                                strcpy(filename, "g5.csv");
                                break;
                            case 7:
                            case 8:
                                strcpy(filename, "g7.csv");
                                break;
                            case 9:
                            case 10:
                                strcpy(filename, "g9.csv");
                                break;
                            case 11:
                            case 12:
                                strcpy(filename, "g11.csv");
                                break;
                            case 13:
                            case 14:
                                strcpy(filename, "g13.csv");
                                break;
                            case 15:
                            case 16:
                                strcpy(filename, "g15.csv");
                                break;
                            case 17:
                            case 18:
                                strcpy(filename, "g17.csv");
                                break;
                            case 19:
                            case 20:
                                strcpy(filename, "g19.csv");
                                break;
                            default:
                                printf("Invalid group number.\n");
                                break;
                        }

                        readFileAdmin(filename,day);
                        break;
                        }
                case 4: inputMenu("cvramanmenu.csv");
                        break;
                case 5: editMenu("cvramanmenu.csv");
                        break;
                case 6: {printf("On which day you want to read the menu?\n");
                        scanf("%s",day);
                        DailyMenu menu;
                        menu=readMenu("cvramanmenu.csv",day);
                        printMenuAdmin(menu,day);
                        break;}
                case 7: printf("THANK YOU FOR VISITING\n");
                        exit(0);
                default: {printf("INVALID INPUT\n");
                           continue; }   
            }
        }
    }
        else {
            printf("Okay Then you are a student.\n");
            printf("So,Do you have an existing account?(y/n)\n");
            scanf(" %c",&a);
            bool realstudent=1;
            if(a=='y') {
                student=getStudentData();
                group = student.group;
                while (student.year == 0) {
                 int choice;
                printf("Wrong credentials entered. Press\n 1.Try Again\n 2.Register:\n");
                scanf("%d", &choice);
                if (choice == 1) {
                    student = getStudentData();
                    group = student.group;
                } 
                else if (choice == 2) {
                    addStudent();
                     printf("Enter your credentials.\n");
                student=getStudentData();
                group = student.group;
                while (student.year == 0) {
                 int choice;
                printf("Wrong credentials entered. Press\n 1.Try Again\n 2.Register:\n");
                scanf("%d", &choice);
                if (choice == 1) {
                    student = getStudentData();
                    group = student.group;
                } 
                else if (choice == 2) {
                    addStudent();
                    break; // Exit the loop after registration
                        }
                }

                        }
                }
            }
            else {
                addStudent();
                printf("Enter your credentials.\n");
                student=getStudentData();
                group = student.group;
                while (student.year == 0) {
                 int choice;
                printf("Wrong credentials entered. Press\n 1.Try Again\n 2.Register:\n");
                scanf("%d", &choice);
                if (choice == 1) {
                    student = getStudentData();
                    group = student.group;
                } 
                else if (choice == 2) {
                    addStudent();
                    break; // Exit the loop after registration
                        }
                }
            }
            printf("Now What do you want to do?\n");
            char ch='y';
            while(ch=='y') {
                printf("Choose any:\n");
                printf("1.Read the Timetable.\n");
                printf("2.Read the menu.\n");
                printf("3.Read your details.\n");
                printf("4.EXIT\n");
                bool t=true;
                while(t){
                    scanf(" %c",&n1);
                    if(!(n1>='1' && n1<='4')) {
                        printf("Not a vaild choice. Try Again\n");
                    }
                    else t=false;
                    }
                n=n1-'0';
                switch(n) {
                    case 1: {
                        getCurrentDay(day);
                       char filename[10];
                        switch (group) {
                            case 1:
                            case 2:
                                strcpy(filename, "g1.csv");
                                break;
                            case 3:
                            case 4:
                                strcpy(filename, "g3.csv");
                                break;
                            case 5:
                            case 6:
                                strcpy(filename, "g5.csv");
                                break;
                            case 7:
                            case 8:
                                strcpy(filename, "g7.csv");
                                break;
                            case 9:
                            case 10:
                                strcpy(filename, "g9.csv");
                                break;
                            case 11:
                            case 12:
                                strcpy(filename, "g11.csv");
                                break;
                            case 13:
                            case 14:
                                strcpy(filename, "g13.csv");
                                break;
                            case 15:
                            case 16:
                                strcpy(filename, "g15.csv");
                                break;
                            case 17:
                            case 18:
                                strcpy(filename, "g17.csv");
                                break;
                            case 19:
                            case 20:
                                strcpy(filename, "g19.csv");
                                break;
                            default:
                                printf("Invalid group number.\n");
                                break;
                        }
                        readFileStudent(filename,day);
                        break;
                    }
                    case 2: {
                            char currentDay[10];
                            getCurrentDay(currentDay);

                            // Convert the input day to lowercase
                            for (int i = 0; currentDay[i]; i++) {
                            currentDay[i] = tolower(currentDay[i]);
                            }


                            DailyMenu currentMenu = readMenu("cvramanmenu.csv", currentDay);

                            // Print the current menu based on the time of day
                            printCurrentMenu(currentMenu);
                            break;}
                    case 3: {displayStudentData(student.username);
                            break;}
                    case 4: {printf("THANK YOU FOR VISITING\n");
                            exit(0);}
                    default: {printf("INVALID INPUT\n");
                            continue; }   
                }
            }
        }
    return 0;
}

char* encrypt(const char *plaintext) {
    int length = strlen(plaintext);
    char* ciphertext = (char*)malloc(length + 1); // Allocate memory for ciphertext

    if (ciphertext == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int i;

    for (i = 0; i < length; i++) {
        char current_char = plaintext[i];

        if (isalpha(current_char)) {
            // Encrypt alphabetic characters based on position
            if (current_char >= 'a' && current_char <= 'e') {
                ciphertext[i] = current_char + 1;
            } else if (current_char >= 'A' && current_char <= 'E') {
                ciphertext[i] = current_char + 6;
            } else if (current_char >= 'f' && current_char <= 'j') {
                ciphertext[i] = current_char + 2;
            } else if (current_char >= 'F' && current_char <= 'J') {
                ciphertext[i] = current_char + 7;
            } else if (current_char >= 'k' && current_char <= 'o') {
                ciphertext[i] = current_char + 3;
            } else if (current_char >= 'K' && current_char <= 'O') {
                ciphertext[i] = current_char + 8;
            } else if (current_char >= 'p' && current_char <= 't') {
                ciphertext[i] = current_char + 4;
            } else if (current_char >= 'P' && current_char <= 'T') {
                ciphertext[i] = current_char + 9;
            } else if (current_char >= 'u' && current_char <= 'z') {
                ciphertext[i] = current_char + 5;
            } else if (current_char >= 'U' && current_char <= 'Z') {
                ciphertext[i] = current_char + 10;
            }
        } else if (isdigit(current_char)) {
            // Encrypt digits by taking mod 10
            int digit = current_char - '0';
            int encrypted_digit = (digit + 5) % 10;
            ciphertext[i] = encrypted_digit + '0';
        } else {
            // Keep non-alphanumeric characters unchanged
            ciphertext[i] = current_char;
        }
    }

    ciphertext[length] = '\0'; // Null-terminate the output string
    return ciphertext;
}