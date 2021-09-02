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



////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
void GOTOXY(int XPos, int YPos) {
	COORD Coord;
	Coord.X = XPos;
	Coord.Y = YPos;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Coord);
}
#define CLEAR() system("cls")
#else
#define CLEAR() printf("\033[H\033[J")
#define GOTOXY(x,y) printf("\033[%d;%dH", (x), (y))
#endif


////////////////////////////////////////////////////////////////////////////////


#define V1       out.b3
#define V2       out.b4
#define V3       out.b5

#define prato1   in.b2
#define prato2   in.b3
#define paragem  in.b4



ms_timer timer1;  // Declare a timer (miliseconds)

int main() {
	initialize_all();
	start_timer(&timer1, 5000);  // initialize timer for first time (miliseconds)

	bool x10, x20=false, x50, x60=false, t0=false, t1=false, t6=false, t7=false;

	/* ativar etapas iniciais*/

	x10=true;
	x50=true;

	while (1) {            // infinite control cycle
		read_all_inputs(); // global variables "in.b0" up to "in.b7" are updated from <<buttons>>



		/* ZONA DO DEBUGGER */
		GOTOXY(30,1);
		printf("DEBUGGER\n\n\n");
		GOTOXY(1,5);
		printf("%09.1f",cur_time/10.0);

		GOTOXY(1,7);
		printf("Entradas:  PRATO1=%01d, PRATO2=%01d, PARAG=%01d\n\r",
		       prato1 ,     prato2,      paragem);

		GOTOXY(1,10);
		printf("Saidas: V1=%01d 	V2=%01d 	V3=%01d \n\r",
		       V1,     V2,     V3);
		GOTOXY(8,12);
		printf("estados: x10=%d  x20= %d  x50= %d    x60= %d\n\r",
		       x10,    x20,     x50,      x60);

		GOTOXY(8,14);
		printf("transicao: t0=%d  t1= %d   t6= %d   t7= %d\n\r",
		       t0,    t1,     t6  ,    t7);
		/* ********************************************************************** */

		/********************************/
		/** 	*****PAGINA 1******		**/
		/********************************/
		/* calcular trans. disparadas*/
		t6= (paragem) ? 1:0;
		t7= (!paragem) ? 1:0;

		/* desligar etapas a montante */
		if(t6) {
			x50=0;
			x20=0;
		}
		if(t7)
			x60=0;

		/* ligar etapas a montante */

		if(t6) {
			x10=1;
			x60=1;
		}
		if(t7)
			x50=1;

		/* ligar/desligar saidas */
		if(x50)
			V3=1;

		if(x60)
			V3=0;

		/********************************/
		/** 	*****PAGINA 0******		**/
		/********************************/

		/* calcular trans. disparadas*/
		t0= (prato1) ? 1:0;
		t1= (prato2) ? 1:0;

		/* desligar etapas a montante */
		if(t0)
			x10=0;
		if(t1)
			x20=0;

		/* ligar etapas a montante */
		if(t0)
			x20=1;
		if(t1)
			x10=1;

		/* ligar/desligar saidas */
		if(x10) {
			V1=1;
			V2=0;
		}
		if(x20) {
			V2=1;
			V1=0;
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


