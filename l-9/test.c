#include<stdio.h>
#include<stdint.h>

int finddeg(uint64_t n,uint8_t size){
    size*=8;
    uint32_t szz=1<<(size-1);
    int i=0;
    while (! (n& szz))
    {
        i++;
        n<<=1;
    }
    return size-1-i;
    
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
    int degree = finddeg(divisor,sizeof(divisor));
    uint32_t ch=1;
    for (size_t i = 0; i < degree; i++)
    {
        ch<<=1;
        ch|=1;
        
    }
    ch<<=sizeof(ch)*8-4;
    if (ch&num)
    {
        return -1;

    }
    int m=finddeg(num,sizeof(num));
    
    num<<=degree;
    uint8_t rag =num>>m;
    uint32_t ptr=1<<(m-1),d_p=m-1;
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