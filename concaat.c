#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main() {
    sqlite3 *db;
    char *errMsg = 0;
    char sql[256];
    char name[100];
    int n;

    if (sqlite3_open("example.db", &db) != SQLITE_OK) {
        printf("Cannot open database!\n");
        return 1;
    }

    sqlite3_exec(db,
        "CREATE TABLE IF NOT EXISTS students("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT);",
        callback, 0, &errMsg);

    sqlite3_exec(db,
        "DELETE FROM students;",
        callback, 0, &errMsg);

    printf("Enter number of students: ");
    scanf("%d", &n);
    getchar(); // Consume newline left by scanf

    for (int i = 0; i < n; i++) {
        printf("Enter name of student %d: ", i + 1);
        fgets(name, sizeof(name), stdin);

        // Remove newline character
        name[strcspn(name, "\n")] = '\0';

        sprintf(sql,
                "INSERT INTO students(name) VALUES('%s');",
                name);

        sqlite3_exec(db, sql, callback, 0, &errMsg);
    }

    printf("\nNames stored in database:\n");

    sqlite3_exec(db,
        "SELECT * FROM students;",
        callback, 0, &errMsg);

    printf("Result of GROUP_CONCAT():\n");

    sqlite3_exec(db,
        "SELECT GROUP_CONCAT(name, ', ') AS All_Names FROM students;",
        callback, 0, &errMsg);

    sqlite3_close(db);

    return 0;
}