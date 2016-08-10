
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "osm.h"
#include <cstdio>

#define STRING_TO_WRITE "orly and idan forever <3"
#define DEFAULT_ITERATION 1000
#define TO_NANO(s, us) s*1000000000.0 + us*1000.0
#define CALCULATE_TIME(start, end) (end - start)/(5.0*iterations) 
#define MAX_MACHINE_NAME_LEN 255

static unsigned int fd;



/* Initialization function that the user must call
 * before running any other library function.
 * The function may, for example, allocate memory or
 * create/open files.
 * Returns 0 uppon success and -1 on failure
 */
int osm_init(){
    fd = open("/tmp/.orlykor12", O_CREAT|O_TRUNC|O_RDWR|O_SYNC, 0777);
    if(fd < 0){
        return -1;
    }
    return 0;
}


/* finalizer function that the user must call
 * after running any other library function.
 * The function may, for example, free memory or
 * close/delete files.
 * Returns 0 uppon success and -1 on failure
 */
int osm_finalizer()
{
    if (unlink("/tmp/.orlykor12") == -1) {
        return -1;
    }
    return 0;
}


/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
   struct timeval t1;
   struct timeval t2;
   
   if (iterations == 0)
    {
        iterations = DEFAULT_ITERATION;
    }
   
    if (gettimeofday(&t1, NULL) == -1) {
        return -1;
    }
    unsigned int a = 0, b = 0, c = 0, d = 0, e = 0;
    for (unsigned int i = 0; i < iterations; i++) {
        a++;
        b++;
        c++;
        d++;
        e++;
    }
    if (gettimeofday(&t2, NULL) == -1) {
        return -1;
    }
    double s = CALCULATE_TIME(t1.tv_sec, t2.tv_sec);
    double us = CALCULATE_TIME(t1.tv_usec, t2.tv_usec);
    return TO_NANO(s, us);

}

/* Empty function used for osm_function_time()
 */
static void emptyFunc() {
    
}

/* Time measurement function for an empty function call.
   returns time in nano-seconds upon success, 
   and -1 upon failure.
   */
double osm_function_time(unsigned int iterations){
    struct timeval t1;
    struct timeval t2;
    
    if (iterations == 0)
    {
        iterations = DEFAULT_ITERATION;
    }
    
    if (gettimeofday(&t1, NULL) == -1) {
        return -1;
    }
   
    for (unsigned int i = 0; i < iterations; i++) {
        emptyFunc();
        emptyFunc();
        emptyFunc();
        emptyFunc();
        emptyFunc();
    }
    if (gettimeofday(&t2, NULL) == -1) {
        return -1;
    }
    double s = CALCULATE_TIME(t1.tv_sec, t2.tv_sec);
    double us = CALCULATE_TIME(t1.tv_usec, t2.tv_usec);
    return TO_NANO(s, us);
}


/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success, 
   and -1 upon failure.
   */
double osm_syscall_time(unsigned int iterations){
    struct timeval t1;
    struct timeval t2;
    
    if (iterations == 0)
    {
        iterations = DEFAULT_ITERATION;
    }
    
    if (gettimeofday(&t1, NULL) == -1) {
        return -1;
    }
   
    for (unsigned int i = 0; i < iterations; i++) {
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
    }
    if (gettimeofday(&t2, NULL) == -1) {
        return -1;
    }
    double s = CALCULATE_TIME(t1.tv_sec, t2.tv_sec);
    double us = CALCULATE_TIME(t1.tv_usec, t2.tv_usec);
    return TO_NANO(s, us);
}

/* Time measurement function for accessing the disk.
   returns time in nano-seconds upon success, 
   and -1 upon failure.
   */
double osm_disk_time(unsigned int iterations){
    struct timeval t1;
    struct timeval t2;
    
    if (iterations == 0)
    {
        iterations = DEFAULT_ITERATION;
    }
    
    if (gettimeofday(&t1, NULL) == -1) {
        return -1;
    }
    char buffer[] = STRING_TO_WRITE;
    
    /* for every writing, we use fflush() function for synchronic writing and 
     * we check if we succeeded on writing to the file.
     */
    for (unsigned int i = 0; i < iterations; i++) {       
        if (write(fd, buffer, sizeof(buffer)) < 0) {
			return -1;
		}
        if (write(fd, buffer, sizeof(buffer)) < 0) {
			return -1;
		}
		if (write(fd, buffer, sizeof(buffer)) < 0) {
			return -1;
		}
		if (write(fd, buffer, sizeof(buffer)) < 0) {
			return -1;
		}
		if (write(fd, buffer, sizeof(buffer)) < 0) {
			return -1;
		}
    }
    
    if (gettimeofday(&t2, NULL) == -1) {
        return -1;
    }

    double s = CALCULATE_TIME(t1.tv_sec, t2.tv_sec);
    double us = CALCULATE_TIME(t1.tv_usec, t2.tv_usec);
    return TO_NANO(s, us);
}

/* Time measurement function for all operations.
 * returns a struct containing all the measures.
 * on any calculation error the value will be -1.
 * on any error in the machine name, the value will be NULL.
 * The machineName field in the returned struct is dynamically allocated and
 * it is the user responsibility to free the memory.
 */ 
timeMeasurmentStructure measureTimes (unsigned int operation_iterations,
                                        unsigned int function_iterations,
        				unsigned int syscall_iterations,
            				unsigned int disk_iterations)
{   
    timeMeasurmentStructure measures;
    measures.machineName = new char;
    if(gethostname(measures.machineName, MAX_MACHINE_NAME_LEN) == -1) {
        measures.machineName = NULL;
    }
    measures.instructionTimeNanoSecond = 
                                osm_operation_time(operation_iterations);
    measures.functionTimeNanoSecond = osm_function_time(function_iterations);
    measures.trapTimeNanoSecond = osm_syscall_time(syscall_iterations);
    measures.diskTimeNanoSecond = osm_disk_time(disk_iterations);
    if (measures.instructionTimeNanoSecond == 0){
		measures.functionInstructionRatio = -1;
		measures.trapInstructionRatio = -1;
		measures.diskInstructionRatio = -1;
	}
	else {
		measures.functionInstructionRatio = measures.functionTimeNanoSecond / 
										measures.instructionTimeNanoSecond;
		measures.trapInstructionRatio = measures.trapTimeNanoSecond / 
										measures.instructionTimeNanoSecond;
		measures.diskInstructionRatio = measures.diskTimeNanoSecond / 
										measures.instructionTimeNanoSecond;
	}
    return measures;
};
