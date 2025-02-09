#include "scheduler.h"
void FCFS(int size, process processes[])
{
    int finished_processes = 0, processor[4], WillGoToTheQ[4], i = 0, pr;
    memset(processor, -1, sizeof processor);
    memset(WillGoToTheQ, -1, sizeof WillGoToTheQ);
    queue<int> waiting;
    for (int t = 0;; t++)
    {
        int idx_p = 0;
        for (idx_p = 0; idx_p < 4 && i < size && processes[i].arrive_time <= t; ++idx_p) // if processor is idle and processes
        {
            if (processor[idx_p] == -1)
            {
                processor[idx_p] = i;
                processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first; // In this case we use only 1 phase
                processes[i].state = processes[i].phases[processes[i].phase_idx].second; // In this case we use only 1 phase
                processes[i].last_processor = idx_p;
                i++;
            }
        }
        while (i < size && processes[i].arrive_time <= t) // processes arrived but there is no place at cpu so it will go in waiting queue
        {
            waiting.push(i);
            processes[i].state = processes[i].phases[processes[i].phase_idx].second; // In this case we use only 1 phase
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first; // In this case we use only 1 phase
            i++;
        }
        for (idx_p = 0; idx_p < 4; idx_p++)
        {
            if (processor[idx_p] == -1) // means that , that processor is idle
            {
                output[t][idx_p] = "i";
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.front();
                    waiting.pop();
                }
                continue;
            }
            pr = processor[idx_p]; // index of the process in the processor
            output[t][idx_p] = processes[pr].process_name;
            processes[pr].time_consumed++;
            if (processes[pr].time_consumed == processes[pr].current_brust_time) // proceses finished exec
            {
                processes[pr].phase_idx++;
                processes[pr].time_consumed = 0;
                if (processes[pr].phase_idx < processes[pr].n_phases) // still want more ?
                {
                    processes[pr].state = processes[pr].phases[processes[pr].phase_idx].second; //  NoteIn this case we use only 1 phase
                    processes[pr].current_brust_time = processes[pr].phases[processes[pr].phase_idx].first; // Note In this case we use only 1 phase
                }
                else // process finished
                {
                    finished_processes++;
                    processes[pr].state = 2;
                    processes[pr].complete_time = t;
                    processes[pr].turn_around_time = t - processes[pr].arrive_time;
                }
                if (!waiting.empty())
                {
                    WillGoToTheQ[idx_p] = waiting.front();
                    waiting.pop();
                }
                processor[idx_p] = -1;
            }
        }
        match_prefrences(processor, WillGoToTheQ, processes);

        if (finished_processes == size)
        {
            print(t, processes, size);
            return;
        }
    }
}
