/* GLCD Library für's Nokia 6100
   copyright 2004 Hagen Reddmann 

   Umsetzung für STK1000, Christian Hecht, Berufsakademie Berlin 
   
*/
#include "lib2d.h"
#include "arial16.h"
//#include "ComicSansMS26.h"
#include "test3.h"

#ifndef FONTLIB_H_
#define FONTLIB_H_

#define FONTS_ARRAY_LENGTH 		2

#define COLOR_TABLE_BITS        4      // 2   2^2 = 4 Farben in Farbtabelle, wichtig für Fonts
#define FONT_HEADER_SIZE        7      // Header größe eines Fonts
#define lcdBkColor lcd_Colors[0]
#define lcdFgColor lcd_Colors[1]
#define lcdSetColor(index, color) {lcd_Colors[index] = color;}
#define lcdSetBkColor(color) {lcdSetColor(0, color);}
#define lcdSetFgColor(color) {lcdSetColor(1, color);}
#define lcdSetColors(fgColor, bkColor) {lcdSetFgColor(fgColor); lcdSetBkColor(bkColor);}
#define lcdSetRect(r,x1,y1,x2,y2) {r.X1 = x1; r.Y1 = y1; r.X2 = x2; r.Y2 = y2;}


typedef u2 *lcdFontData_t;      // zeiger auf Fontdaten

typedef struct {
    lcdCoord_t X;
    lcdCoord_t Y;
} lcdPoint_t;

typedef struct {
    lcdCoord_t X1;
    lcdCoord_t Y1;
    lcdCoord_t X2;
    lcdCoord_t Y2;
} lcdRect_t;

typedef struct {
    u2 AutoLineFeed:1,        // Fontroutine nutzt einen impliziten Zeilenumbruch innerhalb von glcd_Window
            FixedFont:1;           // Fontroutine soll Font mit fester Zeichenbreite darstellen, ansonsten proportional
} lcdFlags_t;

typedef u2 (*lcdFontFunc_t)(const lcdFontData_t index); // Fontdaten-Lese-Callback, somit können die Fontdaten extern gespeichert sein


lcdPoint_t     lcd_Cursor;          // aktuelle Position für Schriften
lcdRect_t      lcd_Window;          // aktuelles Fenster für Textausgaben
//lcdRect_t      lcd_Clip;            // Clipping Koordinaten, alles was ausserhalb ist wird nicht dargestellt
char            lcd_FontFirstChar;   // erstes Zeichen im Font
char            lcd_FontLastChar;    // letztes Zeichen im Font
u2         lcd_FontWidth;       // fixed Zeichenbreite des Fonts
u2         lcd_FontHeight;      // Zeichenhöhe
u2         lcd_FontBitsPixel;   // Farbtiefe des Fonts
lcdFontData_t  lcd_FontData;        // die eigentlichen Fontdaten beginnen hier, abhängig von der Readcallback
lcdFlags_t     lcd_Flags;
lcdFontFunc_t  lcd_FontRead;        // User definierte Readcallback für die Fontdaten
lcdColor_t     lcd_Colors[1 << COLOR_TABLE_BITS]; // Farbtabelle, Colors[0] = Hintergrund = Füllfarbe, Color[1] = Vordergrund = Linienfarbe
lcdFontData_t  lcd_Fonts[FONTS_ARRAY_LENGTH];


void initFont(void);
void lcdSelectFontFromArray(const u2 index);
void lcdSelectFont(const void * data, const lcdFontFunc_t callback);
u2 lcdFontRead(const lcdFontData_t index);
void lcdNewLine(void);
u2 lcdDrawChar(const char c);
u2 lcdCharWidth(const char c);
u4 lcdPrint(const char *addr);
u4 lcdCharsWidth(const char *addr);
u4 lcdDrawStringAt(const lcdCoord_t x, const lcdCoord_t y, const char *addr, const lcdColor_t fgcolor, const lcdColor_t bkcolor);
u2 lcdDrawCharAt(const lcdCoord_t x, const lcdCoord_t y, const char c, const lcdColor_t fgcolor, const lcdColor_t bkcolor);

#endif /*FONTLIB_H_*/
