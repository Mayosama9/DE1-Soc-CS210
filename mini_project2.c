#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


#define BOARD                 "DE1-SoC"

#define BLACK 0x0000 		
#define GREEN 0x07E0 	
#define RED 0xF800 	 	
#define WHITE 0xFFFF	
#define LGRAY 0xC618 	
#define NAVY 0x000F 

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs

#define TIMER_BASE      0xFF202000


volatile int pixel_buffer_start; 
volatile int * SW_ptr         = (volatile int *)0xFF200040;    
volatile int * LEDR_ptr       = (volatile int *)0xFF200000;    
volatile int * KEY_ptr       =(volatile int *) 0xFF200050;  
volatile int *TIMER_ptr = (int *) TIMER_BASE;
volatile int *HEX1_ptr = (int *)HEX5_HEX4_BASE;
volatile int *HEX2_ptr = (int *)HEX3_HEX0_BASE;

unsigned short lfsr = 0xACE1u;
  unsigned bit;

  unsigned random()
  {
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  (lfsr >> 1) | (bit << 15);
  }

int switch_value;
int key_value;

int ma[10]={63,6,91,79,102,109,125,7,127,103};

void clear_screen () {
    int x,y;
    for ( x = 0; x < 320; x++) {
        for ( y = 0; y < 240; y++) {
            plot_pixel(x, y,BLACK);
        }
    }
}

void end_screen(){

}
void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
  *character_buffer = c;
}

void transition_screen(){
    char* s="                                                            ";
    int x=10;
    while(*s){
        write_char(x,30,*s);
        x++;
        s++;
    }
}

