/*
* FHW-Berlin, Fachbereich Berufsakademie, Fachrichtung Informatik
* See the file "license.terms" for information on usage and redistribution of this file.
*/
import java.graphics.*;

public class B
{

	public static void main(String [] args)
	{		

		System.out.println ("Starting a.class ...");
		
		System.out.println (" make a Display Object...");
		//DisplayZBuffer dp = new DisplayZBuffer();
		Display dp = new Display();
		
		//dp.drawPoint(2, 2); 		

		//dp.setColor(0x000000); //schwarz
		dp.drawRectangle(10, 100, 60, 160);
		//BGR
		dp.setColor(0xff0000); //blau
		dp.drawFillRect(20,20,30,30);

		//dp.drawClearRect(int x1, int y1, int x2, int y2)
		dp.setColor(0x00ff00); //gruen...
		dp.drawRoundRect(70, 70, 100, 100, 10, 20);
		dp.setColor(0x0000ff); //rot
		dp.drawEllipse(110, 110, 70, 220);
		dp.drawCircle(160, 160, 30);
		dp.drawLine(1,1,310,230);

		/*Point a, e;
		a=new Point(1,1);
		a.HSBColor(0, 255, 255);
		e=new Point(310,230);
		e.HSBColor(255, 255, 255);
		
		((DisplayHSB)dp).drawLine(a, e);
		
		Point p0, p1, p2;
		p0=new Point(50,120);
		p0.HSBColor(0, 255, 255);
		p1=new Point(230,160);
		p1.HSBColor(127, 255, 255);
		p2=new Point(70,180);
		p2.HSBColor(255, 255, 255);

		((DisplayHSB)dp).triangleFill(p0, p1, p2);
		*/
		/*Font f;
		f = new Font();
		//f=dp.font;

		f.drawString("Java draw String\n");
*/

/*		System.platform.clearScreenRGB(0xffffff);
		
		//Z-Buffer
		Point pa, pb;
		pa=new Point(100,10,50);
		pa.HSBColor(255, 255, 255);
		pb=new Point(100,100,80);
		pb.HSBColor(255, 255, 255);
		
		dp.drawLine(pa, pb);


		//Point pa, pb;
		pa=new Point(120,10,10);
		pa.HSBColor(255, 255, 0);
		pb=new Point(80,100,30);
		pb.HSBColor(255, 255, 0);
	
		dp.drawLine(pa,pb);*/

		System.out.println (" ENDE");

		while (true) {

		}

	}
}
