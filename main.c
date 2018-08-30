#include <allegro.h>
#include <alleggl.h>
#include <GL/glu.h>
#include "StereoGL.h"
#include <dirent.h>

char *help_str="StereoGL - StereoConfig \n-----------------------------

StereoConfig.exe is part of the StereoGL engine.
It is a configuration program that writes a savefile 'stereogl.cfg'. This savefile contains values
that are recommended to become altered for every different user.
All values got an 'activation' checkbox, letting a StereoGL program know whether it should use this value, or
a default value. It depends on the respective program how these values are used.
Color table files (.ct) are stored in a subordinate folder /ct.

SCREEN DISTANCE
Describes the distance between your face and your screen. In most cases that's 50-70 centimeters.
A short screen distance value causes a wide frustum angle, while a far one a close, 'zoomed'
frustum angle. For maximum realism, you should choose a realistic value.

SCREEN WIDTH & HEIGHT
It's important to enter the right screen size. Note that most of the vendor's monitor diagonals like '17 inch'
are exaggerated. My '15 inch' monitor got 14 inch, my '14 inch' got 12.6, my '17 inch' got 15.5.
Also note that the aspect ratio can differ from the typical 3/4. Some monitors got an aspect ratio of 4/5,
for example.

EYE SEPERATION
Enter your eye seperation (most people have got an eye seperation of circa 6.5 centimeters).
If you've got problems 'seeing' the stereo effect, lowering the eye seperation to 1 or 2 centimeters
will help you 'getting into the 3D view'. But at last you get the best effect with realistic values.

COLOR TABLES
The .ct files you can choose here contain color values for each eye. The color bars help you picking the
right color table for your monitor (very important).
1. Put on your red-green or red-blue 3D glasses.
2. Close your left eye.
3. Cylcle threw the color table box and watch the upperst (red) color bar at the same time.
4. Choose the color table that lets the red bar dissolve best possible";

struct resolution_entry
{
	int w,h;
	char *s;
} resolutions[] =
{
	{  320,  240, " 320x240 "  },
	{  640,  480, " 640x480 "  },
	{  800,  600, " 800x600 "  },
	{ 1024,  768, "1024x768 "  },
	{ 1280, 1024, "1280x1024" },
	{ 1600, 1200, "1600x1200" },
};

bool update_bars=1;
bool loaded=0;
char **dir_content;
char buf[64];
char eye_speration_str[32] = "6.0";
char screen_width_str[32] = "32.4";
char screen_height_str[32] = "24.3";
char screen_distance_str[32] = "60.0";
char bgcolorbuf[32];
GLubyte bg_red, bg_green, bg_blue;
volatile int close_button_pressed = FALSE;
int dir_content_size=0;
int table_selection=0;
DIALOG *dg;

void close_button_handler(void)
{
  close_button_pressed = TRUE;
}END_OF_FUNCTION(close_button_handler);

void get_dir_content()
{
   int i, j=0, k=0;
   DIR *dirHandle;
   struct dirent * dirEntry;
   dirHandle = opendir("ct/.");
   rewinddir(dirHandle);
   if (dirHandle)
   {
      while (0 != (dirEntry = readdir(dirHandle)))
      if(strstr(dirEntry->d_name, ".ct")!=NULL)dir_content_size++;
      rewinddir(dirHandle);
      dir_content = new(char*[dir_content_size]);
      while (0 != (dirEntry = readdir(dirHandle)))
      if(strstr(dirEntry->d_name, ".ct")!=NULL)
      {
         dir_content[k]=new(char[sizeof(dirEntry->d_name)]);
         strcpy(dir_content[k], dirEntry->d_name);
         k++;
      }
      closedir(dirHandle);
   }
}

int find_dir_content(char *name)
{
   int i;
   for(i=0;i<dir_content_size;i++)
   {
      if(strstr(name, dir_content[i]))return(i);
   }
   return(0);
}

static char *resolution_lister (int i, int *size)
{
	if (i < 0) {
		*size = sizeof resolutions / sizeof *resolutions;
		return NULL;
	}
	return resolutions[i].s;
}

