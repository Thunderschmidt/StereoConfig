//HARBST
// Hardware Accelerated Red/Blue STereoscopy
//

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <allegro.h>
#include <alleggl.h>
#include <GL/glu.h>
#include "StereoGL.h"
#include "input3DO.h"
#include "ctload.h"

//#define DEBUG

////////////////////////////////////////////////////////////////////////////////
////////////////////////////        TIMER         //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

volatile int chrono = 0;
float        fcount, fmult, fps_rate = 60.0, fmult_limit = 1.0;
int          frame_count = 0,
             frame_count_time = 0,
             frame_nr=0;

void the_timer(void)
{
	chrono++;
} END_OF_FUNCTION(the_timer);

void proceed_timer()
{
    frame_count++;
    frame_nr++;
    fmult=(chrono-fcount)/1000;
    if(fmult>fmult_limit)fmult=fmult_limit;
    fcount=chrono;
    if (frame_count >= 15)
	{
		fps_rate = frame_count * 1000.0 / (chrono - frame_count_time);
		frame_count -= 15;
		frame_count_time = chrono;
	}
}

float get_fps(){return(fps_rate);}
float get_fmult(){return(fmult);}
int get_chrono(){return(chrono);}
void limit_fmult(float limit){fmult_limit=limit;}

void init_timer()
{
   install_int(the_timer, 1);
	LOCK_FUNCTION(the_timer);
	LOCK_VARIABLE(chrono);
   fcount=chrono;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////        MATH         ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MATR_STACK matr_stack(64);  //zur Simulation von OpenGL Matrixoperationen
GLfloat glm[16];

VECTOR vect(float x, float y, float z)
{
   VECTOR ret;
   ret.x=x;
   ret.y=y;
   ret.z=z;
   return(ret);
}

MATRIX_f matr(float x, float y, float z, float xrot, float yrot, float zrot)
{
   MATRIX_f m;
   get_transformation_matrix_f(&m, 1.0, xrot, yrot, zrot, x, y, z);
   return(m);
}

void glMultMatrix_allegro(MATRIX_f *m) //multipliziere Allegro-Matrix mit OpenGL-Modelview-Matrix
{
   allegro_gl_MATRIX_f_to_GLfloat(m, glm);
   glMultMatrixf(glm);
}

void glMultMatrix_allegro_inv(MATRIX_f *m)//multipliziere Allegro-Matrix mit OpenGL-Modelview-Matrix invers
{
   glm[0]=m->v[0][0];
   glm[1]=m->v[1][0];
   glm[2]=m->v[2][0];

   glm[4]=m->v[0][1];
   glm[5]=m->v[1][1];
   glm[6]=m->v[2][1];
    
   glm[8]=m->v[0][2];
   glm[9]=m->v[1][2];
   glm[10]=m->v[2][2];
    
   glm[12]=0;
   glm[13]=0;
   glm[14]=0;
   glMultMatrixf(glm);
   glTranslatef(-m->t[0],-m->t[1],-m->t[2]);
}

VECTOR trans_matrix_to_vector(MATRIX_f *m)
{
   VECTOR pos;
   pos.x=m->t[0];
   pos.y=m->t[1];
   pos.z=m->t[2];
   return(pos);
}

void translate_matrix_v(MATRIX_f *m, VECTOR *pos)
{
   m->t[0]=pos->x;
   m->t[1]=pos->y;
   m->t[2]=pos->z;
}

void get_matrix_delta(MATRIX_f *m_act, MATRIX_f *m_old, MATRIX_f *m_delta)
{
   MATRIX_f m;
   invert_matrix(m_old, &m);
   matrix_mul_f(m_act, &m, &m);
   invert_matrix(&m, m_delta);
   m_delta->t[0]=m_act->t[0]-m_old->t[0];
   m_delta->t[1]=m_act->t[1]-m_old->t[1];
   m_delta->t[2]=m_act->t[2]-m_old->t[2];

}

bool accelerate(float soll, float *ist, float a)
{
	a*=fmult;
	if (*ist<soll )
	{
		*ist+=a;
		if (*ist > soll)
        {
           *ist=soll;
           return(TRUE);
        }
	}
	if (*ist > soll)
	{
		*ist-=a;
		if (*ist < soll)
        {
           *ist=soll;
           return(TRUE);
        }
	}
    if (*ist==soll) return(TRUE);
    return(FALSE);
}

void MATR_STACK::mul_alg(MATRIX_f *m) //Klasse für simulierten OpenGL-Stack. Wird für Unterobjekte benötigt
{
   matrix_mul_f(&actual, m, &actual);
}

void MATR_STACK::translate(VECTOR *v)
{
   actual.t[0]+=v->x*actual.v[0][0];
   actual.t[1]+=v->x*actual.v[0][1];
   actual.t[2]+=v->x*actual.v[0][2];
     
   actual.t[0]+=v->y*actual.v[1][0];
   actual.t[1]+=v->y*actual.v[1][1];
   actual.t[2]+=v->y*actual.v[1][2];

   actual.t[0]+=v->z*actual.v[2][0];
   actual.t[1]+=v->z*actual.v[2][1];
   actual.t[2]+=v->z*actual.v[2][2];
}

void MATR_STACK::mul(MATRIX_f *m)
{
   translate((VECTOR*)&m->t[0]);
   MATRIX_f m2=*m;
   m2.t[0]=0;m2.t[1]=0;m2.t[2]=0;
   matrix_mul_f(&m2, &actual, &actual);
}

void MATR_STACK::push()
{
   stack[total]= new MATRIX_f;
   *stack[total]=actual;
   total++;
}

void MATR_STACK::pop()
{
   if(total>0)
   {
      total--;
      actual=*stack[total];
      delete stack[total];
   }
}

void MATR_STACK::loadIdentity()
{
   int i;
   for(i=0; i<total; i++)delete stack[i];
   total=0;
   actual=identity_matrix_f;
}

MATRIX_f MATR_STACK::get()
{
   return(actual);
}

MATR_STACK::MATR_STACK(int maxstackmatrices)
{
   stack=new MATRIX_f*[maxstackmatrices];
   total=0;
}

void invert_matrix(MATRIX_f *in, MATRIX_f *out)
{
	out->v[0][0]=in->v[0][0]; out->v[0][1]=in->v[1][0]; out->v[0][2]=in->v[2][0];
	out->v[1][0]=in->v[0][1]; out->v[1][1]=in->v[1][1]; out->v[1][2]=in->v[2][1];
	out->v[2][0]=in->v[0][2]; out->v[2][1]=in->v[1][2]; out->v[2][2]=in->v[2][2];
	out->t[0]=-in->t[0];out->t[1]=-in->t[1];out->t[2]=-in->t[2];
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////       STEREO       ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* Hier wird alles Stereoskopische erledigt.
 * - Es werden Berechnungen ausgeführt
 * - Es werden die 3D-Farben initiiert.
 */

const GLfloat light_ambient_l[] = {0.93, 1.0, 1.0, 1.0};
const GLfloat light_diffuse_l[] = {0.1 , 0.0, 0.0, 1.0};
const GLfloat light_ambient_r[] = {1.0 , 1.0, 0.93, 1.0};
const GLfloat light_diffuse_r[] = {0.0 , 0.0, 0.1, 1.0};
bool act_eye;

STEREO::STEREO()
{
   act_eye=LEFT_EYE;
   monitor_height=0.271;
   half_monitor_height=0.1355;
   monitor_width=0.34;
   half_monitor_width=0.17;
   eye_seperation=0.068;
   half_eye_seperation=0.034;
   distance_to_monitor=0.55;
   near_clip=distance_to_monitor;
   far_clip=3000.0;
   clipscale=1.0;
   scale=1.0;
}

void STEREO::set_eye(bool eye)
{
   act_eye=eye;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if(!act_eye)
   {
	   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //zeichne alle Farben
      glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_l);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_l);
      glFrustum((-half_monitor_width+half_eye_seperation)*clipscale*scale,
                 (half_monitor_width+half_eye_seperation)*clipscale*scale,
                (-half_monitor_height)*clipscale*scale,
                 (half_monitor_height)*clipscale*scale,
                  distance_to_monitor*clipscale*scale,
                  far_clip*scale);
   }
   else
   {
      glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE); //zeichne nur blau!
      glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_r);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_r);
      glFrustum((-half_monitor_width-half_eye_seperation)*clipscale*scale,
                ( half_monitor_width-half_eye_seperation)*clipscale*scale,
                (-half_monitor_height)*clipscale*scale,
                ( half_monitor_height)*clipscale*scale,
                  distance_to_monitor*clipscale*scale,
                  far_clip*scale);
    }
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
}

