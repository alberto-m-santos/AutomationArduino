#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include "timer_tools.h"
#include "udp_remote.h"
#include "s_a_hacks.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>



////////////////
#ifdef WIN32
  void GOTOXY(int XPos, int YPos) { COORD Coord; Coord.X = XPos; Coord.Y = YPos; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord); }
  #define CLEAR() system("cls")
#else
  #define CLEAR() printf("\033[H\033[J")
  #define GOTOXY(x,y) printf("\033[%d;%dH", (x), (y))
#endif


/////////////////


#define ESQUERDA out.b0
#define BOMBA_V5 out.b1
#define MOTOR_PA out.b2
#define V1       out.b3
#define V2       out.b4
#define V3       out.b5
#define V4       out.b6
#define V7       out.b7

#define M_max    in.b0
#define M_min    in.b1
#define prato1   in.b2
#define prato2   in.b3
#define paragem  in.b4
#define ciclo    in.b5


ms_timer timer1;  // Declare a timer (miliseconds)
ms_timer timer2;  // Declare a timer (miliseconds)

int main()
{
    initialize_all();
	
		 	start_timer(&timer1, 3000);  // initialize timer for first time (miliseconds)
		   	start_timer(&timer2, 4000);  // initialize timer for first time (miliseconds)

bool x0, x1=false, x2=false, x3=false, x4=false, x5=false, x6=false, x7=false, x8=false, x9=false, x10=false, x100, x101=false, x102=false, M100=false;
bool t0=false, t1=false, t2=false, t3=false, t4=false, t5=false, t6=false, t7=false, t8=false, t9=false, t10=false, t11=false, t12=false, t13=false, t14=false;

	/* ativar etapas iniciais*/

	x0=true;
x100=true;

	while (1)              // infinite control cycle
	{
		read_all_inputs(); // global variables "in.b0" up to "in.b7" are updated from <<buttons>>
        GOTOXY(1,1);
        printf("%09.1f",cur_time/10.0);

		GOTOXY(1,3);
		printf("Entradas: M_MAX=%01d, M_MIN=%01d, PRATO1=%01d, PRATO2=%01d, PARAG=%01d, CICLO=%01d\n\r",
    		              M_max,      M_min  ,    prato1 ,     prato2,     paragem, ciclo);
    
		GOTOXY(1,5);
		printf("Saidas: ESQ=%01d B_V5=%01d MOT_PA=%01d V1=%01d V2=%01d V3=%01d V4=%01d V7=%01d\n\r",
    		            ESQUERDA, BOMBA_V5, MOTOR_PA, V1, V2, V3, V4, V7);

	GOTOXY(1,7);
		printf("Estados: X0=%d x1=%d x2=%d x3=%d x4=%d x5=%d x6=%d x7=%d x8=%d x9=%d x10=%d timer1:%d timer2:%d  timer1=%d\n\r",
    		            x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, get_timer(&timer1), get_timer(&timer2), timer1);
		
		
			t11= (x100 && (x2 || x3 || x4 || x5 || x6 || x7 || x8)) ? 1:0;
			t12= (x9 && x101) ? 1:0;
			t13= (paragem && x101) ? 1:0;
			t14= (!paragem && x102) ? 1:0;

	
	/* desligar etapas a montante */
		if(t11) 
			x100=0;
		if(t12)
			x101=0;
		if(t13) 
			x101=0;
		if(t14) 
			x102=0;
	
		/* ligar etapas a montante */

		if(t11) {
			x101=1;
		}
		if(t12)
		{
		x100=1;
		}	
		if(t13) {
			x102=1;
		}
		if(t14) {
			x100=1;
		}	
			/* ligar/desligar saidas */
		if(x100){
			M100=0;
		}
		if(x102){
			M100=1;
			x0=1;
		}
		
		if(M100){ //Var. para desligar saidas
			ESQUERDA=0; 
 			BOMBA_V5=0;
 			MOTOR_PA=0;
 			V1=0;
 			V2=0;    
	    	V3=0;   
 	    	V4=0;  
	    	V7=0;  
		}
		
		
		/* calcular trans. disparadas*/
		t0= (ciclo && x0) ? 1:0;
		t1= (x1) ? 1:0;
		t2= (prato1 && x2) ? 1:0;
		t3= (prato2 && x3) ? 1:0;
		t4= (M_max && x4 && x5) ? 1:0;
		t5= (M_max && prato1 && prato2 && x6) ? 1:0;
		t6= (!M_max && x6) ? 1:0;
		t7= (M_max) && x7 ? 1:0;
		t8= (get_timer(&timer1) && x8) ? 1:0;
		t9= (get_timer(&timer2) && x9) ? 1:0;
		t10= (M_min && x10) ? 1:0;

		/* desligar etapas a montante */
		if(t0) 
			x0=0;
		if(t1)
			x1=0;
		if(t2) 
			x2=0;
		if(t3)
			x3=0;
		if(t4) {
			x4=0;
			x5=0;
		}
		if(t5)
			x6=0;
		if(t6) 
			x6=0;
		if(t7)
			x7=0;
		if(t8) 
			x8=0;
		if(t9)
			x9=0;
		if(t10)
			x10=0;

		/* ligar etapas a montante */

		if(t0) {
			x1=1;
		}
		if(t1)
		{
		x2=1;
		x3=1;
		}	
		if(t2) {
			x4=1;
		}
		if(t3)
			x5=1;
		if(t4) {
			x6=1;
		}
		if(t5)
		{
			x8=1;
		   	start_timer(&timer1, 3000);  // initialize timer for first time (miliseconds)

		}	
		if(t6) {
			x7=1;
		}
		if(t7)
			x6=1;
		if(t8) {
			x9=1;
			start_timer(&timer2, 4000);  // initialize timer for first time (miliseconds)
		}
		if(t9)
			x10=1;
		if(t10) {
			x0=1;
		}
		/* ligar/desligar saidas */
		if(x1){
			V7=0;
			V1=1;
			V3=1;
			BOMBA_V5=1;
		}

		if(x4){
			V1=0;
		}
		
		if(x5){
			V3=0;
		}
		
		if(x6){
			BOMBA_V5=0;
		}
		
		if(x7){
			BOMBA_V5=1;
		}
		
		if(x8){
			MOTOR_PA=1;
			ESQUERDA=1;
			V2=1;
			V4=1;
		}
		
		if(x9){
			V2=0;
			V4=0;
			ESQUERDA=0;
		}
		
		if(x10){
			MOTOR_PA=0;
			V7=1;
		}
		fflush(stdout);	

		write_all_outputs(); // write global variables "out.b0" up to "out.b7" the <<leds>>
       
		if (kbhit()) { 
			if (getch()==27) break;
		}
	}
	
	printf("\n\r\n\r      Acabei ;) \n\r\n\r");
    fflush(stdout);	
	
	return 0;
}


