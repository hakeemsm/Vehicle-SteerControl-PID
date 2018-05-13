#include "PID.h"

#include <limits>
#include <chrono>
#include <iostream>
#include <numeric>
#include <list>
#include <ctime>


using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {

    this->p_error = 0.0;
}

PID::~PID() {}


void PID::Init(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->prev_cte = 0;    
    this->p_error = 0.0;  

}


void PID::UpdateError(double cte) {
   if(prev_cte == 0.0){
        prev_cte = cte;
    }
    double diff_cte = cte - prev_cte;
    
    p_error = Kp * cte;
    d_error = Kd * diff_cte;
    i_error = Ki * int_cte;
    int_cte += cte;
    prev_cte = cte;
    

}


double PID::TotalError() {
    return -(p_error + i_error + d_error);
    
}