void STEREO::position_camera(MATRIX_f *camera_matrix)
{
   if(!act_eye) glTranslatef( half_eye_seperation*scale, 0, 0);
   else         glTranslatef(-half_eye_seperation*scale, 0, 0);
   glMultMatrix_allegro_inv(camera_matrix);
   glPushMatrix();
}

void STEREO::init_lighting()
{
   GLfloat lmodel_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat light_position[] = {1, 1, 1, 0};
   GLfloat light_specular[] = {0, 0, 0, 0};
   GLfloat light_ambient[] = {0.9, 1, 1, 1};
   GLfloat light_diffuse[] = {0.1, 0, 0, 1};
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
}

#ifndef M_PI
#define M_PI   3.1415926535897932384626433832795
#endif
#define RAD_2_DEG(x) ((x) * 180 / M_PI)
#define DEG_2_RAD(x) ((x) / (180 / M_PI))
#define RAD_2_256(x) ((x) * 128 / M_PI)

float STEREO::get_frustum_angle_x_360()
{
   return(RAD_2_DEG(atan(half_monitor_width/distance_to_monitor)*2));
}

float STEREO::get_frustum_angle_y_360()
{
   return(RAD_2_DEG(atan(half_monitor_height/distance_to_monitor)*2));
}

bool save_cfg(char *file_name, CFG_FILE *cfg_file)
{
   FILE *out;

   if ((out = fopen(file_name, "wb"))
       == NULL)
   {
//      allegro_message("Cannot open file: %s !\n", file_name);
      return 1;
   }

   fwrite(cfg_file, sizeof(CFG_FILE), 1, out);
   fclose(out);
   return(0);
}

bool load_cfg(char *file_name, CFG_FILE *cfg_file)
{
   FILE *in;
   int bts;

   if ((in = fopen(file_name, "rb"))
       == NULL)
   {
//      allegro_message("Cannot open file: %s !\n", file_name);
      return 1;
   }

   fread(cfg_file, sizeof(CFG_FILE), 1, in);
   fclose(in);
   return(0);
}


////////////////////////////////////////////////////////////////////////////////
//////////////////////////       STEREO-COLOR        ///////////////////////////
////////////////////////////////////////////////////////////////////////////////


GLubyte col256[2][256][3];
GLubyte col16[2][24][3] =
{
{{ 0, 40, 40},
{ 11, 40, 40},
{ 21, 40, 40},
{ 32, 40, 40},
{ 43, 40, 39},
{ 53, 40, 39},
{ 64, 39, 39},
{ 75, 38, 39},
{ 85, 37, 38},
{ 96, 36, 37},
{107, 32, 34},
{117, 28, 34},
{128, 24, 33},
{139, 17, 29},
{149, 15, 29},
{160,  5, 20},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0}},

{{ 0,  0,  0},
{  0,  0, 16},
{  0,  0, 32},
{  0,  0, 48},
{  0,  0, 64},
{  0,  0, 80},
{  0,  0, 96},
{  0,  0,112},
{  0,  0,128},
{  0,  0,144},
{  0,  0,160},
{  0,  0,176},
{  0,  0,192},
{  0,  0,208},
{  0,  0,224},
{  0,  0,240},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0},
{  0,  0,  0}}
};

GLubyte backgr_r, backgr_g, backgr_b;

char* table_file;
char* get_table_file() {return(table_file);}

void load_color_table(char file[])
{
   table_file=file;
   int i;
   ColorConversion colortable;
   colortable.LoadConf(file);//"color_tables/editor.conf"
   for(i=0; i<255; i++)
   {
      //links
      col256[0][i][0]=colortable.GetValue(SIDE_LEFT, COLOR_RED, i);
      col256[0][i][1]=colortable.GetValue(SIDE_LEFT, COLOR_GREEN, i);
      col256[0][i][2]=colortable.GetValue(SIDE_LEFT, COLOR_BLUE, i);
      //rechts
      col256[1][i][0]=colortable.GetValue(SIDE_RIGHT, COLOR_RED, i);
      col256[1][i][1]=colortable.GetValue(SIDE_RIGHT, COLOR_GREEN, i);
      col256[1][i][2]=colortable.GetValue(SIDE_RIGHT, COLOR_BLUE, i);
   }
   for(i=0; i<16; i++)
   {
      //links
      col16[0][i][0]=colortable.GetValue(SIDE_LEFT, COLOR_RED, i*17);
      col16[0][i][1]=colortable.GetValue(SIDE_LEFT, COLOR_GREEN, i*17);
      col16[0][i][2]=colortable.GetValue(SIDE_LEFT, COLOR_BLUE, i*17);
      //rechts
      col16[1][i][0]=colortable.GetValue(SIDE_RIGHT, COLOR_RED, i*17);
      col16[1][i][1]=colortable.GetValue(SIDE_RIGHT, COLOR_GREEN, i*17);
      col16[1][i][2]=colortable.GetValue(SIDE_RIGHT, COLOR_BLUE, i*17);
   }
   colortable.GetBackgnd(&backgr_r, &backgr_g, &backgr_b);
   float r=backgr_r/255.0, g=backgr_g/255.0, b=backgr_g/255.0;
   glClearColor(r, g, b, 1.0);
}

void update_color_fx()
{
   int a;
   a=(chrono)%16;
   col16[0][16][0]=col16[0][a][0];
   col16[0][16][1]=col16[0][a][1];
   col16[0][16][2]=col16[0][a][2];

   col16[1][16][0]=col16[1][a][0];
   col16[1][16][1]=col16[1][a][1];
   col16[1][16][2]=col16[1][a][2];
////////////////////////////////////
   a=((frame_nr/2)%2)*9+6;
   col16[0][17][0]=col16[0][a][0];
   col16[0][17][1]=col16[0][a][1];
   col16[0][17][2]=col16[0][a][2];

   col16[1][17][0]=col16[1][a][0];
   col16[1][17][1]=col16[1][a][1];
   col16[1][17][2]=col16[1][a][2];
////////////////////////////////////
   a=(chrono/70)%8;
   col16[0][18][0]=col16[0][8+a][0];
   col16[0][18][1]=col16[0][8+a][1];
   col16[0][18][2]=col16[0][8+a][2];

   col16[1][18][0]=col16[1][4+a/2][0];
   col16[1][18][1]=col16[1][4+a/2][1];
   col16[1][18][2]=col16[1][4+a/2][2];
}

void color16(char lumi)
{
   glColor3ubv(col16[act_eye][lumi]);
}

void color16(char lumi, unsigned char alpha)
{
   glColor4ub(col16[act_eye][lumi][0], col16[act_eye][lumi][1], col16[act_eye][lumi][2], alpha);
}

void color256(unsigned char lumi)
{
   glColor3ubv(col256[act_eye][lumi]);
}

void color256(unsigned char lumi, unsigned char alpha)
{
   glColor4ub(col256[act_eye][lumi][0], col256[act_eye][lumi][1], col256[act_eye][lumi][2], alpha);
}

bool get_eye() {return (act_eye);}   //inline machen!

void convert_to_stereo_bitmap(BITMAP *bmp, bool eye)
{
   int y, x;
   unsigned char lum_val;
   BITMAP *luminance = create_bitmap(bmp->w, bmp->h);
   set_luminance_blender(255, 255, 255, 255);
   draw_lit_sprite(luminance, bmp, 0, 0, 255);
   bmp_select(luminance);
   for (y=0; y<(luminance->h); y++)
   {
	   for (x=0; x<luminance->w; x++)
      {
         bmp_select(luminance);
         lum_val=getb32(_getpixel32(luminance, x, y))-1;
         _putpixel32(bmp,  x, y, makecol24(col256[eye][lum_val][0], col256[eye][lum_val][1], col256[eye][lum_val][2]));
      }
   }
 	destroy_bitmap(luminance);
}

