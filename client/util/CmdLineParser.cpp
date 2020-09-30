//
// Created by john on 2020/9/30.
//

#include <iostream>
#include <vector>
#include <sstream>
#include "CmdLineParser.h"


vector<std::string> *CmdLineParser::parse_cmd(std::string cmd) {
    //string::iterator iterA;


    parts.clear();

    //control
    bool inThePart = false;

    int index = 0;

    //char[] array = line.toCharArray();
    char *array = (char *) cmd.c_str();

    int quotationMode = 0; // 0: no quotation 1: double quotation 2：single quotation

    //StringBuilder temp = new StringBuilder();

    int length = cmd.length();
    ostringstream temp;

    while (index < length) {

        if (inThePart) {

            if (quotationMode == 0) {

                if (isEmptyChar(array[index])) {
                    inThePart = false;

                    parts.push_back(temp.str());
                    temp.str("");

                } else if (array[index] == '\"') {
                    throw ("bad format");
                } else {

                    temp << array[index];
                }

            } else if (quotationMode == 1) {

                if (array[index] == '\"') {
                    //close

                    quotationMode = 0;
                    inThePart = false;

                    parts.push_back(temp.str());
                    temp.str("");

                } else if (array[index] == '\\') {

                    //check length

                    if (index >= length - 1) {
                        throw ("No closing quotation");
                    }

                    ++index;//go to next char

                    temp << array[index];

                } else {

                    temp << array[index];


                }


            } else if (quotationMode == 2) {

                //

                if (array[index] == '\'') {

                    int next = index + 1;

                    if ((next > length - 1)
                        || array[next] != '\''

                            ) {

                        //close
                        quotationMode = 0;
                        inThePart = false;

                        parts.push_back(temp.str());
                        temp.str("");


                    } else {
                        ++index;

                        temp << array[index];

                    }


                } else {

                    temp << array[index];
                }
            } else {
                throw ("bad");
            }


        }
            //not int the part
        else {

            if (isEmptyChar(array[index])) {

                //do nothing
            } else if (array[index] == '"') {

                if (quotationMode == 1) {
                    //close quotation do nothing

                } else if (quotationMode == 0) {

                    int prefIndex = index - 1;
                    if (prefIndex >= 0) {
                        //check paralyzed ""
                        if (!isEmptyChar(array[prefIndex])) {
                            throw ("Bad format");
                        }
                    }

                    inThePart = true;

                    //temp = new StringBuilder();
                    quotationMode = 1;


                } else {

                    throw ("Bad format");
                }


            } else if (array[index] == '\'') {

                if (quotationMode == 0) {

                    int prefIndex = index - 1;
                    if (prefIndex >= 0) {

                        //check paralyzed ''
                        if (!isEmptyChar(array[prefIndex])) {
                            throw ("Bad format");
                        }
                    }

                    inThePart = true;
                    //temp = new StringBuilder();

                    quotationMode = 2;


                } else if (quotationMode == 2) {

                    //close quotation do nothing

                } else {
                    throw ("Bad format");
                }

            } else {


                int prev = index - 1;

                if (prev >= 0) {

                    if (array[prev] == '\"' || array[prev] == '\'') {
                        throw ("bad format");
                    }
                }

                inThePart = true;

                if (array[index] == '\"') {
                    quotationMode = 1;
                } else if (array[index] == '\'') {
                    quotationMode = 2;

                } else {

                    temp << array[index];
                }
            }

        }

        ++index;
    }

    if (quotationMode != 0) {
        throw ("No closing quotation");
    }

    if (!temp.str().empty()) {
        parts.push_back(temp.str());
    }

    return &parts;
}


