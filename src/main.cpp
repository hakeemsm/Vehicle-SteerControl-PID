#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}
double p_val[] = {0.0,0.0,0.0};
double dp_val[] = {0.00005, 0.001, 0.001};
int twiddle_level = 0;
double best_error = std::numeric_limits<long int>::max();

double twiddle(double cte, PID pid){
    
    double tolerance = 1;
    
    pid.UpdateError(cte);
    double error = pid.TotalError();
    
    if((dp_val[0] + dp_val[1] + dp_val[2]) < tolerance){
      std::cout << "tolerance value reached " << std::endl;
    }
    
    while((dp_val[0] + dp_val[1] + dp_val[2]) > tolerance){
      
      
      for(int i = 0; i < 3; i++){
        std::cout << "running for level " << twiddle_level << " parameter " << i << std::endl;
        switch (twiddle_level){
						case 0:
							p_val[i] += dp_val[i];							
							twiddle_level = 1;		
              std::cout << "initializing with values " << p_val[0] << std::endl;
              pid.Init(p_val[0], p_val[1], p_val[2]);
              pid.UpdateError(cte);
              error = pid.TotalError();
              return error;
							break;
						case 1:
							if(error < best_error){
								best_error = error;
								dp_val[i] *= 1.1;
								twiddle_level = 0;
								//return best_error;
								
							} 
							else {
								p_val[i] -= 2*dp_val[i];
								twiddle_level = 2;								
                
							}
							break;
						case 2:
							if(error < best_error){
								best_error = error;
								dp_val[i] *= 1.05;
								
							} 
              else {
								p_val[i] += dp_val[i];
								dp_val[i] *= 0.90;
							}
							twiddle_level = 0;
							
							break;
					}
					
      }
      
      //std::cout << "sum of dps " << dp_val[0] + dp_val[1] + dp_val[2] << std::endl;
    }
    std::cout << "returning best error" << std::endl;
    return best_error;
}

int main()
{
  uWS::Hub h;

  PID pid;
  pid.Init(0.05, 0.005, 5);
  bool use_twiddle = false;
  // TODO: Initialize the pid variable.
  //pid.Init(p_val[0], p_val[1], p_val[2]);
  h.onMessage([&pid, &use_twiddle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          //double speed = std::stod(j[1]["speed"].get<std::string>());
          //double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          
          // DEBUG
          
          
            if(use_twiddle){                
                std::cout << "setting value from twiddle" << std::endl;
                steer_value = twiddle(cte,pid);                
            }
            else{
                pid.UpdateError(cte);
                steer_value = pid.TotalError();
            }
            
      
            if(steer_value > 1.0){
              steer_value = 1.0;
            }
            else if(steer_value < -1.0){
              steer_value = -1.0;
            }
            

          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.5;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          
          
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
