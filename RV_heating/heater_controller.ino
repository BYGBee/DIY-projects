#define OVERTEMP_PIN 4
#define RESET_PIN 5
#define RELAY_PIN 12

#define CYCLE_TIME_SEC 10
#define SECONDS_PER_HOUR 3600
#define CYCLES_PER_HOUR (SECONDS_PER_HOUR / CYCLE_TIME_SEC)

#define CYCLES_BETWEEN_POT_READ 6   // ~1 minute (6 × 10s)

// ---------- SELECT BEHAVIOR MODE ----------
// 0 = Option A (apply change at next hour)
// 1 = Option B (apply change immediately)
// 2 = Option C (apply proportional change)
#define BEHAVIOR_MODE 1
// ------------------------------------------

const int command_pin = A0;

unsigned int remaining_cycles = 0;
unsigned int commanded_cycles = 0;
unsigned int cycle_counter_hour = CYCLES_PER_HOUR;
unsigned int pot_read_counter = 0;

void setup()
{
  pinMode(OVERTEMP_PIN, INPUT);
  pinMode(RESET_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
}

void loop()
{
  // -------- SAFETY FIRST --------
  bool safe = (digitalRead(OVERTEMP_PIN) == HIGH);

  if (!safe)
  {
    digitalWrite(RELAY_PIN, LOW);

    static bool led = false;
    led = !led;
    digitalWrite(LED_BUILTIN, led);
    delay(500);  // half-second blink

    return;
  }

  // -------- READ POTENTIOMETER --------
  if (pot_read_counter == 0)
  {
    int pot_raw = analogRead(command_pin);
    unsigned int new_command =
        map(pot_raw, 0, 1023, 0, CYCLES_PER_HOUR);

    if (new_command != commanded_cycles)
    {
      // ----- OPTION A: Update only at hour boundary -----
#if BEHAVIOR_MODE == 0
      commanded_cycles = new_command;
#endif

      // ----- OPTION B: Apply immediately -----
#if BEHAVIOR_MODE == 1
      commanded_cycles = new_command;
      remaining_cycles = commanded_cycles;
      cycle_counter_hour = CYCLES_PER_HOUR;
#endif

      // ----- OPTION C: Adjust proportionally -----
#if BEHAVIOR_MODE == 2
      {
        commanded_cycles = new_command;
        float hour_progress =
            1.0f - (float)cycle_counter_hour / (float)CYCLES_PER_HOUR;
        float remaining_fraction = 1.0f - hour_progress;
        remaining_cycles = (unsigned int)(commanded_cycles * remaining_fraction);
      }
#endif
    }

    pot_read_counter = CYCLES_BETWEEN_POT_READ;
  }
  pot_read_counter--;

  // -------- HOURLY RESET --------
  if (cycle_counter_hour == 0)
  {
    cycle_counter_hour = CYCLES_PER_HOUR;
    remaining_cycles = commanded_cycles;
  }
  cycle_counter_hour--;

  // -------- HEATING CONTROL --------
  if (remaining_cycles > 0)
  {
    digitalWrite(RELAY_PIN, HIGH);
    remaining_cycles--;
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(CYCLE_TIME_SEC * 1000UL);
}