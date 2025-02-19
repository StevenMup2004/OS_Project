#include "scheduler.h"

void round_robin(int n, process processes[])
{
    queue<int> my_queue;
    int finised_processes = 0, WillGoToTheQ[4], processor[4], p, i = 0, time_slice = 5;
    memset(processor, -1, sizeof(processor));
    memset(WillGoToTheQ, -1, sizeof(WillGoToTheQ));
    for (int t = 0;; ++t)
    {
        // push in queue
        int idx_p = 0;

        //Which process arive time came to enter Q or CPU
        for (idx_p = 0; idx_p < 4 && i < n && processes[i].arrive_time <= t; ++idx_p)
        {
            if (processor[idx_p] == -1)
            {
                processor[idx_p] = i;
                processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first; // In this case we use only 1 phase
                processes[i].state = processes[i].phases[processes[i].phase_idx].second;// In this case we use only 1 phase
                processes[i].last_processor = idx_p;
                i++;
            }
        }
        while (i < n && processes[i].arrive_time <= t)
        {
            my_queue.push(i);

            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first; // In this case we use only 1 phase
            processes[i].state = processes[i].phases[processes[i].phase_idx].second;// In this case we use only 1 phase
            i++;
        }

        // dowork in processor
        for (idx_p = 0; idx_p < 4; ++idx_p)
        {
            // idle
            if (processor[idx_p] == -1)
            {
                output[t][idx_p] = "i";
                if (!my_queue.empty())
                {
                    WillGoToTheQ[idx_p] = my_queue.front();
                    my_queue.pop();
                }
                continue;
            }

            // do the work for one unit of time
            p = processor[idx_p];
            output[t][idx_p] = processes[p].process_name;
            processes[p].time_consumed++;

            // check to remove the process from the processor
            if (processes[p].time_consumed % time_slice == 0 || processes[p].time_consumed == processes[p].current_brust_time) // remove from processor
            {
                if (processes[p].time_consumed == processes[p].current_brust_time) // removed after finishing EXE
                {
                    processes[p].phase_idx++;
                    processes[p].time_consumed = 0;
                    if (processes[p].phase_idx < processes[p].n_phases)
                    {
                        processes[p].state = processes[p].phases[processes[p].phase_idx].second;//  Note In this case we use only 1 phase
                        processes[p].current_brust_time = processes[p].phases[processes[p].phase_idx].first; // Note In this case we use only 1 phase
                    }
                    else
                    {
                        finised_processes++;
                        processes[p].state = 2;
                        processes[p].complete_time = t;
                        processes[p].turn_around_time = t - processes[p].arrive_time;
                    }
                }
                else // removed after time slice
                {
                    my_queue.push(p);
                }

                // add process for the processor
                if (!my_queue.empty())
                {
                    WillGoToTheQ[idx_p] = my_queue.front();
                    my_queue.pop();
                }
                processor[idx_p] = -1;
            }
        }
        match_prefrences(processor, WillGoToTheQ, processes);
        // 5lsna
        if (finised_processes == n)
        {
            print(t, processes, n);
            return;
        }
    }
}
