/*

Copyright (C) 2015-2018 Night Dive Studios, LLC.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
/*
 * $Header: n:/project/lib/src/edms/RCS/test_bed.cc 1.1 1994/02/28 17:07:22 roadkill Exp $
 */

//	Some graphical output routines for the Physics testbed program. At some point
//	this should all be further generalized to object based rendering...
//	===================================================================



//#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include "EDMS_Int.h"
#include "lg.h"

//#include "mout.h"

#include "2d.h"
extern int gen_rect( short, short, short, short );
extern int gen_int_circle (short x0, short y0, short r);

#include "simple.h"

extern WindowPtr	gMainWindow;


//---------------------------
// PROTOTYPES
//---------------------------
void ShowScreenCoords(char *tag, Q X, Q Y, Q Z);

void sample_field(void);
void draw_surface(void);
void draw_jello_cube(void);
void draw_octahedron(void);
void draw_chair(void);
void silly_marble(void);
void silly_biped(void);
void screen_coords( Q, Q, Q );
void transproj( Q X, Q Y, Q Z, int object );
void no_t_screen_coords( Q, Q, Q );
void make_terrain( int harshness );
Q test_bed_terrain( Q X, Q Y, int deriv );


//-------------------------------------------------------------------------
//	Get the state vectors and such, and define the graphics polygon data...
//-------------------------------------------------------------------------

extern Q 	*utility_pointer[MAX_OBJ];	// Biped skeletons, Jello translucencies,
										// etc...

//-------------------------------------------------------------------------
//	These are used to draw trails behind things...
//-------------------------------------------------------------------------
int	points = 0,								//Number so far...
		proj[2];


//-------------------------------------------------------------------------
//	Here is the test surface in raw form...
//-------------------------------------------------------------------------
Q	surface[10][10][3];
//Q	surface[2][2][3];


//-------------------------------------------------------------------------
//	This is ridiculous...
//-------------------------------------------------------------------------
static Q	fix_twenty = 20,
			fix_zero = 0;

//-------------------------------------------------------------------------
//	This is worse...
//-------------------------------------------------------------------------
static Q	one_point_five = 1.5,
			x_offset = -1.,
			y_offset = 125.,
			z_offset = 10.;




//========================================================================
//	Graphics routines (please don't laugh!);
//========================================================================

//	Setup the 2D...
//	===============
void setup_graphics()
{
	simp_startup();
}

//	Kill the 2D...
//	==============
void kill_graphics()
{
	simp_shutdown();
}



//	Some debugging stuff...
//	=========================
void ShowScreenCoords(char *tag, Q X, Q Y, Q Z)
{
	GrafPtr		savePort;
	RGBColor	black = {0, 0, 0};
	RGBColor	col = {0xFFFF, 0xAAAA, 0xAAAA};
	Rect			r;
	char			Xb[16];
	char			Yb[16];
	char			Zb[16];
	char			buff[256];

	SetRect(&r, 0, -30, 600, 0);
	PaintRect(&r);

	fix_sprint (Xb, X.to_fix());
	fix_sprint (Yb, Y.to_fix());
	fix_sprint (Zb, Z.to_fix());
	sprintf(buff, "%s X:%s Y:%s Z:%s -> x:%d y:%d", tag,
								Xb, Yb, Zb, proj[0], proj[1]);

	TextFont(geneva);
	TextSize(9);
	TextMode(srcOr);

	MoveTo(1, -15);
	RGBForeColor(&col);
	DrawText(buff, 0, strlen(buff));
	RGBForeColor(&black);
}



//========================================================================
//	Higher-Level graphics routines (keep laughing)...
//========================================================================

//	Draw the screen, with page flipping and all that...
//	================================================
void draw_object(int page)
{
	// simp_clear();
	// Let's try to make this a bit faster.
	PaintRect(&gMainWindow->portRect);

//	Shut up, would you please shut up!
//	----------------------------------
	page += 0;
	silly_marble();
//	silly_biped();
}


//	Draw the damned object.
//	=======================
void sample_field()
{
	int number = 0;
//char	text_number[3];

	for ( number = 0; number < MAX_OBJ && S[number][0][0] > END; number ++ )
	{
		if ( I[number][30] == FIELD_POINT )
		{

//			Where is the damn thing...
//			==========================
			screen_coords( S[number][0][0], S[number][1][0], S[number][2][0] );

//			itoa( number, text_number, 10 );
//			outtextxy( proj[0], proj[1], text_number );
//			circle( proj[0], proj[1], 3);
		}
	}

//	All Done...
//	===========

}


