#include <iostream>
#include <vector>
#include <sstream>
#include "client/SSDB_impl.h"
#include "client/util/CmdLineParser.h"
#include "string.h"

using namespace std;


int getline2(char s[], int lim);

void printVector(vector<string> *vec);

void nextRound(const char *ip, int port);

void nscan(ssdb::Client *client, const vector<std::string> &paramList);

void ncmget(ssdb::Client *client, const vector<std::string> &paramList);

void ncmset(ssdb::Client *client, const vector<std::string> &paramList);

void nlist(ssdb::Client *client, const vector<std::string> &paramList);

void set(ssdb::Client *client, const vector<std::string> &paramList, const string &val);

int main(int argc, char **argv) {

    CmdLineParser clientHelper = CmdLineParser();

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

    while (1) {


        try {
            string cmdline;

            getline(cin, cmdline);

            std::vector<std::string> paramList = *clientHelper.parse_cmd(cmdline);

            if (paramList.size() == 0) {
                printf("wrong number of arguments\n");
                nextRound(ip, port);
                continue;
            }

            string cmd = paramList[0];

            if (cmd.empty()) {
                printf("wrong number of arguments\n");
                nextRound(ip, port);
                continue;
            }

            std::string val;

            if ("get" == cmd) {
                //check param size;

                if (paramList.size() != 2) {
                    printf("wrong number of arguments\n");
                    nextRound(ip, port);
                    continue;
                }

                string param0 = paramList[1];
                ssdb::Status s = client->get(param0, &val);
                if (s.ok()) {
                    printf("%s\n", val.c_str());
                } else if (s.not_found()) {
                    printf("%s\n", s.code().c_str());
                }

            } else if ("set" == cmd) {

                if (paramList.size() != 3) {
                    printf("wrong number of arguments\n");
                    nextRound(ip, port);
                    continue;
                }

                set(client, paramList, val);

            } else if ("nlist" == cmd) {
                if (paramList.size() != 4) {
                    printf("wrong number of arguments\n");
                    nextRound(ip, port);
                    continue;
                }

                nlist(client, paramList);

            } else if ("nscan" == cmd) {
                if (paramList.size() != 6) {
                    printf("wrong number of arguments\n");
                    nextRound(ip, port);
                    continue;
                }

                nscan(client, paramList);

            } else if ("ncmget" == cmd) {
                if (paramList.size() != 4) {
                    printf("wrong number of arguments\n");
                    nextRound(ip, port);
                    continue;
                }

                ncmget(client, paramList);

            } else if ("ncmset" == cmd) {
                if (paramList.size() != 5) {
                    printf("wrong number of arguments\n");
                    nextRound(ip, port);
                    continue;
                }

                ncmset(client, paramList);

            } else if ("quit" == cmd) {
                delete client;
                printf("bye.\n");
                return 0;

            }  else {
                printf("client_error: Unknown Command: %s\n", cmd.c_str());
            }

            nextRound(ip, port);

        }
        catch (std::exception oor) {
            cout << "has error" << oor.what() << endl;
            nextRound(ip, port);
        }
    }

    return 0;
}

void set(ssdb::Client *client, const vector<std::string> &paramList, const string &val) {
    string key = paramList[1];
    string value = paramList[2];

    ssdb::Status s = client->set(key, value);
    if (s.ok()) {
        printf("%s\n", val.c_str());
    } else if (s.not_found()) {
        printf("%s\n", s.code().c_str());
    }
}

void nlist(ssdb::Client *client, const vector<std::string> &paramList) {
    string start = paramList[1];
    string end = paramList[2];
    int limit = atoi(paramList[3].c_str());
    vector<string> list;
    ssdb::Status s = client->nlist(start, end, limit, &list);

    if (s.ok()) {
        printVector(&list);
    } else if (s.not_found()) {
        printf("%s\n", s.code().c_str());
    }
}

void ncmset(ssdb::Client *client, const vector<std::string> &paramList) {
    string key = paramList[1];
    string key1 = paramList[2];
    string key2 = paramList[3];
    string value = paramList[4];
    vector<string> list;
    ssdb::Status s = client->ncmset(key, key1, key2, value);

    if (s.ok()) {
        printVector(&list);
    } else if (s.not_found()) {
        printf("%s\n", s.code().c_str());
    }
}

void ncmget(ssdb::Client *client, const vector<std::string> &paramList) {
    string key = paramList[1];
    string key1 = paramList[2];
    string key2 = paramList[3];
    vector<string> list;
    ssdb::Status s = client->ncmget(key, key1, key2, &list);

    if (s.ok()) {
        printVector(&list);
    } else if (s.not_found()) {
        printf("%s\n", s.code().c_str());
    }
}

void nscan(ssdb::Client *client, const vector<std::string> &paramList) {
    string key = paramList[1];
    long fromTime = atol(paramList[2].c_str());
    long toTime = atol(paramList[3].c_str());
    long offset = atol(paramList[4].c_str());
    long limit = atol(paramList[5].c_str());
    vector<string> list;

    ssdb::Status s = client->nscan(key, fromTime, toTime, offset, limit, &list);

    if (s.ok()) {
        printVector(&list);
    } else if (s.not_found()) {
        printf("%s\n", s.code().c_str());
    }
}

void nextRound(const char *ip, int port) {
    printf("(%4.3f sec)\n", 0.015);
    printf("ssdb %s:%d>", ip, port);


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


//                string::size_type loc1 = start_temp.find("\"", 0);
//
//                if (loc1 != string::npos) {
//                    start = start_temp.substr(1, start_temp.size() - 2);
//                }
//
//                string::size_type loc2 = end_temp.find("\"", 0);
//
//                if (loc2 != string::npos) {
//                    end = end_temp.substr(1, end_temp.size() - 2);
//                }