void dat(){
    int x=100;
    int y=100;
    //E
    draw_box(x,y,3,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+2,3,1,WHITE);
    draw_box(x,y+4,3,1,WHITE);
    x=x+4;
    //N
    draw_box(x,y,2,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x+2,y+1,1,1,WHITE);
    draw_box(x+3,y+2,1,1,WHITE);
    draw_box(x+4,y+3,1,1,WHITE);
    draw_box(x+5,y+4,1,1,WHITE);
    draw_box(x+6,y,1,5,WHITE);
    x=x+8;
    //T
    draw_box(x,y,5,1,WHITE);
    draw_box(x+2,y,1,5,WHITE);
    x=x+6;
    //E
    draw_box(x,y,3,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+2,3,1,WHITE);
    draw_box(x,y+4,3,1,WHITE);
    x=x+4;
    //R
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y+2,5,1,WHITE);
    draw_box(x+4,y,1,3,WHITE);
    draw_box(x+4,y+4,1,1,WHITE);
    draw_box(x+3,y+3,1,1,WHITE);
    x=x+8;
    //1

    draw_box(x,y,1,5,WHITE);
    x=x+4;
    //T
    draw_box(x,y,5,1,WHITE);
    draw_box(x+2,y,1,5,WHITE);
    x=x+6;
    //O
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    draw_box(x+4,y,1,5,WHITE);
    x=x+8;
    //P
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,4,1,WHITE);
    draw_box(x,y+2,4,1,WHITE);
    draw_box(x+3,y,1,3,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    x=x+6;
    //L
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    x=x+6;
    //A
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,4,1,WHITE);
    draw_box(x,y+2,4,1,WHITE);
    draw_box(x+3,y,1,5,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    x=x+6;
    // Y
    draw_box(x,y,1,1,WHITE);
    draw_box(x+1,y+1,1,1,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    draw_box(x+3,y+1,1,1,WHITE);
    draw_box(x+4,y,1,1,WHITE);
    draw_box(x+1,y+3,1,1,WHITE);
    draw_box(x,y+4,1,1,WHITE);
    x=x+8;
    //P
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,4,1,WHITE);
    draw_box(x,y+2,4,1,WHITE);
    draw_box(x+3,y,1,3,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    x=x+6;
    //O
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    draw_box(x+4,y,1,5,WHITE);
    x=x+6;
    //N
    draw_box(x,y,2,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x+2,y+1,1,1,WHITE);
    draw_box(x+3,y+2,1,1,WHITE);
    draw_box(x+4,y+3,1,1,WHITE);
    draw_box(x+5,y+4,1,1,WHITE);
    draw_box(x+6,y,1,5,WHITE);
    x=x+8;
    //G
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+4,3,1,WHITE);
    draw_box(x+2,y+2,3,1,WHITE);
    draw_box(x+2,y+2,1,3,WHITE);
    draw_box(x+4,y+2,1,3,WHITE);
    x=100,y=110;
    //E
    draw_box(x,y,3,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+2,3,1,WHITE);
    draw_box(x,y+4,3,1,WHITE);
    x=x+4;
    //N
    draw_box(x,y,2,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x+2,y+1,1,1,WHITE);
    draw_box(x+3,y+2,1,1,WHITE);
    draw_box(x+4,y+3,1,1,WHITE);
    draw_box(x+5,y+4,1,1,WHITE);
    draw_box(x+6,y,1,5,WHITE);
    x=x+8;
    //T
    draw_box(x,y,5,1,WHITE);
    draw_box(x+2,y,1,5,WHITE);
    x=x+6;
    //E
    draw_box(x,y,3,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+2,3,1,WHITE);
    draw_box(x,y+4,3,1,WHITE);
    x=x+4;
    //R
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y+2,5,1,WHITE);
    draw_box(x+4,y,1,3,WHITE);
    draw_box(x+4,y+4,1,1,WHITE);
    draw_box(x+3,y+3,1,1,WHITE);
    x=x+8;
    //1
    draw_box(x,y,1,5,WHITE);
    x=x+2;
    //1
    draw_box(x,y,1,5,WHITE);
    x=x+4;
    //T
    draw_box(x,y,5,1,WHITE);
    draw_box(x+2,y,1,5,WHITE);
    x=x+6;
    //O
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    draw_box(x+4,y,1,5,WHITE);
    x=x+8;
    //P
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,4,1,WHITE);
    draw_box(x,y+2,4,1,WHITE);
    draw_box(x+3,y,1,3,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    x=x+6;
    //L
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    x=x+6;
    //A
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,4,1,WHITE);
    draw_box(x,y+2,4,1,WHITE);
    draw_box(x+3,y,1,5,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    x=x+6;
    // Y
    draw_box(x,y,1,1,WHITE);
    draw_box(x+1,y+1,1,1,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    draw_box(x+3,y+1,1,1,WHITE);
    draw_box(x+4,y,1,1,WHITE);
    draw_box(x+1,y+3,1,1,WHITE);
    draw_box(x,y+4,1,1,WHITE);
    x=x+8;
    //S
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y,1,3,WHITE);
    draw_box(x,y+2,5,1,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    draw_box(x+4,y+2,1,3,WHITE);
    x=x+6;
    //I
    draw_box(x,y,5,1,WHITE);
    draw_box(x+2,y,1,5,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    x=x+6;
    //M
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,2,1,WHITE);
    draw_box(x+2,y+1,1,1,WHITE);
    draw_box(x+3,y+2,1,1,WHITE);
    draw_box(x+4,y+1,1,1,WHITE);
    draw_box(x+5,y,2,1,WHITE);
    draw_box(x+6,y,1,5,WHITE);
    x=x+8;
    //P
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,4,1,WHITE);
    draw_box(x,y+2,4,1,WHITE);
    draw_box(x+3,y,1,3,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    x=x+6;
    //L
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    x=x+6;
    //E
    draw_box(x,y,3,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+2,3,1,WHITE);
    draw_box(x,y+4,3,1,WHITE);
    x=x+6;
    //P
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,4,1,WHITE);
    draw_box(x,y+2,4,1,WHITE);
    draw_box(x+3,y,1,3,WHITE);
    draw_box(x+2,y+2,1,1,WHITE);
    x=x+6;
    //O
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y+4,5,1,WHITE);
    draw_box(x+4,y,1,5,WHITE);
    x=x+6;
    //N
    draw_box(x,y,2,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x+2,y+1,1,1,WHITE);
    draw_box(x+3,y+2,1,1,WHITE);
    draw_box(x+4,y+3,1,1,WHITE);
    draw_box(x+5,y+4,1,1,WHITE);
    draw_box(x+6,y,1,5,WHITE);
    x=x+8;
    //G
    draw_box(x,y,5,1,WHITE);
    draw_box(x,y,1,5,WHITE);
    draw_box(x,y+4,3,1,WHITE);
    draw_box(x+2,y+2,3,1,WHITE);
    draw_box(x+2,y+2,1,3,WHITE);
    draw_box(x+4,y+2,1,3,WHITE);



}

