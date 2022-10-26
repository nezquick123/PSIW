#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 10


void buff_swap(char * buf, int n){
    for(int i = 0; i < n/2; i++){
        char temp = buf[i];
        buf[i] = buf[n-i-1];
        buf[n-i-1] = temp;
    }
}

int main(int argc, char *argv[]) {
    char buf[MAX];
    int desc_zrod, desc_cel;
    int lbajt;


    if (argc<3){
        fprintf(stderr, "Za malo argumentow. Uzyj:\n");
        fprintf(stderr, "%s <plik zrodlowy> <plik docelowy>\n", argv[0]);
        exit(1);
    }

    desc_zrod = open(argv[1], O_RDONLY);
    if (desc_zrod == -1){
        perror("Blad otwarcia pliku zrodlowego");
        exit(1);
    }

    desc_cel = creat(argv[2], 0640);
    if (desc_cel == -1){
        perror("Blad utworzenia pliku docelowego");
        exit(1);
    }
    int input_len = lseek(desc_zrod, 0, SEEK_END);
    if(MAX <= input_len)
        lseek(desc_zrod, -MAX, SEEK_END);
    else{
        lseek(desc_zrod, 0, SEEK_SET);
    }
    int offset =  MAX;
    lbajt = read(desc_zrod, buf, MAX);
    while (lbajt > 0) {
        buff_swap(buf, lbajt);
        if (write(desc_cel, buf, lbajt) == -1) {
            perror("Blad zapisu pliku docelowego");
            exit(1);
        }
        offset+=MAX;
        if (lseek(desc_zrod,  -offset, SEEK_END) < 0 )
        {
            lseek(desc_zrod,  0, SEEK_SET);
            lbajt = read(desc_zrod, buf, offset - input_len);
            if(lbajt > 0){
                buff_swap(buf, lbajt);
                write(desc_cel, buf, lbajt);
            }
            break;
        }
        lbajt = read(desc_zrod, buf, MAX);
    }
    if (lbajt == -1) {
        perror("Blad odczytu pliku zrodlowego");
        exit(1);
    }
    if (close(desc_zrod) == -1 || close(desc_cel) == -1) {
        perror("Blad zamkniecia pliku");
        exit(1);
    }
    exit(0);
}