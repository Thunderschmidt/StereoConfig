/*
 *  Da muﬂ man Hannes fragen
 */

#include <allegro.h>
#include <alleggl.h>

#include "ctload.h"


void ColorConversion::InitData(void)
{

    tab_ptr[0] = new COL_TABLE;
    tab_ptr[1] = new COL_TABLE;
    tab_ptr[0]->Max_Lum = MAX_LUM;
    tab_ptr[1]->Max_Lum = MAX_LUM;
    tab_ptr[0]->Backgnd[0] = BACKGROUND;
    tab_ptr[0]->Backgnd[1] = BACKGROUND;
    tab_ptr[0]->Backgnd[2] = BACKGROUND;
    tab_ptr[1]->Backgnd[0] = BACKGROUND;
    tab_ptr[1]->Backgnd[1] = BACKGROUND;
    tab_ptr[1]->Backgnd[2] = BACKGROUND;

// Right side
   // Red
   tab_ptr[0]->Lines[0].Style = STYLE_LINEAR;
   tab_ptr[0]->Lines[0].Name  = 'R';
   tab_ptr[0]->Lines[0].Max_Brness = MAX_LUM;

   // Red Nodes
   tab_ptr[0]->Lines[0].nodes[0].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[0].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[0].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[0].tangent = 0;   

   tab_ptr[0]->Lines[0].nodes[1].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[1].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[1].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[1].tangent = 0;   

   tab_ptr[0]->Lines[0].nodes[2].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[2].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[2].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[2].tangent = 0;   

   tab_ptr[0]->Lines[0].nodes[3].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[3].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[3].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[3].tangent = 0;   
                             
   tab_ptr[0]->Lines[0].nodes[4].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[4].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[4].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[4].tangent = 0;   
     
   tab_ptr[0]->Lines[0].nodes[5].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[5].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[5].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[5].tangent = 0;   
                             
   tab_ptr[0]->Lines[0].nodes[6].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[6].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[6].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[6].tangent = 0;   

   tab_ptr[0]->Lines[0].nodes[7].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[7].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[7].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[7].tangent = 0;   
                             
   tab_ptr[0]->Lines[0].nodes[8].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[8].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[8].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[8].tangent = 0;   

   tab_ptr[0]->Lines[0].nodes[9].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[9].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[9].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[9].tangent = 0;   
                    
   tab_ptr[0]->Lines[0].nodes[10].dummy   = 0;
   tab_ptr[0]->Lines[0].nodes[10].xval    = 0;
   tab_ptr[0]->Lines[0].nodes[10].yval    = 0;
   tab_ptr[0]->Lines[0].nodes[10].tangent = 0;   

   // Green
   tab_ptr[0]->Lines[1].Style = STYLE_SPLINED;
   tab_ptr[0]->Lines[1].Name  = 'G';
   tab_ptr[0]->Lines[1].Max_Brness = MAX_LUM;

   // Green Nodes
   tab_ptr[0]->Lines[1].nodes[0].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[0].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[0].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[0].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[1].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[1].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[1].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[1].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[2].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[2].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[2].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[2].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[3].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[3].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[3].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[3].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[4].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[4].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[4].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[4].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[5].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[5].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[5].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[5].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[6].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[6].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[6].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[6].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[7].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[7].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[7].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[7].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[8].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[8].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[8].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[8].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[9].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[9].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[9].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[9].tangent = 0;   
                                     
   tab_ptr[0]->Lines[1].nodes[10].dummy   = 0;
   tab_ptr[0]->Lines[1].nodes[10].xval    = 0;
   tab_ptr[0]->Lines[1].nodes[10].yval    = 0;
   tab_ptr[0]->Lines[1].nodes[10].tangent = 0;   

   // Blue
   tab_ptr[0]->Lines[2].Style = STYLE_SPLINED;
   tab_ptr[0]->Lines[2].Name  = 'B';
   tab_ptr[0]->Lines[2].Max_Brness = MAX_LUM;

   // Blue Nodes
   tab_ptr[0]->Lines[2].nodes[0].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[0].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[0].yval    = 0;
   tab_ptr[0]->Lines[2].nodes[0].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[1].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[1].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[1].yval    = 0;
   tab_ptr[0]->Lines[2].nodes[1].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[2].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[2].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[2].yval    = 30;
   tab_ptr[0]->Lines[2].nodes[2].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[3].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[3].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[3].yval    = 60;
   tab_ptr[0]->Lines[2].nodes[3].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[4].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[4].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[4].yval    = 90;
   tab_ptr[0]->Lines[2].nodes[4].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[5].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[5].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[5].yval    = 120;
   tab_ptr[0]->Lines[2].nodes[5].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[6].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[6].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[6].yval    = 150;
   tab_ptr[0]->Lines[2].nodes[6].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[7].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[7].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[7].yval    = 180;
   tab_ptr[0]->Lines[2].nodes[7].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[8].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[8].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[8].yval    = 210;
   tab_ptr[0]->Lines[2].nodes[8].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[9].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[9].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[9].yval    = 240;
   tab_ptr[0]->Lines[2].nodes[9].tangent = 0;   
                                     
   tab_ptr[0]->Lines[2].nodes[10].dummy   = 0;
   tab_ptr[0]->Lines[2].nodes[10].xval    = 0;
   tab_ptr[0]->Lines[2].nodes[10].yval    = 0;
   tab_ptr[0]->Lines[2].nodes[10].tangent = 0;   

// Left side
   // Red
   tab_ptr[1]->Lines[0].Style = STYLE_LINEAR;
   tab_ptr[1]->Lines[0].Name  = 'R';
   tab_ptr[1]->Lines[0].Max_Brness = MAX_LUM;

   // Red Nodes
   tab_ptr[1]->Lines[0].nodes[0].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[0].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[0].yval    = 0;
   tab_ptr[1]->Lines[0].nodes[0].tangent = 0;   

   tab_ptr[1]->Lines[0].nodes[1].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[1].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[1].yval    = 0;
   tab_ptr[1]->Lines[0].nodes[1].tangent = 0;   

   tab_ptr[1]->Lines[0].nodes[2].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[2].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[2].yval    = 20;
   tab_ptr[1]->Lines[0].nodes[2].tangent = 0;   
        
   tab_ptr[1]->Lines[0].nodes[3].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[3].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[3].yval    = 40;
   tab_ptr[1]->Lines[0].nodes[3].tangent = 0;   
                             
   tab_ptr[1]->Lines[0].nodes[4].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[4].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[4].yval    = 60;
   tab_ptr[1]->Lines[0].nodes[4].tangent = 0;   
        
   tab_ptr[1]->Lines[0].nodes[5].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[5].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[5].yval    = 80;
   tab_ptr[1]->Lines[0].nodes[5].tangent = 0;   
                             
   tab_ptr[1]->Lines[0].nodes[6].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[6].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[6].yval    = 100;
   tab_ptr[1]->Lines[0].nodes[6].tangent = 0;   
        
   tab_ptr[1]->Lines[0].nodes[7].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[7].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[7].yval    = 120;
   tab_ptr[1]->Lines[0].nodes[7].tangent = 0;   
                             
   tab_ptr[1]->Lines[0].nodes[8].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[8].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[8].yval    = 140;
   tab_ptr[1]->Lines[0].nodes[8].tangent = 0;   
        
   tab_ptr[1]->Lines[0].nodes[9].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[9].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[9].yval    = 160;
   tab_ptr[1]->Lines[0].nodes[9].tangent = 0;   
                    
   tab_ptr[1]->Lines[0].nodes[10].dummy   = 0;
   tab_ptr[1]->Lines[0].nodes[10].xval    = 0;
   tab_ptr[1]->Lines[0].nodes[10].yval    = 0;
   tab_ptr[1]->Lines[0].nodes[10].tangent = 0;   

   // Green
   tab_ptr[1]->Lines[1].Style = STYLE_SPLINED;
   tab_ptr[1]->Lines[1].Name  = 'G';
   tab_ptr[1]->Lines[1].Max_Brness = MAX_LUM;

   // Green Nodes
   tab_ptr[1]->Lines[1].nodes[0].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[0].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[0].yval    = 0;
   tab_ptr[1]->Lines[1].nodes[0].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[1].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[1].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[1].yval    = 40;
   tab_ptr[1]->Lines[1].nodes[1].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[2].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[2].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[2].yval    = 40;
   tab_ptr[1]->Lines[1].nodes[2].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[3].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[3].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[3].yval    = 40;
   tab_ptr[1]->Lines[1].nodes[3].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[4].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[4].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[4].yval    = 39;
   tab_ptr[1]->Lines[1].nodes[4].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[5].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[5].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[5].yval    = 37;
   tab_ptr[1]->Lines[1].nodes[5].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[6].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[6].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[6].yval    = 32;
   tab_ptr[1]->Lines[1].nodes[6].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[7].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[7].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[7].yval    = 24;
   tab_ptr[1]->Lines[1].nodes[7].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[8].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[8].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[8].yval    = 15;
   tab_ptr[1]->Lines[1].nodes[8].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[9].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[9].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[9].yval    = 8;
   tab_ptr[1]->Lines[1].nodes[9].tangent = 0;   
                                     
   tab_ptr[1]->Lines[1].nodes[10].dummy   = 0;
   tab_ptr[1]->Lines[1].nodes[10].xval    = 0;
   tab_ptr[1]->Lines[1].nodes[10].yval    = 0;
   tab_ptr[1]->Lines[1].nodes[10].tangent = 0;   

   // Blue
   tab_ptr[1]->Lines[2].Style = STYLE_SPLINED;
   tab_ptr[1]->Lines[2].Name  = 'B';
   tab_ptr[1]->Lines[2].Max_Brness = MAX_LUM;

   // Blue Nodes
   tab_ptr[1]->Lines[2].nodes[0].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[0].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[0].yval    = 0;
   tab_ptr[1]->Lines[2].nodes[0].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[1].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[1].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[1].yval    = 40;
   tab_ptr[1]->Lines[2].nodes[1].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[2].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[2].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[2].yval    = 40;
   tab_ptr[1]->Lines[2].nodes[2].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[3].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[3].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[3].yval    = 39;
   tab_ptr[1]->Lines[2].nodes[3].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[4].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[4].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[4].yval    = 39;
   tab_ptr[1]->Lines[2].nodes[4].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[5].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[5].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[5].yval    = 38;
   tab_ptr[1]->Lines[2].nodes[5].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[6].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[6].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[6].yval    = 34;
   tab_ptr[1]->Lines[2].nodes[6].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[7].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[7].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[7].yval    = 33;
   tab_ptr[1]->Lines[2].nodes[7].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[8].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[8].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[8].yval    = 29;
   tab_ptr[1]->Lines[2].nodes[8].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[9].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[9].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[9].yval    = 24;
   tab_ptr[1]->Lines[2].nodes[9].tangent = 0;   
                                     
   tab_ptr[1]->Lines[2].nodes[10].dummy   = 0;
   tab_ptr[1]->Lines[2].nodes[10].xval    = 0;
   tab_ptr[1]->Lines[2].nodes[10].yval    = 0;
   tab_ptr[1]->Lines[2].nodes[10].tangent = 0;   
}