void next_screen () {
    int x,y;
    for ( x = 0; x < 320; x++) {
        for ( y = 0; y < 240; y++) {
            plot_pixel(x, y,BLACK);
        }
    }
    dat();
   

}

void plot_pixel(int x, int y, short int line_colour) {
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_colour;
}

void draw_box(int x, int y, int width, int height, short int colour) {
    int x_increment,y_increment;
	for ( x_increment = 0 ; x_increment < width; x_increment++) {
		for ( y_increment = 0 ; y_increment < height; y_increment++) {
			plot_pixel(x + x_increment, y + y_increment, colour);
		}
	}
}

int bar_controller_1(int y1){
            if(key_value%4==1&&y1<=200){
            draw_box(0,y1,10,39,BLACK);

            y1=y1+1;
            draw_box(0,y1,10,39,WHITE);

        }
	   	if(key_value%4==2&&y1>=10){
            draw_box(0,y1+30,10,39,BLACK);

            y1=y1-1;
            draw_box(0,y1,10,39,WHITE);


        }
        
        return y1;
}

int bar_controller_2(int y3){
        if(key_value/4==1&&y3<=200){

            draw_box(309,y3,10,39,BLACK);
            y3=y3+1;

            draw_box(309,y3,10,39,WHITE);
        }
	   	if(key_value/4==2&&y3>=10){

            draw_box(309,y3+30,10,39,BLACK);
            y3=y3-1;


            draw_box(309,y3,10,39,WHITE);
        }
        
        return y3;
}

int bar_controller(int y1){
        if(key_value%4==1&&y1<=200){
            draw_box(0,y1,10,39,BLACK);
            draw_box(309,y1,10,39,BLACK);
            y1=y1+1;
            draw_box(0,y1,10,39,WHITE);
            draw_box(309,y1,10,39,WHITE);
        }
	   	if(key_value%4==2&&y1>=10){
            draw_box(0,y1+30,10,39,BLACK);
            draw_box(309,y1,10,39,BLACK);
            y1=y1-1;
            draw_box(0,y1,10,39,WHITE);
            draw_box(309,y1,10,39,WHITE);

        }
        
        return y1;
}

void dislpay_2p(int score_1,int score_2){
    int arr_1[3];
    int arr_2[3];
    for(int i=0;i<3;i++){
        arr_1[i]=ma[score_1%10];
        score_1=score_1/10;
    }
    for(int i=0;i<3;i++){
        arr_2[i]=ma[score_2%10];
        score_2=score_2/10;
    }
    *HEX1_ptr=(0<<24)|(arr_1[2]<<16)|(arr_1[1]<<8)|arr_1[0];
    *HEX2_ptr=(64<<24)|(arr_2[2]<<16)|(arr_2[1]<<8)|arr_2[0];

}

void display_1p(int score){
    int arr_1[3];
    
    for(int i=0;i<3;i++){
        arr_1[i]=ma[score%10];
        score=score/10;
    }
    *HEX2_ptr=(0<<24)|(arr_1[2]<<16)|(arr_1[1]<<8)|arr_1[0];
    
}

