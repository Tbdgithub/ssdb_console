#include <iostream>
#include <vector>
#include "client/SSDB_impl.h"


using namespace std;


int getline2(char s[], int lim);

int main(int argc, char **argv) {
    std::cout << "Hello, World ,good to see u!" << std::endl;

    printf("Usage: %s [host] [port]\n", argv[0]);
    const char *ip = (argc >= 2) ? argv[1] : "127.0.0.1";
    int port = (argc >= 3) ? atoi(argv[2]) : 8888;

    printf("host:%s,port:%d\n", ip, port);


    // connect to server
    ssdb::Client *client = ssdb::Client::connect(ip, port);

    if (client == NULL) {
        printf("fail to connect to server!\n");
        return 0;
    }


    printf("connected host:%s,port:%d\n", ip, port);

    printf("ssdb %s:%d>", ip, port);

    const int maxlen = 1000l;
    char line[maxlen];

    while (1) {


        int len = getline2(line, maxlen);


        char *pch = strtok(line, "\t \n");
        int word_count = 0;
        std::vector<std::string> list;

        string cmd;
        string param0;
        while (pch != NULL) {
            // printf("%s\n", pch);

            if (word_count == 0) {

                //  string temp = pch;

                cmd = pch;
                // cout << "temp:" << cmd << endl;
            }

            if (word_count == 1) {
                param0 = pch;
            }

            string temp(line);
            list.push_back(temp);
            ++word_count;
            pch = strtok(NULL, " \t\n");
        }
        // printf("other\n");
        //printf(cmd.c_str());
        // cout<<"cmd is:"<<cmd<<endl;

        //printf("cmd is %s",cmd.c_str());
        // printf("cmd is:%s \n", cmd.c_str() );
        // printf("param0 is:%s \n", param0.c_str() );


//        std::string key = "key";
        std::string val;

        if ("get" == cmd) {
            ssdb::Status s = client->get(param0, &val);
            //assert(s.ok() && (val == "val"));
            //printf("%s = %s\n", cmd.c_str(), val.c_str());

            if (s.ok()) {
                printf("%s\n", val.c_str());
            } else if (s.not_found()) {
                printf("%s\n", s.code().c_str());
            }


        } else if ("quit" == cmd) {
            delete client;
            printf("bye.\n");
            return 0;

        } else if (cmd.length() == 0) {
            //printf("ctrl +d");

            printf("\n");
            return 0;
        } else {
            printf("client_error: Unknown Command: %s\n", cmd.c_str());
        }

        printf("(%4.3f sec)\n", 0.015);
        printf("ssdb %s:%d>", ip, port);

    }


    return 0;
}


int getline2(char s[], int lim) {
    int c, i;

    for (i = 0; (i < lim - 1) && ((c = getchar()) != EOF) && (c != '\n'); ++i)
        s[i] = c;

    if (c == '\n') {
        s[i] = c;
        ++i;
    }

    s[i] = '\0';
    return i;
}
