#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int siz = 32;
char ***has;
char **slow;
int HaslaSize;
int SlownikSize;
int il=0;
int ilehasel=0;
int indeks = -1;
char buf[100];
char deshyfr[30][4][100];

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void bytes2md5(const char *data, int len, char *md5buf)
{
    pthread_mutex_lock(&mut);

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data, len);
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);

    for (i = 0; i < md_len; i++) {
        snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
    }

    pthread_mutex_unlock(&mut);
}

int sprawdzhaslo(char ***has, char *slow, int HaslaSize, char *md5buf)
{
    int ile=0;
    for (int j=0; j<HaslaSize; j++){
        if (has[j][0][0] != '0'){
            for (int k = 0; k < siz; k++){
                if (has[j][1][k] != md5buf[k]){
                    ile++;
                    break;
                }
            }
            if (ile == 0){
                pthread_mutex_lock(&mut);

                indeks = j;
                sprintf(buf, "%s", slow);
                usleep(20000);

                pthread_mutex_unlock(&mut);
                return 1;
            }
        ile = 0;
        }
    }
    return 0;
}

void *hash(void* arg)
{
    char md5buf[33];

    for (int i=0; i<SlownikSize; i++){
        bytes2md5(slow[i], strlen(slow[i]), md5buf);
        sprawdzhaslo(has, slow[i], HaslaSize, md5buf);
    }
    il++;
}

void *liczbyp(void* arg )
{
    char md5buf[33];
    char pomslow[100];

    for (int i=0; i<SlownikSize; i++){
        for (int j = 0; j < 10; j++) {
            sprintf(pomslow, "%01d%s", j, slow[i]);
            bytes2md5(pomslow, strlen(pomslow), md5buf);
            if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))break;

            sprintf(pomslow, "%s%01d", slow[i], j);
            bytes2md5(pomslow, strlen(pomslow), md5buf);
            if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))break;

            for (int k = 0; k < 10; k++){
                sprintf(pomslow, "%01d%s%01d", j, slow[i], k);
                bytes2md5(pomslow, strlen(pomslow), md5buf);
                if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))break;
            }
        }

        for (int j = 0; j < 100; j++) {
            sprintf(pomslow, "%02d%s", j, slow[i]);
            bytes2md5(pomslow, strlen(pomslow), md5buf);
            if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))break;

            sprintf(pomslow, "%s%02d", slow[i], j);
            bytes2md5(pomslow, strlen(pomslow), md5buf);
            if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))break;

            for (int k = 0; k < 100; k++){
                sprintf(pomslow, "%02d%s%02d", j, slow[i], k);
                bytes2md5(pomslow, strlen(pomslow), md5buf);
                if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))break;
            }
        }
    }
    il++;
}


int liczby(char ***has, char *slow, int HaslaSize, char *md5buf)
{
    char pomslow[100];

    for (int j = 0; j < 10; j++) {
        sprintf(pomslow, "%01d%s", j, slow);
        bytes2md5(pomslow, strlen(pomslow), md5buf);
        if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))return 0;

        sprintf(pomslow, "%s%01d", slow, j);
        bytes2md5(pomslow, strlen(pomslow), md5buf);
        if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))return 0;

        for (int k = 0; k < 10; k++){
            sprintf(pomslow, "%01d%s%01d", j, slow, k);
            bytes2md5(pomslow, strlen(pomslow), md5buf);
            if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))return 0;
        }
    }

    for (int j = 0; j < 100; j++) {
        sprintf(pomslow, "%02d%s", j, slow);
        bytes2md5(pomslow, strlen(pomslow), md5buf);
        if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))return 0;

        sprintf(pomslow, "%s%02d", slow, j);
        bytes2md5(pomslow, strlen(pomslow), md5buf);
        if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))return 0;

        for (int k = 0; k < 100; k++){
            sprintf(pomslow, "%02d%s%02d", j, slow, k);
            bytes2md5(pomslow, strlen(pomslow), md5buf);
            if(sprawdzhaslo(has, pomslow, HaslaSize, md5buf))return 0;
        }
    }
}

void *litters(void* arg)
{
    char md5buf[33];
    char pomslow[100];
    int licz=0;

    for (int i=0; i<SlownikSize; i++){
        memset(pomslow, 0, 100);
        for (int j=0; j<strlen(slow[i]); j++){
            if (slow[i][j]>96 && slow[i][j]<123){
                pomslow[j] = slow[i][j]-32;
            } else{
                pomslow[j] = slow[i][j];
            }
        }

        bytes2md5(pomslow, strlen(pomslow), md5buf);
        sprawdzhaslo(has, pomslow, HaslaSize, md5buf);
        liczby(has, pomslow, HaslaSize, md5buf);

        for (int j=0; j<strlen(slow[i]); j++){
            if (slow[i][j]>96 && slow[i][j]<123 && licz == 0){
                pomslow[j] = slow[i][j]-32;
                licz++;
            } else{
                pomslow[j] = slow[i][j];
            }
        }

        licz = 0;
        bytes2md5(pomslow, strlen(pomslow), md5buf);
        sprawdzhaslo(has, pomslow, HaslaSize, md5buf);
        liczby(has, pomslow, HaslaSize, md5buf);
    }
    il++;
}

