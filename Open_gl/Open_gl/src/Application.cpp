#include <GL/glut.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

class Function {
	double* x;
	double* y;

	int number_of_lines;
	const char* File;
	double a, b, c;
public:
	Function(char* name)
	:File(name){
		number_of_lines = 0;
		string l;
		ifstream f(File);

		if (!(f.is_open()))  
			cout << "ERROR: " << File;

		while (!f.eof()) {
			getline(f, l);
			number_of_lines++;
		}
		f.close();

		x = new double[number_of_lines];
		y = new double[number_of_lines];
	}
	~Function() {
		delete[] x;
		delete[] y;
	}

	void read(char* name, int n) {
				
		ifstream f(name);

		if (!(f.is_open())) 
			cout << "ERROR: " << name;
		else
		{
			for (int i = 0; !f.eof();i++) 
			{	
				f >> a >> b >> c;
				x[i] = a;
				if (n == 1)y[i] = b;
				else y[i] = c;

			}
		}

		f.close();

	}
	void dekart() {
		glClear(GL_COLOR_BUFFER_BIT);  

		glColor3f(0.0, 0.0, 0.0);

		glBegin(GL_LINES);  
		glVertex2f(-70, 0);   
		glVertex2f(70, 0); 
		glEnd();

		glBegin(GL_LINES); 
		glVertex2f(0, 70);
		glVertex2f(0, -70);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex3d(-1.5, 68, 0);
		glVertex3d(0, 70, 0);
		glVertex3d(1.5, 68, 0);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex3d(68, -1.5, 0);
		glVertex3d(70, 0, 0);
		glVertex3d(68, 1.5, 0);

		glEnd();
		glColor3d(0.0, 0.0, 0.0);

		glRasterPos2d(65, -5);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,"X"[0]);

		glRasterPos2d(-5, 65);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, "Y"[0]);

		glRasterPos2d(-3, -3);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,"0"[0] );

	

	}
	void display(float gl_1,float gl_2,float gl_3) {

		glBegin(GL_LINE_STRIP); 
		glColor3f(gl_1, gl_2, gl_3); 

		
		for (float i = x[0]; i <= x[number_of_lines-1]; i+=0.1) {
			glVertex2f(i, polinom_lagranz(x,y,i));
		}
		glEnd();

		glFlush();
	}
	double polinom_lagranz(double* x, double* y, double X) {
		double z, p1, p2;
		z = 0;
		for (int j = 0; j < number_of_lines; j++) {
			p1 = 1; p2 = 1;
			for (int i = 0; i < number_of_lines; i++) {
				if (i != j) {
				
				
					p1 = p1 * (X - x[i]);
					p2 = p2 * (x[j] - x[i]);
				}
			}
			z = z + y[j] * p1 / p2;
		}
		return z;
	}

	void write() {
		const  char* new_File = "new.txt";

		ifstream f_(File);
		if (!(f_.is_open()))  
			cout << "ERROR: " << File;

		ofstream f(new_File);

		for (int i = 0; !f_.eof(); i++) {

			f_>> a >> b >> c;

			f <<  setw(10) << showpos << fixed << setprecision(3) << a ;

			f << "\t";

			f <<  setw(10) << showpos << fixed << setprecision(3) << b ;
		
			f << "\t";

		    f <<  setw(10) << showpos << fixed << setprecision(3) << c << endl;
			
			}
		f.close();
		f_.close();
	}
	void RenderString(GLdouble x, GLdouble y, string s)
	{
		glColor3d(0.0, 0.0, 0.0);
		glRasterPos2d(x, y);
		for (int n = 0; n < s.size(); ++n) {
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, s[n]);
		}
	}
	friend void Min_Max(Function &obj);
};

void Min_Max(Function &obj) {
		int* min = new int[2];
		int* max = new int[2];

		min[0] = obj.y[0];
		min[1] = obj.x[0];

		max[0] = obj.y[0];
		max[1] = obj.x[0];

		for (int i = 1; i < obj.number_of_lines; i++)
		{
			if (obj.y[i] < min[0]) {
				min[0] = obj.y[i];
				min[1] = obj.x[i];
			}
			if (obj.y[i] > max[0]) {
				max[0] = obj.y[i];
				max[1] = obj.x[i];
			}


		}
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(0, max[0]);
		glVertex2f(max[1], max[0]);
		glVertex2f(max[1], 0);
		glEnd();

		obj.RenderString(max[1], max[0], "(" + to_string(max[1]) + "," + to_string(max[0]) + ")");

		cout << "Max of function in file(green): "<<"("<<max[1]<<","<<max[0]<<")"<<endl;

		glBegin(GL_LINE_STRIP);
		glColor3f(1.0, 0.0, 1.0);
		glVertex2f(0, min[0]);
		glVertex2f(min[1], min[0]);
		glVertex2f(min[1], 0);
		glEnd();
		
		obj.RenderString(min[1], min[0]-2.5, "(" + to_string(min[1]) + "," + to_string(min[0]) + ")");

		cout << "Min of function in file(purple): " << "(" << min[1] << "," << min[0] << ")" << endl;

		glFlush();
	}
	
int main(int argc, char** argv) {

	char name[] = "cordinates.txt";
	Function f(name);
	f.read(name, 1);
	f.write();

	Function f_2(name);
	f_2.read(name, 2);

	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700); 
	glutInitWindowPosition(600, 80);  
	glutCreateWindow("Ghraphic"); 
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-75.0, 75.0, -70.0, 70.0, -1.0, 1.0);
	//////////
	f.dekart();

	f.display(1.0, 0.0, 0.0);

	f_2.display(0.0, 0.0, 1.0);

	Min_Max(f);
	Min_Max(f_2);
	//////////
	glutMainLoop();  

	return 0;
}


