#include <unistd.h>
#include <iostream>
#include "NetworkHelper.hpp"
#include <stdexcept>

using namespace constrained_based_networks;

void printHelp()
{
    std::cout << "Please use -f <filename> -c <components_to_start>" << std::endl;
    exit(-1);
}

char* base_network_file;

volatile bool thread_alive[NUM_THREADS];

pthread_mutex_t mutex_for_list;
std::list<std::vector<unsigned int>> currently_processed_ids;

std::vector<unsigned int> params[NUM_THREADS];

void* runClassThread(void* id_)
{
    long int id = (long int)id_;
    std::cout << "Starting " << id << std::endl;
    NetworkHelper::createClassSolution(base_network_file, std::list<std::string>(), params[id]);
    pthread_mutex_lock(&mutex_for_list);
    currently_processed_ids.remove(params[id]);
    pthread_mutex_unlock(&mutex_for_list);
    thread_alive[id] = false;
    return 0;
}

void* runInstanceThread(void* id_)
{
    long int id = (long int)id_;
    std::cout << "Starting " << id << std::endl;
    NetworkHelper::createInstanceSolution(base_network_file, params[id]);
    pthread_mutex_lock(&mutex_for_list);
    currently_processed_ids.remove(params[id]);
    pthread_mutex_unlock(&mutex_for_list);
    thread_alive[id] = false;
    return 0;
}

// main test function
int main(int argc, char* argv[])
{
//    currently_processed_ids.push_back({0 ,0 ,10 ,0 ,0 ,8 ,0 ,0 ,10 ,0 ,0 ,10 ,0 ,0 ,10 ,0 ,0 ,10 ,3});
    NetworkHelper::initializeExporter();
    char c;
    base_network_file = 0;
    bool list_unresolved = false;

    while ((c = getopt(argc, argv, "lf:")) != -1) {
        switch (c) {
            case 'l':
                list_unresolved = true;
                break;
            case 'f':
                base_network_file = optarg;
                break;
            default:
                printHelp();
        }
    }
    if (!base_network_file) {
        printHelp();
    }

    for (unsigned int i = 0; i < NUM_THREADS; ++i) {
        thread_alive[i] = false;
    }

    std::vector<unsigned int> ids;
    if (list_unresolved) {
        if (NetworkHelper::getNextUncalculatedIDs(base_network_file, ids, currently_processed_ids)) {
            std::cout << "Unresolved ids: " << std::endl;
            for (auto id : ids) {
                std::cout << " -i " << id;
            }
            std::cout << std::endl;
        } else {
            std::cerr << "Everything is resolved" << std::endl;
        }
        return 0;
    }

    pthread_t threads[NUM_THREADS];

    // Okay we don't should list only, then we do a full resolution
    // outer loop make sure that we really capture all solutions even some was irgnored
    while (NetworkHelper::getNextUncalculatedIDs(base_network_file, ids, currently_processed_ids) || currently_processed_ids.size() != 0) {

        while (NetworkHelper::getNextUncalculatedIDs(base_network_file, ids, currently_processed_ids)) {
            long int free_thread = -1;
            while (free_thread == -1) {
                for (unsigned int i = 0; i < NUM_THREADS; i++) {
                    if (!thread_alive[i]) {
                        thread_alive[i] = true;
                        free_thread = i;
                        break;
                    }
                }
                sleep(1);
            }

            params[free_thread] = ids;
            pthread_mutex_lock(&mutex_for_list);
            currently_processed_ids.push_back(ids);
            pthread_mutex_unlock(&mutex_for_list);

            std::cout << "Resolving: " << std::endl;
            for (auto id : ids) {
                std::cout << " " << id;
            }

            std::cout << std::endl;
            if (ids.size() % 3 == 0) {
                if (pthread_create(&threads[free_thread], NULL, &runClassThread, (void*)free_thread)) {
                    throw std::runtime_error("Cannot start thread");
                }
            } else if (ids.size() % 3 == 1) {
                if (pthread_create(&threads[free_thread], NULL, &runInstanceThread, (void*)free_thread)) {
                    throw std::runtime_error("Cannot start thread");
                }
            }
            ids.clear();
        }
        std::cout << "Waiting for other thread to complete, are " << currently_processed_ids.size() << " threads active." << std::endl;
        sleep(1);
    }

    return 0;
}
