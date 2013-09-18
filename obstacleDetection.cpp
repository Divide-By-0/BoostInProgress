#include "obstacleDetection.h"

using namespace boost::asio;

obstacleDetection::obstacleDetection(boost::asio::io_service& io) : m_laser(io) {
  m_scan.setSize(NUM_SCANS);
}

obstacleDetection::~obstacleDetection() {
  delete [] m_displayMap;
}

void obstacleDetection::init(char* port,uint32_t baud, float edgeSize, float cellSize) {
  m_laser.init(port,baud);
  m_edgeSize = edgeSize;
  m_cellSize = cellSize;
  m_numCells = int(edgeSize/cellSize);

  m_obstacleMap.resize(m_numCells);
  for(int i=0; i<m_numCells; i++) {
    m_obstacleMap[i].resize(m_numCells);
  }
  
  m_displayMap = new unsigned char[m_numCells*m_numCells];
  
  for(int i=0; i<m_numCells; i++) {
    for(int j=0; j<m_numCells; j++) {
      m_displayMap[i+m_numCells*j] = 255;
      m_obstacleMap[i][j]= 0;
    }
  }
}

void obstacleDetection::updateMap() {
  static std::vector<int> xcoord(NUM_SCANS,0);
  static std::vector<int> ycoord(NUM_SCANS,0);

  m_laser.poll(m_scan);

  // fprintf(stderr,"\n");
  // for(int i=0; i<m_scan.ranges.size(); i++) {
  //   fprintf(stderr,"%d %3.3f %3.3f\n",m_scan.flags[i],m_scan.ranges[i],m_scan.intensities[i]);
  // }
  // fprintf(stderr,"\n");

  for(int i=0; i<NUM_SCANS; i++) {
    m_obstacleMap[xcoord[i]][ycoord[i]] = 0;
    m_displayMap[xcoord[i] + m_numCells*ycoord[i]] = 255;
  }

  for(int i=0; i<NUM_SCANS; i++) {
    //float theta=float(i)*3.141596/180;
    float theta=m_scan.idx[i]*3.141596/180;
    float x,y;
    if (m_scan.ranges[i]>=0.2) {
      x = m_scan.ranges[i]*cos(theta);
      y = m_scan.ranges[i]*sin(theta);
    } else {
      x = 100; // out of bound values
      y = 100;
    }

    xcoord[i] = round((x+m_edgeSize/2)/m_cellSize);
    ycoord[i] = round((y+m_edgeSize/2)/m_cellSize);

    if (xcoord[i]>=0 && xcoord[i]<m_numCells &&
	ycoord[i]>=0 && ycoord[i]<m_numCells) {
      m_obstacleMap[xcoord[i]][ycoord[i]] = 1;
      m_displayMap[xcoord[i] + m_numCells*ycoord[i]] = 0;
    }
    else {
      xcoord[i] = 0;
      ycoord[i] = 0;
    }
  }
}


unsigned char* obstacleDetection::getMap() {
  return m_displayMap;
}

void obstacleDetection::finish() {
  m_laser.close();
} 
