#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string_view> 
#include <thread>
#include <fstream>
#include <deque>
#include <sstream>
#include <mutex>
#include <unordered_map>
#include <set>

bool operator <(const timespec& lhs, const timespec& rhs)
{
    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}

struct read_line_args {
    char *fd_string;
    char *line_buffer;
    struct timespec record_time; 
};

struct pagefault_values {
    u_int64_t page_address; 
    u_int64_t pagecache_address;
    timespec record_time; 
};

struct mem_values {
    u_int64_t reg_no; 
    u_int64_t reg_value;
    timespec record_time; 
};

std::deque<pagefault_values *> pagefaults; 
std::deque<mem_values *> mem_trace_values; 
std::mutex pagefaults_lock; 
std::mutex mem_trace_lock;
std::unordered_map<int, u_int64_t> active_registers; 
std::set<u_int64_t> active_reg_values;

// Merge the two traces and calculate required stats.
void merge_traces_and_collect_stats(void *args) {
    struct mem_values *curr_mem_trace_val = NULL; 
    struct pagefault_values *curr_pagefault_val = NULL; 

    int pointer_pagefaults = 0; 
    int non_pointer_pagefaults = 0;
    while (1) {
        // sleep here.
        std::cout << "Pointer based pagefaults : " << pointer_pagefaults << " Non pointer based pagefaults : " << non_pointer_pagefaults << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        struct timespec curr;
        clock_gettime(CLOCK_MONOTONIC, &curr);

        if (mem_trace_values.size() > 0) {
            mem_trace_lock.lock();
            if (!mem_trace_values.empty()) 
                curr_mem_trace_val = mem_trace_values.front(); 
            mem_trace_lock.unlock();
        }

        if (!curr_mem_trace_val)
            continue; 

        if (pagefaults.size() > 0) {
            pagefaults_lock.lock();
            if (!pagefaults.empty()) 
                curr_pagefault_val = pagefaults.front();
            pagefaults_lock.unlock();
        }
        
        if (!curr_pagefault_val) 
            continue; 

        while (curr_mem_trace_val->record_time.tv_sec < curr.tv_sec - 10 && curr_pagefault_val && curr_mem_trace_val) {
            // Pagefault is in future update the state we know.
            if (curr_mem_trace_val->record_time < curr_pagefault_val->record_time) {
                // Reg existed, update active reg values and register state.
                // std::cout << "Processing register: " << curr_mem_trace_val->record_time.tv_sec << "." << curr_mem_trace_val->record_time.tv_nsec << " "<< curr_mem_trace_val->reg_no << " " << curr_mem_trace_val->reg_value << std::endl;
                if (active_registers.count(curr_mem_trace_val->reg_no)) {
                    u_int64_t value = active_registers[curr_mem_trace_val->reg_no]; 
                    //XXX: This assumes that only one register held a particular value at any time.
                    active_reg_values.erase(value);
                    active_registers[curr_mem_trace_val->reg_no] = curr_mem_trace_val->reg_value; 
                    active_reg_values.insert(curr_mem_trace_val->reg_value);
                } else {
                    active_registers[curr_mem_trace_val->reg_no] = curr_mem_trace_val->reg_value; 
                    active_reg_values.insert(curr_mem_trace_val->reg_value);
                }

                mem_trace_lock.lock();
                free(curr_mem_trace_val);
                curr_mem_trace_val = NULL;
                mem_trace_values.pop_front();
                if (!mem_trace_values.empty()) 
                    curr_mem_trace_val = mem_trace_values.front();
                mem_trace_lock.unlock();
            } else {
                // Pagefault encountered check current state.
                // std::cout << "Processing pagefault: " << curr_pagefault_val->record_time.tv_sec << "." << curr_pagefault_val->record_time.tv_nsec << " "<< curr_pagefault_val->page_address << std::endl;
                if (active_reg_values.count(curr_pagefault_val->page_address))
                    pointer_pagefaults++;
                else 
                    non_pointer_pagefaults++;

                pagefaults_lock.lock();
                free(curr_pagefault_val);
                curr_pagefault_val = NULL;
                pagefaults.pop_front();
                if (!pagefaults.empty())
                    curr_pagefault_val = pagefaults.front();
                pagefaults_lock.unlock();
            }
        }
    }

}