void get_color256(bool eye, unsigned char lumi, unsigned char *r, unsigned char *g, unsigned char *b)
{
  *r=col256[eye][lumi][0];
  *g=col256[eye][lumi][1];
  *b=col256[eye][lumi][2];
}

void get_background_color(unsigned char *r, unsigned char *g, unsigned char *b)
{
   *r=backgr_r;*g=backgr_g;*b=backgr_b;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////       MODEL3D       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// 9 3 9 23
typedef enum ACTIONTYPE
{
   ACT_TURN=0,  //0
   ACT_MOVE,
   ACT_TURN_NOW,
   ACT_MOVE_NOW,
   ACT_SPIN,
   ACT_STOP_SPIN,
   ACT_WAITFOR,
   ACT_WAIT,
   ACT_ACTIVE,
   ACT_INACTIVE,
   ACT_FINISHED, //10
   ACT_HIDE,
   ACT_SHOW,
   ACT_ALL,
   ACT_FUNCTION0,
   ACT_FUNCTION1,
   ACT_FUNCTION2,
   ACT_HIDE_GRID,
   ACT_SHOW_GRID,
   ACT_HIDE_FILL,
   ACT_SHOW_FILL,//20
   ACT_BLINK,
   ACT_STOP_BLINK,
   ACT_SET_COLOR,
   ACT_RESET,
   ACT_CHANGE_INTEGER,
   ACT_CHANGE_FLOAT,
   ACT_EXPLODE
};

typedef struct ACTION
{
   int index;         //index
   int action_group;
   bool active;
   ACTIONTYPE type;    //ACT_TURN, ACT_MOVE, ACT_WAIT, ACT_WAITFORACT_TURN, ACT_WAITFORACT_MOVE
   MODEL3D *model3d;          //welches model3d?
   int o;             //welches Objejkt?
   char axis;           //x- y- z-Achse?
   int t;               //für wait-Actions
   float amount;        //Winkel oder Translation
   float speed;         //Geschwindigkeit der Bewegung
   ACTION *link;        //nächste Aktion (bei Wait und Waitfor)
   ACTION *waitforaction; //für Aktionstyp ACT_WAITFOR: warte auf Beendigung dieser Aktion in dieser Verknüpfung
   ACTION *whowaitsforme; //gibt es eine Funktion, die auf mich wartet? Wenn ja, welche?
   ACTION_FUNCTION0 action_function0; //Funktionspointer
   ACTION_FUNCTION1 action_function1; //Funktionspointer
   ACTION_FUNCTION2 action_function2; //Funktionspointer
   void *pointer, *pointer2;
} ACTION;

typedef struct BLINKDATA
{
   int time[2], col16[2];
   bool is_on;
   bool active; //schon geblinkt?
}BLINKDATA;

typedef struct EXPLODEDATA
{
   float winkel[3], dir[3], gravity, rot_speed;
}EXPLODEDATA;



   

EXPLODEDATA *exploding;
BLINKDATA *blinking;

ACTION act_none;
ACTION *ACT_NONE=&act_none;
ACTION *a_actions; //alle aktiven Actions
ACTION **a_action; //alle inaktiven Actions
ACTION *i_actions;
ACTION **i_action;
volatile int a_action_total;
volatile int i_action_total;
int max_act_act, max_inact_act;
MODEL **models;      //Zeiger auf alle geladenen Models
int models_total=0;
int stereo_bitmap_total=0;
STEREO_BITMAP **stereo_bitmaps;

void init_models(int max_models)
{
   models=new MODEL*[max_models];
}

void init_stereo_bitmaps(int max_stereo_bitmaps)
{
   stereo_bitmaps=new STEREO_BITMAP*[max_stereo_bitmaps];
}

MODEL* load_3do(const char name[])
{
   int i;
   for (i=0; i<models_total; i++)
   {
       if(!strcmp(name, models[i]->name)) return (models[i]);
   }
   models[models_total]= new MODEL;
   lese_3do(models[models_total], name);
   models_total++;
   return(models[models_total-1]);
}

void unload_3do(const char name[])
{
   int i;
   for (i=0; i<models_total; i++)
   {
      if(!strcmp(name, models[i]->name))
      {
         delete models[i];
         models_total--;
         models[i]=models[models_total];
      }
   }
}

OBJECT::OBJECT()
{
   m=identity_matrix_f;
   moved=FALSE;
   visible=TRUE;
   grid_is_visible=TRUE;
   fill_is_visible=TRUE;
   winkel[0]=0;
   winkel[1]=0;
   winkel[2]=0;
   single_colored=FALSE;
   alpha=255;
   is_textured=FALSE;
}

void MODEL3D::assign_3do(const char name[])  //weist einer MODEL3D ein 3do-Modell zu
{
      this->model=load_3do(name);
   if(!this->assigned)
   {
      this->object=new OBJECT[this->model->obj_total];
      this->assigned=true;
   }
   else
   {
      this->clear_all_actions();
      delete this->object;
      this->object=new OBJECT[this->model->obj_total];

   }
}

MODEL3D::~MODEL3D()
{
   delete this->object;
   this->action_group=0;
}

bool MODEL3D::get_object_matrix_recursive(int o, int o_soll)
{
   matr_stack.push();
   matr_stack.translate(&this->model->o3d[o].pos);
   if(this->object[o].moved)matr_stack.mul(&this->object[o].m);
   if(o==o_soll) return(TRUE);
   else
   {
      if(this->model->o3d[o].child_o)
      {
         if(get_object_matrix_recursive(this->model->o3d[o].child_o, o_soll)) return(TRUE);
      }
      matr_stack.pop();
      if(this->model->o3d[o].sibl_o)
      {
         if(get_object_matrix_recursive(this->model->o3d[o].sibl_o, o_soll)) return(TRUE);
      }
   }
   return(FALSE);
}

MATRIX_f MODEL3D::get_object_matrix_world(int o, MATRIX_f *m)
{
   matr_stack.loadIdentity();
   matr_stack.mul_alg(m);
   this->get_object_matrix_recursive(0, o);
   return(matr_stack.get());
}

MATRIX_f MODEL3D::get_object_matrix(int o)
{
   matr_stack.loadIdentity();
   this->get_object_matrix_recursive(0, o);
   return(matr_stack.get());
}

int MODEL3D::get_object_by_name(const char name[])
{
   char o;
   for(o=0;o<this->model->obj_total;o++) if(!strcmp(name, this->model->o3d[o].name)) return(o);
   return(0);
}

STEREO_BITMAP *get_stereo_bitmap(const char name[]) //konvertiert normale Bitmaps zu Stereo-Texturen
{
   int i, y, x;
   unsigned char lum_val;
   for(i=0;i<stereo_bitmap_total; i++)if(!strcmp(name, stereo_bitmaps[i]->name))return (stereo_bitmaps[i]);
   stereo_bitmaps[stereo_bitmap_total]=new STEREO_BITMAP;
   stereo_bitmaps[stereo_bitmap_total]->name=name;
	BITMAP *bmp = load_bmp (name, NULL);
	if (!bmp)
   {
		allegro_message ("Error loading texture bitmap!");
		exit (1);
	}
	BITMAP *luminance = create_bitmap(bmp->w, bmp->h);
	BITMAP *left  = create_bitmap(bmp->w, bmp->h);
	BITMAP *right = create_bitmap(bmp->w, bmp->h);
 	set_luminance_blender(255, 255, 255, 255);
   draw_lit_sprite(luminance, bmp, 0, 0, 255);
   for (y=0; y<(luminance->h); y++)
   {
	   for (x=0; x<luminance->w; x++)
      {
         lum_val=getb32(_getpixel32(luminance, x, y))-1;
         _putpixel32(left,  x, y,  makecol24(col256[0][lum_val][0], col256[0][lum_val][1], col256[0][lum_val][2]));
         _putpixel32(right,  x, y, makecol24(col256[1][lum_val][0], col256[1][lum_val][1], col256[1][lum_val][2]));
      }
   }
   allegro_gl_begin();
	glEnable (GL_TEXTURE_2D);
//	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   stereo_bitmaps[stereo_bitmap_total]->glbmp[0]=allegro_gl_make_texture(left);
   stereo_bitmaps[stereo_bitmap_total]->glbmp[1]=allegro_gl_make_texture(right);
	destroy_bitmap(bmp);
	destroy_bitmap(luminance);
	destroy_bitmap(left);
	destroy_bitmap(right);
	stereo_bitmap_total++;
	glDisable (GL_TEXTURE_2D);
	return(stereo_bitmaps[stereo_bitmap_total-1]);
}

STEREO_BITMAP *get_stereo_bitmap_alpha(const char name[], const char alpha_name[]) //konvertiert normale Bitmaps zu Stereo-Texturen
{
   int i, y, x;
   unsigned char lum_val, alpha_val;
   for(i=0;i<stereo_bitmap_total; i++)if(!strcmp(name, stereo_bitmaps[i]->name))return (stereo_bitmaps[i]);
   stereo_bitmaps[stereo_bitmap_total]=new STEREO_BITMAP;
   stereo_bitmaps[stereo_bitmap_total]->name=name;
	BITMAP *bmp = load_bmp (name, NULL);
	if (!bmp)
   {
		allegro_message ("Error loading texture bitmap!");
		exit (1);
	}
	BITMAP *alpha=load_bmp (alpha_name, NULL);
	if (!bmp)
   {
		allegro_message ("Error loading alpha texture bitmap!");
		exit (1);
	}
	BITMAP *luminance = create_bitmap(bmp->w, bmp->h);
	BITMAP *left  = create_bitmap(bmp->w, bmp->h);
	BITMAP *right = create_bitmap(bmp->w, bmp->h);
	BITMAP *alpha_lum=create_bitmap(alpha->w, alpha->h);
 	set_luminance_blender(255, 255, 255, 255);
   draw_lit_sprite(luminance, bmp, 0, 0, 255);
   draw_lit_sprite(alpha_lum, alpha, 0, 0, 255);
   allegro_gl_use_alpha_channel(1);
   for (y=0; y<(luminance->h); y++)
   {
	   for (x=0; x<luminance->w; x++)
      {
         alpha_val=getb32(_getpixel32(alpha_lum, x, y));
         lum_val=getb32(_getpixel32(luminance, x, y))-1;
         _putpixel32(left,  x, y,  makeacol32(col256[0][lum_val][0], col256[0][lum_val][1], col256[0][lum_val][2], alpha_val));
         _putpixel32(right,  x, y, makeacol32(col256[1][lum_val][0], col256[1][lum_val][1], col256[1][lum_val][2], alpha_val));
      }
   }
   allegro_gl_begin();
	glEnable (GL_TEXTURE_2D);
//	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   stereo_bitmaps[stereo_bitmap_total]->glbmp[0]=allegro_gl_make_texture(left);
   stereo_bitmaps[stereo_bitmap_total]->glbmp[1]=allegro_gl_make_texture(right);
	destroy_bitmap(bmp);
	destroy_bitmap(luminance);
	destroy_bitmap(left);
	destroy_bitmap(right);
	destroy_bitmap(alpha);
	destroy_bitmap(alpha_lum);
	stereo_bitmap_total++;
	glDisable (GL_TEXTURE_2D);
   allegro_gl_use_alpha_channel 	(0);
	return(stereo_bitmaps[stereo_bitmap_total-1]);
}

bool MODEL3D::assign_texture(const char name[], int face, const char texture_name[], float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
{
   int i;
   int o=get_object_by_name(name);
   int face_total=this->model->o3d[o].poly_total;
   if(!this->object[o].is_textured)this->object[o].texture=new TEXTURE[face_total];
   for(i=0;i<face_total;i++)this->object[o].texture[i].active=FALSE;
   this->object[o].texture[face].active=TRUE;
   this->object[o].texture[face].texcoord[0][0]=ax;
   this->object[o].texture[face].texcoord[0][1]=ay;
   this->object[o].texture[face].texcoord[1][0]=bx;
   this->object[o].texture[face].texcoord[1][1]=by;
   this->object[o].texture[face].texcoord[2][0]=cx;
   this->object[o].texture[face].texcoord[2][1]=cy;
   this->object[o].texture[face].texcoord[3][0]=dx;
   this->object[o].texture[face].texcoord[3][1]=dy;
   this->object[o].texture[face].stereo_bitmap=get_stereo_bitmap(texture_name);
   this->object[o].is_textured=TRUE;
}

bool MODEL3D::assign_texture(const char name[], int face, const char texture_name[],const char alpha_texture_name[],  float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy)
{
   int i;
   int o=get_object_by_name(name);
   int face_total=this->model->o3d[o].poly_total;
   if(!this->object[o].is_textured)this->object[o].texture=new TEXTURE[face_total];
   for(i=0;i<face_total;i++)this->object[o].texture[i].active=FALSE;
   this->object[o].texture[face].active=TRUE;
   this->object[o].texture[face].texcoord[0][0]=ax;
   this->object[o].texture[face].texcoord[0][1]=ay;
   this->object[o].texture[face].texcoord[1][0]=bx;
   this->object[o].texture[face].texcoord[1][1]=by;
   this->object[o].texture[face].texcoord[2][0]=cx;
   this->object[o].texture[face].texcoord[2][1]=cy;
   this->object[o].texture[face].texcoord[3][0]=dx;
   this->object[o].texture[face].texcoord[3][1]=dy;
   this->object[o].texture[face].stereo_bitmap=get_stereo_bitmap_alpha(texture_name, alpha_texture_name);
   this->object[o].is_textured=TRUE;
}
#define FLASHSIZE 128


void init_actions(int max_active_actions, int max_inactive_actions)
{
   int i;
   i_actions=new ACTION[max_inactive_actions];
   a_actions=new ACTION[max_active_actions];
   i_action= new ACTION*[max_inactive_actions];
   a_action= new ACTION*[max_active_actions];

   for(i=0;i<max_active_actions;i++)
   {
      a_actions[i].index=i;
      a_actions[i].type=ACT_INACTIVE;
      a_actions[i].active=TRUE;
      a_actions[i].link=ACT_NONE;
      a_actions[i].waitforaction=ACT_NONE;
      a_actions[i].whowaitsforme=ACT_NONE;
      a_action[i]=&a_actions[i];
    }
   for(i=0;i<max_inactive_actions;i++)
   {
      i_actions[i].index=i;
      i_actions[i].type=ACT_INACTIVE;
      i_actions[i].active=FALSE;
      i_actions[i].link=ACT_NONE;
      i_actions[i].waitforaction=ACT_NONE;
      i_actions[i].whowaitsforme=ACT_NONE;
      i_action[i]=&i_actions[i];
    }
    max_act_act=max_active_actions;
    max_inact_act=max_inactive_actions;
}

void free_actions()
{
   delete(i_actions);
   delete(a_actions);
   delete(i_action);
   delete(a_action);
   max_act_act=0;
   max_inact_act=0;
   a_action_total=0;
   i_action_total=0;
}

   
int get_active_actions_total(){return (a_action_total);}
int get_inactive_actions_total(){return (i_action_total);}
int get_models_total(){return(models_total);}


void kill_a_action(int i)
{
   a_action[i]->type=ACT_INACTIVE;
   a_action[i]->link=ACT_NONE;
   a_action[i]->waitforaction=ACT_NONE;
   a_action[i]->whowaitsforme=ACT_NONE;

   ACTION *temp=a_action[i];
   a_action[i]= a_action[a_action_total-1];
   a_action[a_action_total-1]=temp;
   a_action[a_action_total-1]->index=a_action_total-1;
   a_action[i]->index=i;
   a_action_total--;

}

void kill_i_action(int i)
{
   i_action[i]->type=ACT_INACTIVE;
   i_action[i]->link=ACT_NONE;
   i_action[i]->waitforaction=ACT_NONE;
   i_action[i]->whowaitsforme=ACT_NONE;

   ACTION *temp=i_action[i];
   i_action[i]=i_action[i_action_total-1];
   i_action[i_action_total-1]=temp;
   i_action[i_action_total-1]->index=i_action_total-1;
   i_action[i]->index=i;
   i_action_total--;
}

void clear_similar_active_actions(ACTION *act)
{
   int i;
   for(i=0;i<a_action_total;i++)
   if(a_action[i]!=act)
   if((a_action[i]->model3d==act->model3d) &&
      (a_action[i]->o==act->o) &&
      (a_action[i]->axis==act->axis) &&
      (a_action[i]->type==act->type) &&
      (a_action[i]->action_group==act->action_group))
      {
         a_action[i]->type=ACT_FINISHED;
 //      allegro_message("hey!");
#ifdef DEBUG
        printf("similar active action nr. %d finished", i);
#endif
      }
}

ACTION* find_action(MODEL3D *model3d, int o, char axis, signed char type)
{
   int i;
   ACTION *ret, *link;
   for(i=0; i<i_action_total; i++)
   {
      if ((i_action[i]->model3d==model3d) && (i_action[i]->o==o) && (i_action[i]->axis==axis) && (i_action[i]->type==type) && (i_action[i]->action_group==model3d->get_action_group()))
      {
         ret=i_action[i];
         link=ret->link;
         while(link!=ACT_NONE)
         {
            if ((link->model3d==model3d) && (link->o==o) && (link->axis==axis)&& (link->type==type) && (link->action_group==model3d->get_action_group())) ret=link;
            link=link->link;
         }
         return(ret);
      }
   }
   for(i=0; i<a_action_total; i++)
   if ((a_action[i]->model3d==model3d) && (a_action[i]->o==o) && (a_action[i]->axis==axis)&& (a_action[i]->type==type) && (a_action[i]->action_group==model3d->get_action_group()))
   return(a_action[i]);
   return(ACT_NONE);
}

ACTION* get_action(MODEL3D *model3d)//Funktion findet die letzte Action einer Verknüpfungsreihe
{
   int i, j;
   ACTION *link;
   for(i=0; i<a_action_total; i++)
   if(((a_action[i]->type == ACT_WAIT) || (a_action[i]->type == ACT_WAITFOR)) && (a_action[i]->model3d == model3d)&&(a_action[i]->action_group==model3d->action_group))  //es gibt immer nur höchstens eine AKTIVE WAIT(_FOR)-Action. Ist eine aktiv?
   {
      link=a_action[i];
	   while(link->link != ACT_NONE) link=link->link;//stoße zur letzten Funktion der Verknüpfunsreihe vor
	   link->link=i_action[i_action_total];//verknüpfe die neue Aktion
      i_action_total++;
//      if(a_action_total>MAXACTIONS)allegro_message("harbst: too much ACTIONS. Increase MAXACTIONS.");
      i_action[i_action_total-1]->action_group=model3d->action_group;
      return(i_action[i_action_total-1]);             //gib Pointer auf die neue Aktion zurück
   }//wenn keine aktive wait-funktion gefunden wurde
   a_action_total++;
//   if(a_action_total>MAXACTIONS)allegro_message("harbst: too much ACTIONS. Increase MAXACTIONS.");
   a_action[a_action_total-1]->action_group=model3d->action_group;
   return(a_action[a_action_total-1]);
}

void activate_action(ACTION *act)//Diese Funktion aktiviert inaktive Actions (rekursiv, alles zwischen Wait-Befehlen)
{
#ifdef DEBUG
   printf("*");
#endif
   int i;
   ACTION link;
   clear_similar_active_actions(act);     //lösche gleichartige active actions
   *a_action[a_action_total]=*act;        //kopiere inactive action in active-action-Schlange
   a_action[a_action_total]->active=TRUE;  //durch das Kopieren wurde auch der active-Wert überschrieben, deshalb diese Neuzuweisung
   if(act->whowaitsforme!=ACT_NONE)act->whowaitsforme->waitforaction=a_action[a_action_total];//neuverlinkung zu einer möglichen WAIT_FOR-action
   a_action_total++;
   if(act->link != ACT_NONE)                                  //wenn link ungleich null ist...
   if((act->type != ACT_WAIT) && (act->type != ACT_WAITFOR))  //und die aktuelle action keine wait... - action...
   activate_action(act->link);                                //dann aktiviere action im "link"!
   kill_i_action(act->index);                                 //und dann lösche die aktivierte action
}

void MODEL3D::clear_action_group(int a_g)
{
#ifdef DEBUG
   printf("\nclear_action_group: ");
#endif
   int i, t;
   for(i=0;i<a_action_total;i++)
   if((a_action[i]->model3d==this) && (a_action[i]->action_group==a_g))
   {
      kill_a_action(i);
      i--;
   }
   for(i=0;i<i_action_total;i++)
   if((i_action[i]->model3d==this)&&(i_action[i]->action_group==a_g))
   {
      kill_i_action(i);
      i--;
#ifdef DEBUG
      printf("%d", a_g);
#endif
   }
}

void MODEL3D::clear_all_actions()
{
   int i;
   for(i=0;i<a_action_total;i++)if(a_action[i]->model3d==this){kill_a_action(i);i--;}
   for(i=0;i<i_action_total;i++)if(i_action[i]->model3d==this){kill_i_action(i);i--;}
}

void MODEL3D::reset_object_matrix(int o)
{
   this->object[o].m=identity_matrix_f;
   this->object[o].winkel[0]=0;
   this->object[o].winkel[1]=0;
   this->object[o].winkel[2]=0;
   this->object[o].moved=FALSE;
}

void MODEL3D::reset_all_object_matrices()
{
   int o;
   for(o=0;o<this->model->obj_total;o++)
   {
      this->object[o].m=identity_matrix_f;
      this->object[o].winkel[0]=0;
      this->object[o].winkel[1]=0;
      this->object[o].winkel[2]=0;
      this->object[o].moved=FALSE;
   }
}

void MODEL3D::kill_all_actions() //löscht alle actions eines MODEL3D-Objektes
{
   int i;
   for(i=0;i<a_action_total;i++)
   if(a_action[i]->model3d==this)
   {
      kill_a_action(i);
      i--;
   }
   for(i=0;i<i_action_total;i++)
   if(i_action[i]->model3d==this)
   {
      kill_i_action(i);
      i--;
   }
}

//////////////////////////   ACTIONS  //////////////////////////////////////////

void MODEL3D::blink(const char name[], int time_on, int time_off, int col16_on, int col16_off)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->type=ACT_BLINK;
   BLINKDATA *blinkdata=new BLINKDATA;
   blinkdata->col16[0]=col16_off;
   blinkdata->col16[1]=col16_on;
   blinkdata->time[0]=time_off;
   blinkdata->time[1]=time_on;
   blinkdata->is_on=FALSE;
   blinkdata->active=FALSE;
   act->pointer=blinkdata;
   act->t=chrono;
   act->axis=0;
}

bool MODEL3D::stop_blink(const char name[])
{
   int o=this->get_object_by_name(name);
   ACTION *act, *link=find_action(this, o, 0, ACT_BLINK);
   if(link==ACT_NONE) return(FALSE);
   act=get_action(this);
   act->o=o;
   link->whowaitsforme=act;
   act->model3d=this;
   act->type=ACT_STOP_BLINK;
   act->waitforaction=link;
   return(TRUE);
}

void MODEL3D::hide_fill(const char name[])
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->type=ACT_HIDE_FILL;
}

