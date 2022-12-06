#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <pthread.h>

using json = nlohmann::json;
const char *status_json_check_path = "/home/kiencate/Documents/benzenx_job/Rfid_rc522_i2c_linux/status.json";
int main()
{
    std::ifstream file_status_read;
    file_status_read.open(status_json_check_path);
    while (!file_status_read) 
    {
        std::cout<<"videostreamer: open status file failed";
    }
    nlohmann::json status = nlohmann::json::parse(file_status_read);
    file_status_read.close();
    std::ofstream file_status_write;
    file_status_write.open(status_json_check_path);
    while (!file_status_write) 
    {
        std::cout<<"videostreamer: open status file failed";
    }

    // change status wifi
    status["is_person"] = 1;
    status["wifi_configured"] = 1;
    status["is_closed_door"] =1;
    status["is_face_detected"] = 0;
    file_status_write << status << std::endl;
    file_status_write.close();
    return 0;
}