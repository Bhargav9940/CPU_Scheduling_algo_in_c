#include <stdio.h>
#include <stdlib.h>
char q = 'n';
struct process {
    int id, at, bt, pr, ft, tat, wt, rt, flag;
};
void swap(struct process *x, struct process *y) {
    struct process temp = *x;
    *x = *y;
    *y = temp;
}
void sort_by_at(struct process ar1[], int n) {
        for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-1-i; j++) {
            if(ar1[j].at > ar1[j+1].at) {
                swap(&ar1[j+1], &ar1[j]);
            }
            else if(ar1[j].at == ar1[j+1].at) {
                if(ar1[j].id > ar1[j+1].id) {
                    swap(&ar1[j], &ar1[j+1]);
                }
            }
        }
    }
}
void sort_by_id(struct process ar1[], int n) {
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-1-i; j++) {
            if(ar1[j].id > ar1[j+1].id) {
                swap(&ar1[j+1], &ar1[j]);
            }
        }
    }
}
void tat_wt(struct process ar1[], int n) {
    for(int i=0; i<n; i++) {
        ar1[i].tat = ar1[i].ft - ar1[i].at;
        ar1[i].wt = ar1[i].tat - ar1[i].bt;
    }
}
void avg_print(struct process ar[], int n) {
    float totaltat = 0, totalwt = 0;
    for(int i=0; i<n; i++) {
        totaltat = totaltat + ar[i].tat;
        totalwt = totalwt + ar[i].wt;
    }
    float avgtat = totaltat/n;
    float avgwt = totalwt/n;
    if(q == 'y') {
        printf("P\tAT\tBT\tFT\tTAT\tWT\tPr\n");
        for(int i=0; i<n; i++) 
            printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", ar[i].id, ar[i].at, ar[i].bt, ar[i].ft, ar[i].tat, ar[i].wt, ar[i].pr);
    }
    else {
        printf("P\tAT\tBT\tFT\tTAT\tWT\n");
        for(int i=0; i<n; i++) 
            printf("%d\t%d\t%d\t%d\t%d\t%d\n", ar[i].id, ar[i].at, ar[i].bt, ar[i].ft, ar[i].tat, ar[i].wt);
    }
    printf("\n");
    printf("Average TAT: %.2fms\n", avgtat);
    printf("Average WT: %.2fms\n", avgwt);
}
int time_overhead() {
    char q;
    int to = 0;
    getchar();
    printf("Time overhead require? (y/n): ");
    q = getchar();
    if(q == 'y') {
        getchar();
        printf("Enter Time Overhead: ");
        scanf("%d", &to);
    }
    return to;
}
void fcfs(struct process ar[], int n) {
    int to = time_overhead();
    sort_by_at(ar, n);
    int totalft = 0;    //Calculation of ft
    for(int i=0; i<n; i++) { 
        if(ar[i].at > totalft) {
            totalft = totalft + (ar[i].at - totalft);  //Non-process Zone
        }
        totalft = totalft + ar[i].bt;
        ar[i].ft = totalft;
        if(to != 0)
            totalft = totalft + to;
    }
    sort_by_id(ar, n);
    tat_wt(ar, n);
    avg_print(ar, n);
}
void sjf(struct process ar[], int n) {
    int to = time_overhead();
    sort_by_at(ar, n);  
    int totalft = 0;
    int front = 0;
    for(int i=0; i<n; i++) {
        int min = front;
        for(int j=front; j<n; j++) {    //to find minimum
            if(ar[j].bt < ar[min].bt && ar[j].at <= totalft) {
                swap(&ar[min], &ar[j]);
            }
            else if(ar[j].bt == ar[min].bt && ar[j].at <= totalft) {   //if bt same
                if(ar[j].at < ar[min].at) {
                    swap(&ar[min], &ar[j]);
                }
                else if(ar[j].at == ar[min].at) {   //if at is also same
                    if(ar[j].id < ar[min].id) {
                        swap(&ar[min], &ar[j]);
                    }
                }
            }
        }
        if(ar[min].at > totalft) {                     
            totalft = totalft + (ar[min].at - totalft);  //Non-process Zone
        }
        totalft = totalft + ar[min].bt;  
        ar[min].ft = totalft;
        if(to != 0)
            totalft = totalft + to;        //time overhead
        front++;
    }
    sort_by_id(ar, n);   
    tat_wt(ar, n);
    avg_print(ar, n);
}
void srtn(struct process ar[], int n) {
    int to = time_overhead();
    sort_by_at(ar, n);
    int totalft = 0;
    int totalbt = 0;
    for(int i=0; i<n; i++) {
        totalbt = totalbt + ar[i].bt;
    }
    for(int i=0; i<n; i++) {    //copying bt to rt
        ar[i].rt = ar[i].bt;
    }
    int front = 0;        //ft
    for(int i=0; i<totalbt; i++) {
        int min = front;
        for(int j=front; j<n; j++) {    //to find minimum
            if(ar[j].rt < ar[min].rt && ar[j].at <= totalft) {
                swap(&ar[j], &ar[min]);
            }
            else if(ar[j].rt == ar[min].rt && ar[j].at <= totalft) {   //if bt same
                if(ar[j].at < ar[min].at) {
                    swap(&ar[j], &ar[min]);
                }
                else if(ar[j].at == ar[min].at) {   //if at is also same
                    if(ar[j].id < ar[min].id) {
                        swap(&ar[j], &ar[min]);
                    }
                }
            }
        }
        if(ar[min].at > totalft) {                            //perfact 
            totalft = totalft + (ar[min].at - totalft);  //Non-process Zone
        }
        totalft = totalft + 1;  
        ar[min].rt -= 1;
        if(ar[min].rt == 0) {
            ar[min].ft = totalft;
            front++;
        }
        if(to != 0)
            totalft = totalft + to;        //time overhead
    }
    sort_by_id(ar, n);
    tat_wt(ar, n);
    avg_print(ar, n);
}
void priority_nonpreemptive(struct process ar[], int n) {
    //considering 1 (minimum) as the highest priority
    int to = time_overhead();
    sort_by_at(ar, n);     
    int totalft = 0;   //ft
    int front = 0;
    for(int i=0; i<n; i++) {
        int min = front;
        for(int j=front; j<n; j++) {    //to find minimum
            if(ar[j].pr < ar[min].pr && ar[j].at <= totalft) {
                swap(&ar[j], &ar[min]);
            }
            else if(ar[j].pr == ar[min].pr && ar[j].at <= totalft) {   //if bt same
                if(ar[j].at < ar[min].at) {
                    swap(&ar[j], &ar[min]);
                }
                else if(ar[j].at == ar[min].at) {   //if at is also same
                    if(ar[j].id < ar[min].id) {
                        swap(&ar[j], &ar[min]);
                    }
                }
            }
        }
        if(ar[min].at > totalft) {                            //perfact 
            totalft = totalft + (ar[min].at - totalft);  //Non-process Zone
        }
        totalft = totalft + ar[min].bt;  
        ar[min].ft = totalft;
        if(to != 0)
            totalft = totalft + to;        //time overhead
        front++;
    }
    sort_by_id(ar, n);
    tat_wt(ar, n);
    avg_print(ar, n);
}
void priority_preemptive(struct process ar[], int n) {
    //Considering 1 (minimum) as the highest priority
    int to = time_overhead();
    sort_by_at(ar, n); 
    int totalft = 0;
    int totalbt = 0;
    for(int i=0; i<n; i++) {
        totalbt = totalbt + ar[i].bt;
    }
    //copying bt to rt
    for(int i=0; i<n; i++) {
        ar[i].rt = ar[i].bt;
    }
    //ft
    int front = 0;
    for(int i=0; i<totalbt; i++) {
        int min = front;
        for(int j=front; j<n; j++) {    //to find minimum
            if(ar[j].pr < ar[min].pr && ar[j].at <= totalft) {
                swap(&ar[j], &ar[min]);
            }
            else if(ar[j].pr == ar[min].pr && ar[j].at <= totalft) {   //if bt same
                if(ar[j].at < ar[min].at) {
                    swap(&ar[j], &ar[min]);
                }
                else if(ar[j].at == ar[min].at) {   //if at is also same
                    if(ar[j].id < ar[min].id) {
                        swap(&ar[j], &ar[min]);
                    }
                }
            }
        }
        if(ar[min].at > totalft) {                            //perfact 
            totalft = totalft + (ar[min].at - totalft);  //Non-process Zone
        }
        totalft = totalft + 1;  
        ar[min].rt -= 1;
        if(ar[min].rt == 0) {
            ar[min].ft = totalft;
            front++;
        }
        if(to != 0)
            totalft = totalft + to;        //time overhead
    }
    sort_by_id(ar, n);
    tat_wt(ar, n);
    avg_print(ar, n);
}
void round_robin(struct process ar[], int n) {
    int tq = 0;
    printf("Enter Time Quantum: ");
    scanf("%d", &tq);
    int to = time_overhead();
     sort_by_at(ar, n);     
    int totalft = 0, totalbt = 0, totalrt = 0;
    for(int i=0; i<n; i++) {
        totalbt = totalbt + ar[i].bt;
    }
    totalrt = totalbt;
    //copying bt to rt
    for(int i=0; i<n; i++) {
        ar[i].rt = ar[i].bt;
    }
    int k = 0;  
    int totalflag = n;
    int flag;
    while(totalflag != 0) {
        if(ar[k].at > totalft) {
            totalft = totalft + (ar[k].at - totalft); //no process zone
        }
        if(ar[k].rt <= tq && ar[k].rt > 0) {
            totalft = totalft + ar[k].rt;
            ar[k].rt = 0;
            flag = 1;
        }
        else if(ar[k].rt > 0 && totalflag == 1) {
            totalft = totalft + ar[k].rt;
            ar[k].rt = 0;
            flag = 1;
        }
        else if(ar[k].rt > 0) {
            totalft = totalft + tq;
            ar[k].rt = ar[k].rt - tq;
        }
        if(ar[k].rt == 0 && flag == 1) {
            ar[k].ft = totalft;
            totalflag--;
            flag = 0;
        }
        if(to != 0)
            totalft = totalft + to;
        if(k == n-1)
            k = 0;
        else if(ar[k+1].at <= totalft) {
            k++;
        }
        else 
            k = 0;
    }
    sort_by_id(ar, n);
    tat_wt(ar, n);
    avg_print(ar, n);
}
int main() {
    int n;
    printf("Enter no. of process: ");
    scanf("%d", &n);
    struct process pr[n];
    int countid = 1;
    for(int i=0; i<n; i++) {
        printf("Enter Arrival Time for P%d: ", i+1);
        scanf("%d", &pr[i].at);
        pr[i].id = countid++;
    }
    for(int i=0; i<n; i++) {
        printf("Enter Burst Time for P%d: ", i+1);
        scanf("%d", &pr[i].bt);
    }
    getchar();
    printf("Priority require? (y/n): ");
    scanf("%c", &q);
    if(q == 'y') {
        for(int i=0; i<n; i++) {
            printf("Enter priority for P%d: ", pr[i].id);
            scanf("%d", &pr[i].pr);
        }
    }
    printf("\nSelect Algorithm to apply: \n");
    int option;
    while(1) {
        printf("Enter 1 for FCFS.\n");
        printf("Enter 2 for SJF.\n");
        printf("Enter 3 for SRTN.\n");
        printf("Enter 4 for Priority(Non-preemptive).\n");
        printf("Enter 5 for Priority(Preemptive).\n");
        printf("Enter 6 for Round Robin.\n");
        printf("Enter 7 for exit.\n");
        printf("Enter option: ");
        scanf("%d", &option);
        switch(option) {
            case 1: fcfs(pr, n);
            break;
            case 2: sjf(pr, n);
            break;
            case 3: srtn(pr, n);
            break;
            case 4: priority_nonpreemptive(pr, n);
            break;
            case 5: priority_preemptive(pr, n);
            break;
            case 6: round_robin(pr, n);
            break;
            case 7: exit(0);
            break;
        }
    }
}