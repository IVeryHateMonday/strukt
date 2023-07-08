#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int age;
    float salary;
} Employee;

void createFile(char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("File creation error!\n");
        return;
    }

    printf("Enter employee information (id, name, age, salary) or enter -1 to finish:\n");
    Employee emp;
    while (1) {
        scanf("%d", &emp.id);
        if (emp.id == -1) {
            break;
        }

        scanf("%s %d %f", emp.name, &emp.age, &emp.salary);
        fwrite(&emp, sizeof(Employee), 1, file);
    }

    fclose(file);
}

void outputAll(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Employee emp;
    printf("ID\tName\tAge\tSalary\n");
    while (fread(&emp, sizeof(Employee), 1, file) == 1) {
        printf("%d\t%s\t%d\t%.2f\n", emp.id, emp.name, emp.age, emp.salary);
    }

    fclose(file);
}

void searchByKey(char* filename, float key) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    Employee emp;
    int found = 0;
    while (fread(&emp, sizeof(Employee), 1, file) == 1) {
        if (emp.salary == key) {
            printf("%d\t%s\t%d\t%.2f\n", emp.id, emp.name, emp.age, emp.salary);
            found = 1;
        }
    }

    if (!found) {
        printf("No salaried employees found%.2f\n", key);
    }

    fclose(file);
}

void addInfo(char* filename) {
    FILE* file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter employee information (id, name, age, salary):\n");
    Employee emp;
    scanf("%d %s %d %f", &emp.id, emp.name, &emp.age, &emp.salary);
    fwrite(&emp, sizeof(Employee), 1, file);

    fclose(file);
}

void deleteInfo(char* filename, float key) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    FILE* temp = fopen("temp.dat", "wb");
    if (temp == NULL) {
        printf("Error creating temporary file!\n");
        fclose(file);
        return;
    }

    Employee emp;
    int deleted = 0;
    while (fread(&emp, sizeof(Employee), 1, file) == 1) {
        if (emp.salary != key) {
            fwrite(&emp, sizeof(Employee), 1, temp);
        } else {
            deleted = 1;
        }
    }

    fclose(file);
    fclose(temp);

    if (!deleted) {
        printf("No salaried employees found%.2f\n", key);
        remove("temp.dat");
    } else {
        remove(filename);
        rename("temp.dat", filename);
        printf("Information about employees with a salary of %.2f has been successfully deleted!\n", key);
    }
}

int main() {
    char filename[100];
    printf("Enter a file name: ");
    scanf("%s", filename);

    int choice;
    float key;
    do {
        printf("\nMenu:\n");
        printf("1. Creating a file and entering information\n");
        printf("2. Output of all information\n");
        printf("3. Search for information by key field\n");
        printf("4. Adding information to a file\n");
        printf("5. Deleting information from a file\n");
        printf("6. Exit the program\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createFile(filename);
                break;
            case 2:outputAll(filename);
                break;
            case 3:
                printf("Enter salary to search: ");
                scanf("%f", &key);
                searchByKey(filename, key);
                break;
            case 4:
                addInfo(filename);
                break;
            case 5:
                printf("Enter the salary to delete: ");
                scanf("%f", &key);
                deleteInfo(filename, key);
                break;
            case 6:
                printf("Exiting the program...\n");
                break;
        }
    }while(choice!=6);
}
