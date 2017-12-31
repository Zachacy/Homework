#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Box.H>
#include <vector>
#include <iostream>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <string>
#define pi 3.14159
#define point_dis 0.01
int window_width = 600;
int window_heigh = 600;
const float width = 0.3f;
const float heigh = 0.05f;
const float range = pi;
std::vector<double> theta(4);
std::vector<double> point(2);
std::vector<double> End_point(10);
Fl_Input *angel_1;
Fl_Input *angel_2;
Fl_Input *angel_3;
Fl_Input *angel_4;
Fl_Input *text;
Fl_Text_Buffer *buffer;
Fl_Text_Display *text_coordinate;
bool flag = true;
bool flag_convert = true;
using namespace std;
class openGL_window : public Fl_Gl_Window { // Create a OpenGL class in FLTK
	static void Timer_CB(void *userdata) {
		openGL_window *pb = (openGL_window*)userdata;
		pb->redraw();
		Fl::repeat_timeout(1.0 / 24.0, Timer_CB, userdata);
	}
public:
	// Class constructor
	int handle(int e) {//Detect Mouse click event
		if (e == FL_PUSH) {
			fprintf(stderr, "event_x,event_y: %d,%d\n", Fl::event_x(), Fl::event_y());
			point[0] = (Fl::event_x() - window_width / 2);
			point[1] = (-Fl::event_y() + window_heigh / 2);
			flag = false;
		}
		else {
			//printf("%f, %f\n", point[0], point[1]);
			return(1);
		}
	}
	openGL_window(int x, int y, int w, int h, const char *l = 0) :
		Fl_Gl_Window(x, y, w, h, l)//Update Draw canvas
	{
		mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);
		Fl::add_timeout(1.0 / 24.0, Timer_CB, (void*)this);

	}
	void draw() {// Draw function.
				 // the valid() property may be used to avoid reinitializing your
				 // GL transformation for each redraw:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		window_heigh = h();
		window_width = w();
		//
		if (!valid()) {
			valid(1);
			glLoadIdentity();
			glViewport(0, 0, w(), h());
		}
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		glLineWidth(0.2);
		glVertex2f(-1, 0);
		glVertex2f(1, 0);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(0, -1);
		glVertex2f(0, 1);
		glEnd();
		int i;
		if (flag_convert == true) {
			calc();
			glLineWidth(3.0);
			glBegin(GL_POINTS);//point
			for (int i = -2; i < 3; i++)
				for (int j = -2; j < 3; j++)
					glVertex2f((point[0] + i) / w() * 2, (point[1] + j) / h() * 2);
			glEnd();
			glLineWidth(2.0);
			glColor3f(0.0, 0.0, 1.0);
			for (i = 0; i < 4 && flag == true; i++)
				draw_line(End_point[i * 2], End_point[i * 2 + 1], theta[i], i);
			glColor3f(1.0, 1.0, 1.0);
			glLineWidth(3.0);
			for (int i = -2; i < 3; i++) //origial_point
				for (int j = -2; j < 3; j++)
					glVertex2f((0 + i) / w() * 2, (0 + j) / h() * 2);
			if (flag == false)
				text->value("No solution");
			else
				text->value("");
		}
		else {
			glLineWidth(2.0);
			for (i = 0; i < 4; i++)
				draw_line(End_point[i * 2], End_point[i * 2 + 1], theta[i], i);
			glColor3f(1.0, 1.0, 1.0);
			glLineWidth(3.0);
			for (int i = -1; i < 2; i++) //origial_point
				for (int j = -1; j < 2; j++)
					glVertex2f((0 + i) / w() * 2, (0 + j) / h() * 2);
		}
		float coordinate[8];
		string str;
		str.clear();
		for (int i = 2; i < 10; i += 2)
			str += "position" + to_string(i/2) + ": " + to_string(End_point[i] * w()) + ", " + to_string(End_point[i + 1] * h()) + "\n";
		buffer->text(str.c_str());
	}
	void draw_line(double intial_x, double intial_y, double alpha, int i) {
		double dis = sqrt(pow(width, 2) + pow(heigh, 2));
		double angle = atan2(heigh, width);
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(intial_x, intial_y);
		glVertex2f(intial_x + width*cos(alpha), intial_y + width*sin(alpha));
		glVertex2f(intial_x + dis*cos(alpha + angle), intial_y + dis*sin(alpha + angle));
		glVertex2f(intial_x + heigh*cos(alpha + pi / 2), intial_y + heigh*sin(alpha + pi / 2));
		glVertex2f(intial_x, intial_y);
		glEnd();
		///
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINE_LOOP);
		int num_segments = 50;
		double radius = sqrt(pow(heigh*cos(alpha + pi / 2) / 2, 2) + pow(heigh*sin(alpha + pi / 2) / 2, 2));
		for (int ii = 0; ii < num_segments; ii++)
		{
			float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
			float x = radius * cosf(theta);//calculate the x component
			float y = radius * sinf(theta);//calculate the y component
			glVertex2f(x + intial_x + heigh*cos(alpha + pi / 2) / 2, y + intial_y + heigh*sin(alpha + pi / 2) / 2);//output vertex
		}
		glEnd();
		///
		End_point[(i + 1) * 2] = intial_x + width*cos(alpha);
		End_point[(i + 1) * 2 + 1] = intial_y + width*sin(alpha);
	}
	void calc() {
		double a, b, c, d, temp, x, y;
		double dis = sqrt(pow(width, 2) + pow(heigh, 2));
		double angle = atan2(heigh, width);
		for (a = -pi; a < pi && flag == false; a += 0.1)
			for (b = -range / 2; b < range && flag == false / 2; b += 0.1)
				for (c = -range / 2; c < range && flag == false / 2; c += 0.1)
					for (d = -range / 2; d < range && flag == false / 2; d += 0.1) {
						theta[0] = a; theta[1] = b; theta[2] = c; theta[3] = d;
						x = width*cos(theta[0]) + width*cos(theta[1]) + width*cos(theta[2]) + width*cos(theta[3]);
						y = width*sin(theta[0]) + width*sin(theta[1]) + width*sin(theta[2]) + width*sin(theta[3]);
						temp = sqrt(pow(x - point[0] / w() * 2, 2) + pow(y - point[1] / h() * 2, 2));
						if (temp < point_dis)
							flag = true;
						else
							flag = false;
					}
	}
};//
  //-------Button callback funtion
