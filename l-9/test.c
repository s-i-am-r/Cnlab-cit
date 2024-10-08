#include<stdio.h>
#include<stdint.h>
int finddeg(uint8_t n){
    #define sz 0b10000000
    int i=0;
    while (! (n& sz))
    {
        i++;
        n<<=1;
    }
    return 7-i;
    
}
int finddeg32(uint32_t n){
    uint32_t szz=1<<31;
    int i=0;
    while (! (n& szz))
    {
        i++;
        n<<=1;
    }
    return 31-i;
    
}
void p_bin(uint32_t b){
    while (b)
    {
        printf("%d",b&1);
        b>>=1;
    }
    printf("\n");
    
}
uint8_t crc_check(uint32_t num,uint8_t divisor){
    #define ch 0b11110000000000000000000000000000
    if (ch&num)
    {
        return -1;
    }
    int m=finddeg32(num);
    
    int degree = finddeg(divisor);
    uint32_t ptr = 1<<m+degree-4,d_p=m+degree-4;
    num<<=degree;
    uint8_t rag =num>>m;
    p_bin(num);
    p_bin(divisor);
    while (ptr)
    {
        rag^=divisor;
        rag<<=1;
        rag|=(num>>(d_p--))&1;
        ptr>>=1;
        p_bin(rag);
    }
    rag^=divisor;
    return rag&0b111;



}
int main(){
    uint8_t x=crc_check(0b11010,0b1001);
	printf("===============\n");
    p_bin(x);


}