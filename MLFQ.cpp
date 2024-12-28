#include "scheduler.h"
#include <iostream>
using namespace std;

void changeThePriorty(int n, process processes[])
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].state == 2)
            continue;
        processes[i].priority = 0;
        processes[i].qunta = 0;
    }
}

void MLFQ(int n, process processes[])
{
    int time_to_moveup = 10, finised_processes = 0, MAX_Q = 4, current_queue = 0, WillGoToTheQ[4], processor[4], p, i = 0, time_slice = 10;

    queue<int> my_queue[MAX_Q];
    memset(processor, -1, sizeof(processor));
    memset(WillGoToTheQ, -1, sizeof(WillGoToTheQ));

    for (int t = 0;; ++t)
    {
        // cout << "Time: " << t << endl;

        // Handle new process arrivals
        while (i < n && processes[i].arrive_time <= t)
        {
            processes[i].priority = 0;
            processes[i].current_brust_time = processes[i].phases[processes[i].phase_idx].first; // Note In this case we use only 1 phase
            processes[i].state = processes[i].phases[processes[i].phase_idx].second; // Note In this case we use only 1 phase

            // Check if all queues are empty first
            bool queues_empty = true;
            for (int q = 0; q < MAX_Q; q++) {
                if (!my_queue[q].empty()) {
                    queues_empty = false;
                    break;
                }
            }

            bool process_assigned = false;

            // If queues are empty, try to find an empty processor
            if (queues_empty) {
                for (int idx_p = 0; idx_p < 4; idx_p++) {
                    if (processor[idx_p] == -1) {
                        processor[idx_p] = i;
                        processes[i].last_processor = idx_p;
                        process_assigned = true;
                        // cout << "Process " << processes[i].process_name << " added to processor " << idx_p << endl;
                        break;
                    }
                }
            }

            // If no empty processor found and queues are not empty, try preemption
            if (!process_assigned) {
                for (int idx_p = 0; idx_p < 4; idx_p++) {
                    if (processor[idx_p] != -1 && processes[processor[idx_p]].priority > 0) {
                        my_queue[processes[processor[idx_p]].priority].push(processor[idx_p]);
                        processor[idx_p] = i;
                        processes[i].last_processor = idx_p;
                        process_assigned = true;
                        // cout << "Process " << processes[i].process_name << " preempted processor " << idx_p << endl;
                        break;
                    }
                }
            }

            // If still not assigned, add to queue 0
            if (!process_assigned) {
                my_queue[0].push(i);
                // cout << "Process " << processes[i].process_name << " added to queue 0" << endl;
            }

            i++;
        }

        // [Dowork in processor section]
        for (int idx_p = 0; idx_p < 4; ++idx_p)
        {
            if (processor[idx_p] == -1)
            {
                output[t][idx_p] = "i";
                continue;
            }

            p = processor[idx_p];
            output[t][idx_p] = processes[p].process_name;
            processes[p].qunta++;
            processes[p].time_consumed++;

            if (processes[p].qunta % (1 << (processes[p].priority + 1)) == 0 || processes[p].time_consumed == processes[p].current_brust_time)
            {
                if (processes[p].qunta % (1 << (processes[p].priority + 1)) == 0)
                {
                    processes[p].priority++;
                    processes[p].qunta = 0;
                    // cout << "Process " << processes[p].process_name << " moved to queue level " << processes[p].priority << endl;
                }
                if (processes[p].time_consumed == processes[p].current_brust_time)
                {
                    processes[p].phase_idx++;
                    processes[p].time_consumed = 0;
                    if (processes[p].phase_idx < processes[p].n_phases) // In this case we use only 1 phase
                    {
                        processes[p].state = processes[p].phases[processes[p].phase_idx].second;
                        processes[p].current_brust_time = processes[p].phases[processes[p].phase_idx].first;
                    }
                    else
                    {
                        finised_processes++;
                        processes[p].state = 2;
                        processes[p].complete_time = t;
                        processes[p].turn_around_time = t - processes[p].arrive_time;
                       // cout << "Process " << processes[p].process_name << " finished execution" << endl;
                    }
                }
                else
                {
                    my_queue[processes[p].priority].push(p);
                }

                processor[idx_p] = -1;
            }
        }

        // Add process for the processor
        current_queue = 0;
        for (int idx_p = 0; idx_p < 4; ++idx_p)
        {
            if (processor[idx_p] != -1)
                continue;

            while (current_queue < MAX_Q && my_queue[current_queue].empty())
            {
                current_queue++;
            }
            if (current_queue < MAX_Q && !my_queue[current_queue].empty())
            {
                WillGoToTheQ[idx_p] = my_queue[current_queue].front();
                my_queue[current_queue].pop();
            }
        }

        match_prefrences(processor, WillGoToTheQ, processes);

        if (finised_processes == n)
        {
            print(t, processes, n);
            return;
        }
    }
}