void pokaz()
{
    printf("\nLista hasel:\n");
    for (int i=0; i<ilehasel; i++){
        printf("Uzytkownik: %s z mailem: %s ma haslo: %s hash %s\n", deshyfr[i][0], deshyfr[i][1], deshyfr[i][3], deshyfr[i][2]);
    }
    printf("\n");
}

void* kons(void* arg)
{
    while (il!=5){
        if (indeks != -1){
            printf("Uzytkownik: %s z mailem: %s ma haslo: %s hash %s\n\n", has[indeks][3], has[indeks][2], buf, has[indeks][1]);

            pthread_mutex_lock(&mut);
            has[indeks][0][0] = '0';
            sprintf(deshyfr[ilehasel][0], "%s", has[indeks][3]);
            sprintf(deshyfr[ilehasel][1], "%s", has[indeks][2]);
            sprintf(deshyfr[ilehasel][2], "%s", has[indeks][1]);
            sprintf(deshyfr[ilehasel][3], "%s", buf);
            indeks = -1;
            memset(buf, 0, 100);
            ilehasel++;
            pthread_mutex_unlock(&mut);
        }
    }

    pokaz();

    pthread_exit(NULL);
}

void* niszczf(void* arg)
{
    int ind;
    char pomoc[33];
    char slowo[100];

    while (il!=3){
        if (ilehasel != 0){
            ind = rand() % ilehasel;
            memset(slowo, 0, 100);
            sprintf(slowo, "%s", slow[rand()%SlownikSize]);
            bytes2md5(slowo, strlen(slowo), pomoc);

            pthread_mutex_lock(&mut);
            sprintf(deshyfr[ind][2], "%s", pomoc);
            pthread_mutex_unlock(&mut);

            printf("zniszczyl hash pod ind: %d na hash %s\n\n", ind, pomoc);
        }
        usleep(100000);
    }
    il++;
}

void* naprawf(void* arg)
{
    char pomoc[33];

    while (il!=4){
        for (int i=0; i<ilehasel; i++){
            bytes2md5(deshyfr[i][3], strlen(deshyfr[i][3]), pomoc);

            for (int k = 0; k < siz; k++){
                if (deshyfr[i][2][k] != pomoc[k]){
                    pthread_mutex_lock(&mut);
                    sprintf(deshyfr[i][2], "%s", pomoc);
                    pthread_mutex_unlock(&mut);
                    printf("naprawil  hash pod ind: %d na hash %s\n\n", i, pomoc);
                    break;
                }
            }
        }
    usleep(50000);
    }
    il++;
    //pokaz();
}

int ilelinijek(FILE *file)
{
    char buffer[1024];
    int ilosc=0;
    while (fgets(buffer, sizeof(buffer), file) != NULL){
            ilosc++;
    }

    fseek(file, 0, SEEK_SET);
    return ilosc;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr, "Uzycie: %s plik hasel plik slownik\n", argv[0]);
        return 1;
    }

    srand(time(NULL));

    FILE *hasla = fopen(argv[1], "r");
    if (hasla == NULL) {
        perror("Error opening file hasla");
        return 1;
    }

    HaslaSize = ilelinijek(hasla);

    FILE *slownik = fopen(argv[2], "r");

    if (slownik == NULL) {
        perror("Error opening file slownik");
        return 1;
    }

    SlownikSize = ilelinijek(slownik);

    has = (char ***)malloc(sizeof(char**)*HaslaSize);
    for (int i=0; i<HaslaSize;i++){
        has[i] = (char **)malloc(sizeof(char*)*4);
        for (int j=0; j<4;j++){
            has[i][j] = (char*)malloc(sizeof(char)*100);
        }
    }

    slow = (char **)malloc(sizeof(char*)*SlownikSize);
    for (int i=0; i<SlownikSize;i++){
        slow[i] = (char*)malloc(sizeof(char)*100);
    }

    for (int i=0; i<HaslaSize; i++){
        fscanf(hasla, "%s %s %s %[^\n]", has[i][0], has[i][1], has[i][2], has[i][3]);
    }

    for (int i=0; i<SlownikSize; i++){
        fscanf(slownik, "%s\n", slow[i]);
    }

    pthread_t litter, numb, word, konsum, niszcz, napraw;

    pthread_create(&word, NULL, hash, NULL);
    pthread_create(&numb, NULL, liczbyp, NULL);
    pthread_create(&litter, NULL, litters, NULL);
    pthread_create(&konsum, NULL, kons, NULL);
    pthread_create(&niszcz, NULL, niszczf, NULL);
    pthread_create(&napraw, NULL, naprawf, NULL);


    signal(SIGHUP, pokaz);

    pthread_join(word, NULL);
    pthread_join(numb, NULL);
    pthread_join(litter, NULL);
    pthread_join(konsum, NULL);
    pthread_join(niszcz, NULL);
    pthread_join(napraw, NULL);

    fclose(hasla);
    fclose(slownik);

    return 0;
}