// TODO(shaurp): Check if we can change this to string_view
void parse_pagefault(std::string pagefault_str) {
    std::string data;
    
    // TODO(shaurp): Make this application agnostic
    if (pagefault_str.find("sequential") == std::string::npos)
        return; 
    
    struct pagefault_values *pagefault = (struct pagefault_values *) malloc(sizeof(pagefault_values));
    std::stringstream ss(pagefault_str);
    while(ss >> data) { 
        if (data.find("vpage") != std::string::npos) {
            std::string page_num = data.substr(8);
            pagefault->page_address = std::stoull(page_num, 0, 16);
        } else if (data.find("page=") != std::string::npos) {
            std::string pte_num = data.substr(7);
            if (pte_num.compare("0x0") == 0) 
                pagefault->pagecache_address = 0;
            else
                pagefault->pagecache_address = 1;
        } else if (data.find(".")  != std::string::npos) {
            
            std::string::size_type pos = data.find('.');
            if (data.substr(0,pos).compare("") == 0 || data.substr(pos + 1).compare("") == 0) {
                std::cout << "Pagefault freed" << std::endl;
                free(pagefault);
                return;
            }

            pagefault->record_time.tv_sec = stol(data.substr(0, pos));
            pagefault->record_time.tv_nsec = stol(data.substr(pos + 1));
        }
    }
    pagefaults_lock.lock();
    pagefaults.push_back(pagefault);
    pagefaults_lock.unlock();

    return;
}

void parse_mem_trace(std::string mem_trace) {
    std::string data;
    
    struct mem_values *mem_values = (struct mem_values *) malloc(sizeof(struct mem_values));
    std::stringstream ss(mem_trace);
    while(ss >> data) {
        if (data.find(".") != std::string::npos) {
            if (data.find_first_not_of("0123456789.") != std::string::npos) {
                free(mem_values);
                return;
            }
            std::string::size_type pos = data.find('.');
            if (data.substr(0,pos).compare("") == 0 || data.substr(pos + 1).compare("") == 0) {
                free(mem_values);
                return;
            }

            mem_values->record_time.tv_sec = stol(data.substr(0, pos));
            mem_values->record_time.tv_nsec = stol(data.substr(pos + 1));
        } else if (data.find("Reg:") != std::string::npos) {
            if(data.size() < 7) {
                free(mem_values);
                return;
            }
             
            if (data.substr(6).find_first_not_of("0123456789abcdefABCDEF") == std::string::npos) {
               
                if (data.find("0x") == std::string::npos) {
                    free(mem_values);
                    return; 
                }

                if (data.substr(6).size() > 12) {
                    free(mem_values);
                    return;
                }
                mem_values->reg_value = (std::stoull(data.substr(6), 0, 16) &  ~((1 << 12)-1));
            }
        } else if (data.find("RegNo:") != std::string::npos) {
            if(data.size() < 7) {
                free(mem_values);
                return;
            }
            if (data.substr(6).find_first_not_of("0123456789") == std::string::npos) {
                //std::cout << data.substr(6) << " " << data.substr(6).size() << std::endl; 
                mem_values->reg_no = std::stoi(data.substr(6));
                //std::cout << mem_values->reg_no << std::endl;
            }
                
        }
    }
    mem_trace_lock.lock();
    mem_trace_values.push_back(mem_values);
    mem_trace_lock.unlock();
}

void read_lines_for_pagefault(void * args) {
    struct read_line_args *read_args = (struct read_line_args *) args;
    std::string data; 
    std::ifstream input_stream;
    input_stream.open(read_args->fd_string, std::ios::in);
    while(1) {
        while(getline(input_stream, data)) {
            data.append("\n");
            parse_pagefault(data);
        }
    }
}

void read_lines_for_memtrace(void * args) {
    struct read_line_args *read_args = (struct read_line_args *) args;
    std::string data; 
    std::ifstream input_stream;
    input_stream.open(read_args->fd_string, std::ios::in);
    while(1) {
        while(getline(input_stream, data)) {
            if (data.find("eof") != std::string::npos) {
                std::cout << "eof" << std::endl;
                return;
            }
            data.append("\n");
            parse_mem_trace(data);
        }
    }
}


int main(int argc, char *argv[]) {
    char * line_buffer = (char *) malloc(4096);
    char * mem_buffer = (char *) malloc(4096);

    pthread_t thread_id, thread_id_2;
    struct read_line_args args = {argv[1], line_buffer};
    std::thread t1(read_lines_for_pagefault, &args);
    struct read_line_args args2 = {argv[2], mem_buffer};

    std::thread t2(read_lines_for_memtrace, &args2);
    std::thread t3(merge_traces_and_collect_stats, &args);
    t1.join();
    t2.join();
    t3.join();
    exit(EXIT_SUCCESS);
}