static char *color_table_lister (int i, int *size)
{
	if (i < 0) {
		*size = dir_content_size;
		return NULL;
	}
	if(dg[4].d1!=table_selection) // ARGHHHHHHHHHHHHHHHHHHHH!!!!
	{

      sprintf(buf, "./ct/%s", dir_content[dg[4].d1]);
 	   load_color_table(buf);
      get_background_color(&bg_red, &bg_green, &bg_blue);
      update_bars=1;
   }
   table_selection=dg[4].d1;// ARGHHHHHHHHHHHHHHHHHHHH!!!!
	return (dir_content[i]);//zbuffer_depths[i].s;
}

int bg_color_proc(int msg, DIALOG *d, int c)
{
   int ret;
   ret = d_text_proc(msg, d, c);
   return(D_REDRAWME);
}

int d_h_color_bars_proc(int msg, DIALOG *d, int c)
{
   int i;
   unsigned char r0, g0, b0, r1, g1, b1;
   if(update_bars)
   {
      set_dialog_color (d, makecol(0, 0, 0), makecol(bg_red, bg_green, bg_blue));
      sprintf(bgcolorbuf, "r:%d g:%d b:%d ", bg_red, bg_green, bg_blue);
      rectfill(screen, 0, 144, 640, 320, makecol(bg_red, bg_green, bg_blue));
      for(i=0;i<510;i++)
      {
         get_color256(0, i/2, &r0, &g0, &b0);
         get_color256(1, i/2, &r1, &g1, &b1);
         line( screen, d->x+i, d->y, d->x+i, d->y+d->w, makecol24(r0, g0, b0));
         line( screen, d->x+i, d->y+d->w+d->h, d->x+i, d->y+2*d->w+d->h, makecol24(bg_red, bg_green, b1));
         line( screen, d->x+i, d->y+2*d->w+2*d->h, d->x+i, d->y+3*d->w+2*d->h, makecol24(r0, g0, b1));
      }
      update_bars=0;
   }
}

int rectangle_proc(int msg, DIALOG *d, int c)
{
   rectfill(screen, d->x, d->y, d->x+d->w, d->y+d->h, makecol(bg_red, bg_green, bg_blue));
}

int rectangle2_proc(int msg, DIALOG *d, int c)
{
   rect(screen, d->x, d->y, d->x+d->w, d->y+d->h, makecol(bg_red, bg_green, bg_blue));
}

int d_help_button_proc(int msg, DIALOG *d, int c)
{
   int ret;
   ret = d_button_proc(msg, d, c);
   if (ret == D_CLOSE)
   {
      allegro_message(help_str);
      return D_REDRAWME;
   }
   return ret;
}

int d_save_button_proc(int msg, DIALOG *d, int c)
{
  int ret;
  ret = d_button_proc(msg, d, c);
  if (ret == D_CLOSE)
   {
      CFG_FILE cfg;
      sprintf(cfg.ct_file_name, "./ct/%s", dir_content[dg[4].d1]);
      cfg.eye_seperation=atof(eye_speration_str)/100;
      cfg.screen_width=atof(screen_width_str)/100;
      cfg.screen_height=atof(screen_height_str)/100;
      cfg.screen_distance=atof(screen_distance_str)/100;
      cfg.resolution_x=dg[3].w;
      cfg.resolution_y=dg[3].y;
      cfg.is_windowed=(dg[14].flags & D_SELECTED);
      cfg.resolution_index=dg[3].d1;

      cfg.resolution_is_used=     (dg[7].flags ? 1 :0);
      cfg.ct_file_is_used=        (dg[8].flags ? 1 :0);
      cfg.eye_seperation_is_used= (dg[9].flags ? 1 :0);
      cfg.screen_width_is_used=   (dg[10].flags ? 1 :0);
      cfg.screen_height_is_used=  (dg[11].flags ? 1 :0);
      cfg.screen_distance_is_used=(dg[12].flags ? 1 :0);
      cfg.is_windowed_is_used=    (dg[13].flags ? 1 :0);
      cfg.is_windowed=            (dg[14].flags ? 1 :0);
      allegro_message("     saved stereogl.cfg     ");
      save_cfg("stereogl.cfg", &cfg);
      return D_REDRAW;
   }
   return ret;
}