void MODEL3D::explode(const char name[], float dir_x, float dir_y, float dir_z, float rot_speed, float gravity, int duration)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->type=ACT_EXPLODE;
   EXPLODEDATA *explodedata=new EXPLODEDATA;
   explodedata->winkel[0]=rand()%256;
   explodedata->winkel[1]=rand()%256;
   explodedata->winkel[2]=rand()%256;
   explodedata->dir[0]=dir_x;
   explodedata->dir[1]=dir_y;
   explodedata->dir[2]=dir_z;
   explodedata->gravity=gravity;
   explodedata->rot_speed=rot_speed;
   act->t=duration;
   act->speed=0;
   act->amount=0;

   act->pointer=(void*)explodedata;
}

void MODEL3D::show_fill(const char name[])
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->type=ACT_SHOW_FILL;
}

void MODEL3D::hide_grid(const char name[])
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->type=ACT_HIDE_GRID;
}

void MODEL3D::hide_grid(int o)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=o;
   act->type=ACT_HIDE_GRID;
}

void MODEL3D::show_grid(const char name[])
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->type=ACT_SHOW_GRID;
}

void MODEL3D::hide(int o)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=o;
   act->type=ACT_HIDE;
}

void MODEL3D::hide(const char name[])
{
   int o=this->get_object_by_name(name);
   this->hide(o);
}

