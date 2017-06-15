#include <stdlib.h>

/* TODO */
char *itoab(int in, char *ascii){
    return ascii;
}

/* TODO */
char *itoao(int in, char *ascii){
    return ascii;
}

char *itoad(int in, char *ascii){
    int tmp;
    int id,d;
    char temp;
    for(id=0; in>=1; id++){
        tmp = in % 10;
        ascii[id]=(char) (((int)'0') + tmp);
        in = in/10;
    } id--;
    for(d=0; d<=id/2; d++){
        temp = ascii[d];
        ascii[d] = ascii[id-d];
        ascii[id-d] = temp;
    }
    id++;
    ascii[id]='\0';
    return ascii;
}

char *itoax(int in, char *ascii){
    int tmp;
    int id,d;
    char temp;
    for(id=0; in>1; id++){
        tmp = in % 16;
        if(tmp<10)
           ascii[id]=(char) (((int)'0') + tmp);
        else{
            if(tmp==10)
                ascii[id]='a';
            else if(tmp==11)
                ascii[id]='b';
            else if(tmp==12)
                ascii[id]='c';
            else if(tmp==13)
                ascii[id]='d';
            else if(tmp==14)
                ascii[id]='e';
            else if(tmp==15)
                ascii[id]='f';
        }
        in = in/16;
    } id--;
    for(d=0; d<=id/2; d++){
        temp = ascii[d];
        ascii[d] = ascii[id-d];
        ascii[id-d] = temp;
    }
    id++;
    ascii[id]='\0';
    return ascii;
}

char *itoa(int value, char *str, int base){
    if(base == 2)
        return itoab(value, str);
    else if(base == 8)
        return itoao(value, str);
    else if(base == 10)
        return itoad(value, str);
    else if(base == 16)
        return itoax(value, str);
    else
        return str;
}
