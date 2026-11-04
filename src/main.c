#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "client.h"
#include "server.h"


int main(int argc, char **argv)
{
    bool scan_flag = false;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-scan")==0) {
            scan_flag = true;
        }
    }

    if (scan_flag) {
        printf("scan\n");
        client();
    } else {
        printf("server\n");
        server();

    }



    return 0;
}
