#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define EMAIL_LEN 50
#define PASS_LEN 50
#define NAME_LEN 50
#define PHONE_LEN 20

char currentUser[EMAIL_LEN]; // To track who is logged in

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
} Contact;

void signup() {
    char email[EMAIL_LEN], password[PASS_LEN];
    printf("Enter new email: ");
    scanf("%s", email);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *f = fopen("users.txt", "a");
    if (f == NULL) {
        printf("Error opening users file.\n");
        return;
    }
    fprintf(f, "%s:%s\n", email, password);
    fclose(f);

    printf("Signup successful!\n");
}

int login() {
    char email[EMAIL_LEN], password[PASS_LEN], line[100];
    printf("Enter email: ");
    scanf("%s", email);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *f = fopen("users.txt", "r");
    if (f == NULL) {
        printf("No users found. Sign up first.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), f)) {
        char storedEmail[EMAIL_LEN], storedPass[PASS_LEN];
        sscanf(line, "%[^:]:%s", storedEmail, storedPass);
        if (strcmp(email, storedEmail) == 0 && strcmp(password, storedPass) == 0) {
            strcpy(currentUser, email);
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    printf("Login failed.\n");
    return 0;
}

void addContact() {
    Contact c;
    printf("Enter contact name: ");
    scanf("%s", c.name);
    printf("Enter phone number: ");
    scanf("%s", c.phone);

    char filename[100];
    snprintf(filename, sizeof(filename), "contacts_%s.txt", currentUser);

    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        printf("Could not open contact file.\n");
        return;
    }

    fprintf(f, "%s,%s\n", c.name, c.phone);
    fclose(f);
    printf("Contact added!\n");
}

void listContacts() {
    char filename[100];
    snprintf(filename, sizeof(filename), "contacts_%s.txt", currentUser);

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("No contacts found.\n");
        return;
    }

    printf("\n--- Your Contacts ---\n");
    char name[NAME_LEN], phone[PHONE_LEN];
    while (fscanf(f, "%[^,],%s\n", name, phone) == 2) {
        printf("%s - %s\n", name, phone);
    }
    fclose(f);
}

void contactMenu() {
    int choice;
    do {
        printf("\n1. Add Contact\n2. List Contacts\n3. Logout\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addContact(); break;
            case 2: listContacts(); break;
            case 3: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 3);
}

int main() {
    int choice;
    do {
        printf("\n--- Contact Book ---\n");
        printf("1. Sign Up\n2. Login\n3. Exit\nChoice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: signup(); break;
            case 2:
                if (login()) {
                    printf("Login successful!\n");
                    contactMenu();
                }
                break;
            case 3: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choice != 3);

    return 0;
}
