#ifndef STEREOGL
#define STEREOGL


////////////////////////////////////////////////////////////////////////////////
////////////////////////////        TIMER         //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void  init_timer();
void  proceed_timer();
void limit_fmult(float limit);

float get_fps();
float get_fmult();
int   get_chrono();
////////////////////////////////////////////////////////////////////////////////
////////////////////////////        MATH         ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct VECTOR
{
   float x, y, z;
} VECTOR;

class MATR_STACK  //Zur Simulation von OpenGL-Matrix-Stackberechnungen
{
   private:
   int total;
   MATRIX_f actual;
   MATRIX_f **stack;
   public:
   void mul(MATRIX_f *m);
   void mul_alg(MATRIX_f *m);
   void push();
   void pop();
   void loadIdentity();
   void translate(VECTOR *v);
   MATRIX_f get();
   MATR_STACK(int maxstackmatrices);
   ~MATR_STACK(){delete stack;}
};

VECTOR trans_matrix_to_vector(MATRIX_f *m);
bool accelerate(float soll, float *ist, float a);
void translate_matrix_v(MATRIX_f *m, VECTOR *pos);
void glMultMatrix_allegro(MATRIX_f *m);
MATRIX_f matr(float x, float y, float z, float xrot, float yrot, float zrot);
VECTOR vect(float x, float y, float z);
void invert_matrix(MATRIX_f *in, MATRIX_f *out);
void glMultMatrix_allegro_inv(MATRIX_f *m);
void get_matrix_delta(MATRIX_f *m_act, MATRIX_f *m_old, MATRIX_f *m_delta);

////////////////////////////////////////////////////////////////////////////////
////////////////////      MODEL LOADING AND ANIMATION      /////////////////////
////////////////////////////////////////////////////////////////////////////////


#define   NOW       0
#define   DEFAULT   81948
#define   X_AXIS    0
#define   Y_AXIS    1
#define   Z_AXIS    2


class O3D  //Unterobjekt, Teil der Model-Datenstruktur
{
   public:
   ~O3D(){delete n, vert, color, line, i, c;}
   char name[32];   //Name (aus 3do-File)
   int vert_total;  //wieviel Koordinatenpunkte?
   int poly_total;  //wieviel Flächen?
   int line_total;   //wieviel Linien (f. Outlines)
   int sibl_o;	     //Schwesterobjekt
   int child_o;   //Tochterobjekt
   VECTOR pos;      //Offset
   VECTOR *n;       //Zeiger auf Normalvektoren
   VECTOR *vert;    //Zeiger auf Koordinatenpunkte
   char *ind_total;   //Zeiger auf Datenfeld mit der Anzahl der Indizes zum Inhalt
   char *color;     //Zeiger auf Farbinformationen
   short *line;     //Zeiger auf Linien
   short *i;        //Zeiger auf Indizes
   char *c;         //
   bool single_colored; //ist das Objekt einfarbig?
};

class MODEL
{
   public:
   ~MODEL(){delete o3d;}
   int obj_total;               //Anzahl der Objekte (O3D)
   O3D  *o3d;                  //Zeiger auf Unterobjekte
   float size;                 //
   float radius;               //Größter Abstand vom Mittelpunkt
   VECTOR min;                 //der kleinste x, y und z-Wert des Modells
   VECTOR max;                 //der größte x, y und z-Wert des Modells
   const char *name;           //filename des 3dos
};

typedef struct STEREO_BITMAP
{
   const char *name;
   GLuint glbmp[2];
} STEREO_BITMAP;

typedef struct TEXTURE
{
   int orientation;
   bool active;
   STEREO_BITMAP *stereo_bitmap;
   float texcoord[4][2];
} TEXTURE;

class OBJECT
{
   public:
   OBJECT::OBJECT();
   ~OBJECT(){if(is_textured)delete texture;}
   MATRIX_f m;                   //Trans. und Rot. des Objektes
   bool moved;                   //ist Objekt nicht auf Standardposition?
   bool visible;                 //ist Objekt sichtbar?
   float winkel[3];              //Drehung des Objektes in Winkelangaben
   bool grid_is_visible;         //Outlines sichtbar?
   bool fill_is_visible;         //Flächen sichtbar?
   int color;                    //Farbe des Objektes (überschreibt Standard)
   bool single_colored;          //Objekt einfarbig?
   unsigned char alpha;           //Objekt (halb)durchsichtig?
   bool is_textured;             //werden Texturen verwendet
   TEXTURE *texture;             //Array mit Texturendaten
};

void init_models(int max_models);
void init_stereo_bitmaps(int max_stereo_bitmaps);
void init_actions(int max_active_actions, int max_inactive_actions);
void free_actions();
void proceed_actions();
int get_active_actions_total();
int get_inactive_actions_total();
int get_models_total();
void set_line_color(unsigned char color);
MODEL* load_3do(const char name[]);
void unload_3do(const char name[]);

typedef void (*ACTION_FUNCTION0)();
typedef void (*ACTION_FUNCTION1)(void*);
typedef void (*ACTION_FUNCTION2)(void*, void*); //Funktionspointer

class MODEL3D
{
   private:
   bool get_object_matrix_recursive(int o, int o_soll);
   MODEL *model;
   bool assigned;
   
   public:
   int action_group;
   MODEL3D(){assigned=0;action_group=0;}
   OBJECT *object;
   void kill_all_actions();
   int get_object_by_name(const char name[]);
   MATRIX_f get_object_matrix_world(int o, MATRIX_f *m);
   MATRIX_f get_object_matrix(int o);
   float get_size(){return(model->size);}
   
