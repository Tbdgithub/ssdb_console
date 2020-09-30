#include <iostream>
#include <vector>
#include <sstream>
#include "client/SSDB_impl.h"
#include "client/util/CmdLineParser.h"
#include "string.h"

using namespace std;


int getline2(char s[], int lim);

void printVector(vector<string> *vec);

int main(int argc, char **argv) {
    std::ostringstream oss;

    oss << "hi";

    cout << oss.str() << endl;

    oss.str("");
    cout << "after " << oss.str() << endl;

    oss << "second";

    cout << "after 2 " << oss.str() << endl;
    CmdLineParser clientHelper = CmdLineParser();


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

        string cmdline;

        getline(cin, cmdline);

        std::vector<std::string> paramList = *clientHelper.parse_cmd(cmdline);

        printVector(&paramList);
        string cmd = paramList[0];

        std::string val;

        if ("get" == cmd) {
            //check param size;
            string param0 = paramList[1];
            ssdb::Status s = client->get(param0, &val);
            if (s.ok()) {
                printf("%s\n", val.c_str());
            } else if (s.not_found()) {
                printf("%s\n", s.code().c_str());
            }

        } else if ("set" == cmd) {


            std::string key = paramList[1];
            std::string value = paramList[2];

            ssdb::Status s = client->set(key, value);
            if (s.ok()) {
                printf("%s\n", val.c_str());
            } else if (s.not_found()) {
                printf("%s\n", s.code().c_str());
            }

        } else if ("nlist" == cmd) {

            std::string start_temp = paramList[1];
            std::string end_temp = paramList[2];

            std::string start;
            std::string end;

            string::size_type loc1 = start_temp.find("\"", 0);

            if (loc1 != string::npos) {
                start = start_temp.substr(1, start_temp.size() - 2);
            }

            string::size_type loc2 = end_temp.find("\"", 0);

            if (loc2 != string::npos) {
                end = end_temp.substr(1, end_temp.size() - 2);
            }

            int limit = atoi(paramList[3].c_str());

            std::vector<std::string> list;

            ssdb::Status s = client->nlist(start, end, limit, &list);
            //ssdb::Status s = client->nlist("","", limit, &list);


            if (s.ok()) {


                // printf("%s\n", val.c_str());


                printVector(&list);
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

void printVector(vector<string> *ptr) {

    vector<string> obj = *ptr;

    cout << "size=" << obj.size() << endl;
    for (int i = 0; i < obj.size(); i++)//size()容器中实际数据个数
    {
        cout << "len=" << obj[i].length() << " val=" << obj[i] << " ";
    }
    cout << endl;
}

//
//        string cmd;
//        string param0;
//        while (pch != NULL) {
//
//            paramList.push_back(pch);
//            ++word_count;
//            pch = strtok(NULL, " \t\n");
//        }
//
//        if (paramList.size() > 0) {
//            cmd = paramList[0];
//        }

// string cmdline = str(line);

//cmdline=clientHelper.trim_line(cmdline);


//        char *pch = strtok(line, "\t \n");
//        int word_count = 0;
