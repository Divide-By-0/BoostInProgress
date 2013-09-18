#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <string>
#include <vector>

struct LaserScan
{
  void setSize(int sz) {
    idx.resize(sz);
    ranges.resize(sz);
    intensities.resize(sz);
    flags.resize(sz);
  }
  
  float angle_min;
  float angle_max;
  float angle_increment;
  float time_increment;
  float range_min;
  float range_max;

  std::vector<int> idx;
  std::vector<float> ranges;
  std::vector<float> intensities;
  std::vector<uint8_t> flags;
};


class XV11Laser
{
 public:
  /**
   * @brief Constructs a new XV11Laser
   * @param io Boost ASIO IO Service to use when creating the serial port object
   */
  XV11Laser(boost::asio::io_service& io);
  
  /**
   * @brief Default destructor
   */
  ~XV11Laser() {};

  /**
   * @brief Constructs a new XV11Laser attached to the given serial port
   * @param port The string for the serial port device to attempt to connect to, e.g. "/dev/ttyUSB0"
   * @param baud_rate The baud rate to open the serial port at.
   */
  void init(char* port,uint32_t baud);
  
  /**
   * @brief Poll the laser to get a new scan. Blocks until a complete new scan is received or close is called.
   * @param scan LaserScan message pointer to fill in with the scan. The caller is responsible for filling in the ROS timestamp and frame_id
   */
  int poll(LaserScan& scan);
	    
  /**
   * @brief Close the driver down and prevent the polling loop from advancing
   */
  void close();
	    
 private:
  std::string port_; ///< @brief The serial port the driver is attached to
  uint32_t baud_rate_; ///< @brief The baud rate for the serial connection	    
  boost::asio::serial_port serial_; ///< @brief Actual serial port object for reading/writing to the XV11 Laser Scanner
  uint16_t motor_speed_; ///< @brief current motor speed as reported by the XV11.
};