void MODEL3D::show(int o)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=o;
   act->type=ACT_SHOW;
}

void MODEL3D::show(const char name[])
{
   int o=this->get_object_by_name(name);
   this->show(o);
}

void MODEL3D::set_color(int o, int col)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=o;
   act->type=ACT_SET_COLOR;
   act->t=col;
}
void MODEL3D::set_color(const char name[], int col)
{
   this->set_color(this->get_object_by_name(name), col);
}
void MODEL3D::start_function(ACTION_FUNCTION0 action_function)
{
   ACTION *act=get_action(this);
   act->model3d=this;
//   printf("action_f:%d\n", this);
   act->type=ACT_FUNCTION0;
   act->action_function0=action_function;
}

void MODEL3D::start_function(ACTION_FUNCTION1 action_function, void *pointer)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->type=ACT_FUNCTION1;
   act->pointer=pointer;
   act->action_function1=action_function;
}

void MODEL3D::start_function(ACTION_FUNCTION2 action_function, void *pointer, void *pointer2)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->type=ACT_FUNCTION2;
   act->action_function2=action_function;
   act->pointer=pointer;
   act->pointer2=pointer2;
}

void MODEL3D::move(const char name[], int axis, float amount, float speed)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->axis=axis;
   act->amount=amount;
   act->speed=speed;
   if(speed==NOW)act->type=ACT_MOVE_NOW;
   else act->type=ACT_MOVE;
   if(act->active)clear_similar_active_actions(act);
