Welcome to the ESP32 port of the Klipper project!

The objective is to port Klipper to the esp32 platform, with a focus on
performance and predictability. Lots of users seek some sort of wifi/bt
support when looking for a firmware for the esp32, it is important to
clarify that this project DOES NOT aim for that, and will likely never
support it. It _might_ be technically possible but part of the focus is
in getting ESP-IDF and FreeRTOS out of the way to get klipper to work
reliably, so it is not considering a path worth following. 

With that out of the way, PRs are welcome! One of the main objectives is
to eventually be able to merge this upstream, so have that in mind when
submitting code. There's also heavy work been done "getting freertos out
of the way", so please have that in mind also. 

ESP-IDF is still used for this project, but only the HAL/Low-Level layers
are used for anything time critical, ideally evereything should only rely
on it but that's a work in progress. 

So long, and thanks for all the fish!
