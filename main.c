#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <stdbool.h>

struct cvor{
    int ep1;
    int ep2;
    int br;
    int nivo;
    bool pob;
    struct cvor *otac;
    struct cvor *dete;
    struct cvor *brat;
};

struct cvor* noviCvor(int ep1, int ep2, int br, struct cvor* sBrat, struct cvor* otac){
    struct cvor* cvor=(struct cvor*)malloc(sizeof(struct cvor));
    cvor->ep1=ep1;
    cvor->ep2=ep2;
    cvor->br=br;
    cvor->pob=0;
    if(sBrat){
        sBrat->brat=cvor;
    }
    else{
        otac->dete=cvor;
    }
    cvor->nivo=otac->nivo+1;
    cvor->otac=otac;
    cvor->dete=NULL;
    cvor->brat=NULL;
    return cvor;
}

bool provera(int n, int mtr[20][4]){
    for(int i=0; i<n; i++){
        for(int j=0; j<3; j++){
            if(mtr[i][j]!=mtr[i][j+1])
                return false;
        }
    }
    return true;
}

int min(int a, int b){
    if(a<b)
        return a;
    else
        return b;
}

void presipanje(int ep1, int ep2, int broj, int mtr[][4]){
    int br=broj;
    int vrh = 3;
    while (mtr[ep1][vrh] == 0)
        vrh--;
    for(int i=0; i<4 && br>0; i++){
        if(mtr[ep2][i]==0){
            mtr[ep2][i]=mtr[ep1][vrh];
            br--;
        }
    }
    br=broj;
    for(int i=vrh; i>=0 && br>0; i--){
        mtr[ep1][i]=0;
        br--;
    }
}

int brojac=0;

void stanja(int n, struct cvor* otac, int mtr[20][4]){
    struct cvor *sBrat = otac->dete;
    if(!otac->pob){
        for (int i = 0; i < n; i++) {
            int vrh1 = 3;
            while (mtr[i][vrh1] == 0)
                vrh1--;
            if (vrh1 >= 0) {
                for (int j = 0; j < n; j++) {
                    int vrh2 = 3;
                    while (mtr[j][vrh2] == 0 && vrh2>=0)
                        vrh2--;
                    if ((vrh2 == -1 || mtr[i][vrh1]==mtr[j][vrh2]) && vrh2 != 3 && i != j) {
                        int br = 1;
                        for (int inc = vrh1 - 1; inc >= 0; inc--) {
                            if (mtr[i][inc] == mtr[i][vrh1])
                                br++;
                            else
                                break;
                        }
                        struct cvor *cvor = noviCvor(i, j, min(br, 3 - vrh2), sBrat, otac);
                        sBrat = cvor;
                        presipanje(i, j, cvor->br, mtr);
                        if(provera(n, mtr)){
                            brojac++;
                            struct cvor* pobednik=cvor;
                            while(pobednik && !pobednik->pob){
                                pobednik->pob=1;
                                pobednik=pobednik->otac;
                            }
                        }
                        presipanje(j, i, cvor->br, mtr);
                    }
                }
            }
        }
    }
}

void stampanje(int n, int nivo, int mtr[20][4], bool f){
    char znak[4]={'-',' ',' ',' '};
    for(int i=0;i<4;i++){
        if(f){for (int t = nivo; t > 0; t--) {
                putchar(znak[i]);
            }
        }
        for(int j=0;j<n;j++){
            printf("%d ",mtr[j][i]);
        }
        putchar('\n');
    }
    putchar('\n');
}

void kreiranje(int n, int k, int p, struct cvor* otac, int mtr[20][4]){
    int t;
    int lfg[10+4*n-4*k-1];
    srand(time(0));
    for(int i=0; i<10; i++)
        lfg[i]=rand();
    int j=10, j1=0;
    int boja[n-k];
    memset( boja, 0, (n-k)*sizeof(int) );
    for(int i=0; i<n-k;i++){
        for(int i1=0; i1<4; i1++){
            t=(lfg[j]=lfg[j-7]+lfg[j-10])%(n-k-j1);
            j++;
            for(int inc=0; inc<t+1; inc++){
                if(boja[inc]==4)
                    t++;
            }
            mtr[i][i1]=t+1;
            boja[t]++;
            if(boja[t]==4)
                j1++;
        }
    }
    while(otac) {
        presipanje(otac->ep1, otac->ep2, otac->br, mtr);
//        stampanje(n, otac->nivo, mtr, 1);
        stanja(n, otac, mtr);
        if(otac->dete && otac->nivo<p-1){
            otac=otac->dete;
        }
        else {
            while (!otac->brat && otac->otac) {
                presipanje(otac->ep2, otac->ep1, otac->br, mtr);
                otac=otac->otac;
            }
            if(otac->brat){
                presipanje(otac->ep2, otac->ep1, otac->br, mtr);
                otac=otac->brat;
            }
            else{
                stampanje(n, otac->nivo, mtr, 1);
                printf("%d\n", brojac);
                otac=otac->otac;
            }
        }
    }
}

struct cvor* potez(int n, int mtr[20][4], struct cvor* otac){
    int ep1, ep2;
    printf("unesite epruvete:\n");
    scanf("%d %d", &ep1, &ep2);
    while(ep1>n || ep2>n || ep1<0 || ep2<0){
        printf("nepostojece epruvete");
        printf("unesite epruvete:\n");
        scanf("%d %d", &ep1, &ep2);
    }
    struct cvor* cvor=otac->dete;
    while(cvor){
        if(cvor->ep1==ep1 && cvor->ep2==ep2){
            presipanje(ep1, ep2, cvor->br, mtr);
            stampanje(n, cvor->nivo, mtr, 0);
            if(cvor->pob && !cvor->dete) {
                printf("pobedili ste");
                return;
            }
            else if(!cvor->pob && !cvor->dete){
                printf("izgubili ste");
                return;
            }
            return cvor;
        }
        cvor=cvor->brat;
    }
    printf("nedozvoljen potez");
    return otac;
}

