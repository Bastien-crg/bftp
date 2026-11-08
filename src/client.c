#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

int client() {

    inquiry_info *ii = NULL;
    char str_addr[19] = { 0 };
    char name[248] = { 0 };

    const int dev_id = hci_get_route(NULL);
    const int sock = hci_open_dev(dev_id);
    int s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    const int len = 8;
    const int max_rsp = 255;
    const int flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    const int num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) {
        perror("hci_inquiry");
    } else {
        printf("%d device found\n", num_rsp);
    }
    for (int i = 0; i < num_rsp; i++) {
        ba2str(&ii[i].bdaddr, str_addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &ii[i].bdaddr, sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");

        printf("%s  %s\n", str_addr, name);
        if (strcmp(name, "bastien-Vostro-3591") == 0) {
            printf("yp");
            break;
        }
    }


    struct sockaddr_rc addr = { 0 };
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( str_addr, &addr.rc_bdaddr );


    // connect to server
    int status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == -1 ) {
        status = write(sock, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");


    free( ii );
    close( sock );
    return 0;

}