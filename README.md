# LoRaESP32RTTY
Radio Teletype (RTTY) on LoRa on ESP32, not using LoRaWAN
This is a radio teletype sketch for a LilyGO TTGO platform.

This is essentially an Arduino sketch of a sort of serial-to-wireless bridge, both sender(s) and receiver(s) using the same sketch, whereby traffic is encrypted by turning each letter (of a restricted 62-letter character set: A-Z, 0-9 and a bit of punctuation, { and } being used for Enter and Backspace, respectively) typed into five letters during transmission, which groups of five-letters are again matched into single letters upon receipt.

How to use this:

1. You need a key to say how to turn five-letter-combinations into single letters and vice versa. To get a key, run "gencode.lisp" in order to generate a new code. I assume you have not much less than 2MB flash space on the board. E.g. in SBCL, do this in Linux:

sbcl --script gencode.lisp > key.txt

2. Open key.txt and copen the generated array in add_key.ino at the marked place. This sketch is then to be uploaded to any and all senders and receivers.

Tweak in particular the pin arrangements at the top if you have any other hardware than a LilyGo TTGO, if necessary.

ALTERNATIVELY:

If you just want to "see it in action", quickly, just use with_key.ino.

Mind that due to the public nature of with_key.ino, this is now not at all secure.

Having got the sketch into the microcontroller, you can e.g. connect to it in Linux like this from each machine and simply type away at the terminal:

picocom --baud 300 --parity n --databits 8 --stopbits 1 --flow n /dev/ttyUSB0

The video demonstrating the usage you may find here:

https://www.youtube.com/watch?v=50AafpdRuIM


Nino Ivanov
