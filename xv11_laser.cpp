#include <xv11_laser.h>
#include <iostream>
#include <stdio.h>

using namespace boost::asio;

XV11Laser::XV11Laser(boost::asio::io_service& io) : serial_(io)
{ 
}

void XV11Laser::init(char* port,uint32_t baud) 
{
  port_ = port;
  baud_rate_ = baud;

  if (serial_.is_open())
    serial_.close();

  serial_.open(port_);

  serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate_));
  serial_.set_option(boost::asio::serial_port_base::character_size(8));
  serial_.set_option(boost::asio::serial_port_base::flow_control(serial_port_base::flow_control::none));
  serial_.set_option(boost::asio::serial_port_base::parity(serial_port_base::parity::none));
  serial_.set_option(boost::asio::serial_port_base::stop_bits(serial_port_base::stop_bits::one));

  boost::asio::serial_port_base::character_size C;
  boost::asio::serial_port_base::flow_control FLOW;
  boost::asio::serial_port_base::parity PARITY;
  boost::asio::serial_port_base::stop_bits STOP;

  serial_.get_option(C);
  serial_.get_option(FLOW);
  serial_.get_option(PARITY);
  serial_.get_option(STOP);

  //std::cerr<<"serial options: "<<C.value()<<" "<<FLOW.value()<<" "<<PARITY.value()<<" "<<STOP.value()<<std::endl;
}

void XV11Laser::close() 
{
  if (serial_.is_open())
    serial_.close();
}

int XV11Laser::poll(LaserScan& scan) 
{
  uint8_t temp_char;
  unsigned char idx;
  int scan_idx;
  uint16_t checksum;
  boost::array<uint8_t, 4> raw_bytes;
  int range_idx=0;
  int max_tries = 360;

  //std::cerr<<"\nAcquiring data"<<std::endl;
  int tries = 0;
  do { // Replace tries with timeout
    boost::asio::read(serial_,boost::asio::buffer(&temp_char,1));
    tries++;
  } while(temp_char!=0xFA && tries<=max_tries);

  if (temp_char!=0xFA) {
    std::cerr << "Could not acquire data\n";
    return -1;
  }
  //std::cerr <<"Acquired data\n";    
    
  scan.angle_min = 0.0;
  scan.angle_max = 2.0*M_PI;
  scan.angle_increment = (2.0*M_PI/360.0);
  scan.range_min = 0.06;
  scan.range_max = 5.0;

  for (int i=0; i<90; i++) 
    {
      // Read index
      boost::asio::read(serial_,boost::asio::buffer(&idx, 1));
      scan_idx = 4*((int)idx-(int)0xA0);
      //fprintf(stderr,"\nidx=%c idx=%d idx=%2X scan_idx=%d",idx,idx,idx,scan_idx);

      // Now read speed
      boost::asio::read(serial_,boost::asio::buffer(&motor_speed_,2));
      scan.time_increment = motor_speed_/1e8;
	  
      // Read in 4 consecutive readings 	  
      for(int j = 0; j < 4; j++) 
	{
	  boost::asio::read(serial_,boost::asio::buffer(&raw_bytes,4));
	  // Four bytes per reading
	  uint8_t byte0 = raw_bytes[0];
	  uint8_t byte1 = raw_bytes[1];
	  uint8_t byte2 = raw_bytes[2];
	  uint8_t byte3 = raw_bytes[3];
	  // First two bits of byte1 are status flags
	  uint8_t flag1 = (byte1 & 0x80) >> 7;  // No return/max range/too low of reflectivity
	  uint8_t flag2 = (byte1 & 0x40) >> 6;  // Object too close, possible poor reading due to proximity kicks in at < 0.6m
	  // Remaining bits are the range in mm
	  uint16_t range = ((byte1 & 0x3F)<< 8) + byte0;
	  // Last two bytes represent the uncertanty or intensity, might also be pixel area of target...
	  uint16_t intensity = (byte3 << 8) + byte2;
	      
	  scan.idx[range_idx] = scan_idx+j;
	  scan.ranges[range_idx]= (range / 1000.0);
	  scan.intensities[range_idx]= intensity;
	  scan.flags[range_idx++] = (flag1<<1) | flag2;
	}
	  
      //read checksum. ignore (for now)
      boost::asio::read(serial_,boost::asio::buffer(&checksum,2));
	  
      //Read start byte for next packet
      boost::asio::read(serial_, boost::asio::buffer(&temp_char,1));	 	  
    }
}
