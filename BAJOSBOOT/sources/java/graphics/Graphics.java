package java.graphics;
// not intantionable, but extendable
public class Graphics	implements Konstanten {

public static float r;		// globale Farben für Zustandsmaschine
public static float g;
public static float b;		// 0.0 .... 1.0
/*
void rgb2hsv( int r, int g, int b, int *h, int *s, int *v )
// r,g,b in 0..255 h in 0..360 s,v in 0..100 
{ int d; float fs;
   *v = r; *h = 0; d = g - b;
   if ( *v < g ) { *v = g; *h = 120; d = b - r; }
   if ( *v < b ) { *v = b; *h = 240; d = r - g; }
   // *v = max( r, g, b ) 
   if ( *h == 0 && d < 0 ) *h = 360; // max = r; b > g 
   if ( *v == 0 ) fs = 0.0; else fs = ( *v - min( r, g, b ) ) / *v;
   if ( fs == 0.0 ) *h = 0; else *h += 60 * d / ( fs * *v );
   *s = 100 * fs; *v /= 2.55;
}
*/
/*
void hsv2rgb( int h, int s, int v)
// h in 0..360 s,v in 0..100 
{ float fs = s / 255, fv = v / 255, min, f;
   if ( s == 0 ) { *r = *g = *b = v; }
   min = 255 * fv * ( 1 - fs ); f = 4.25 * fs * fv;
   if ( h > 60 && h < 180 )
    { *g = v;
       if ( h < 120 ) { *b = min; *r = min - f * ( h - 120 ); }
        else { *r = min; *b = min - f * ( h - 120 ); }
    }
    else if ( h > 180 && h < 300 )
     { *b = v;
       if ( h < 240 ) { *r = min; *g = min - f * ( h - 240 ); }
        else { *g = min; *r = min - f * ( h - 240 ); }
     }
     else { *r = v;
                if ( h > 300 ) { *g = min; *b = min - f * ( h - 360 ); }
                 else { *b = min; *g = min - f * h; }
            }
}
*/

void drawPoint(Point p)	{
PlatForm.drawPoint(p.x,p.y,p.r,p.g,p.b);
}
}