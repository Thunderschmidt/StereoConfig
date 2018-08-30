#ifndef CTLOAD
#define CTLOAD
/*
 *   Header file of the library class to translate graphics to 3D 
 *
 */

#define MAX_NODES   9            // Ungerade zahlen für gerade Abstände
#define MAX_LINES   3
#define MAX_POINTS  256
#define MAX_POINTS_SPLINE  256
#define MAX_LUM     255
#define BACKGROUND 40


// Adjustable nodes on the spline curves
typedef struct NODE
{
   int   dummy;
   int   xval,yval;
   fixed tangent;
} NODE;

typedef enum ENM_LINE_STYLE
{
   STYLE_SPLINED = 0,
   STYLE_LINEAR
}ENM_LINE_STYLE;

// Splined lines of one color 
typedef struct LINE
{
   ENM_LINE_STYLE Style;
   char           Name;
   GLubyte            Max_Brness;
   NODE           nodes[MAX_NODES+2];
   GLubyte        Lum[MAX_POINTS];
//   int    xval[MAX_POINTS];
//   int    yval[MAX_POINTS];   
} LINE;
 
// Table of one color set
typedef struct COL_TABLE
{
   LINE   Lines[MAX_LINES];
   GLubyte    Max_Lum;
   GLubyte    Backgnd[3];
} COL_TABLE;


typedef enum ENM_LINE
{
   COLOR_RED = 0,
   COLOR_GREEN,
   COLOR_BLUE
}ENM_LINE;

typedef enum ENM_SIDE
{
   SIDE_RIGHT = 0,
   SIDE_LEFT
}ENM_SIDE;

      
class ColorConversion
{
   private:
      COL_TABLE *tab_ptr[];
         
   public:
      ColorConversion();
      ~ColorConversion();
      
      void InitData(void);
      
      void GetNode(ENM_SIDE Side,ENM_LINE Line, int Num, NODE *Node);
      void SetNode(ENM_SIDE Side,ENM_LINE Line, NODE *Node, int Num);

      // An Array with all luminance values of a color 
      void GetLum(ENM_SIDE Side,ENM_LINE Line, GLubyte Lum[MAX_POINTS]);
      void SetLum(ENM_SIDE Side,ENM_LINE Line, GLubyte Lum[MAX_POINTS]);
      
      // Limit the luminance of a color
      GLubyte  GetMaxLum(ENM_SIDE Side,ENM_LINE Line);
      void SetMaxLum(ENM_SIDE Side,ENM_LINE Line, GLubyte MaxLum);

      void GetBackgnd(GLubyte *Red, GLubyte *Green, GLubyte *Blue);
      void SetBackgnd(GLubyte Red, GLubyte Green, GLubyte Blue);
      
      // Get or store the table
      int LoadConf(char* file_name);
      int SaveConf(char* file_name);
      
      // Delivers a luminance value (Eye, Color, Lum. Value)
      GLubyte  GetValue(ENM_SIDE Side,ENM_LINE Line, int Val);
};


#endif