//   stop_a_action(act);
}

void MODEL3D::turn(const char name[], int axis, float amount, float speed)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->axis=axis;
   act->amount=-amount;
   act->speed=speed;
   act->t=0; //zeigt an, daß dies nicht turn_short ist!
   if(speed==NOW)act->type=ACT_TURN_NOW;
   else act->type=ACT_TURN;
   if(act->active)clear_similar_active_actions(act);
}

void MODEL3D::turn_short(const char name[], int axis, float amount, float speed)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->axis=axis;
   act->amount=-amount;
   act->speed=speed;
   act->t=0;
   if(speed==NOW)act->type=ACT_TURN_NOW;
   else
   {
       act->type=ACT_TURN;
       act->t=1; //zeigt an, daß dies turn_short ist!
   }
   if(act->active)clear_similar_active_actions(act);
}

void MODEL3D::spin(const char name[], char axis, float speed)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->axis=axis;
   act->speed=-speed;
   act->type=ACT_SPIN;
//   stop_a_action(act);
}

void MODEL3D::change_integer(int *change_it, int value)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->pointer=(void*)change_it;
   act->t=value;
   act->type=ACT_CHANGE_INTEGER;
}
void MODEL3D::change_float(float *change_it, float value)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->pointer=(void*)change_it;
   act->amount=value;
   act->type=ACT_CHANGE_FLOAT;
}

bool MODEL3D::stop_spin(const char name[], int axis)
{
   int o=this->get_object_by_name(name);
   ACTION *act, *link=find_action(this, o, axis, ACT_SPIN);
   if(link==ACT_NONE) return(FALSE);
   act=get_action(this);
   act->o=o;
   link->whowaitsforme=act;
   act->model3d=this;
   act->type=ACT_STOP_SPIN;
   act->waitforaction=link;
   return(TRUE);
}

void MODEL3D::reset(const char name[])
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->o=this->get_object_by_name(name);
   act->type=ACT_RESET;
}

void MODEL3D::wait(int t)
{
   ACTION *act=get_action(this);
   act->model3d=this;
   act->t=t;
   act->type=ACT_WAIT;
}

bool MODEL3D::wait_for_move(const char name[], char axis)
{
   ACTION *act, *link=find_action(this, this->get_object_by_name(name), axis, ACT_MOVE);
   if(link==ACT_NONE) return(FALSE);
   act=get_action(this);
   link->whowaitsforme=act;
   act->model3d=this;
   act->type=ACT_WAITFOR;
   act->waitforaction=link;
   return(TRUE);
}

bool MODEL3D::wait_for_turn(const char name[], char axis)
{
   ACTION *act,
   *link=find_action(this, this->get_object_by_name(name), axis, ACT_TURN);
   if(link==ACT_NONE) return(FALSE);
   act=get_action(this);
   link->whowaitsforme=act;
   act->model3d=this;
   act->type=ACT_WAITFOR;
   act->waitforaction=link;
   return(TRUE);
}

