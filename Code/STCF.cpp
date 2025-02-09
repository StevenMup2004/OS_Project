#include "scheduler.h"

void STCF(int size, process processes[])
{
    sort(processes, processes + size, comparedByBurst);
    int finished_processes = 0, processor[4], WillGoToTheQ[4], i = 0, pr;
    memset(processor, -1, sizeof processor);
    memset(WillGoToTheQ, -1, sizeof WillGoToTheQ);
    CompareProcess cmp(processes);
    priority_queue<int, vector<int>, CompareProcess> waiting(cmp);

    for (int t = 0;; t++)
    {
        // cout << "Time: " << t << endl;
        int idx_p = 0;

        // Check for newly arrived processes
        while (i < size && processes[i].arrive_time <= t)
        {
            processes[i].state = processes[i].phases[processes[i].phase_idx].second; // In this case we use only 1 phase
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first; // In this case we use only 1 phase
            waiting.push(i);
            //cout << "Process " << processes[i].process_name << " arrived and added to waiting queue.\n";
            i++;
        }

        // Reassign processors back to waiting queue
        for (int q = 0; q < 4; q++)
        {
            if (processor[q] != -1)
            {
                //cout << "Process " << processes[processor[q]].process_name << " pushed back to waiting queue.\n";
                waiting.push(processor[q]);
            }
            processor[q] = -1;
        }

        // Assign processes to processors from waiting queue
        for (int q = 0; q < 4; q++)
        {
            if (!waiting.empty())
            {
                WillGoToTheQ[q] = waiting.top();
                //cout << "Process " << processes[WillGoToTheQ[q]].process_name << " selected from waiting queue for processor " << q << ".\n";
                waiting.pop();
            }
        }

        match_prefrences(processor, WillGoToTheQ, processes);

        // Execute processes on processors
        for (idx_p = 0; idx_p < 4; idx_p++)
        {
            if (processor[idx_p] == -1) // means that processor is idle
            {
                output[t][idx_p] = "i";
                //cout << "Processor " << idx_p + 1 << " is idle.\n";
                continue;
            }

            pr = processor[idx_p]; // index of the process in the processor
            output[t][idx_p] = processes[pr].process_name;
            processes[pr].time_consumed++;
            //cout << "Processor " << idx_p +1 << " executing process " << processes[pr].process_name << " (Time consumed: " << processes[pr].time_consumed << "/" << processes[pr].current_brust_time << ").\n";

            if (processes[pr].time_consumed == processes[pr].current_brust_time) // process finished execution
            {
                processes[pr].phase_idx++;
                processes[pr].time_consumed = 0;
                if (processes[pr].phase_idx < processes[pr].n_phases) // still has more phases
                {
                    processes[pr].state = processes[pr].phases[processes[pr].phase_idx].second;
                    processes[pr].current_brust_time = processes[pr].phases[processes[pr].phase_idx].first;
                    // cout << "Process " << processes[pr].process_name << " moved to next phase.\n";
                }
                else // process finished completely
                {
                    finished_processes++;
                    processes[pr].state = 2;
                    processes[pr].complete_time = t;
                    processes[pr].turn_around_time = t - processes[pr].arrive_time;
                    // cout << "Process " << processes[pr].process_name << " finished execution.\n";
                }
                processor[idx_p] = -1;
            }
        }

        if (finished_processes == size)
        {
            print(t, processes, size);
            cout << "All processes completed.\n";
            return;
        }
    }
}
