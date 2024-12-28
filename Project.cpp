#include "scheduler.h"

using namespace std;

void Input(int n, process processes[])
{
    for (int i = 0; i < n; i++)
    {
        cin >> processes[i].process_name;
        processes[i].n_phases = 1;
        cin >> processes[i].arrive_time;
        processes[i].phases.resize(processes[i].n_phases);
        for (int j = 0; j < processes[i].n_phases; j++)
        {
            cin >> processes[i].phases[j].first;
            processes[i].phases[j].second = 1;
        }
        processes[i].last_processor = -1;
        processes[i].phase_idx = 0;
        processes[i].time_consumed = 0;
        processes[i].process_idx = i;
        processes[i].state = 1;
        processes[i].qunta = 0;
        processes[i].tickets = calculateTickets(processes[i]);
        processes[i].pass_value = 0;
        processes[i].time_consumed = 0;
        processes[i].stride = MAX_STRIDE / processes[i].tickets;
    }
}

void Input_Priority(int n, process processes[])
{
    for (int i = 0; i < n; i++)
    {
        cin >> processes[i].process_name;
        processes[i].n_phases = 1;
        cin >> processes[i].arrive_time;
        cin >> processes[i].priority;
        processes[i].phases.resize(processes[i].n_phases);
        for (int j = 0; j < processes[i].n_phases; j++)
        {
            cin >> processes[i].phases[j].first;
            processes[i].phases[j].second = 1;
        }
        processes[i].last_processor = -1;
        processes[i].phase_idx = 0;
        processes[i].time_consumed = 0;
        processes[i].process_idx = i;
        processes[i].state = 1;
        processes[i].qunta = 0;
    }
}

void choose_scheduler(int n, string scheduler, process processes[])
{
    cout << "Running scheduler: " << scheduler << endl;

    if (scheduler == "RR")
        round_robin(n, processes);
    else if (scheduler == "MLFQ")
        MLFQ(n, processes);
    else if (scheduler == "SJF")
        SJF(n, processes);
    else if (scheduler == "STCF")
        STCF(n, processes);
    else if (scheduler == "FCFS")
        FCFS(n, processes);
    else if (scheduler == "Priority")
        Priority(n, processes);

   // cout << "Completed scheduler: " << scheduler << endl;
}

void run_all_schedulers(int n, process processes[])
{
    vector<string> schedulers = {"RR", "MLFQ", "SJF", "STCF", "FCFS"}; // Exclude "Priority"

    for (const string& scheduler : schedulers)
    {
        cout << "Running scheduler: " << scheduler << endl;

        // Create a copy of processes to prevent interference between runs
        process temp_processes[n];
        copy(processes, processes + n, temp_processes);

        if (scheduler == "RR")
            round_robin(n, temp_processes);
        else if (scheduler == "MLFQ")
            MLFQ(n, temp_processes);
        else if (scheduler == "SJF")
            SJF(n, temp_processes);
        else if (scheduler == "STCF")
            STCF(n, temp_processes);
        else if (scheduler == "FCFS")
            FCFS(n, temp_processes);

    }
}

int main()
{
    freopen("input.txt", "r", stdin);

    string scheduler;
    cin >> scheduler;
    int n;
    cin >> n;
    process processes[n];

    if (scheduler == "Priority")
        Input_Priority(n, processes);
    else
        Input(n, processes);

    if (scheduler == "All")
    {
        run_all_schedulers(n, processes);
    }
    else
    {
        choose_scheduler(n, scheduler, processes);
    }

    return 0;
}
