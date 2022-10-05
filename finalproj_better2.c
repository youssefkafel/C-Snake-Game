#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <time.h>

//#define ROW  40
//#define COL  80

unsigned int randint(unsigned int min, unsigned int max);
int main(int av, char * diff[]){
	initscr(); //init screen
    //------------------------Spencer Nauss---------------------------:
    //--------------------SPEED INITIALIZATION------------------------:
    int speed = 100; //initialize initial speed
    int score = 0;   //initialize score
    if(av == 2){
        speed = atoi(diff[1]); //convert string to int
    }

    //------------------------Daniel Summa----------------------------:
    //----------------Random Number and Tick Init---------------------:
    int randTick = 0;    //initialize tick count
    int randNum = randint(1, (9000*speed/10000)); //initialize random number used in trophy expiration

    //Sl = snakelength, Sx = snake head x location, Sy = snake head y location
    //Tx = trophy x location, Ty = trophy y location
    int row,col,Sl,Sx,Sy,Tx,Ty;
    //Bx[] = x location of each body part, By = y location of each body part
    int Bx[100], By[100];


    getmaxyx(stdscr, row , col); //get max size
    noecho(); //noecho disable character from printing on terminal
    keypad(stdscr, TRUE); //enable keypad inputs
    curs_set(0); //enable cursor visibility 
    Sl = 5, Sx = col/2, Sy = row/2; //initial snake length = 5, start x and y of head at midpoint of screen

    //------------------------SAMRAH WASEEM---------------------------:
    //----------------INITIAL SNAKE LENGHT & MOVE---------------------:
    for(int i = 0; i < Sl; i++){ //for loop to create/update body parts 
        Bx[i] = Sx;
        By[i] = Sy;
    }

    char d = 'd';

    //------------------------SAMRAH WASEEM---------------------------:
    //------------------------THE SNAKE PIT---------------------------:
    for(int i = 0; i <= row; i++){ 
        if(i == 0 || i == row-1) //if i == 0 (top) or last row from the buttom
            for(int j = 0; j <= col; j++){ //increase j until <= col
                mvaddch(i,j,'-'); //add character at each position, increasing the column
            }
        else {
            mvaddch(i,0    , '|'); //add char for left border
            mvaddch(i,col-2, '|'); //add char for right border
        }
    }

    //------------------------Spencer Nauss---------------------------:
    //-------------------TROPHY INITIALIZATION------------------------:
    while(mvinch((Ty = randint(1,row-1)), (Tx = randint(1,col-1))) != ' '); //check to see if there is no character
    mvaddch(Ty, Tx, '*'); //add * character
    while(1){
        Bx[0] = Sx; //move the body part up to where the head was
        By[0] = Sy; //move the body part up to where the head was
        char c; //initialize char c
        timeout(10000/speed); 
        //-----------------------SAMRAH WASEEM----------------------------:
        //-----------------------SNAKE MOVEMENT---------------------------:
        c = getch(); // c = input from keyboard
        if(c !=ERR) 
            d = c; //let d = input
        ((int *)Bx)[Sl] = Sx; ((int *)By)[Sl] = Sy;
        switch(d){
            case 'w': //if input is w or W, change direction to up (Sy--)
            case (char)KEY_UP:
            case 'W':
                Sy--;
                break;
            case 'a': //if input is a or A, change direction to left (Sx--)
            case (char)KEY_LEFT:
            case 'A':
                Sx--;
                break;
            case 's': //if input is s or S, change direction to up (Sy++)
            case (char)KEY_DOWN:
            case 'S':
                Sy++;
                break;
            case 'd': //if input is d or D, change direction to down (Sx++)
            case (char)KEY_RIGHT:
            case 'D':
                Sx++;
                break;
            default:
                break;
        }
        if(mvinch(Sy,Sx) == ' '){ //if next point is empty
            mvaddch(Sy,Sx,'@'); //move head to new position
            mvaddch(By[Sl-1], Bx[Sl-1], ' '); //add blank space to last where the last body part was
            for(int i = Sl-1; i > 0; i--){ // i = length-1, while i>0 
                Bx[i] = Bx[i-1]; //move to i - 1
                By[i] = By[i-1]; //move to i - 1
                mvaddch(By[i], Bx[i], '#'); //add # at new coordinate
            }
        //-----------------------DANIEL SUMMA--------------------:
        //--------------TROPHY AND SNAKE LENGTH INCREASE---------:
        } else if(mvinch(Sy,Sx) == '*'){ //if next point is trophy
            mvaddch(Sy,Sx,'@'); //move head
            score++; //increase score
            speed = speed + 10; //increase speed
            Sl++; //add one to length
            for(int i = Sl-1; i > 0; i--){ // i = length-1, while i>0 
                Bx[i] = Bx[i-1]; //move to i - 1
                By[i] = By[i-1]; //move to i - 1
                attron(COLOR_PAIR(1));
                mvaddch(By[i], Bx[i], '#'); //add # at new coordinate
                attroff(COLOR_PAIR(1));
            }
            while(mvinch((Ty = randint(1,row-1)), (Tx = randint(1,col-1))) != ' '); //check to see if there is no character
            mvaddch(Ty, Tx, '*'); //add * character
            
        //---------------------YOUSSEF KAFEL----------------------:
        //----------------------PLAYER WINS-----------------------:
        }else if(Sl == col/2){ //if length = col/2
            clear(); //clear screen
            fflush(stdin); //flush buffer
            mvaddstr(row/2,col/2,"You Win"); //display you win in center of screen            
            refresh(); //refresh
            getchar(); //get input
            echo(); //turn on echo
            endwin(); //end window
            exit(1); //exit program
        }
        //---------------------YOUSSEF KAFEL----------------------:
        //----------------------PLAYER LOSES----------------------:
        else{
            char strScore[3]; //creates a 4 character string
            sprintf(strScore, "%d", score); //stores score in strScore 
            clear(); //clear screen
            fflush(stdin); //flush buffer
            mvaddstr(row/2,col/2,"GAME OVER"); //display you win in center of screen
            mvaddstr(row/2+1,col/2,"SCORE: ");  //displays "SCORE:" below "GAME OVER"
            mvaddstr(row/2+1,col/2+8, strScore); //displays current score after "SCORE:" 
            refresh(); //refresh
            getchar(); //get input
            echo(); //get input
            endwin(); //end window
            exit(1); //exit program
        }
        //-----------------------DANIEL SUMMA--------------------:
        //----------------RANDOMIZE TROPHY PLACEMENT-------------:
        if(randTick >= (9000*speed/10000) || randTick == randNum){ //checks if the number of ticks passed is equal to the random number
            randNum = randint(1, (9000*speed/10000));              //Random number interval between 1 and 9 seconds of time
            randTick = 0;                                          //resets ticks to 0 
            mvaddch(Ty, Tx, ' ');                                  //sets trophy location to space
            while(mvinch((Ty = randint(1,row-1)), (Tx = randint(1,col-1))) != ' '); //checks for empty space
            mvaddch(Ty, Tx, '*');                                  //sets new location for trophy
        }randTick++;
        refresh();
    }
	endwin();
	return 0;
}


//random integer algorithm, input min and max (evenly distributes random numbers)
unsigned int randint(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;
 
    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}