void button_convert(Fl_Widget *, void *) {
	flag_convert = true;
	point.clear();
	printf("Invert\n");
}
void button_line(Fl_Widget *, void *) {
	exit(0);
}
void Input_theta(Fl_Widget *, void *) {
	flag_convert = false;
	point.clear();
	End_point.clear();
	theta.clear();
	theta[0] = (atoi(angel_1->value()) % 360)*pi / 180;
	theta[1] = (atoi(angel_2->value()) % 360)*pi / 180 + theta[0];
	theta[2] = (atoi(angel_3->value()) % 360)*pi / 180 + theta[1];
	theta[3] = (atoi(angel_4->value()) % 360)*pi / 180 + theta[2];
}

//  main function
int main(int argc, char **argv) {
	Fl_Window window(900, 800);  // Create a FLTK window. Resolution 600*400.
	openGL_window gl_win(10, 10, window_width, window_heigh);   // Add openGL window in position (10,10).Resolution 600*400
	window.resizable(&gl_win);

	Fl_Widget *point;
	point = new Fl_Button(10, 650, 100, 50, "Invert");
	point->callback(button_convert);

	Fl_Widget *line;
	line = new Fl_Button(160, 650, 100, 50, "End");
	line->callback(button_line);

	Fl_Widget *angel;
	angel = new Fl_Button(310, 650, 100, 50, "Forward");
	angel->callback(Input_theta);

	text = new Fl_Input(500, 650, 150, 50, "I-K result");
	buffer = new Fl_Text_Buffer();
	text_coordinate = new Fl_Text_Display(650, 410, 250, 100);
	text_coordinate->buffer(buffer);

	angel_1 = new Fl_Input(750, 10, 50, 50, "theta[1]");
	angel_2 = new Fl_Input(750, 110, 50, 50, "theta[2]");
	angel_3 = new Fl_Input(750, 210, 50, 50, "theta[3]");
	angel_4 = new Fl_Input(750, 310, 50, 50, "theta[4]");

	window.end();                  // End of FLTK windows setting.
	window.show(argc, argv);        // Show the FLTK window
	gl_win.show();                 // Show the openGL window
	gl_win.redraw_overlay();       // redraw
	return Fl::run();
}
