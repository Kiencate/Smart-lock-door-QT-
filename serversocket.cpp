#include "serversocket.h"
using namespace std;
ServerSocket::ServerSocket()
{

}

void ServerSocket::run()
{

    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int socket_bluetooth, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    socket_bluetooth = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

//     bind socket to port 1 of the first available
//     local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    bdaddr_t addr={ };
    loc_addr.rc_bdaddr = addr;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(socket_bluetooth, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    qDebug()<<"bluetooth: server created";

    // put socket into listening mode
    qDebug()<<"bluetooth: server is listening";
    listen(socket_bluetooth, 1);

//    // accept one connection
    client = accept(socket_bluetooth, (struct sockaddr *)&rem_addr, &opt);
    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("bluetooth: received [%s]\n", buf);
    }
    std::string config_wifi = buf;
    QFile file("../wifi.json");

    while(!file.open(QIODevice::WriteOnly)){
    }
    QTextStream stream(&file);
    stream << QString::fromStdString(config_wifi) << endl;
    file.close();

    // close connection
    close(client);
    close(socket_bluetooth);
    emit Receive_wifi_success();
}
