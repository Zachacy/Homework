#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <GL/glu.h>
//#include <FL/glut.H>
#include <stdio.h>
#include <math.h>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_Box.H>
#include <vector>
int widows_weight = 600;
int widows_heigh = 600;
static GLint imagewidth;
static GLint imageheight;
int flag_shape = 0;//Draw type : 1 = point , 2 = line , 3 = triangle , 4 = square , 5 = circle
bool flag_clear = true;//Flag of clear event
bool flag_bmp = false;// Flag of idle event
bool flag_zoom = false;
std::vector<double> point;
std::vector<double> point_line;
std::vector<double> point_triangle;
std::vector<double> point_square;
std::vector<double> point_circle;
std::vector<double>::iterator iter;
std::vector<double> zoom_point(4);
int point_index;
class openGL_window : public Fl_Gl_Window { // Create a OpenGL class in FLTK
  static void Timer_CB(void *userdata) {
        openGL_window *pb = (openGL_window*)userdata;
        pb->redraw();
        Fl::repeat_timeout(1.0/24.0, Timer_CB, userdata);
    }
public:
     // Class constructor
    int handle(int e) {//Detect Mouse click event
           if (e == FL_PUSH ) {
                fprintf(stderr, "event_x,event_y: %d,%d\n", Fl::event_x(),Fl::event_y());
                switch(flag_shape){
                    case 1:{//point(1)

                        point.push_back(Fl::event_x()-widows_weight/2);
                        point.push_back(-Fl::event_y()+widows_heigh/2);
                        break;
                    }
                    case 2:{//Line(2)
                        point_line.push_back(Fl::event_x()-widows_weight/2);
                        point_line.push_back(-Fl::event_y()+widows_heigh/2);
                        printf("%d\n",point_line.size());
                        break;
                    }
                    case 3:{//triangle(3)
                        point_triangle.push_back(Fl::event_x()-widows_weight/2);
                        point_triangle.push_back(-Fl::event_y()+widows_heigh/2);
                        break;
                    }
                    case 4:{//square(4)
                        point_square.push_back(Fl::event_x()-widows_weight/2);
                        point_square.push_back(-Fl::event_y()+widows_heigh/2);
                        break;
                    }
                case 5:{//circle
                        point_circle.push_back(Fl::event_x()-widows_weight/2);
                        point_circle.push_back(-Fl::event_y()+widows_heigh/2);
                        break;
                    }
                case 6:{//zoom_point
                        if(zoom_point.size() == 0){
                            zoom_point[0] = Fl::event_x()-widows_weight/2;
                            zoom_point[1] = -Fl::event_y()+widows_heigh/2;
                        }
                        else{
                            zoom_point[2] = Fl::event_x()-widows_weight/2;
                            zoom_point[3] = -Fl::event_y()+widows_heigh/2;
                        }
                    }
                   default :
                        break;
                }
               flag_clear = false;
            }
           if(e == FL_RELEASE && flag_shape == 5){
               point_circle.push_back(Fl::event_x()-widows_weight/2);
               point_circle.push_back(-Fl::event_y()+widows_heigh/2);
                }
           return(1);
    }
    openGL_window(int x,int y,int w,int h,const char *l=0) :
    Fl_Gl_Window(x,y,w,h,l)//Update Draw canvas
    {
        mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );
        Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);

    }
    void draw() {// Draw function.
    // the valid() property may be used to avoid reinitializing your
    // GL transformation for each redraw:
        if(flag_clear == true){
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            for (iter = point.begin(); iter != point.end(); ++iter)
                *iter = 0;
            point.clear();
            for (iter = point_line.begin(); iter != point_line.end(); ++iter)
                *iter = 0;
            point_line.clear();
            for (iter = point_triangle.begin(); iter != point_triangle.end(); ++iter)
                *iter = 0;
            point_triangle.clear();
            for (iter = point_square.begin(); iter != point_square.end(); ++iter)
                *iter = 0;
            point_square.clear();
            for (iter = point_circle.begin(); iter != point_circle.end(); ++iter)
                *iter = 0;
            point_circle.clear();
        }//reset_bound_box
        widows_heigh = h();
        widows_weight = w();
        //
        if (!valid()) {
          valid(1);
          glLoadIdentity();
          glViewport(0,0,w(),h());
        }
        // draw an amazing but slow graphic:--------------
   // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) ;
        glLineWidth(3.0);
        if(flag_clear == false && point.empty()==false){//Point
           glColor3f(1.0, 1.0, 1.0);
           for(point_index = 0; point[point_index]!=NULL; point_index+=2){//point
               glBegin(GL_POINTS);
               for(int i = -1 ; i < 2 ; i++)
                   for(int j = -1 ; j < 2 ; j++)
                        glVertex2f((point[point_index]+i)/widows_weight*2, (point[point_index+1]+j)/widows_heigh*2);
               glEnd();
           }
         }
          if(flag_clear == false && point_line.size() > 3){//Line
               glColor3f(1.0, 0.0, 0.0);
               for(point_index = 0; point_line[point_index]!=NULL; point_index+=4){
                 if(point_line.size()%4 != 0)
                   break;
                   glBegin(GL_LINES);
                   glVertex2f(point_line[point_index]/widows_weight*2, point_line[point_index+1]/widows_heigh*2);
                   glVertex2f(point_line[point_index+2]/widows_weight*2, point_line[point_index+3]/widows_heigh*2);
                   glEnd();
               }
           }
          if(flag_clear == false && point_triangle.size() > 5){//triangle
              glColor3f(0.0, 1.0, 0.0);
              for(point_index = 0; point_triangle[point_index]!=NULL; point_index+=6){
                  if(point_triangle.size()%6 != 0)
                   break;
                  glBegin(GL_TRIANGLES);
                      glVertex2f(point_triangle[point_index]/widows_weight*2, point_triangle[point_index+1]/widows_heigh*2);
                      glVertex2f(point_triangle[point_index+2]/widows_weight*2, point_triangle[point_index+3]/widows_heigh*2);
                      glVertex2f(point_triangle[point_index+4]/widows_weight*2, point_triangle[point_index+5]/widows_heigh*2);
                  glEnd();
              }
            }
            if(flag_clear == false && point_square.size() > 7){//square
                glColor3f(0.0, 0.0, 1.0);
                for(point_index = 0; point_square[point_index]!=NULL; point_index+=8){
                    if(point_square.size()%8 != 0)
                      break;
                    glBegin(GL_QUADS);
                    glVertex2f(point_square[point_index]/widows_weight*2, point_square[point_index+1]/widows_heigh*2);
                    glVertex2f(point_square[point_index+2]/widows_weight*2, point_square[point_index+3]/widows_heigh*2);
                    glVertex2f(point_square[point_index+4]/widows_weight*2, point_square[point_index+5]/widows_heigh*2);
                    glVertex2f(point_square[point_index+6]/widows_weight*2, point_square[point_index+7]/widows_heigh*2);
             glEnd();
                }
              }
            if(flag_clear == false && point_circle.capacity() > 3){//circle
                glColor3f(1.0,1.0,0.0);
                for(point_index = 0; point_circle[point_index]!=NULL; point_index+=4){
                    if(point_circle.size()%4 != 0)
                        break;
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(point_circle[point_index]/widows_weight*2,point_circle[point_index+1]/widows_heigh*2);
                const float triangles = 50;
                const float twoPi = 2.0f * 3.1415926f;
                float delta = twoPi / triangles;
                double radius = sqrt(pow(point_circle[point_index]-point_circle[point_index+2],2) + pow(point_circle[point_index+1]-point_circle[point_index+3],2))/widows_heigh*2;
                for(unsigned int i = 0; i <= triangles; i++)
                  glVertex2f(point_circle[point_index]/widows_weight*2+(radius * cos(i *delta)),
                          point_circle[point_index+1]/widows_heigh*2+(radius * sin(i * delta)));
                    glEnd();
                }
            }

        //--------------------------------------------------
    }
    void draw_overlay() { // Draw overlay function.
    // the valid() property may be used to avoid reinitializing your
    // GL transformation for each redraw:
        if (!valid()) {valid(1);
            glLoadIdentity();
            glViewport(0,0,w(),h());
          }

        // draw an amazing graphic:-------------

        //---------------------------------------
    }
};//
class openGL_BMP : public Fl_Gl_Window{
    static void Timer_CB(void *userdata) {
          openGL_BMP *pb = (openGL_BMP*)userdata;
          pb->redraw();
          Fl::repeat_timeout(1.0/24.0, Timer_CB, userdata);
      }
public:
    openGL_BMP(int x,int y,int w,int h,const char *l=0) :
    Fl_Gl_Window(x,y,w,h,l)
    {
        mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );
        Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);

    }
    void draw(){
        //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT) ;
      if(flag_bmp == true){
          Fl_BMP_Image *img = new Fl_BMP_Image("disney.bmp");
              if ( img->d() == 0 ) {
                  perror("disney.bmp");
                  exit(1);
              }
              uchar r,g,b;
              glLoadIdentity();
              glViewport(0,0,img->w(),img->h());
              const char *buf = img->data()[0];
              for ( int y=0; y<img->h(); y++ ) {                               // X loop
                  for ( int x=0; x<img->w(); x++ ) {                           // Y loop
                      long index = (y * img->w() * img->d()) + (x * img->d()); // X/Y -> buf index
                      r = *(buf+index+0);
                      g = *(buf+index+1);
                      b = *(buf+index+2);
                      glColor3f(float(r/255.0f),float(g/255.0f),float(b/255.0f));
                      glBegin(GL_POINTS);
                         glVertex2f((x-img->w()/2.0f)/img->w()*2.0f,(-y+img->h()/2.0f)/img->h()*2.0f);
                      glEnd();
                  }
              }
      }
      else
          glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    void draw_overlay() { // Draw overlay function.
    // the valid() property may be used to avoid reinitializing your
    // GL transformation for each redraw:
        if (!valid()) {valid(1);
            glLoadIdentity();
            glViewport(0,0,imagewidth,imageheight);
          }
        // draw an amazing graphic:-------------

        //---------------------------------------
    }

};
class openGL_zoom : public Fl_Gl_Window{
    static void Timer_CB(void *userdata) {
          openGL_zoom *pb = (openGL_zoom*)userdata;
          pb->redraw();
          Fl::repeat_timeout(1.0/24.0, Timer_CB, userdata);
      }
public :

