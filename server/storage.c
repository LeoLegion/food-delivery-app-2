#include <stdio.h>
#include "storage.h"

void save_user(User *u) {
    FILE *fp = fopen("data/users.dat", "ab");
    fwrite(u, sizeof(User), 1, fp);
    fclose(fp);
}