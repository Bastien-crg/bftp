#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>



int scan_and_send() {

    inquiry_info *ii = NULL;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    const int dev_id = hci_get_route(NULL);
    const int sock = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

    const int len = 8;
    const int max_rsp = 255;
    const int flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

    const int num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (int i = 0; i < num_rsp; i++) {
        ba2str(&ii[i].bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &ii[i].bdaddr, sizeof(name), name, 0) < 0)
            strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
    }





    // connect to server
    int status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    // send a message
    if( status == 0 ) {
        status = write(sock, "hello!", 6);
    }

    if( status < 0 ) perror("uh oh");






    free( ii );
    close( sock );
    return 0;

}

int server() {

    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    close(client);
    close(s);
    return 0;
}






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
        scan_and_send();
    } else {
        printf("server\n");
    }



    return 0;
}