   void assign_3do(const char name[]);
   void clear_action_group(int a_g);
   void clear_all_actions();
   void reset_object_matrix(int o);
   void reset_all_object_matrices();
   bool assign_texture(const char name[], int face, const char texture_name[], float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy);
   bool assign_texture(const char name[], int face, const char texture_name[],const char alpha_texture_name[],  float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy);

   void set_action_group(int i){action_group=i;}
   int  get_action_group(){return(action_group);}

   void change_integer(int *change_it, int value);
   void change_float(float *change_it, float value);
   void explode(const char name[], float dir_x, float dir_y, float dir_z, float rot_speed, float gravity, int duration);
   void turn(const char name[], int axis, float amount, float speed);
   void turn_short(const char name[], int axis, float amount, float speed);
   void move(const char name[], int axis, float amount, float speed);
   void spin(const char name[], char axis, float speed);
   bool stop_spin(const char name[], int axis);
   void hide(int o);
   void hide(const char name[]);
   void show(int o);
   void show(const char name[]);
   void hide_grid(const char name[]);
   void hide_grid(int o);
   void hide_fill(const char name[]);
   void show_fill(const char name[]);
   void show_grid(const char name[]);
   void blink(const char name[], int time_on, int time_off, int col16_on, int col16_off);
   bool stop_blink(const char name[]);
   void set_color(const char name[], int col);
   void set_color(int o, int col);
   void start_function(ACTION_FUNCTION0 action_function); //for starting callback-functions
   void start_function(ACTION_FUNCTION1 action_function, void *pointer);
   void start_function(ACTION_FUNCTION2 action_function, void *pointer1, void *pointer2);
   void reset(const char name[]);
   bool wait_for_move(const char name[], char axis);
   bool wait_for_turn(const char name[], char axis);
   void wait(int t);

   void draw_object(int o);
   void draw_object_alpha(int o, unsigned char alpha);
   void draw_object_lines(int o);
   void draw_object_color(int o);
   void draw_objects(int o);
   void draw_objects_lines(int o);
   void draw_objects_color(int o);
   void draw_objects_alpha(int o, unsigned char alpha);
   void draw_object_textures(int o);
   void draw();
   ~MODEL3D();
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////       STEREO-VIEW       ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define LEFT_EYE 0
#define RIGHT_EYE 1


class STEREO
{
   private:
   float distance_to_monitor;
   float half_eye_seperation;
   float monitor_width;
   float half_monitor_width;
   float monitor_height;
   float half_monitor_height;
   float far_clip;
   float clipscale;
   float near_clip;
   float eye_seperation;
   float scale;

   public:
   STEREO();
   void init_lighting();
   void set_eye(bool eye);
   void position_camera(MATRIX_f *camera_matrix);

   void  set_eye_seperation(float value){eye_seperation=value; half_eye_seperation=value/2;}
   float get_eye_seperation(){return(eye_seperation);}
   void  set_monitor_height(float value){monitor_height=value;half_monitor_height=value/2;}
   float get_monitor_height(){return(monitor_height);}
   void  set_monitor_width(float value){monitor_width=value;half_monitor_width=value/2;}
   float get_monitor_width(){return(monitor_width);}
   void  set_distance_to_monitor(float value){distance_to_monitor=value;clipscale=near_clip/distance_to_monitor;}
   float get_distance_to_monitor(){return(distance_to_monitor);}
   void  set_far_clip(float value){far_clip=value;}
   float get_far_clip(){return(far_clip);}
   void  set_near_clip(float distance_to_face){if(distance_to_face > 0){near_clip=distance_to_face;clipscale=near_clip/distance_to_monitor;}}
   void  set_near_clip_to_monitor(){near_clip=distance_to_monitor;clipscale=1.0;}
   float get_near_clip(){return(near_clip);}
   void  set_scale(float value){scale=value;}
   float get_scale(){return(scale);}
   float get_frustum_angle_x_360();
   float get_frustum_angle_y_360();
};

typedef struct CFG_FILE
{
   char ct_file_name[32];
   float eye_seperation;
   float screen_width;
   float screen_height;
   float screen_distance;
   long resolution_x;
   long resolution_y;
   long resolution_index;
   bool is_windowed;

   bool ct_file_is_used;
   bool resolution_is_used;
   bool eye_seperation_is_used;
   bool screen_width_is_used;
   bool screen_height_is_used;
   bool screen_distance_is_used;
   bool is_windowed_is_used;
}CFG_FILE;

bool load_cfg(char *file_name, CFG_FILE *cfg_file);
bool save_cfg(char *file_name, CFG_FILE *cfg_file);



////////////////////////////////////////////////////////////////////////////////
//////////////////////////       STEREO-COLOR        ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

void load_color_table(char file[]);  //initialize graphic before calling this function
void color16(char lumi);
void color16(char lumi, unsigned char alpha);
void color256(unsigned char lumi);
void color256(unsigned char lumi, unsigned char alpha);
void update_color_fx();
void convert_to_stereo_bitmap(BITMAP *bmp, bool eye);
inline bool get_eye();
char* get_table_file();
void get_color256(bool eye, unsigned char lumi, unsigned char *r, unsigned char *g, unsigned char *b);
void get_background_color(unsigned char *r, unsigned char *g, unsigned char *b);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////     FONT-DRAWING     //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class FONT3D
{
   private:
   void draw_char(unsigned char a);
   public:
   MODEL3D model3d;
   void draw(char string[]);
   void assign_3do(const char filename[]);
};

#endif