ColorConversion::ColorConversion()
{       
    InitData();
}

ColorConversion::~ColorConversion()
{       
    delete tab_ptr[0];
    delete tab_ptr[1];
}


         
// Load Configuration
int ColorConversion::LoadConf(char* file_name)
{
   FILE *in;
   int bts;

   if ((in = fopen(file_name, "rb"))
       == NULL)
   {
      allegro_message("Cannot open input file: %s !\n", file_name);
      return 1;
   }

   bts = fread(tab_ptr[0], sizeof(COL_TABLE), 1, in);
   bts = fread(tab_ptr[1], sizeof(COL_TABLE), 1, in);
   
   fclose(in);

   return(0);
}


// Save Config to File
int ColorConversion::SaveConf(char* file_name)
{
   FILE *out;

   if ((out = fopen(file_name, "wb"))
       == NULL)
   {
      allegro_message("Cannot open output file: %s !\n", file_name);
      return 1;
   }

   fwrite(tab_ptr[0], sizeof(COL_TABLE), 1, out);
   fwrite(tab_ptr[1], sizeof(COL_TABLE), 1, out);
   
   fclose(out);

   return(0);
}

void ColorConversion::GetNode(ENM_SIDE Side, ENM_LINE Line, int Num, NODE *Node)
{
   Node->dummy = 0;
   Node->dummy   = tab_ptr[Side]->Lines[Line].nodes[Num].dummy;
   Node->tangent = tab_ptr[Side]->Lines[Line].nodes[Num].tangent;
   Node->xval    = tab_ptr[Side]->Lines[Line].nodes[Num].xval;
   Node->yval    = tab_ptr[Side]->Lines[Line].nodes[Num].yval;
}