struct cvor* pomoc(int n, int mtr[20][4], struct cvor* otac){
    if(otac->dete){
        struct cvor *cvor = otac->dete;
        while (cvor) {
            if (cvor->pob) {
                presipanje(cvor->ep1, cvor->ep2, cvor->br, mtr);
                stampanje(n, cvor->nivo, mtr, 0);
                printf("%d %d\n", cvor->ep1, cvor->ep2);
                return cvor;
            }
            cvor = cvor->brat;
        }
        printf("ne mozete pobediti u preostalom broju poteza\n");
    }
    else if(otac->pob){
        printf("pobedili ste\n");
        return 0;
    }
}

void resenje(int n, int mtr[20][4], struct cvor* koren){
    if(koren->pob){
        struct cvor* cvor=koren;
        while(cvor->dete){
            cvor=cvor->dete;
            presipanje(cvor->ep1, cvor->ep2, cvor->br, mtr);
            while(!cvor->pob){
                presipanje(cvor->ep2, cvor->ep1, cvor->br, mtr);
                cvor=cvor->brat;
                presipanje(cvor->ep1, cvor->ep2, cvor->br, mtr);
            }
            stampanje(n, cvor->nivo, mtr, 0);
            printf("%d %d\n\n\n", cvor->ep1, cvor->ep2);
        }
        return;
    }
    else
        printf("ne postoji resenje");
}

void novaIgra(struct cvor* koren){
    while(koren->dete){
        koren=koren->dete;
    }
    koren=koren->otac;
    free(koren->dete);
    koren->dete=NULL;
    while(!koren->brat && koren->otac){
        koren=koren->otac;
        free(koren->dete);
        koren->dete=NULL;
    }
    if(!koren->otac){
        free(koren);
        return;
    }
    else
        koren=koren->brat;
}

void ispis(int n, struct cvor* koren, int mtr[20][4]){
    struct cvor* otac=koren;
    while(otac) {
        presipanje(otac->ep1, otac->ep2, otac->br, mtr);
        stampanje(n, otac->nivo, mtr, 1);
        if(otac->dete){
            otac=otac->dete;
        }
        else {
            while (!otac->brat && otac->otac) {
                presipanje(otac->ep2, otac->ep1, otac->br, mtr);
                otac=otac->otac;
            }
            if(otac->brat){
                presipanje(otac->ep2, otac->ep1, otac->br, mtr);
                otac=otac->brat;
            }
            else{
//                stampanje(n, otac->nivo, mtr, 1);
//                printf("%d", brojac);
                otac=otac->otac;
            }
        }
    }
}

int n, k, p;

int main() {
    setbuf(stdout, 0);
    printf("unesite parametre n, k i p:\n");
    scanf("%d %d %d", &n, &k, &p);
//    n=5;
//    k=2;
//    p=10;
//    int mtr[5][4]={
//            {3,3,2,1},
//            {1,3,2,1},
//            {2,1,3,2},
//            {0,0,0,0},
//            {0,0,0,0}
//    };
//    int mtr[5][4]={
//            {2,1,2,0},
//            {1,2,2,1},
//            {1,0,0,0},
//            {0,0,0,0}
//    };
    int mtr[20][4];
    memset( mtr, 0, n*4*sizeof(int) );
    struct cvor* koren=(struct cvor*)malloc(sizeof(struct cvor));
    koren->nivo=0;
    koren->ep1=0;
    koren->ep2=0;
    koren->br=0;
    koren->pob=0;
    koren->otac=NULL;
    koren->dete=NULL;
    koren->brat=NULL;
    kreiranje(n, k, p, koren, mtr);
    struct cvor* trCvor=koren;
//    scanf("%d %d %d", &otac->ep1, &otac->ep2, &otac->br);
    printf(" 1. prespi tecnost\n 2. pomoc\n 3. resenje\n 4. ispis stabla\n 0. izlaz iz programa\n");
    while(1){
        int t;
        scanf("%d", &t);
        switch(t){
            case 1:
                trCvor=potez(n, mtr, trCvor);
                break;
            case 2:
                trCvor=pomoc(n, mtr, trCvor);
                break;
            case 3:
                while(trCvor!=koren){
                    presipanje(trCvor->ep2, trCvor->ep1, trCvor->br, mtr);
                    trCvor=trCvor->otac;
                }
                resenje(n, mtr, koren);
                return 0;
            case 4:
                ispis(n, koren, mtr);
                break;
//            case 5:
//                printf("unesite parametre n, k i p:\n");
//                scanf("%d %d %d", &n, &k, &p);
//                novaIgra(koren);
//                int mtr[20][4];
//                memset( mtr, 0, n*4*sizeof(int) );
//                struct cvor* koren=(struct cvor*)malloc(sizeof(struct cvor));
//                koren->nivo=0;
//                koren->ep1=0;
//                koren->ep2=0;
//                koren->br=0;
//                koren->pob=0;
//                koren->otac=NULL;
//                koren->dete=NULL;
//                koren->brat=NULL;
//                kreiranje(n, k, p, koren, mtr);
//                trCvor=koren;
//                break;
            case 0:
                return 0;
            default:
                printf("nepostojeca komanda\n");
                break;
        }
    }
    return 0;
}