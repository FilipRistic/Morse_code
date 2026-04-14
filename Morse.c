//KEY0 je 21 a LED3 je 28

//Kompajlirati sa gcc ime.c -o ime -lwiringPi -lwiringPiDev   

#include <wiringPi.h>

#include <stdio.h>

#include <string.h>

#include <ctype.h>   //za f.-ju konvertovanja velikih u mala slova,ako su mala,ostavlja ih takvim



char alphabet[] = {' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',

				    '1','2','3','4','5','6','7','8','9','0','.',',',':',';','?','!','-','/'};



char *pomocni_alphabet[] = {"","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",

				    "1","2","3","4","5","6","7","8","9","0",".",",",":",";","?","!","-","/"};



char* code[] = {"","sl","lsss","lsls","lss","s","ssls","lls","ssss","ss","slll","lsl","slss","ll","ls","lll","slls","llsl","sls",

                "sss","l","ssl","sssl","sll","lssl","lsll","llss","sllll","sslll","sssll","ssssl","sssss","lssss","llsss","lllss",

                "lllls","lllll","slslsl","llssll","lllsss","lslsls","ssllss","lslsll","lssssl","lssls"};


double fast = 500;

double slow = 1000;



void blinkletter(char letter) {   //za realizaciju blinkanja jednog slova

    int i = 0;

    int blinkspeed = 0;

    char currentletter[100];



    for (i = 0; i <= 44; i++) {

        if (alphabet[i] == letter) {

            if (alphabet[i] == ' '){

                delay(1500);    //1.5 sekundi

		break;

	    }

            strcpy(currentletter, code[i]);

            int j = 0;

            for (j = 0; j <= strlen(currentletter) - 1; j++) {

                if (currentletter[j] == 'l')

                    blinkspeed = slow;

                if (currentletter[j] == 's')

                    blinkspeed = fast;



                wiringPiSetup();

                pinMode(28, OUTPUT);

                digitalWrite(28, 1);

		delay(blinkspeed);

                digitalWrite(28, 0);

		delay(200);    //pauza izmedju svakog l,s u jednom slovu

            }

        }

    }

    delay(1200);    //ovo ce biti pauza izmedju svakog slova

}



void playmessage(char message[100]) {

    int i = 0;



    for (i = 0; i <= strlen(message) - 1; i++)

        blinkletter(tolower(message[i]));

}



char* letterlookup(char stringvalue[]) {

    int i = 0;

    for (i = 0; i <= 44; i++) {

        if (strcmp(code[i],stringvalue) == 0)
            return pomocni_alphabet[i];
    }
    return "";
}



void recordmessage() {

    printf("Start typing your Morse Code message! Wait 7 seconds to exit\n");

    int TimeCount = 0;

    int DelayCount = 0;

    char CurrentLetter[100] = {""};

    char CurrentWord[100] = {""};

    int r_value = 0;

    char pomocni[100] = {""};



    int i = 1;



    wiringPiSetup();

    pinMode(21, INPUT);



    while (1) {

	r_value = digitalRead(21);

        if (r_value == 0) {     //Dugme pritisnuto

            DelayCount = 0;

            TimeCount += 1;

	    i = 0;

        }



        if (r_value == 1) {     //Dugme nije pritisnuto

            if (TimeCount > 0) {

                if (TimeCount <= 15)

                    strcat(CurrentLetter, "s");

                else if (TimeCount > 15)

                    strcat(CurrentLetter, "l");

            }



            TimeCount = 0;

            DelayCount = DelayCount + 1;



            if (DelayCount > 80) {    //2 sekunde

                if (strcmp(CurrentLetter,pomocni) != 0) {

		    printf("%s\n",CurrentLetter);		    
                    strcat(CurrentWord,letterlookup(CurrentLetter));
                    strcpy(CurrentLetter, "");

                }

            }



            if (DelayCount >= 160 && i == 0){    //Dodaje space ,4 sekunde

		printf("\n");

		printf("Nova rec:\n");

                strcat(CurrentWord, " ");

		i = 1;

	    }



            if (DelayCount >= 280) {    //7 sekundi

                printf("You recorded %s\n", CurrentWord);

                printf("Exiting recording mode\n");

                break;

            }

        }

        delay(25);

    }

}





int main() {

    char tekst[100];

    printf("Type wanted message,for option 2 press Enter,for exit wait 7 sec:\n");


    fgets(tekst,100,stdin);
    playmessage(tekst);


    printf("You have chosen option 2\n");
    recordmessage();

    
    return 0;

}