int d_load_proc(int msg, DIALOG *d, int c)
{
   if(!loaded)
   {
      CFG_FILE cfg;
      if(!load_cfg("stereogl.cfg", &cfg))
      {
         sprintf(eye_speration_str, "%.2f", cfg.eye_seperation*100);
         sprintf(screen_width_str, "%.2f", cfg.screen_width*100);
         sprintf(screen_height_str, "%.2f", cfg.screen_height*100);
         sprintf(screen_distance_str, "%.2f", cfg.screen_distance*100);
         dg[3].d1=cfg.resolution_index;
         dg[4].d1=find_dir_content(cfg.ct_file_name);

         dg[7].flags=cfg.resolution_is_used ? D_SELECTED : 0;
         dg[8].flags=cfg.ct_file_is_used ? D_SELECTED : 0;
         dg[9].flags=cfg.eye_seperation_is_used ? D_SELECTED : 0;
         dg[10].flags=cfg.screen_width_is_used ? D_SELECTED : 0;
         dg[11].flags=cfg.screen_height_is_used ? D_SELECTED : 0;
         dg[12].flags=cfg.screen_distance_is_used ? D_SELECTED : 0;
         dg[13].flags=cfg.is_windowed_is_used ? D_SELECTED : 0;
         dg[14].flags=cfg.is_windowed ? D_SELECTED : 0;
         loaded=1;
         return D_USER;
      }
   }
   return D_USER;
}

int d_close_proc(int msg, DIALOG *d, int c)
{
   if(close_button_pressed)return D_CLOSE;
   return D_USER;
}

