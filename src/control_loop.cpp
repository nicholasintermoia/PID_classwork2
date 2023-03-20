#include "control_loop.h"

//We can use the class constructor to set parameters
CONTROLLER::CONTROLLER( const float & kp,const float & initial_value,const float & ki,const float & kd, float xdes) {

    _Kp=kp;
    _Ki=ki;
    _Kd=kd;
    _initial_value=initial_value;
    set_xdes(xdes);
    system_start();

    boost::thread loop_th(&CONTROLLER::loop, this);

}


//Sense: get input to change the state of our System
void CONTROLLER::set_xdes(float xdes) {
    //cout<<"Inserisci valore di rif: ";
    _xdes = xdes;
}


//Random initial value
void CONTROLLER::system_start() {
    _cmd=_initial_value;
}

void CONTROLLER::loop() {
    float e_old=0.0;
    float e_curr=0.0;
    float x_new=0.0;
    float x_curr=0.0;
    float I=0.0;
    float D=0.0;
    float T=0.01;
    double time=0.0;
    bool q=true;
    ofstream data;
    ofstream time_out;
    ofstream command;
    ofstream riferimento;
    data.open ("data.txt");
    time_out.open("time.txt");
    command.open("control.txt");
    riferimento.open("ref.txt");
    //float I_curr=0.0;

    
    while(q){
        command<< _cmd << endl;
        cout<<"controllo: "<<_cmd<<"\n";
        cout<<"stato: "<<x_curr<<"\n";
        data<< x_curr << endl;
        time_out<< time << endl;
        riferimento<< _xdes << endl;
        e_curr=_xdes-x_curr;
        if(fabs(e_curr)<=0.0001){
            q=false;
        }
        //Azione derivativa
        D=(e_curr-e_old)/T;
        e_old=e_curr;
        //azione integ
        I=I+e_curr*T;
        _cmd=_Kp*e_curr+_Ki*I+_Kd*D;
        x_new=x_curr*0.99+_cmd*0.00995;
        x_curr=x_new;
        cout<<"errore: "<<e_curr<<"\n";
        time+=T;
        cout<<"tempo: "<<time<<"\n";
        usleep(T*1e6);

    }
    data.close();
    time_out.close();
    command.close();
    riferimento.close();

}
/*

void CONTROLLER::run() {

  std::vector< boost::thread> th;
  
    th.push_back(boost::thread(&CONTROLLER::system_start, this));
    //th.push_back(boost::thread(&CONTROLLER::set_xdes, this));
    th.push_back(boost::thread(&CONTROLLER::loop, this));

    th[0].join();
    th[1].join();
    

}

*/