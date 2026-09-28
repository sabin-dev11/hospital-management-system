// Hospital Management System in C
// Author: Sabin Tamang

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "hospital_records.dat"

// Structure for patient details
struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
};

// Function declarations
void addPatient();
void viewPatients();
void searchPatient();
void updatePatient();
void deletePatient();
int isUniqueID(int id);
int getValidatedID();

int main() {
    int choice;

    while (1) {
        printf("\n==== Hospital Management System ====\n");
        printf("1. Add Patient Record\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient by ID\n");
        printf("4. Update Patient Record\n");
        printf("5. Delete Patient Record\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }

        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// Function to validate ID (integer only)
int getValidatedID() {
    int id;
    char c;
    if (scanf("%d", &id) != 1) {
        printf("Error: ID must be an integer!\n");
        while ((c = getchar()) != '\n' && c != EOF); // clear invalid input
        return -1; // invalid
    }
    return id;
}

// Function to check if ID is unique
int isUniqueID(int id) {
    FILE *fp;
    struct Patient p;
    fp = fopen(FILE_NAME, "rb");
    if (!fp) return 1; // file not found, so unique by default

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id == id) {
            fclose(fp);
            return 0; // not unique
        }
    }
    fclose(fp);
    return 1; // unique
}

// Function to add new patient
void addPatient() {
    FILE *fp;
    struct Patient p;
    int id;

    fp = fopen(FILE_NAME, "ab+");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Patient ID : ");
    id = getValidatedID();
    if (id == -1) return; // invalid input

    if (!isUniqueID(id)) {
        printf("Error: Patient ID already exists! Please try again.\n");
        fclose(fp);
        return;
    }

    p.id = id;
    printf("Enter Name: ");
    scanf("%s", p.name);
    printf("Enter Age: ");
    scanf("%d", &p.age);
    printf("Enter Disease: ");
    scanf("%s", p.disease);

    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);

    printf("Patient added successfully!\n");
}

// Function to view all patients
void viewPatients() {
    FILE *fp;
    struct Patient p;

    fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    printf("\n--- Patient Records ---\n");
    while (fread(&p, sizeof(p), 1, fp)) {
        printf("ID: %d | Name: %s | Age: %d | Disease: %s\n",
               p.id, p.name, p.age, p.disease);
    }
    fclose(fp);
}   

// Function to search patient by ID
void searchPatient() {
    FILE *fp;
    struct Patient p;
    int id, found = 0;

    fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Patient ID to search: ");
    id = getValidatedID();
    if (id == -1) return;

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id == id) {
            printf("Record Found: ID: %d | Name: %s | Age: %d | Disease: %s\n",
                   p.id, p.name, p.age, p.disease);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Patient with ID %d not found.\n", id);
    }
    fclose(fp);
}

// Function to update patient record
void updatePatient() {
    FILE *fp, *temp;
    struct Patient p;
    int id, found = 0;

    fp = fopen(FILE_NAME, "rb");
    temp = fopen("temp.dat", "wb");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Patient ID to update: ");
    id = getValidatedID();
    if (id == -1) return;

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id == id) {
            printf("Enter New Name: ");
            scanf("%s", p.name);
            printf("Enter New Age: ");
            scanf("%d", &p.age);
            printf("Enter New Disease: ");
            scanf("%s", p.disease);
            found = 1;
        }
        fwrite(&p, sizeof(p), 1, temp);
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Patient record updated successfully!\n");
    else
        printf("Patient with ID %d not found.\n", id);
}

// Function to delete patient record
void deletePatient() {
    FILE *fp, *temp;
    struct Patient p;
    int id, found = 0;

    fp = fopen(FILE_NAME, "rb");
    temp = fopen("temp.dat", "wb");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Patient ID to delete: ");
    id = getValidatedID();
    if (id == -1) return;

    while (fread(&p, sizeof(p), 1, fp)) {
        if (p.id != id) {
            fwrite(&p, sizeof(p), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Patient record deleted successfully!\n");
    else
        printf("Patient with ID %d not found.\n", id);
}
