#include <cpu.h>
#include <inttypes.h>
#include <string.h>

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

uint16_t *ptr_mem(uint32_t lig, uint32_t col){
    return (uint16_t*) (0xB8000+2*(lig*80+col));
}
void ecrit_car(uint32_t lig, uint32_t col, char c){
    uint16_t* p=(uint16_t*)ptr_mem(lig,col);
    uint16_t b=(uint16_t)c;
    uint16_t a= (1<<8|1<<9|1<<10|1<<11)&(~(1<<15));
    uint16_t x=(uint16_t)(a|b);
    *p=x;

}

void efface_ecran(void){
    
    for(int i=0;i<25;i++){
        for(int j=0;j<80;j++){
            ecrit_car(i,j,' ');
        }
    }
}

void place_curseur(uint32_t lig, uint32_t col){
    uint16_t m=255;
    uint16_t p=col+lig*80;
    uint8_t a=(uint8_t)(p&m);
    uint16_t b=(uint16_t)(p&(~m));
    b=(uint8_t)(b>>8);
    outb(0x0F, 0x3D4);
    outb(a,0x3D5);
    outb(0x0E,0x3D4);
    outb(b,0x3D5);
}

uint16_t position_curseur(){
    outb(0x0F, 0x3D4);
    uint16_t basse=(uint16_t)inb(0x3D5);
    outb(0x0E, 0x3D4);
    uint16_t haute=(uint16_t)inb(0x3D5);
    haute=haute<<8;
    return haute+basse;

}

void defilement(void){
    for(int i=0;i<24;i++){
        void*m=(void*)ptr_mem(i,0);
        void*n=(void*)ptr_mem(i+1,0);
        memmove(m,n,80*sizeof(uint16_t));
    }
    for(int j=0;j<80;j++){
        ecrit_car(24,j, ' ');
    }
}

void console_putbytes(char*s,int len){
    uint16_t pos=position_curseur();
    uint32_t lig=(uint32_t)(pos/80);
    uint32_t col=(uint32_t)(pos%80);
    for(int i=0;i<len;i++){
        ecrit_car(lig,col,s[i]);
        if(col+1>79 && lig!=24){
            lig=lig+1;
            col=0;
            continue;
        }
        if( col+1>79 && lig==24){
            defilement();
            lig=24;
            col=0;
            continue;
        }
        else{
            col=col+1;
            continue;
        }
    }
}

void ecrire_haut_droite(char* s,int taille){
    place_curseur(0,79-taille);
    console_putbytes(s,taille);
}

void tic_PIT(void){
    
}




void kernel_start(void)
{   


    efface_ecran();
    place_curseur(0,74);
    console_putbytes("yassine elkhaldi",16);
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}


