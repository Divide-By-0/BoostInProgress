#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#include "obstacleDetection.h"
#include "displayMap.h"

using namespace boost::asio;

boost::asio::io_service io;
obstacleDetection obsDetect(io);
displayMap laserMap;

// void mapReshapeFunc(int w, int h) {
//   laserMap.reshape(w,h);
// }

void mapKeyboardFunc(unsigned char key, int x, int y) {
  laserMap.keyboard(key,x,y);
}

void mapDisplayFunc() {
  obsDetect.updateMap();
  laserMap.display(obsDetect.getMap());
  usleep(100000);
}

int main(int argc, char **argv)
{
  int baud_rate = 115200;
  float edge_size = 10;
  float cell_size = 0.025;
  int num_cells = int(edge_size/cell_size);
  fprintf(stderr,"num_cells = %d\n",num_cells);

  obsDetect.init(argv[1],baud_rate,edge_size,cell_size);
  laserMap.init(num_cells);

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(num_cells,num_cells);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Map");

  laserMap.initDisplay(num_cells,num_cells);
  // glutReshapeFunc(mapReshapeFunc);
  glutKeyboardFunc(mapKeyboardFunc);
  glutDisplayFunc(mapDisplayFunc);

  glutMainLoop();

  obsDetect.finish();
  return 0;
}