int main(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;//location of pixxel buffer controller
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen();
   // draw_box(320,240,0,0,WHITE);
   draw_box(0,0,10,40,WHITE);
   draw_box(309,0,10,40,WHITE);
   int x2=10,y2=0,y3=0;
   draw_box(x2,y2,10,10,NAVY);
   int y1=0;
   bool direct1=true;
   bool direct2=true;
    float delay_time=5000;
    int score_1=0;
    int score_2=0;
    int score=0;
    int game_window=0;
    next_screen();
   while(1){
    if(game_window==1){
        if(direct1){
            if(direct2){
                draw_box(x2,y2,10,10,BLACK);
                x2=x2+1;
                y2=y2+1;
                draw_box(x2,y2,10,10,NAVY);
                if(y2+10==240){
                    direct2=false;
                }
                if(x2+10==309){
                    if(y2>=y3-4&&y2<=y3+34){
                        direct1=false;
                        delay_time=delay_time/1.1;
                        score_2++;
                    }else{
                        draw_box(x2,y2,10,10,BLACK);
                        x2=x2+1;
                        y2=y2+1;
                        draw_box(x2,y2,10,10,NAVY);
                        for (int delay = 0; delay < delay_time; delay++);
                        next_screen();
                        game_window=0;
                    }
                }
            }else{
                draw_box(x2,y2,10,10,BLACK);
                x2=x2+1;
                y2=y2-1;
                draw_box(x2,y2,10,10,NAVY);
                if(x2+10==309){
                    if(y2>=y3-4&&y2<=y3+34){
                        direct1=false;
                        delay_time=delay_time/1.1;
                        score_2++;
                    }else{
                        draw_box(x2,y2,10,10,BLACK);
                        x2=x2+1;
                        y2=y2-1;
                        draw_box(x2,y2,10,10,NAVY);
                        for (int delay = 0; delay < delay_time; delay++);
                        next_screen();
                        game_window=0;
                    }
                }
                if(y2==0){
                    direct2=true;
                }
            }
        }else{

                if(direct2){
                    draw_box(x2,y2,10,10,BLACK);
                    x2=x2-1;
                    y2=y2+1;
                    draw_box(x2,y2,10,10,NAVY);
                    if(x2==10){
                        if(y2>=y1-4&&y2<=y1+34){
                        direct1=true;
                        delay_time=delay_time/1.1;
                        score_1++;
                        }else{
                            draw_box(x2,y2,10,10,BLACK);
                            x2=x2-1;
                            y2=y2+1;
                            draw_box(x2,y2,10,10,NAVY);
                            for (int delay = 0; delay < delay_time; delay++);
                            next_screen();
                            game_window=0;
                        }
                    }
                    if(y2+10==240){
                        direct2=false;
                    }
                }else{
                    draw_box(x2,y2,10,10,BLACK);
                    x2=x2-1;
                    y2=y2-1;
                    draw_box(x2,y2,10,10,NAVY);
                    if(y2==0){
                        direct2=true;
                    }
                    if(x2==10){
                        if(y2>=y1-4&&y2<=y1+34){
                            direct1=true;
                            delay_time=delay_time/1.1;
                            score_1++;
                        }else{
                            draw_box(x2,y2,10,10,BLACK);
                            x2=x2-1;
                            y2=y2-1;
                            draw_box(x2,y2,10,10,NAVY);
                            for (int delay = 0; delay < delay_time; delay++);
                            next_screen();
                            game_window=0;
                        }
                    }
                }

        }
		key_value = *KEY_ptr;
        dislpay_2p(score_1,score_2);
        y1=bar_controller_1(y1);
        y3=bar_controller_2(y3);
        delay_time=delay_time/1.001;
        for (int delay = 0; delay < delay_time/10; delay++);
    }else if(game_window==0){
        while(1){
        switch_value = *SW_ptr;
        if(switch_value==1){
            game_window=1;
            clear_screen();
            // draw_box(320,240,0,0,WHITE);
            draw_box(0,0,10,40,WHITE);
            draw_box(309,0,10,40,WHITE);
            x2=10,y2=0,y3=0,y1=0;
            
            direct1=true;
            direct2=true;
            int r=random();
            if(r%2==1){
                x2=300;
                direct1=false;
            }
            draw_box(x2,y2,10,10,NAVY);
            dislpay_2p(0,0);
            score_1=0,score_2=0;
            delay_time=50000;
            transition_screen();
            break;
        }else if(switch_value==2){
            game_window=2;
            clear_screen();
            // draw_box(320,240,0,0,WHITE);
            draw_box(0,0,10,40,WHITE);
            draw_box(309,0,10,40,WHITE);
            x2=10,y2=0,y1=0;
           
            direct1=true;
            direct2=true;
            int r=random();
            printf("%d",r);
            if(r%2){
                x2=300;
                direct1=false;
            }
            draw_box(x2,y2,10,10,NAVY);
            dislpay_2p(0,0);
            score=0;
            delay_time=5000;
            transition_screen();
            break;
        }
        }
    }else if(game_window=2){
        if(direct1){
            if(direct2){
                draw_box(x2,y2,10,10,BLACK);
                x2=x2+1;
                y2=y2+1;
                draw_box(x2,y2,10,10,NAVY);
                if(y2+10==240){
                    direct2=false;
                }
                if(x2+10==309){
                    if(y2>=y1-4&&y2<=y1+34){
                        direct1=false;
                        score++;
                    }else{
                        draw_box(x2,y2,10,10,BLACK);
                        x2=x2+1;
                        y2=y2+1;
                        draw_box(x2,y2,10,10,NAVY);
                        for (int delay = 0; delay < delay_time; delay++);
                        next_screen();
                        game_window=0;
                    }
                }
            }else{
                draw_box(x2,y2,10,10,BLACK);
                x2=x2+1;
                y2=y2-1;
                draw_box(x2,y2,10,10,NAVY);
                if(x2+10==309){
                    if(y2>=y1-4&&y2<=y1+34){
                        direct1=false;
                        score++;
                    }else{
                        draw_box(x2,y2,10,10,BLACK);
                        x2=x2+1;
                        y2=y2-1;
                        draw_box(x2,y2,10,10,NAVY);
                        for (int delay = 0; delay < delay_time; delay++);
                        next_screen();
                        game_window=0;
                    }
                }
                if(y2==0){
                    direct2=true;
                }
            }
        }else{

                if(direct2){
                    draw_box(x2,y2,10,10,BLACK);
                    x2=x2-1;
                    y2=y2+1;
                    draw_box(x2,y2,10,10,NAVY);
                    if(x2==10){
                        if(y2>=y1-4&&y2<=y1+34){
                        direct1=true;
                        score++;
                        }else{
                            draw_box(x2,y2,10,10,BLACK);
                            x2=x2-1;
                            y2=y2+1;
                            draw_box(x2,y2,10,10,NAVY);
                            for (int delay = 0; delay < delay_time; delay++);
                            next_screen();
                            game_window=0;
                        }
                    }
                    if(y2+10==240){
                        direct2=false;
                    }
                }else{
                    draw_box(x2,y2,10,10,BLACK);
                    x2=x2-1;
                    y2=y2-1;
                    draw_box(x2,y2,10,10,NAVY);
                    if(y2==0){
                        direct2=true;
                    }
                    if(x2==10){
                        if(y2>=y1-4&&y2<=y1+34){
                            direct1=true;
                            score++;
                        }else{
                            draw_box(x2,y2,10,10,BLACK);
                            x2=x2-1;
                            y2=y2-1;
                            draw_box(x2,y2,10,10,NAVY);
                            for (int delay = 0; delay < delay_time; delay++);
                            next_screen();
                            game_window=0;
                        }
                    }
                }

        }
		key_value = *KEY_ptr;
        display_1p(score);
        y1=bar_controller(y1);
        delay_time=delay_time/1.001;
        for (int delay = 0; delay < delay_time; delay++);
    }
   }
}