    openGL_zoom(int x,int y,int w,int h,const char *l=0) :
    Fl_Gl_Window(x,y,w,h,l)
    {
        mode( FL_RGB|FL_ALPHA|FL_DOUBLE | FL_STENCIL );
        Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);

    }
    void draw(){
        if(flag_zoom == true){
            glLoadIdentity();
            gluOrtho2D(zoom_point[0]/w()*2,zoom_point[2]/w()*2,zoom_point[1]/h()*2,zoom_point[3]/h()*2);
            glLineWidth(3.0);
            if(flag_clear == false && point.empty()==false){//Point
               glColor3f(1.0, 1.0, 1.0);
               for(point_index = 0; point[point_index]!=NULL; point_index+=2){//point
                   glBegin(GL_POINTS);
                   for(int i = -1 ; i < 2 ; i++)
                       for(int j = -1 ; j < 2 ; j++)
                            glVertex2f((point[point_index]+i)/w()*2, (point[point_index+1]+j)/h()*2);
                   glEnd();
               }
             }
              if(flag_clear == false && point_line.size() > 3){//Line
                   glColor3f(1.0, 0.0, 0.0);
                   for(point_index = 0; point_line[point_index]!=NULL; point_index+=4){
                     if(point_line.size()%4 != 0)
                       break;
                       glBegin(GL_LINES);
                       glVertex2f(point_line[point_index]/w()*2, point_line[point_index+1]/h()*2);
                       glVertex2f(point_line[point_index+2]/w()*2, point_line[point_index+3]/h()*2);
                       glEnd();
                   }
               }
              if(flag_clear == false && point_triangle.size() > 5){//triangle
                  glColor3f(0.0, 1.0, 0.0);
                  for(point_index = 0; point_triangle[point_index]!=NULL; point_index+=6){
                      if(point_triangle.size()%6 != 0)
                       break;
                      glBegin(GL_TRIANGLES);
                          glVertex2f(point_triangle[point_index]/w()*2, point_triangle[point_index+1]/h()*2);
                          glVertex2f(point_triangle[point_index+2]/w()*2, point_triangle[point_index+3]/h()*2);
                          glVertex2f(point_triangle[point_index+4]/w()*2, point_triangle[point_index+5]/h()*2);
                      glEnd();
                  }
                }
                if(flag_clear == false && point_square.size() > 7){//square
                    glColor3f(0.0, 0.0, 1.0);
                    for(point_index = 0; point_square[point_index]!=NULL; point_index+=8){
                        if(point_square.size()%8 != 0)
                          break;
                        glBegin(GL_QUADS);
                        glVertex2f(point_square[point_index]/w()*2, point_square[point_index+1]/h()*2);
                        glVertex2f(point_square[point_index+2]/w()*2, point_square[point_index+3]/h()*2);
                        glVertex2f(point_square[point_index+4]/w()*2, point_square[point_index+5]/h()*2);
                        glVertex2f(point_square[point_index+6]/w()*2, point_square[point_index+7]/h()*2);
                 glEnd();
                    }
                  }
                if(flag_clear == false && point_circle.capacity() > 3){//circle
                    glColor3f(1.0,1.0,0.0);
                    for(point_index = 0; point_circle[point_index]!=NULL; point_index+=4){
                        if(point_circle.size()%4 != 0)
                            break;
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(point_circle[point_index]/widows_weight*2,point_circle[point_index+1]/widows_heigh*2);
                    const float triangles = 50;
                    const float twoPi = 2.0f * 3.1415926f;
                    float delta = twoPi / triangles;
                    double radius = sqrt(pow(point_circle[point_index]-point_circle[point_index+2],2) + pow(point_circle[point_index+1]-point_circle[point_index+3],2))/widows_heigh*2;
                    for(unsigned int i = 0; i <= triangles; i++)
                      glVertex2f(point_circle[point_index]/w()*2+(radius * cos(i *delta)),
                              point_circle[point_index+1]/h()*2+(radius * sin(i * delta)));
                        glEnd();
                    }
                }

            //--------------------------------------------------
        }
        else
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
};
//-------Button callback funtion
void button_point(Fl_Widget *, void *){
    printf("Point\n");
    flag_shape = 1;
}
void button_square(Fl_Widget *, void *){
    printf("square\n");
    flag_shape = 4;
}
void button_line(Fl_Widget *, void *){
    printf("line\n");
    flag_shape = 2;
}