void ColorConversion::SetNode(ENM_SIDE Side,ENM_LINE Line, NODE *Node, int Num)
{
   tab_ptr[Side]->Lines[Line].nodes[Num].dummy   = Node->dummy;
   tab_ptr[Side]->Lines[Line].nodes[Num].tangent = Node->tangent;
   tab_ptr[Side]->Lines[Line].nodes[Num].xval    = Node->xval;
   tab_ptr[Side]->Lines[Line].nodes[Num].yval    = Node->yval;
}

void ColorConversion::GetLum(ENM_SIDE Side,ENM_LINE Line, GLubyte Lum[MAX_POINTS])
{
   register int i;
   
   for(i=0;i <MAX_POINTS;i++)
      Lum[i] = tab_ptr[Side]->Lines[Line].Lum[i];   
}

void ColorConversion::SetLum(ENM_SIDE Side,ENM_LINE Line, GLubyte Lum[MAX_POINTS])
{
   register int i;
   
   for(i=0;i <MAX_POINTS;i++)
      tab_ptr[Side]->Lines[Line].Lum[i] = Lum[i];   
}

GLubyte ColorConversion::GetMaxLum(ENM_SIDE Side,ENM_LINE Line)
{
   return tab_ptr[Side]->Lines[Line].Max_Brness;
}

void ColorConversion::SetMaxLum(ENM_SIDE Side,ENM_LINE Line, GLubyte MaxLum)
{
   tab_ptr[Side]->Lines[Line].Max_Brness = MaxLum;   
}

GLubyte ColorConversion::GetValue(ENM_SIDE Side,ENM_LINE Line, int Val)
{
   return tab_ptr[Side]->Lines[Line].Lum[Val];
}

void ColorConversion::GetBackgnd(GLubyte *Red, GLubyte *Green, GLubyte *Blue)
{
   *Red   = tab_ptr[0]->Backgnd[0];
   *Green = tab_ptr[0]->Backgnd[1];
   *Blue  = tab_ptr[0]->Backgnd[2];
}

void ColorConversion::SetBackgnd(GLubyte Red, GLubyte Green, GLubyte Blue)
{
   tab_ptr[0]->Backgnd[0] = Red;
   tab_ptr[0]->Backgnd[1] = Green;
   tab_ptr[0]->Backgnd[2] = Blue;
}