//	See below...
//	============
#define gr_ix_line( a, b, c, d ) gr_fix_line( (a)<<16, (b)<<16, (c)<<16, (d)<<16 )


//========================================================================
//	Draw the jello cube...
//========================================================================
void	draw_jello_cube()
{
	static int sc[10][2] =
		{ 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }; //Temp screen coords...
	int	count = 0;


//	Connect the dots...
//	-------------------
	gr_set_fcolor( 0 );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[0][0], sc[0][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[4][0], sc[4][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[7][0], sc[7][1], sc[6][0], sc[6][1] );
	gr_ix_line( sc[6][0], sc[6][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[6][0], sc[6][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[7][0], sc[7][1] );

//	Store the screen coords...
//	--------------------------
	for (count = 0; count < 9; count++ )
	{
		screen_coords( S[count][0][0], S[count][1][0], S[count][2][0] );
		sc[count][0] = proj[0];	sc[count][1] = proj[1];
	}

	gr_set_fcolor( 2 );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[0][0], sc[0][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[4][0], sc[4][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[7][0], sc[7][1], sc[6][0], sc[6][1] );
	gr_ix_line( sc[6][0], sc[6][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[6][0], sc[6][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[7][0], sc[7][1] );
}


//========================================================================
//	Draw the octahedron...
//========================================================================
void	draw_octahedron()
{
	int	count = 0;
	static int sc[10][2] = { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }; //Temp screen coords...

	gr_set_fcolor( 0 );
	gr_ix_line( sc[5][0], sc[5][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[4][0], sc[4][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[0][0], sc[0][1] );



//	Store the screen coords...
//	--------------------------
	for (count = 0; count < 6; count++ )
	{
		screen_coords( S[count][0][0], S[count][1][0], S[count][2][0] );
		sc[count][0] = proj[0];	sc[count][1] = proj[1];
	}

//	Connect the dots...
//	-------------------

//	setcolor( 252 );

	gr_set_fcolor( 1 );
	gr_ix_line( sc[5][0], sc[5][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[4][0], sc[4][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[4][0], sc[4][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[2][0], sc[2][1], sc[0][0], sc[0][1] );
}

//========================================================================
//	Draw the chair...
//========================================================================
void	draw_chair()
{
	static int sc[10][2] = { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }; //Temp screen coords...
	int	count = 0;

//	Connect the dots...
//	-------------------
	gr_set_fcolor( 0 );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[4][0], sc[4][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[7][0], sc[7][1], sc[6][0], sc[6][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[8][0], sc[8][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[8][0], sc[8][1], sc[9][0], sc[9][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[9][0], sc[9][1] );


//	Store the screen coords...
//	--------------------------
	for (count = 0; count < 10; count++ )
	{
		screen_coords( S[count][0][0], S[count][1][0], S[count][2][0] );
		sc[count][0] = proj[0];	sc[count][1] = proj[1];
	}


	gr_set_fcolor( 2 );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[3][0], sc[3][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[4][0], sc[4][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[7][0], sc[7][1], sc[6][0], sc[6][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[5][0], sc[5][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[8][0], sc[8][1], sc[7][0], sc[7][1] );
	gr_ix_line( sc[8][0], sc[8][1], sc[9][0], sc[9][1] );
	gr_ix_line( sc[5][0], sc[5][1], sc[9][0], sc[9][1] );
}





extern "C"
{
fix	Terrain( fix X, fix Y, int deriv );			// this is provided by the user
}



//========================================================================
//	Draw the test surface as a height array, oh, how can we be more well behaved...
//========================================================================
void draw_surface()
{
	static int sc[10][2] = { 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0 }; //Temp screen coords...
	int	count = 0;

//	Connect the dots...
//	-------------------
	gr_set_fcolor( 0 );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[0][0], sc[0][1] );


//	Store the screen coords...
//	--------------------------
	screen_coords( 15, 25, Terrain( 15, 25, 0 ) );
	sc[0][0] = proj[0];	sc[0][1] = proj[1];
	screen_coords( 15, 15, Terrain( 15, 15, 0 ) );
	sc[1][0] = proj[0];	sc[1][1] = proj[1];
	screen_coords( 25, 15, Terrain( 25, 15, 0 ) );
	sc[2][0] = proj[0];	sc[2][1] = proj[1];
	screen_coords( 25, 25, Terrain( 25, 25, 0 ) );
	sc[3][0] = proj[0];	sc[3][1] = proj[1];

	gr_set_fcolor( 7 );
	gr_ix_line( sc[0][0], sc[0][1], sc[1][0], sc[1][1] );
	gr_ix_line( sc[1][0], sc[1][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[2][0], sc[2][1] );
	gr_ix_line( sc[3][0], sc[3][1], sc[0][0], sc[0][1] );
}



static int marblex[MAX_OBJ], marbley[MAX_OBJ];

//========================================================================
//	See if there are any marbles, and draw them accordingly.
//========================================================================
void silly_marble()
{
	int number = 0;
	static int	midx = 0,
				midy = 0;
	Q		X, Y, Z;

//	Erase SINGLE MARBLE!!!!
//	=======================
//	gr_set_fcolor( 0 );
//	gr_int_circle( midx, midy, 5 );
//	gen_int_circle( midx, midy, 5 );

	for ( number = 0; number < MAX_OBJ && S[number][0][0] > END; number ++ )
	{
		if ( (I[number][30] == ROBOT) || (I[number][30] == MARBLE) || (I[number][30] == PELVIS) )
		{

//		Faux terrain...
//		===============
			gr_set_fcolor( 4 );
			for (int a = -3; a < 3;a++ )
			{
				for (int b = -6; b < 6; b+=3 )
				{
					X = S[number][0][0].to_int() + .2*a;
					Y = S[number][1][0].to_int() + .2*b;
					Z = terrain( S[number][0][0].to_int() + .2*a, S[number][1][0].to_int() + .2*b, 0 );

					no_t_screen_coords( X, Y, Z);
					gr_point( proj[0], proj[1] );

if ( I[number][30] == PELVIS )
	ShowScreenCoords("Faux terrain for pelvis:  ", X, Y, Z);
				}
			}
// 			Erase the marble!!!!!!
//			gr_set_fcolor( 0 );
//			gen_int_circle( marblex[number], marbley[number], 5 );

//			Where is the damn thing...
//			==========================
			no_t_screen_coords( S[number][0][0], S[number][1][0], Z );
//			midx = proj[0];	midy = proj[1];
			midx = proj[0]-6;	midy = proj[1]-10;

//if ( I[number][30] == PELVIS )
//	ShowScreenCoords("For pelvis:  ", S[number][0][0], S[number][1][0], S[number][2][0]);
			if ( I[number][30] == ROBOT )
			{
				no_t_screen_coords(  S[number][0][0] + cos( S[number][3][0] ),
								S[number][1][0] + sin( S[number][3][0] ),
								Z);
				gr_set_fcolor( 3 );
				gr_ix_line( midx, midy, proj[0]-6, proj[1]-20 );
			}

			gr_set_fcolor( 6 );
			gen_int_circle( midx, midy, 5 );
//			gen_rect(midx-5, midy-5, midx+5, midy+5);

//			marblex[number] = midx;
//			marbley[number] = midy;
		}
	}

//	All Done...
//	===========
}



//========================================================================
//	Go through the objects, drawing the ones that happen to be bipeds...
//========================================================================
void silly_biped()
{
	int	object = 0,
			ox = 0,
			oy = 0;

//	Get 'em all...
//	--------------
	for ( object = 0; object < MAX_OBJ && S[object][0][0] > END; object ++ )
	{
		if( I[object][30] == BIPED )				//Friend or foe?
		{

//		Faux terrain...
//		===============
			gr_set_fcolor( 4 );
			for (int a = -3; a < 3;a++ )
			{
				for (int b = -6; b < 6; b+=3 )
				{
					no_t_screen_coords( S[object][0][0].to_int() + .2*a, S[object][1][0].to_int() + .2*b,
			    		terrain( S[object][0][0].to_int() + .2*a, S[object][1][0].to_int() + .2*b, 0 ) );
					gr_point( proj[0], proj[1] );
				}
			}

			gr_set_fcolor( 10 );

//		Legs...
//		=======
			transproj( utility_pointer[object][0],
							   utility_pointer[object][1],
							   utility_pointer[object][2], object );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][6],
							   utility_pointer[object][7],
							   utility_pointer[object][8], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][12],
							   utility_pointer[object][13],
							   utility_pointer[object][14], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][18],
							   utility_pointer[object][19],
							   utility_pointer[object][20], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][21],
							   utility_pointer[object][22],
							   utility_pointer[object][23], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][15],
							   utility_pointer[object][16],
							   utility_pointer[object][17], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][9],
							   utility_pointer[object][10],
							   utility_pointer[object][11], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][3],
							   utility_pointer[object][4],
							   utility_pointer[object][5], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];

//		Arms...
//		=======
			transproj( utility_pointer[object][42],
							   utility_pointer[object][43],
							   utility_pointer[object][44], object );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][36],
							   utility_pointer[object][37],
							   utility_pointer[object][38], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][30],
							   utility_pointer[object][31],
							   utility_pointer[object][32], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][33],
							   utility_pointer[object][34],
							   utility_pointer[object][35], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][39],
							   utility_pointer[object][40],
							   utility_pointer[object][41], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][45],
							   utility_pointer[object][46],
							   utility_pointer[object][47], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
			ox = proj[0];	oy = proj[1];


//		Neck...
//		=======
			transproj( utility_pointer[object][24],
							   utility_pointer[object][25],
							   utility_pointer[object][26], object );
			ox = proj[0];	oy = proj[1];
			transproj( utility_pointer[object][27],
							   utility_pointer[object][28],
							   utility_pointer[object][29], object );
			gr_ix_line( ox, oy, proj[0], proj[1] );
		}
	}
}



//========================================================================
//	3D transformation...
//========================================================================
void screen_coords( Q X, Q Y, Q Z )
{
	static Q	distance = 2500.;
	Q	 	x = X, y = Y, z = Z;
	static Q 	cos_alpha = .8, cos_beta = 1, cos_gamma = 1,
						sin_alpha = .2, sin_beta = 0, sin_gamma = 0;

		z = -z;		//Thank you, left handed screen!

//	Rotate on local Euler angles...
//	==============================
		X = 	x*( cos_beta*cos_gamma )
		     	+  y*( cos_gamma*sin_beta*sin_alpha + sin_gamma*cos_alpha )
		     	+  z*( sin_gamma*sin_alpha - cos_gamma*sin_beta*cos_alpha );

		Y = 	x*(-sin_gamma*cos_beta )
		     	+  y*(-sin_gamma*sin_beta*sin_alpha + cos_gamma*cos_alpha )
		     	+  z*( sin_gamma*sin_beta*cos_alpha + cos_gamma*sin_alpha );

		Z = 	x*sin_beta
		     	-  y*cos_beta*sin_alpha
		     	+  z*cos_beta*cos_alpha;


//	Translate globally...
//	=====================

		X += x_offset;
		Y += y_offset;
		Z += z_offset;			//Thanks again...

//	Project...
//	==========

		proj[0] = ( distance*( X / Y ) ).to_int();
		proj[1] = ( 1.25*distance*( Z / Y ) ).to_int();
}


//========================================================================
//	Here is a translation + projection for the bipeds which are rotated by
//	the physics, not the 3D...
//========================================================================
void transproj( Q X, Q Y, Q Z, int object )
{
	Q		distance = 2500;
	Q		three = 3.,
			x_offset = -12.,
			y_offset = 75,
			z_offset = 4;

			Z = -Z;								//Thank you, left handed screen!

//	Translate globally...
//	=====================

		X += x_offset + S[object][0][0];
		Y += y_offset + S[object][1][0];
		Z += z_offset - S[object][2][0];			//Thanks again...

//	Project...
//	==========

		proj[0] = ( distance*( X / Y ) ).to_int();
		proj[1] = ( 1.25*distance*( Z / Y ) ).to_int();
}



//========================================================================
//	Here are screen coords without translation...
//========================================================================
void no_t_screen_coords( Q X, Q Y, Q Z )
{
	static Q	distance = 2500.;
	const Q		three = 3.,
						x_offset = -12.,
						y_offset = 75,
						z_offset = 4;
	Q	x = X,
		y = Y,
		z = Z;

static Q 	cos_alpha = 1,
					cos_beta = 1,
					cos_gamma = 1,
					sin_alpha = 0,
					sin_beta = 0,
					sin_gamma = 0;

//	sincos( -S[object][4][0], &sin_alpha, &cos_alpha );
//	sincos( -S[object][3][0], &sin_beta, &cos_beta );
//	sincos( -S[object][5][0], &sin_gamma, &cos_gamma );



//	Rotate on Euler angles...
//	=========================
		X = x*( cos_alpha*cos_gamma )
		  	+  y*( cos_gamma*sin_alpha*sin_beta - cos_beta*sin_gamma )
		 		+  z*( cos_beta*cos_gamma*sin_alpha + sin_beta*sin_gamma );

		Y =	x*( cos_alpha*sin_gamma )
		 		+  y*( cos_beta*cos_gamma + sin_alpha*sin_beta*sin_gamma )
				+  z*( cos_beta*sin_alpha*sin_gamma - cos_gamma*sin_beta );

		Z = x*( -sin_alpha )
				+  y*( cos_alpha*sin_beta )
				+  z*( cos_alpha*cos_beta );

		Z = -Z;						//Thank you, left handed screen!


//	Translate globally...
//	=====================

		X += x_offset;
		Y += y_offset;
		Z += z_offset;				//Thanks again...

//	Project...
//	==========

		proj[0] = ( distance*( X / Y ) ).to_int();
		proj[1] = ( 1.25*distance*( Z / Y ) ).to_int();
}




//========================================================================
//	Manufacture objects such as cars and terrain...
//========================================================================
void make_terrain( int harshness )
{
	int	X = 0,
			Y = 0;

	Q	scale = harshness*.1;

	for ( X = 0; X < 10; X++ )
	{
		for ( Y = 0; Y < 10; Y++ )
		{


//		surface[X][Y][0] = 0;//.03*2.*( (X-20)*(X-20) + (Y-20)*(Y-20) );		 				//Low...
//		surface[X][Y][1] = 0;//.03*2.*(X-20.);				//Flat...
//		surface[X][Y][2] = 0;//.03*2.*(Y-20.);

			surface[X][Y][0] = .03*2.*( (X-20)*(X-20) + (Y-20)*(Y-20) );		 				//Low...
			surface[X][Y][1] = .03*2.*(X-20.);				//Flat...
			surface[X][Y][2] = .03*2.*(Y-20.);
		}
	}

//	surface[18][18][0] = surface[18][17][0] = surface[17][17][0] = surface[17][18][0] = 1.;

}





//	CAUTION! TERRAIN ISSUES BELOW!
//	==============================



static Q offsetx=0., offsety=0.;

//========================================================================
//	Here is the dummy routine that the individual terrain databases will provide...
//========================================================================
Q test_bed_terrain( Q X, Q Y, int deriv )
{
	extern Q 	ttime;
	Q	serf[3];

	serf[0] = surface[X.to_int()][Y.to_int()][0] + (X-fix_twenty)*offsetx + (Y-fix_twenty)*offsety;// + sin(.8*( ttime + X ) )*sin(.5*( ttime + Y ) );
	serf[2] = surface[X.to_int()][Y.to_int()][2] + offsety;// + .8*cos(.5*( ttime + Y ) )*sin(.8*( ttime + X ) );
	serf[1] = surface[X.to_int()][Y.to_int()][1] + offsetx;// + offsetx;// + .3*cos(.1*( ttime + X ) )*sin(.5*( ttime + Y ) );

	return serf[deriv];
}





//extern "C"
//{

//========================================================================
//	Bridge routines to the object drawers
//========================================================================

#pragma require_prototypes off


void EDMS_draw_object( int page )
{
	draw_object( page );
}

void EDMS_setup_graphics( void )
{
	setup_graphics();
}

void EDMS_kill_graphics( void )
{
	kill_graphics();
}

void EDMS_make_terrain( int harshness)
{
	make_terrain( harshness );
}

fix EDMS_test_bed_terrain( fix X, fix Y, int deriv )
{
	Q ans, x, y;

	x.fix_to( X );
	y.fix_to( Y );
	ans = test_bed_terrain( x, y, deriv );
	return ans.to_fix();
}

void EDMS_set_terrain_offsets( fix x, fix y )
{
	offsetx.fix_to( x );
	offsety.fix_to( y );

//   mout << offsetx << " " << offsety << "\n";
}

#pragma require_prototypes on


//} /* extern "C" */

