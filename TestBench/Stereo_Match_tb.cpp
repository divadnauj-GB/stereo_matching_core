#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "VStereo_Match.h"
#include "VStereo_Match___024root.h"

vluint64_t sim_time = 0;
vluint64_t index_data = 0;

int main(int argc, char** argv, char** env) {
    VStereo_Match *dut = new VStereo_Match;
    std::vector<int> image_left;
    std::vector<int> image_right;
    std::vector<int> image_valid;

    std::vector<int> output_data;
    std::vector<int> output_valid;

    std::ifstream file_image_left("../input_vector_left_image.txt"); 
    std::ifstream file_image_right("../input_vector_right_image.txt"); 
    std::ifstream file_image_valid("../input_vector_valid.txt"); 
    std::ofstream outputDato("../output_vector_data.txt"); // create a new output file or overwrite an existing one
    std::ofstream outputValid("../output_vector_valid.txt");


    std::string line;
    if(file_image_left)
    while(std::getline(file_image_left, line)){
        image_left.push_back(std::stoi(line));
    }
    while(std::getline(file_image_right, line)){
        image_right.push_back(std::stoi(line));
    }
    while(std::getline(file_image_valid, line)){
        image_valid.push_back(std::stoi(line));
    }

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 1);
    m_trace->open("waveform.vcd");
    dut->i_Tresh_LRCC = 8;     
    for(int i=0; i<50; i++){
        dut->i_clk ^= 1;
        dut->eval(); 
        m_trace->dump(sim_time);
        sim_time++;       
    }    
    dut->i_rst = 1;
    
    if (outputDato.is_open() && outputValid.is_open()) {

        while (index_data < image_valid.size()) {
            dut->i_clk ^= 1;
            dut->eval();
            m_trace->dump(sim_time);        
            if (dut->i_clk==0){       
                dut->i_dval = image_valid[index_data];
                dut->i_dato_L=image_left[index_data];
                dut->i_dato_R=image_right[index_data];     
                outputDato << int(dut->o_dato)  << "\n";
                outputValid << int(dut->o_dval) << "\n"; 
                //output_data.push_back(dut->o_dato);
                //output_valid.push_back(dut->o_dval);
                index_data++;
            } 
            sim_time++;

        }
        outputDato.close(); // close the file when done
        outputValid.close();
        m_trace->close();
        delete dut;
    }
    else {
        std::cerr << "Error opening file\n";
    }
   
    
    exit(EXIT_SUCCESS);
}