static int setup (void)
{
	#define RESOLUTION_LIST   4
	#define COLOUR_LIST       6
	#define ZBUFFER_LIST      8
	#define WINDOWED_BOX      9
	#define DOUBLEBUFFER_BOX 10
	#define BUTTON_OK        11

	#define EDIT_BOX_X 268
	#define EDIT_BOX_Y 41

	#define BUTTONS_X 675
	#define BUTTONS_Y 25
	#define BUTTONS_SEP 30
	
	#define RESOLUTION_X 86
	#define RESOLUTION_Y 25

	DIALOG dlg[] = {
    /*	proc                    x                y    w     h    fg   bg  key  flags d1 d2 dp */
   {d_box_proc,                  0,               0, 800,  128,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL },
   {d_box_proc,                  0,             128, 800,   16,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL },
   {d_box_proc,                640,             264, 160,   56,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL },
   {d_list_proc,      RESOLUTION_X, RESOLUTION_Y+10,  96,   52,   0,   0,  0,         0, 0, 0, (void*)resolution_lister, NULL, NULL },
   {d_list_proc,               640,             144, 160,  120,   0,   0,  0,         0, 0, 0, (void*)color_table_lister, NULL, NULL },
   {rectangle2_proc,    EDIT_BOX_X,    EDIT_BOX_Y-6, 264,   52,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL },
   {rectangle2_proc,    EDIT_BOX_X,             106, 264,   13,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL },
   {d_check_proc,     RESOLUTION_X,    RESOLUTION_Y,   8,    8,   0,   0,  0,         0, 1, 0, (void*)"resolution", NULL, NULL },
   {d_check_proc,              340,             132,   8,    8,   0,   0,  0,         0, 1, 0, (void*)"color table", NULL, NULL },
   {d_check_proc,     EDIT_BOX_X+4,      EDIT_BOX_Y,   8,    8,   0,   0,  0,         0, 1, 0, (void*)"eye seperation  (cm): ", NULL, NULL },
   {d_check_proc,     EDIT_BOX_X+4,   EDIT_BOX_Y+10,   8,    8,   0,   0,  0,         0, 1, 0, (void*)"screen width    (cm): ", NULL, NULL },
   {d_check_proc,     EDIT_BOX_X+4,   EDIT_BOX_Y+20,   8,    8,   0,   0,  0,         0, 1, 0, (void*)"screen height   (cm): ", NULL, NULL },
   {d_check_proc,     EDIT_BOX_X+4,   EDIT_BOX_Y+30,   8,    8,   0,   0,  0,         0, 1, 0, (void*)"screen distance (cm): ", NULL, NULL },
   {d_check_proc,       EDIT_BOX_X,              96,   8,    8,   0,   0,  0,         0, 1, 0, (void*)"screen mode", NULL, NULL },
   {d_check_proc,     EDIT_BOX_X+4,             109,   77,   8,   0,   0,  0,         0, 1, 0, (void*)"windowed", NULL, NULL },
   {d_edit_proc,    EDIT_BOX_X+186,      EDIT_BOX_Y,   64,  48,   255, 0,  0,         0,       sizeof(eye_speration_str)-1,   0,    eye_speration_str,       NULL, NULL  },
   {d_edit_proc,    EDIT_BOX_X+186,   EDIT_BOX_Y+10,   64,  48,   255, 0,  0,         0,       sizeof(screen_width_str)-1,   0,    screen_width_str,       NULL, NULL  },
   {d_edit_proc,    EDIT_BOX_X+186,   EDIT_BOX_Y+20,   64,  48,   255, 0,  0,         0,       sizeof(screen_height_str)-1,   0,    screen_height_str,       NULL, NULL  },
   {d_edit_proc,    EDIT_BOX_X+186,   EDIT_BOX_Y+30,   64,  48,   255, 0,  0,         0,       sizeof(screen_distance_str)-1,   0,    screen_distance_str,       NULL, NULL  },
   {d_ctext_proc,                0,              10,  800,   0,   0,   0,  0,         0, 0, 0, (void*)"___________________________", NULL, NULL },
   {d_ctext_proc,                0,               8,  800,   0,   0,   0,  0,         0, 0, 0, (void*)"StereoGL - StereoConfig.exe", NULL, NULL },
   {d_ctext_proc,              640,             132,  160,  16,   0,   0,  0,         0, 0, 0, (void*)"./ct/", NULL, NULL },
   {d_ctext_proc,              640,             268,  160,  16,   0,   0,  0,         0, 0, 0, (void*)"table's", NULL, NULL },
   {d_ctext_proc,              640,             278,  160,  16,   0,   0,  0,         0, 0, 0, (void*)"background color:", NULL, NULL },
   {bg_color_proc,             653,             296,  160,  16,   0,   0,  0,         D_REDRAWME, D_REDRAWME, D_REDRAWME, bgcolorbuf, NULL, NULL },
   {d_save_button_proc,  BUTTONS_X,       BUTTONS_Y,   96,  18, 255,   0,  0,         D_EXIT,  0, 0, (void*)"save",   NULL, NULL  },
   {d_help_button_proc,  BUTTONS_X, BUTTONS_Y+BUTTONS_SEP,96,18,255,   0, KEY_F1<<8,  D_EXIT,  0, 0, (void*)"Help",   NULL, NULL  },
   {d_button_proc,       BUTTONS_X, BUTTONS_Y+2*BUTTONS_SEP,96,18,0,   0,  0,         D_EXIT, 0, 0, (void*)"Close", NULL, NULL },
   {d_h_color_bars_proc,        64,             180,   25,  12,   0,   0,  0,         D_REDRAW, 0, 0, (void*)NULL, NULL, NULL },
   {d_load_proc,                 0,               0,    0,   0,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL },
   {d_close_proc,                0,               0,    0,   0,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL },
   {NULL,                        0,               0,    0,   0,   0,   0,  0,         0, 0, 0, (void*)NULL, NULL, NULL }
	};
   dg=dlg;
	set_dialog_color (dlg, makecol(0, 0, 0), makecol(212, 208, 200));
   do_dialog (dlg, 4);
}

void main()
{
	allegro_init();
   set_color_depth(24);
   if
   (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 320, 0, 0) != 0) {
      allegro_message("Error setting graphics mode\n%s\n", allegro_error);
      return;
   }
   LOCK_FUNCTION(close_button_handler);
   set_close_button_callback(close_button_handler);
   get_dir_content();
	install_keyboard();
	install_mouse();
	setup();
	remove_keyboard();
	remove_mouse();
	remove_allegro_gl();
	return;
}
END_OF_MAIN();
