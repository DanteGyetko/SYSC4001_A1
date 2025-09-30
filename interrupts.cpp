/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

// Added this comment to check git functionality, will be reomved before submission

#include<interrupts.hpp>

#define CONTEXT_SAVE_TIME 10

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int current_time = 0;



    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU"){
            execution += std::to_string(current_time) + ", " + std::to_str(duration_intr) + ", " + "CPU burst\n";
            current_time += duration_intr;
        }
        else if (activity == "END_IO"){

        }
        else { //syscall
            std::pair<std::string, int> boilerplate;
            boilerplate = intr_boilerplate(current_time, duration_intr, CONTEXT_SAVE_TIME, vectors);
            execution += boilerplate.first;
            current_time = boilerplate.second;

            //device
            execution += std::to_string(current_time) + ", " + std::to_string(delays[duration_intr - 1]) + ", call device driver " + std::to_string(duration_intr) + "\n";
            current_time += delays[duration_intr - 1];

            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time++;
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
