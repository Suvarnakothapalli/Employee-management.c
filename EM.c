#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    char department[30];
    float salary;
} Employee;

void addEmployee(FILE *fp) {
    Employee emp;
    printf("Enter ID: "); scanf("%d", &emp.id);
    printf("Enter Name: "); scanf(" %[^\n]", emp.name);
    printf("Enter Department: "); scanf(" %[^\n]", emp.department);
    printf("Enter Salary: "); scanf("%f", &emp.salary);

    fwrite(&emp, sizeof(Employee), 1, fp);
    printf("✅ Employee added successfully.\n");
}

void displayEmployees(FILE *fp) {
    Employee emp;
    rewind(fp);
    printf("\n--- Employee List ---\n");
    while (fread(&emp, sizeof(Employee), 1, fp)) {
        printf("ID: %d | Name: %s | Dept: %s | Salary: ₹%.2f\n",
               emp.id, emp.name, emp.department, emp.salary);
    }
}
void searchEmployee(FILE *fp, int id) {
    Employee emp;
    rewind(fp);
    while (fread(&emp, sizeof(Employee), 1, fp)) {
        if (emp.id == id) {
            printf("🔍 Found: %s, Dept: %s, Salary: ₹%.2f\n", emp.name, emp.department, emp.salary);
            return;
        }
    }
    printf("❌ Employee with ID %d not found.\n", id);
}

void updateEmployee(FILE *fp, int id) {
    Employee emp;
    rewind(fp);
    long pos;
    while (fread(&emp, sizeof(Employee), 1, fp)) {
        if (emp.id == id) {
            pos = ftell(fp) - sizeof(Employee);
            printf("Enter new Name: "); scanf(" %[^\n]", emp.name);
            printf("Enter new Department: "); scanf(" %[^\n]", emp.department);
            printf("Enter new Salary: "); scanf("%f", &emp.salary);
            fseek(fp, pos, SEEK_SET);
            fwrite(&emp, sizeof(Employee), 1, fp);
            printf("🔄 Employee updated.\n");
            return;
        }
    }
    printf("❌ Employee not found.\n");
}
void deleteEmployee(FILE *fp, int id) {
    FILE *temp = fopen("temp.dat", "wb");
    Employee emp;
    int found = 0;

    rewind(fp);
    while (fread(&emp, sizeof(Employee), 1, fp)) {
        if (emp.id != id) {
            fwrite(&emp, sizeof(Employee), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("employees.dat");
    rename("temp.dat", "employees.dat");
    fp = fopen("employees.dat", "rb+");

    if (found)
        printf("🗑️ Employee deleted.\n");
    else
        printf("❌ Employee not found.\n");
}

int main(){
     FILE *fp = fopen("employees.dat", "ab+");
    if (!fp) {
        printf("⚠️ Unable to open file.\n");
        return 1;
    }

    int choice, id;
    while (1) {
        printf("\n=== Employee Management System ===\n");
        printf("1. Add Employee\n2. Display All\n3. Search by ID\n4. Update\n5. Delete\n6. Exit\n");
        printf("Enter choice: "); scanf("%d", &choice);

        switch (choice) {
            case 1: addEmployee(fp); break;
            case 2: displayEmployees(fp); break;
            case 3: printf("Enter ID to search: "); scanf("%d", &id); searchEmployee(fp, id); break;
            case 4: printf("Enter ID to update: "); scanf("%d", &id); updateEmployee(fp, id); break;
            case 5: printf("Enter ID to delete: "); scanf("%d", &id); deleteEmployee(fp, id); break;
            case 6: fclose(fp); printf("👋 Goodbye!\n"); exit(0);
            default: printf("❌ Invalid choice.\n");
        }
    }
}