void button_triangle(Fl_Widget *, void *){
    printf("triangle\n");
    flag_shape = 3;
}

void button_circle(Fl_Widget *, void *){
    printf("circle\n");
    flag_shape = 5;
}

void button_clear(Fl_Widget *, void *){
    printf("clear\n");
    flag_clear = true;
}

void button_load(Fl_Widget *, void *){
    printf("load\n");
    flag_shape = 0;
    flag_bmp = true;
}

void button_idle(Fl_Widget *, void *){
    printf("idle\n");
    flag_clear = true;
    flag_bmp = false;

}

void button_zoom(Fl_Widget *, void *){
    printf("zoom\n");
    flag_shape = 6;
    flag_zoom = true;
}

void button_clear_zoom(Fl_Widget *, void *){
    printf("clear_zoom\n");
    flag_zoom = false;
    flag_shape = 6;
    for (iter = zoom_point.begin(); iter != zoom_point.end(); ++iter)
        *iter = 0;
    zoom_point.clear();

}

void button_exit(Fl_Widget *, void *){
    printf("Exit\n");
   exit(0);
}

//  main function
int main(int argc, char **argv) {
point.clear();
point_line.clear();
point_circle.clear();
point_square.clear();
point_triangle.clear();
  Fl_Window window(1600,720);  // Create a FLTK window. Resolution 600*400.

  openGL_window gl_win(10, 10, widows_weight ,widows_heigh);   // Add openGL window in position (10,10).Resolution 600*400
  window.resizable(&gl_win);

  Fl_BMP_Image *img = new Fl_BMP_Image("disney.bmp");
  imagewidth = img->w();imageheight = img->h();
  openGL_BMP bmp(1000,10,imagewidth ,imageheight);
  window.resizable(&bmp);

  openGL_zoom zoom_windows(700,150,200,200);
  window.resizable(&zoom_windows);

  Fl_Widget *point;
  point = new Fl_Button(10,620,100,50,"point");
  point->callback(button_point);

  Fl_Widget *line;
  line = new Fl_Button(160,620,100,50,"line");
  line->callback(button_line);

  Fl_Widget *triangle;
  triangle = new Fl_Button(310,620,100,50,"triangle");
  triangle->callback(button_triangle);

  Fl_Widget *square;
  square = new Fl_Button(460,620,100,50,"Quad");
  square->callback(button_square);

  Fl_Widget *circle;
  circle = new Fl_Button(610,620,100,50,"circle");
  circle->callback(button_circle);

  Fl_Widget *clear;
  clear = new Fl_Button(760,620,100,50,"clear");
  clear->callback(button_clear);

  Fl_Widget *load;
  load = new Fl_Button(910,620,100,50,"load");
  load->callback(button_load);


  Fl_Widget *idle;
  idle = new Fl_Button(1060,620,100,50,"idle");
  idle->callback(button_idle);

  Fl_Widget *exit;
  exit = new Fl_Button(1210,620,100,50,"Exit");
  exit->callback(button_exit);

  Fl_Widget *zoom;
  zoom = new Fl_Button(730,520,100,50,"zoom");
  zoom->callback(button_zoom);

  Fl_Widget *clear_zoom;
  clear_zoom = new Fl_Button(730,420,100,50,"clear_zoom");
  clear_zoom->callback(button_clear_zoom);

  window.end();                  // End of FLTK windows setting.
  window.show(argc,argv);        // Show the FLTK window
  gl_win.show();                 // Show the openGL window
  gl_win.redraw_overlay();       // redraw
  bmp.show();
  bmp.redraw_overlay();
  return Fl::run();
}

