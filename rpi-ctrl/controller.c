// gcc -o controller controller.c -lwiringPi -lpthread -lmicrohttpd
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#define PORT 80

#include <stdbool.h>
#include <wiringPi.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


#define STEP 13
#define DIRECTION 6
#define ENABLE 5

#define STEPS_TO_OPEN 840
#define HALF_STEP_PULSE 1000 // 1ms
#define STEPPER_THREAD_IDLE_SLEEP 10000 //10ms
#define UNSUPERVISED_STEPS 30

// globals
bool be_steppin = false;
int step_cycles = 0;
int target_step_cycles = 0;



int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
  struct MHD_Response *response;
  int ret;
  // extract blind point from url
  int position;
  sscanf(url, "/%d", &position);
  if (0 <= position && position <= 100){
    target_step_cycles = (position * STEPS_TO_OPEN) / 100;
  }
  response = MHD_create_response_from_buffer (strlen (url),
                                            (void*) url, MHD_RESPMEM_PERSISTENT);

  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}


// step_for generates the stepping pulse on a dedicated thread
void *step_for(void *n) {
  int x;
  int step_mod;

  for (;;){
    if (be_steppin){
        // direction check
      if (target_step_cycles < step_cycles){
        fprintf(stderr, "Position: %d, Target: %d\n", step_cycles, target_step_cycles);
        // enable FET otuput
        digitalWrite(ENABLE, LOW);
        // move down
        digitalWrite(DIRECTION, HIGH);
        step_mod = -1;
      }

      if (target_step_cycles > step_cycles){
        fprintf(stderr, "Position: %d, Target: %d\n", step_cycles, target_step_cycles);
        // enable FET otuput
        digitalWrite(ENABLE, LOW);
        // move up
        digitalWrite(DIRECTION, LOW);
        step_mod = 1;
      }

      if (target_step_cycles == step_cycles){
        // disable FET output
        digitalWrite(ENABLE, HIGH);
        usleep(STEPPER_THREAD_IDLE_SLEEP);
        continue;
      }


      x = 0;
      while(x++ < UNSUPERVISED_STEPS){
        // Send a step wave
        digitalWrite (STEP, HIGH);
        usleep(HALF_STEP_PULSE);
        digitalWrite (STEP, LOW);
        usleep(HALF_STEP_PULSE);
      };

    step_cycles = step_cycles + step_mod;

    }else{
      // disable FET output
      digitalWrite(ENABLE, HIGH);
      usleep(STEPPER_THREAD_IDLE_SLEEP);
    }
  }
  // this function should not return
  return NULL;
}


int main(void)
{
  // GPIO Init in BRCM pin mode
  wiringPiSetupGpio();
  pinMode(STEP, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  pinMode(ENABLE, OUTPUT);

  // create the stepping thread reference
  pthread_t step_for_thread;

  // start the stepping thread
  if(pthread_create(&step_for_thread, NULL, step_for, NULL)) {
    fprintf(stderr, "Error creating steppiong thread\n");
    return 1;
  }

  // start web server
  struct MHD_Daemon *web_listener;

  web_listener = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == web_listener) return 1;


  // The real work starts here
  // ** Turns out this thread doesn't need to do anything (yet!)
  //for (;;){  }
  be_steppin = true;

  // step_for_thread never returns, so the main thread just chills out here
  if(pthread_join(step_for_thread, NULL)) {
    fprintf(stderr, "Error joining stepping thread\n");
    return 2;

  }
  return 0 ;
}
