#include<vector>
#include<string>
#include<iostream>

#include "xv11_laser.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>

#define NUM_SCANS 360

typedef std::vector<std::vector<int> > map2d;

class obstacleDetection {

 public:
  obstacleDetection(boost::asio::io_service& io);

  ~obstacleDetection();

  void init(char* port,uint32_t baud,float edgeSize,float cellSize);

  void updateMap();

  unsigned char* getMap();

  void finish();

  unsigned char* m_displayMap;

protected:

  float m_edgeSize;
  float m_cellSize;
  int m_numCells;

  XV11Laser m_laser;
  LaserScan m_scan;

  map2d m_obstacleMap;
}; 