void proceed_actions()
{
   int i, k, *int_;
   MATRIX_f m;
   bool ready;
   VECTOR pos;
   float pl, *float_;
   for(i=0;i<a_action_total; i++)
   {
#ifdef DEBUG
      printf("\n%d i:%d/%d frame: %d group:%d type:%d amount:%.2f", a_action[i], i, a_action_total, frame_nr, a_action[i]->action_group, a_action[i]->type, a_action[i]->amount);
#endif
      switch(a_action[i]->type)
      {
         case ACT_FINISHED:
              kill_a_action(i);
              i--;//wichtig, sonst wird eine Action ausgelassen
         break;
         case ACT_MOVE:
              a_action[i]->model3d->object[a_action[i]->o].moved=TRUE;
              if(accelerate(a_action[i]->amount, &a_action[i]->model3d->object[a_action[i]->o].m.t[a_action[i]->axis], a_action[i]->speed))
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_MOVE_NOW:
              a_action[i]->model3d->object[a_action[i]->o].moved=TRUE;
              a_action[i]->model3d->object[a_action[i]->o].m.t[a_action[i]->axis]=a_action[i]->amount;
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_TURN:
              if (a_action[i]->t)
              {
                  a_action[i]->t=0;
                  k=floor(a_action[i]->amount);
                  k=k%256;
                  a_action[i]->amount=a_action[i]->amount-floor(a_action[i]->amount)+k;


                  float_=&a_action[i]->model3d->object[a_action[i]->o].winkel[a_action[i]->axis];
                  k=floor(*float_);
                  k=k%256;
                  *float_=*float_-floor(*float_)+k;
                  if(fabs(a_action[i]->amount+256-*float_)<fabs(a_action[i]->amount-*float_))a_action[i]->amount+=256;
                  else if(fabs(a_action[i]->amount-256-*float_)<fabs(a_action[i]->amount-*float_))a_action[i]->amount-=256;
              }
              a_action[i]->model3d->object[a_action[i]->o].moved=TRUE;
              ready=accelerate(a_action[i]->amount, &a_action[i]->model3d->object[a_action[i]->o].winkel[a_action[i]->axis], a_action[i]->speed);
              pos=trans_matrix_to_vector(&a_action[i]->model3d->object[a_action[i]->o].m);
              get_rotation_matrix_f(&a_action[i]->model3d->object[a_action[i]->o].m, a_action[i]->model3d->object[a_action[i]->o].winkel[0], a_action[i]->model3d->object[a_action[i]->o].winkel[1], a_action[i]->model3d->object[a_action[i]->o].winkel[2]);
              translate_matrix_v(&a_action[i]->model3d->object[a_action[i]->o].m, &pos);
              if(ready)a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_TURN_NOW:
              a_action[i]->model3d->object[a_action[i]->o].moved=TRUE;
              a_action[i]->model3d->object[a_action[i]->o].winkel[a_action[i]->axis]=a_action[i]->amount;
              pos=trans_matrix_to_vector(&a_action[i]->model3d->object[a_action[i]->o].m);
              get_rotation_matrix_f(&a_action[i]->model3d->object[a_action[i]->o].m, a_action[i]->model3d->object[a_action[i]->o].winkel[0], a_action[i]->model3d->object[a_action[i]->o].winkel[1], a_action[i]->model3d->object[a_action[i]->o].winkel[2]);
              translate_matrix_v(&a_action[i]->model3d->object[a_action[i]->o].m, &pos);
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_SPIN:
              a_action[i]->model3d->object[a_action[i]->o].moved=TRUE;
              a_action[i]->model3d->object[a_action[i]->o].winkel[a_action[i]->axis]+=a_action[i]->speed*fmult;
              pos=trans_matrix_to_vector(&a_action[i]->model3d->object[a_action[i]->o].m);
              get_rotation_matrix_f(&a_action[i]->model3d->object[a_action[i]->o].m, a_action[i]->model3d->object[a_action[i]->o].winkel[0], a_action[i]->model3d->object[a_action[i]->o].winkel[1], a_action[i]->model3d->object[a_action[i]->o].winkel[2]);
              translate_matrix_v(&a_action[i]->model3d->object[a_action[i]->o].m, &pos);
         break;
         case ACT_EXPLODE:
              exploding=(EXPLODEDATA*)a_action[i]->pointer;
              a_action[i]->model3d->object[a_action[i]->o].moved=TRUE;
              pos.x=a_action[i]->model3d->object[a_action[i]->o].m.t[0]+exploding->dir[0]*fmult;
              pos.y=a_action[i]->model3d->object[a_action[i]->o].m.t[1]+exploding->dir[1]*fmult-a_action[i]->speed;
              pos.z=a_action[i]->model3d->object[a_action[i]->o].m.t[2]+exploding->dir[2]*fmult;
              get_vector_rotation_matrix_f(&m, exploding->winkel[0], exploding->winkel[1], exploding->winkel[2], exploding->rot_speed*fmult);
              matrix_mul_f(&a_action[i]->model3d->object[a_action[i]->o].m, &m, &a_action[i]->model3d->object[a_action[i]->o].m);
              translate_matrix_v(&a_action[i]->model3d->object[a_action[i]->o].m, &pos);
              a_action[i]->t-=fmult*1000;
              a_action[i]->speed+=exploding->gravity*fmult;
              if(a_action[i]->t<=0)
              {
                 a_action[i]->type=ACT_FINISHED;
                 delete exploding;
              }
         break;
         case ACT_STOP_SPIN:
              a_action[i]->type = ACT_FINISHED;
              a_action[i]->waitforaction->type = ACT_FINISHED;
         break;
         case ACT_HIDE:
              a_action[i]->model3d->object[a_action[i]->o].visible=FALSE;
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_SHOW:
              a_action[i]->model3d->object[a_action[i]->o].visible=TRUE;
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_HIDE_GRID:
              a_action[i]->model3d->object[a_action[i]->o].grid_is_visible=FALSE;
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_SHOW_GRID:
              a_action[i]->model3d->object[a_action[i]->o].grid_is_visible=TRUE;
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_HIDE_FILL:
              a_action[i]->model3d->object[a_action[i]->o].fill_is_visible=FALSE;
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_SHOW_FILL:
              a_action[i]->model3d->object[a_action[i]->o].fill_is_visible=TRUE;
              a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_BLINK:
              blinking=(BLINKDATA*)a_action[i]->pointer;
              if(!blinking->active)
              {
                 a_action[i]->t=chrono;
                 blinking->active=1;
                 blinking->is_on=1;
              }
              else
              if((chrono-a_action[i]->t)>blinking->time[blinking->is_on])
              {
                 a_action[i]->t=chrono + (chrono-(a_action[i]->t+blinking->time[blinking->is_on]));
                 blinking->is_on=1-blinking->is_on;
//                 pl=blinking->col16[blinking->is_on];
                 if(blinking->col16[blinking->is_on]==DEFAULT)
                 {a_action[i]->model3d->object[a_action[i]->o].single_colored=0;}
                 else
                 {
                    a_action[i]->model3d->object[a_action[i]->o].color=blinking->col16[blinking->is_on];
                    a_action[i]->model3d->object[a_action[i]->o].single_colored=1;
                 }
              }
         break;
         case ACT_STOP_BLINK:
              a_action[i]->type = ACT_FINISHED;
              a_action[i]->waitforaction->type = ACT_FINISHED;
              blinking=(BLINKDATA*)a_action[i]->waitforaction->pointer;
              delete blinking;
              a_action[i]->model3d->object[a_action[i]->o].single_colored=0;
         break;
         case ACT_RESET:
               a_action[i]->model3d->reset_object_matrix(a_action[i]->o);
               a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_FUNCTION0:
 //              printf("frame_nr:%d action_nr: %d action: %d\n", frame_nr, i, a_action[i]);
               a_action[i]->action_function0();
               a_action[i]->action_function0=0;
               a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_FUNCTION1:
               a_action[i]->action_function1(a_action[i]->pointer);
               a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_FUNCTION2:
               a_action[i]->action_function2(a_action[i]->pointer, a_action[i]->pointer2);
               a_action[i]->type=ACT_FINISHED;
         break;
         case ACT_SET_COLOR:
              if(a_action[i]->t==DEFAULT)a_action[i]->model3d->object[a_action[i]->o].single_colored=0;
              else
              {
                 a_action[i]->model3d->object[a_action[i]->o].single_colored=1;
                 a_action[i]->model3d->object[a_action[i]->o].color=a_action[i]->t;
              }
              a_action[i]->type = ACT_FINISHED;
         break;
         case ACT_CHANGE_INTEGER:
              int_=(int*)a_action[i]->pointer;
              *int_=a_action[i]->t;
              a_action[i]->type = ACT_FINISHED;
         break;
         case ACT_CHANGE_FLOAT:
              float_=(float*)a_action[i]->pointer;
              *float_=a_action[i]->amount;
              a_action[i]->type = ACT_FINISHED;
         break;
         case ACT_WAIT:
              a_action[i]->t-=fmult*1000;
              if(a_action[i]->t<=0)
              {
                 a_action[i]->type=ACT_FINISHED;
                 if(a_action[i]->link != ACT_NONE)
                 {
#ifdef DEBUG
                    printf(" activate:");
#endif
                    activate_action(a_action[i]->link);
                  }
#ifdef DEBUG
                 else printf("error: activation after ACT_WAIT failed!");
#endif
              }
         break;
         case ACT_WAITFOR:
              if (a_action[i]->waitforaction->type == ACT_FINISHED)
              {
                 a_action[i]->type=ACT_FINISHED;
                 if(a_action[i]->link != ACT_NONE) activate_action(a_action[i]->link);
              }
         break;
         case ACT_INACTIVE:
#ifdef DEBUG
              printf("Error: finished action %d still in action queue\n", i);
#endif
              allegro_message("error: a finished action is still in action queue");
              allegro_exit();
         break;

      }
   }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////       DRAWING       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned char line_color=15;
void set_line_color(unsigned char color)
{
   line_color=color;
}

void MODEL3D::draw_object_color(int o)
{
   int p, j=0;
   glVertexPointer(3, GL_FLOAT, 0, this->model->o3d[o].vert);
  	for (p = 0; p < this->model->o3d[o].poly_total; p++)
	{
	   glNormal3fv(&this->model->o3d[o].n[p].x);
	   glDrawElements(GL_POLYGON, this->model->o3d[o].ind_total[p], GL_UNSIGNED_SHORT, &this->model->o3d[o].i[j]);
	   j+=this->model->o3d[o].ind_total[p];
	}
}

void MODEL3D::draw_object_alpha(int o, unsigned char alpha)
{
   int p, j=0;
   color16(15, alpha);
   glVertexPointer(3, GL_FLOAT, 0, this->model->o3d[o].vert);
  	for (p = 0; p < this->model->o3d[o].poly_total; p++)
	{ 
	   glNormal3fv(&this->model->o3d[o].n[p].x);
	   glDrawElements(GL_POLYGON, this->model->o3d[o].ind_total[p], GL_UNSIGNED_SHORT, &this->model->o3d[o].i[j]);
	   j+=this->model->o3d[o].ind_total[p];
	} 
}

void MODEL3D::draw_object(int o)
{
   int  p, j=0;
   glVertexPointer(3, GL_FLOAT, 0, this->model->o3d[o].vert);
 	for (p = 0; p < this->model->o3d[o].poly_total; p++)
	{
	   glNormal3fv(&this->model->o3d[o].n[p].x);
	   color16(this->model->o3d[o].color[p]);
	   glDrawElements(GL_POLYGON, this->model->o3d[o].ind_total[p], GL_UNSIGNED_SHORT, &this->model->o3d[o].i[j]);
	   j+=this->model->o3d[o].ind_total[p];
	}
}


void MODEL3D::draw_object_textures(int o)
{
   int  p, j=0;
   glVertexPointer(3, GL_FLOAT, 0, this->model->o3d[o].vert);
	for (p = 0; p < this->model->o3d[o].poly_total; p++)
   {
      if(this->object[o].texture[p].active)
      {
         glColor4f(1, 1, 1, 1);
         glEnable (GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_BLEND, GL_DECAL);
//	      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	      glBindTexture(GL_TEXTURE_2D, this->object[o].texture[p].stereo_bitmap->glbmp[act_eye]);
//	      glBindTexture(GL_TEXTURE_2D, flashtex[act_eye]);
         glBegin (GL_QUADS);
		   glTexCoord2f(this->object[o].texture[p].texcoord[0][0], this->object[o].texture[p].texcoord[0][1]); glVertex3d(this->model->o3d[o].vert[this->model->o3d[o].i[j]].x, this->model->o3d[o].vert[this->model->o3d[o].i[j]].y, this->model->o3d[o].vert[this->model->o3d[o].i[j]].z);
   	   glTexCoord2f(this->object[o].texture[p].texcoord[1][0], this->object[o].texture[p].texcoord[1][1]); glVertex3d(this->model->o3d[o].vert[this->model->o3d[o].i[j+1]].x, this->model->o3d[o].vert[this->model->o3d[o].i[j+1]].y, this->model->o3d[o].vert[this->model->o3d[o].i[j+1]].z);
         glTexCoord2f(this->object[o].texture[p].texcoord[2][0], this->object[o].texture[p].texcoord[2][1]); glVertex3d(this->model->o3d[o].vert[this->model->o3d[o].i[j+2]].x, this->model->o3d[o].vert[this->model->o3d[o].i[j+2]].y, this->model->o3d[o].vert[this->model->o3d[o].i[j+2]].z);
         glTexCoord2f(this->object[o].texture[p].texcoord[3][0], this->object[o].texture[p].texcoord[3][1]); glVertex3d(this->model->o3d[o].vert[this->model->o3d[o].i[j+3]].x, this->model->o3d[o].vert[this->model->o3d[o].i[j+3]].y, this->model->o3d[o].vert[this->model->o3d[o].i[j+3]].z);
         glEnd();
      	glDisable (GL_TEXTURE_2D);
      }
      else
      {
	      glNormal3fv(&this->model->o3d[o].n[p].x);
	      color16(this->model->o3d[o].color[p]);
	      glDrawElements(GL_POLYGON, this->model->o3d[o].ind_total[p], GL_UNSIGNED_SHORT, &this->model->o3d[o].i[j]);
      }
      j+=this->model->o3d[o].ind_total[p];
   }
}

void MODEL3D::draw_object_lines(int o)
{
   glVertexPointer(3, GL_FLOAT, 0, this->model->o3d[o].vert);
   glDrawElements(GL_LINES, this->model->o3d[o].line_total*2, GL_UNSIGNED_SHORT, this->model->o3d[o].line);
}

void MODEL3D::draw_objects_lines(int o)
{
   glPushMatrix();
   glTranslatef(this->model->o3d[o].pos.x, this->model->o3d[o].pos.y, this->model->o3d[o].pos.z);
   if(this->object[o].moved)glMultMatrix_allegro(&this->object[o].m);
   if(this->object[o].visible && this->object[o].grid_is_visible)this->draw_object_lines(o);
   if(this->model->o3d[o].child_o)
   {
      this->draw_objects_lines(this->model->o3d[o].child_o);
   }
   glPopMatrix();
   if(this->model->o3d[o].sibl_o)
   {
      this->draw_objects_lines(this->model->o3d[o].sibl_o);
   }
}

void MODEL3D::draw_objects_alpha(int o, unsigned char alpha)
{
   glPushMatrix();
   glTranslatef(this->model->o3d[o].pos.x, this->model->o3d[o].pos.y, this->model->o3d[o].pos.z);
   if(this->object[o].moved)glMultMatrix_allegro(&this->object[o].m);
   if(this->object[o].visible && this->object[o].grid_is_visible)this->draw_object_alpha(o, alpha);
   if(this->model->o3d[o].child_o)
   {
      this->draw_objects_alpha(this->model->o3d[o].child_o, alpha);
   }
   glPopMatrix();
   if(this->model->o3d[o].sibl_o)
   {
      this->draw_objects_alpha(this->model->o3d[o].sibl_o, alpha);
   }
}

void MODEL3D::draw_objects_color(int o)
{
   glPushMatrix();
   glTranslatef(this->model->o3d[o].pos.x, this->model->o3d[o].pos.y, this->model->o3d[o].pos.z);
   if(this->object[o].moved)glMultMatrix_allegro(&this->object[o].m);
   if(this->object[o].visible && this->object[o].fill_is_visible)
   {
      if(this->object[o].single_colored)
      {
         color16(this->object[o].color);
         this->draw_object_color(o);
      }
      else this->draw_object_color(o);
   }
   if(this->object[o].visible && this->object[o].grid_is_visible)
   {
      glDisable(GL_POLYGON_OFFSET_FILL);
      color16(line_color);
      this->draw_object_lines(o);
      glEnable(GL_POLYGON_OFFSET_FILL);
   }


   if(this->model->o3d[o].child_o)
   {
      this->draw_objects_color(this->model->o3d[o].child_o);
   }
   glPopMatrix();
   if(this->model->o3d[o].sibl_o)
   {
      this->draw_objects_color(this->model->o3d[o].sibl_o);
   }
}


void MODEL3D::draw_objects(int o)
{
   glPushMatrix();
   glTranslatef(this->model->o3d[o].pos.x, this->model->o3d[o].pos.y, this->model->o3d[o].pos.z);
   if(this->object[o].moved)glMultMatrix_allegro(&this->object[o].m);
   if(this->object[o].visible && this->object[o].fill_is_visible)
   {
      if(this->object[o].single_colored)
      {
         color16(this->object[o].color);
         this->draw_object_color(o);
      }
      else if(this->object[o].is_textured) this->draw_object_textures(o);
      else this->draw_object(o);
      }
   if(this->object[o].visible && this->object[o].grid_is_visible)
   {
      glDisable(GL_POLYGON_OFFSET_FILL);
      color16(15);
      this->draw_object_lines(o);
      glEnable(GL_POLYGON_OFFSET_FILL);
   }
   if(this->model->o3d[o].child_o)
   {
      this->draw_objects(this->model->o3d[o].child_o);
   }
   glPopMatrix();
   if(this->model->o3d[o].sibl_o)
   {
      this->draw_objects(this->model->o3d[o].sibl_o);
   }
}

void MODEL3D::draw()
{
   this->draw_objects(0);
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////     FONT-DRAWING     //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void FONT3D::assign_3do(const char filename[])
{
   this->model3d.assign_3do(filename);
}


void FONT3D::draw_char(unsigned char a)
{
   if((a >= 33) && (a <=96)) a-= 33;
   else if((a >= 97) && (a <=122)) a-= 65;
   else if((a >= 123) && (a <=128)) a-= 63;
   else if (a==223) a=73;
   glRotatef(180.0, 0.0, 1.0, 0.0);
   this->model3d.draw_object_color(a);
   glRotatef(-180.0, 0.0, 1.0, 0.0);
}

void FONT3D::draw(char string[])
{
     glFrontFace(GL_CCW);
    int i=0, column=0;
    while(string[i]!='\0')
    {
       if(string[i]=='\n')
       {
          glTranslatef(-column*4,-6, 0);
          column=0;
       }
       else if(string[i]==' ')
       {
          glTranslatef(4,0,0);
          column++;
       }
       else
       {
          this->draw_char(string[i]);
          glTranslatef(4,0,0);
          column++;
       }
       i++;
    }
    glFrontFace(GL_CW);
}
