#include "scheduler.h"
#include <iostream> // Thêm thư viện này để sử dụng std::cout

void Priority(int size, process processes[])
{
    int finished_processes = 0, processor[4], WillGoToTheQ[4], i = 0, pr;
    memset(processor, -1, sizeof processor);
    memset(WillGoToTheQ, -1, sizeof WillGoToTheQ);
    CompareProcesspriority cmp(processes);
    priority_queue<int, vector<int>, CompareProcesspriority> waiting(cmp);

    for (int t = 0;; t++)
    {
        // std::cout << "Time: " << t << "\n"; // In ra thời gian hiện tại

        int idx_p = 0;
        for (idx_p = 0; idx_p < 4 && i < size && processes[i].arrive_time <= t; ++idx_p) // Nếu bộ xử lý rảnh và có process
        {
            if (processor[idx_p] == -1)
            {
                processor[idx_p] = i;
                processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
                processes[i].state = processes[i].phases[processes[i].phase_idx].second;
                processes[i].last_processor = idx_p;
               // std::cout << "Processor " << idx_p << " starts process " << processes[i].process_name << "\n";
                i++;
            }
        }

        while (i < size && processes[i].arrive_time <= t) // Các process đến nhưng không có CPU rảnh
        {
            processes[i].state = processes[i].phases[processes[i].phase_idx].second;
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first;
            waiting.push(i);
            // std::cout << "Process " << processes[i].process_name << " added to waiting queue\n";
            i++;
        }

        for (idx_p = 0; idx_p < 4; idx_p++)
        {
            if (processor[idx_p] == -1) // CPU đang rảnh
            {
                output[t][idx_p] = "i";
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.top();
                    // std::cout << "Processor " << idx_p << " will take process " << processes[waiting.top()].process_name << " from waiting queue\n";
                    waiting.pop();
                }
                continue;
            }

            pr = processor[idx_p]; // Index của process trong CPU
            output[t][idx_p] = processes[pr].process_name;
            processes[pr].time_consumed++;
           //  std::cout << "Processor " << idx_p << " is processing " << processes[pr].process_name
                 //     << " (Time consumed: " << processes[pr].time_consumed << "/" << processes[pr].current_brust_time << ")\n";

            if (processes[pr].time_consumed == processes[pr].current_brust_time) // Process hoàn thành
            {
                processes[pr].phase_idx++;
                processes[pr].time_consumed = 0;
                if (processes[pr].phase_idx < processes[pr].n_phases) // Process cần thêm
                {
                    processes[pr].state = processes[pr].phases[processes[pr].phase_idx].second;
                    processes[pr].current_brust_time = processes[pr].phases[processes[pr].phase_idx].first;
                   // std::cout << "Process " << processes[pr].process_name << " moves to next phase\n";
                }
                else // Process hoàn tất
                {
                    finished_processes++;
                    processes[pr].state = 2;
                    processes[pr].complete_time = t;
                    processes[pr].turn_around_time = t - processes[pr].arrive_time;
                    // std::cout << "Process " << processes[pr].process_name << " finished at time " << t << "\n";
                }

                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.top();
                    // std::cout << "Processor " << idx_p << " will take process " << processes[waiting.top()].process_name << " from waiting queue\n";
                    waiting.pop();
                }

                processor[idx_p] = -1;
            }
        }

        match_prefrences(processor, WillGoToTheQ, processes);

        if (finished_processes == size)
        {
            print(t, processes, size);
            std::cout << "All processes finished.\n";
            return;
        }
    }
}
