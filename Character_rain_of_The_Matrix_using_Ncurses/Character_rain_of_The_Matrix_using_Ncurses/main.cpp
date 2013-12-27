#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

const string CCH = " 01234 5678 9abcd ef ghijklmn opqr stuvwx yzA BCDEF GHIJK LMNO PQRSTUV WXYZ ";

//Add one random character "a" to a string "mesg"
string random_char_to_string(string mesg){
    bool abletospace=true;
    long spacerange;
    if (5<mesg.length()) {
        spacerange=5;
    }else{
        spacerange=mesg.length();
    }
    long where=mesg.substr(0,spacerange).find(" ");
    if (where>0) {
        abletospace=false;
    }else{
        abletospace=true;
    }
    if (!abletospace) {
        string a=CCH.substr(rand()/(RAND_MAX/(CCH.length()-1)),1);
        while (a==" ") {
            a=CCH.substr(rand()/(RAND_MAX/(CCH.length()-1)),1);
        }
        mesg=a+mesg;
    }else{
        string a=CCH.substr(rand()/(RAND_MAX/(CCH.length()-1)),1);
        mesg=a+mesg;
    }
    return mesg;
}



int main()
{
    int row,col,nrow,ncol;
    //init ncurses
    initscr();//start ncurses
    start_color();//start color
    if (has_colors()) {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);//define color pair
    }
    //genrate two vectors to store windows and strings
    getmaxyx(stdscr, row, col);
    int num_col=col/2;
    vector<WINDOW *> winlist;
    vector<string> strlist;
    for (int i=0; i<num_col; i++) {
        //genrate windows one by one
        WINDOW *win=newwin(row, 1, 0, i*2);
        winlist.insert(winlist.end(), win);
        if (has_colors()) {
            wattron(win,COLOR_PAIR(1));
        }
        //genrate strings one by one
        string mesg="";
        for (int j=0; j<row; j++) {
            mesg=random_char_to_string(mesg);
        }
        strlist.insert(strlist.end(), mesg);
    }
    while (1) {
        //adjust whether the size of trminal has been changed
        getmaxyx(stdscr, nrow, ncol);
        if (nrow!=row || ncol!=col) {
            row=nrow;
            col=ncol;
            num_col=col/2;
            winlist.clear();
            strlist.clear();
            for (int i=0; i<num_col; i++) {
                WINDOW *win=newwin(row, 1, 0, i*2);
                winlist.insert(winlist.end(), win);
                if (has_colors()) {
                    wattron(win,COLOR_PAIR(1));
                }
                string mesg="";
                for (int j=0; j<row; j++) {
                    mesg=random_char_to_string(mesg);
                }
                strlist.insert(strlist.end(), mesg);
            }
        }
        //display strings
        for (int i=0; i<num_col; i++) {
            //get window and string
            WINDOW *win=winlist.at(i);
            string mesg=strlist.at(i);
            //change original characters
            for(int j=0;j<mesg.length();j++){
                if (mesg.substr(j,1)!=" ") {
                    string a=CCH.substr(rand()/(RAND_MAX/(CCH.length()-1)),1);
                    while(a==" "){
                        a=CCH.substr(rand()/(RAND_MAX/(CCH.length()-1)),1);
                    }
                    mesg.replace(j, 1, a);
                }
            }
            //Add a new character
            mesg=random_char_to_string(mesg);
            //cut string to fix length
            if (mesg.length()>row) {
                mesg=mesg.substr(0,row);
            }
            //restore string
            strlist.at(i)=mesg;
            //print string and refresh window
            mvwprintw(win, 0, 0, mesg.c_str());
            wrefresh(win);
        }
        //wait xx 1/1,000,000 seconds
        usleep(150000);
    }
    endwin();
}