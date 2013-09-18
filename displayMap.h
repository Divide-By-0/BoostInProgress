//#include<GL/gl.h>
//#include<GL/glu.h>
//#include<GL/glut.h>

struct displayMap {

  void init(int numCells);
  void initDisplay(int w, int h);
  //void reshape(int w, int h);
  void keyboard(unsigned char key, int x, int y);
  void display(unsigned char* img);

  int m_numCells;
  unsigned char* m_img;
  bool m_copy;
  int m_fileid;
};

void displayMap::init(int numCells) {
  m_numCells = numCells;
  m_img = new unsigned char[m_numCells*m_numCells];
  m_copy = false;
  m_fileid = 0;
}

/*void displayMap::initDisplay(int w, int h)		//Whole Function is OpenGL
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLsizei)w, 0.0, (GLsizei)h, -1.0, 1.0);				
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}
*/

/* void displayMap::reshape(int w, int h) */			//OpenGL Function Previously Commented 		
/* { */
/*   glViewport(0,0,(GLsizei)w,(GLsizei)h); */
/*   glMatrixMode(GL_PROJECTION); */
/*   glLoadIdentity(); */
/*   glOrtho(0.0, (GLsizei)w, 0.0, (GLsizei)h, -1.0, 1.0); */
/*   glMatrixMode(GL_MODELVIEW); */
/*   //  glLoadIdentity(); */
/*   glutPostRedisplay(); */
/* } */

void displayMap::keyboard(unsigned char key, int x, int y) 
{
  switch(key) {
  case 'q':
    exit(0);
  case 'c':
    m_copy=true;
    break;
  case 'w':
    char str[15];
    sprintf(str,"laserMap%03d.pgm",m_fileid);
    FILE *fp;
    fp = fopen(str,"w");
    fprintf(fp,"P5\n%d %d\n255\n",m_numCells,m_numCells);
    fwrite(m_img,1,m_numCells*m_numCells,fp);
    fclose(fp);
    m_fileid++;
    break;
  }
}


void displayMap::display(unsigned char* img)
{
  if (m_copy) {
    for(int i=0; i<m_numCells; i++)
      for(int j=0; j<m_numCells; j++)
	m_img[i+j*m_numCells] = img[i+j*m_numCells];
    m_copy = false;
  }

  // 								//converts img to glImg -- unneccesary OpenGL
  /*
  GLubyte glImg[m_numCells][m_numCells][3];
  for(int i=0; i<m_numCells; i++) {
    for(int j=0; j<m_numCells; j++) {
      glImg[i][j][0] = (GLubyte) img[i+j*m_numCells];
      glImg[i][j][1] = (GLubyte) img[i+j*m_numCells];
      glImg[i][j][2] = (GLubyte) img[i+j*m_numCells];
    }
  }

  glClear(GL_COLOR_BUFFER_BIT);  
  glRasterPos2i(0,0);
  glDrawPixels(m_numCells,m_numCells,GL_RGB,GL_UNSIGNED_BYTE,glImg);
  glutPostRedisplay();

  glFlush();
  glFinish();
  */
}

