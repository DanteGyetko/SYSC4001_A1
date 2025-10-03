/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

// Added this comment to check git functionality, will be reomved before submission

#include "interrupts.hpp"

#define CONTEXT_SAVE_TIME 10
#define ISR_ACTIVITY_TIME 40

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
    std::pair<std::string, int> boilerplate;


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU"){
            execution += std::to_string(current_time) + ", " + std::to_string(duration_intr) + ", " + "CPU burst\n";
            current_time += duration_intr;
        }
        else if (activity == "END_IO"){
            execution += std::to_string(current_time) + ", 1, Check if the interrupt is maskable\n";
            current_time++;
            
            execution += std::to_string(current_time) + ", 1, Check the priority of the interrupt\n";
            current_time++;

            boilerplate = intr_boilerplate(current_time, duration_intr, CONTEXT_SAVE_TIME, vectors);
            execution += boilerplate.first;
            current_time = boilerplate.second;

            execution += std::to_string(current_time) + ", " + std::to_string(ISR_ACTIVITY_TIME) + ", END_IO\n";
            current_time += ISR_ACTIVITY_TIME;

            execution += std::to_string(current_time) + ", " + std::to_string(delays[duration_intr - 1] - ISR_ACTIVITY_TIME) + ", check device status\n";
            current_time++;     
        }
        else { //syscall
            boilerplate = intr_boilerplate(current_time, duration_intr, CONTEXT_SAVE_TIME, vectors);

            execution += boilerplate.first;
            current_time = boilerplate.second;

            // SYSCALL ISR Execution:
            int duration_device = delays[duration_intr - 1]; // The time taken by device
            std::random_device rd; // Random Engine

            // 1. Run ISR Activity
            execution += std::to_string(current_time) + ", " + std::to_string(ISR_ACTIVITY_TIME) + ", SYSCALL: run the ISR"  + "\n";
            current_time += ISR_ACTIVITY_TIME;

            // 2. Transfer Data Activity
            int remaining_duration = duration_device - ISR_ACTIVITY_TIME;
            std::uniform_int_distribution<int> dist2(remaining_duration / 3, remaining_duration / 2);
            int duration_transfer_data = dist2(rd);
            execution += std::to_string(current_time) + ", " + std::to_string(duration_transfer_data) + ", transfer data"  + "\n";
            current_time += duration_transfer_data;

            // 3. Check for Errors Activity
            int duration_check_for_errors = duration_device - ISR_ACTIVITY_TIME - duration_transfer_data;
            execution += std::to_string(current_time) + ", " + std::to_string(duration_check_for_errors) + ", check for errors"  + "\n";
            current_time += duration_check_for_errors;

            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time++;
        
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
